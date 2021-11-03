#include <iostream>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include <vector>

#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "Material.h"

static const char* vertex = "Shaders/shader.vert";
static const char* frag = "Shaders/shader.frag";

Window* mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;
Camera* camera;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

Texture brickTexture;
Texture dirtTexture;

DirectionalLight mainLight;
Material shinyMaterial;
Material dullMaterial;

void CalcAverageNormals(unsigned int* indices, unsigned int numOfIndices, GLfloat* vertices, unsigned int verticeCount, unsigned int vLength, unsigned int normalOffset) {
	for (size_t i = 0; i < numOfIndices; i += 3) {
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1 = glm::vec3(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2 = glm::vec3(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset;
		in1 += normalOffset;
		in2 += normalOffset;

		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount/vLength; ++i) {
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec = glm::vec3(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x;
		vertices[nOffset + 1] = vec.y;
		vertices[nOffset + 2] = vec.z;
	}
}

void CreateObjects() { 
	unsigned int indices[] = {
		1, 0, 2,
		3, 4, 2,
		4, 1, 2,
		0, 3, 2,
		1, 4, 3,
		1, 3, 0,
	};

	GLfloat vertices[] = {
		-.5f, -.5f, -.5f,	0.f, 0.f,		0.f, 0.f, 0.f,
		.5f, -.5f, -.5f,	0.5f, 0.f,	    0.f, 0.f, 0.f,
		0.f, 0.5f, 0.f,		0.f, 0.5f,		0.f, 0.f, 0.f,
		-.5f, -.5f, .5f,	 0.f, 0.f,		0.f, 0.f, 0.f,
		.5f, -.5f, .5f,		0.5f, 0.f,		0.f, 0.f, 0.f,
		.5f, -.5f, 0.f,		0.5f, 0.f,		0.f, 0.f, 0.f
	};

	CalcAverageNormals(indices, 18, vertices, 48, 8, 5);

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 48, 18);
	meshList.emplace_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 48, 18);
	meshList.emplace_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(vertices, indices, 48, 18);
	meshList.emplace_back(obj3);
}

void CreateShaders() {
	Shader* shader = new Shader();
	shader->CreateFromFiles(vertex, frag);
	shaderList.emplace_back(shader);
}

int main()
{
	mainWindow = new Window(1336, 768);
	mainWindow->Initialise();
	CreateObjects();
	CreateShaders();

	camera = new Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.f, 0.f, 5.0f, 0.5f);
	brickTexture = Texture((char*)"Images/brick.png");
	brickTexture.LoadTexture();
	dirtTexture = Texture((char*)"Images/dirt.png");
	dirtTexture.LoadTexture();
	mainLight = DirectionalLight(1.f, 1.f, 1.f, .1f, 2.f, 2.f, -2.f, 1.f);
	shinyMaterial = Material(1.f, 32.f);
	dullMaterial = Material(.3f, 4.f);
	glm::mat4x4 projection = glm::perspective(45.f, mainWindow->GetBufferWidth() / mainWindow->GetBufferHeight(), 0.1f, 100.f);

	glEnable(GL_CULL_FACE);

	while (!mainWindow->GetShouldClose()) {

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		glCullFace(GL_BACK);

		glfwPollEvents();
		glClearColor(.125, .125, .125, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		camera->keyControl(mainWindow->getKeys(), deltaTime);
		camera->mouseControl(mainWindow->GetChangeX(), mainWindow->GetChangeY());

		shaderList[0]->UseShader();

		glUniformMatrix4fv(shaderList[0]->GetProjectionLocation(), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(shaderList[0]->GetViewLocation(), 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));
		glUniform3f(shaderList[0]->GetUniformEyePosition(), camera->GetCameraPosition().x, camera->GetCameraPosition().y, camera->GetCameraPosition().z);
		shaderList[0]->SetDirectionalLight(&mainLight);
		glm::mat4x4 model = glm::mat4x4(1.f);
		model = glm::translate(model, glm::vec3(1.2f, 0.f, -3.f));
		glUniformMatrix4fv(shaderList[0]->GetModelLocation(), 1, GL_FALSE, glm::value_ptr(model));
		brickTexture.UseTexture();
		shinyMaterial.UseMaterial(shaderList[0]->GetUniformSpecularIntensity(), shaderList[0]->GetUniformShininess());
		glUniform1f(shaderList[0]->GetTextureUnitLocation(), 0);
		meshList[0]->RenderMesh();

		model = glm::mat4(1.f);
		model = glm::translate(model, glm::vec3(-1.2f, 0.f, -3.f));
		glUniformMatrix4fv(shaderList[0]->GetModelLocation(), 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(shaderList[0]->GetTextureUnitLocation(), 0);
		brickTexture.UseTexture();
		shinyMaterial.UseMaterial(shaderList[0]->GetUniformSpecularIntensity(), shaderList[0]->GetUniformShininess());
		meshList[1]->RenderMesh();

		dullMaterial.UseMaterial(shaderList[0]->GetUniformSpecularIntensity(), shaderList[0]->GetUniformShininess());
		model = glm::mat4(1.f);
		model = glm::translate(model, glm::vec3(0.f, 0.f, -3.f));
		glUniformMatrix4fv(shaderList[0]->GetModelLocation(), 1, GL_FALSE, glm::value_ptr(model));
		dirtTexture.UseTexture();
		meshList[2]->RenderMesh();
		glUniform1f(shaderList[0]->GetTextureUnitLocation(), 0);
		glUseProgram(0);

		mainWindow->SwapBuffers();
	}

	for (auto& mesh : meshList) {
		delete mesh;
	}

	for (auto& shader : shaderList) {
		delete shader;
	}

}
