#include <iostream>
#include <array>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include <vector>

#include "CommonValues.h"
#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "Texture.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "Material.h"
#include "ModelLoader.h"
#include "Skybox.h"

static const char* vertex = "Shaders/shader.vert";
static const char* frag = "Shaders/shader.frag";

static const char* vertex2D = "Shaders/shader.vert2d";
static const char* frag2D = "Shaders/Shader.frag2d";

static const char* depthVertex2D = "Shaders/DepthShader.vert2d";
static const char* depthFrag2D = "Shaders/DepthShader.frag2d";

static const char* directionalShadowMapVert = "Shaders/DirectionalShadowMap.vert";
static const char* directionalShadowMapFrag = "Shaders/DirectionalShadowMap.frag";


static const char* omniShadowMapVert = "Shaders/OmniShadowMap.vert";
static const char* omniShadowMapGeom = "Shaders/OmniShadowMap.geom";
static const char* omniShadowMapFrag = "Shaders/OmniShadowMap.frag";

static const char* humvee = "Models/Humvee/Humvee.obj";
static const char* plane = "Models/plane/11803_Airplane_v1_l1.obj";

Window* mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;
Camera* camera;
Shader directionalShadowShader;
Shader omniShadowShader;

GLuint colorBuffer, depthTexture;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

Texture brickTexture;
Texture dirtTexture;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

Material shinyMaterial;
Material dullMaterial;

ModelLoader humveeModel;
ModelLoader planeModel;
GLuint windowVAO, windowVBO, windowIBO, windowFBO;
GLuint bigWindowVAO, bigWindowVBO, bigWindowIBO;
Skybox skyBox;

unsigned int pointLightCount = 0;
unsigned int spotLightCount = 0;

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

	unsigned int floorIndices[]{
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.f, -10.0f, 0.f, 0.f, 0.f, -1.f, 0.f,
		10.f, 0.f, -10.f, 10.f, 0.f, 0.f, -1.f, 0.f,
		-10.f, 0.f, 10.f, 0.f, 10.f, 0.f, -1.f, 0.f,
		10.0f, 0.f, 10.f, 10.f, 10.f, 0.f, -1.f, 0.f
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

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.emplace_back(obj4);
}

void CreateShaders() {
	Shader* shader = new Shader();
	shader->CreateFromFiles(vertex, frag);
	shaderList.emplace_back(shader);
	Shader* shader2D = new Shader;
	shader2D->CreateFromFiles(vertex2D, frag2D);
	shaderList.emplace_back(shader2D);
	Shader* depthShader2D = new Shader;
	depthShader2D->CreateFromFiles(depthVertex2D, depthFrag2D);
	shaderList.emplace_back(depthShader2D);
	directionalShadowShader = Shader();
	directionalShadowShader.CreateFromFiles(directionalShadowMapVert, directionalShadowMapFrag);
	omniShadowShader.CreateFromFiles(omniShadowMapVert, omniShadowMapGeom, omniShadowMapFrag);
}

//gotta find a smarter way to do that... but later..
void DrawMiniWindows() {
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glScissor(0, 0, mainWindow->GetWidth(), mainWindow->GetHeight() / 8);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);

	shaderList[1]->UseShader();
	glUniform1i(glGetUniformLocation(shaderList[1]->GetShaderID(), "bufferTexture"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, colorBuffer);
	glViewport(0, 0, mainWindow->GetWidth() / 10, mainWindow->GetHeight() / 8);
	glBindVertexArray(windowVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, windowIBO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glActiveTexture(0);
	glUseProgram(0);

	shaderList[2]->UseShader();
	glUniform1i(glGetUniformLocation(shaderList[2]->GetShaderID(), "bufferTexture"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glViewport(mainWindow->GetWidth() / 10, 0, mainWindow->GetWidth() / 10, mainWindow->GetHeight() / 8);
	glBindVertexArray(windowVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, windowIBO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glActiveTexture(0);
	glUseProgram(0);


	glViewport(0, 0, mainWindow->GetWidth(), mainWindow->GetHeight());
	glEnable(GL_DEPTH_TEST);
}

void DrawMainQuad() {
	glDisable(GL_DEPTH_TEST);
	shaderList[1]->UseShader();
	glUniform1i(glGetUniformLocation(shaderList[1]->GetShaderID(), "bufferTexture"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, colorBuffer);
	glBindVertexArray(bigWindowVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bigWindowIBO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glUseProgram(0);
	glEnable(GL_DEPTH_TEST);
}

GLuint uniformModel = 0;
void RenderScene() {
	glm::mat4x4 model = glm::mat4x4(1.f);
	model = glm::translate(model, glm::vec3(1.2f, 0.f, -3.f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	brickTexture.UseTexture();
	shinyMaterial.UseMaterial(shaderList[0]->GetUniformSpecularIntensity(), shaderList[0]->GetUniformShininess());
	glUniform1f(shaderList[0]->GetTextureUnitLocation(), 1);
	meshList[0]->RenderMesh();

	model = glm::mat4(1.f);
	model = glm::translate(model, glm::vec3(-1.2f, 0.f, -3.f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniform1f(shaderList[0]->GetTextureUnitLocation(), 1);
	brickTexture.UseTexture();
	shinyMaterial.UseMaterial(shaderList[0]->GetUniformSpecularIntensity(), shaderList[0]->GetUniformShininess());
	meshList[1]->RenderMesh();

	dullMaterial.UseMaterial(shaderList[0]->GetUniformSpecularIntensity(), shaderList[0]->GetUniformShininess());
	model = glm::mat4(1.f);
	model = glm::translate(model, glm::vec3(0.f, 0.f, -3.f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	dirtTexture.UseTexture();
	meshList[2]->RenderMesh();

	shinyMaterial.UseMaterial(shaderList[0]->GetUniformSpecularIntensity(), shaderList[0]->GetUniformShininess());
	model = glm::mat4(1.f);
	model = glm::translate(model, glm::vec3(0.f, -2.f, 0.f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	dirtTexture.UseTexture();
	meshList[3]->RenderMesh();
	glUniform1f(shaderList[0]->GetTextureUnitLocation(), 1);


	shinyMaterial.UseMaterial(shaderList[0]->GetUniformSpecularIntensity(), shaderList[0]->GetUniformShininess());
	model = glm::mat4(1.f);
	model = glm::translate(model, glm::vec3(-2.f, 0.f, 0.f));
	model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	humveeModel.RenderModel();

	shinyMaterial.UseMaterial(shaderList[0]->GetUniformSpecularIntensity(), shaderList[0]->GetUniformShininess());
	model = glm::mat4(1.f);
	model = glm::translate(model, glm::vec3(4.f, 0.f, 0.f));
	model = glm::rotate(model, glm::radians(-90.f), glm::vec3(1, 0, 0));
	model = glm::scale(model, glm::vec3(0.001f, 0.001f, 0.001f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	planeModel.RenderModel();
	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	DrawMainQuad();

	if (mainWindow->IsMiniWindows()) {
		DrawMiniWindows();
	}
}

void OmniShadowMapPass(PointLight* pointLight) {
	omniShadowShader.UseShader();
	glViewport(0, 0, pointLight->GetShadowMap()->GetShadowWidth(), pointLight->GetShadowMap()->GetShadowHeight());

	pointLight->GetShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = omniShadowShader.GetModelLocation();

	glUniform3f(omniShadowShader.GetOmniLightPos(), pointLight->GetPosition().x, pointLight->GetPosition().y, pointLight->GetPosition().z);
	glUniform1f(omniShadowShader.GetFarPlanePos(), pointLight->GetFarPlane());
	omniShadowShader.SetOmniLightMatrices(pointLight->CalculateLightTransform());

	omniShadowShader.Validate();
	RenderScene();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);
}
 
void DirectionalShadowMapPass(DirectionalLight* dirLight) {
	directionalShadowShader.UseShader();
	glViewport(0, 0, dirLight->GetShadowMap()->GetShadowWidth(), dirLight->GetShadowMap()->GetShadowHeight());
	dirLight->GetShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = directionalShadowShader.GetModelLocation();
	glm::mat4 lightTransform = dirLight->CalculateLightTransform();
	directionalShadowShader.SetDirectionalLightTransform(&lightTransform);

	directionalShadowShader.Validate();
	RenderScene();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glUseProgram(0);
}

void RenderPass(glm::mat4 view, glm::mat4 projection) {
	glBindFramebuffer(GL_FRAMEBUFFER, windowFBO);
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, mainWindow->GetWidth(), mainWindow->GetHeight());
	skyBox.DrawSkyBox(projection, view);

	shaderList[0]->UseShader();
	glViewport(0, 0, mainWindow->GetWidth(), mainWindow->GetHeight());
	uniformModel = shaderList[0]->GetModelLocation();

	glUniformMatrix4fv(shaderList[0]->GetProjectionLocation(), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(shaderList[0]->GetViewLocation(), 1, GL_FALSE, glm::value_ptr(view));
	glUniform3f(shaderList[0]->GetUniformEyePosition(), camera->GetCameraPosition().x, camera->GetCameraPosition().y, camera->GetCameraPosition().z);

	glm::mat4 lightTransform = mainLight.CalculateLightTransform();
	shaderList[0]->SetDirectionalLightTransform(&lightTransform);

		shaderList[0]->SetDirectionalLight(&mainLight);
	shaderList[0]->SetPointLight(pointLights, pointLightCount, 3, 0);
	shaderList[0]->SetSpotLight(spotLights, spotLightCount, 3 + pointLightCount, pointLightCount);

	mainLight.GetShadowMap()->Read(GL_TEXTURE2);
	shaderList[0]->SetTexture(1);
	shaderList[0]->SetDirectionalShadowMap(2);


	glm::vec3 lowerLight = camera->GetCameraPosition();
	lowerLight.y -= 0.3f;
	spotLights[0].SetFlash(lowerLight, camera->GetCameraDirection());

	shaderList[0]->Validate();
	RenderScene();
}

int main()
{
	mainWindow = new Window(1336, 768);
	mainWindow->Initialise();
	CreateObjects();
	CreateShaders();

	camera = new Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.f, 0.f, 5.0f, 0.5f);
	brickTexture = Texture((char*)"Images/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture((char*)"Images/dirt.png");
	dirtTexture.LoadTextureA();

	glGenTextures(1, &colorBuffer);
	glGenTextures(1, &depthTexture);

	humveeModel = ModelLoader();
	humveeModel.LoadModel(humvee);
	
	planeModel = ModelLoader();
	planeModel.LoadModel(plane);

	mainLight = DirectionalLight(2048, 2048, 1.f, 1.f, 1.f, 0.1f, 0.f, -7.f, -1.f, 0.3f);
	//
	//pointLights[0] = PointLight(1024, 1024, 0.01f, 100.f, 0.0f, 1.0f, 0.0f, 1.0f, 0.f, 0.f, 0.f, .1f, 0.3f, 0.2f, 0.1f);
	//pointLightCount++;
	//	
	//pointLights[1] = PointLight(1024, 1024, 0.01f, 100.f, 0.0f, 0.0f, 1.0f, 1.0f, 4.f, 0.f, 0.f, .5f, 0.3f, 0.1f, 0.1f);
	//pointLightCount++;


	spotLights[0] = SpotLight(1024, 1024,
		0.1f, 100.f,
		1.f, 1.f, 1.f,
		2.f, 
		0.0f, 0.0f, 0.0f, 
		0.0f, -1.f, 0.0f, 
		0.0f, 1.0f, 0.0f, 0.0f, 
		20.0f);

	spotLightCount++;

	std::vector<std::string> skyBoxImages;
	skyBoxImages.push_back("Skybox/right.jpg");
	skyBoxImages.push_back("Skybox/left.jpg");
	skyBoxImages.push_back("Skybox/top.jpg");
	skyBoxImages.push_back("Skybox/bottom.jpg");
	skyBoxImages.push_back("Skybox/front.jpg");
	skyBoxImages.push_back("Skybox/back.jpg");
	skyBox = Skybox(skyBoxImages);


	shinyMaterial = Material(1.f, 265.f);
	dullMaterial = Material(.3f, 4.f);

	glm::mat4x4 projection = glm::perspective(glm::radians(45.f), mainWindow->GetBufferWidth() / mainWindow->GetBufferHeight(), 0.1f, 100.f);

	glEnable(GL_CULL_FACE);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, colorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mainWindow->GetWidth(), mainWindow->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, mainWindow->GetWidth(), mainWindow->GetHeight(), 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);




	unsigned int indices[] = {
		1, 0, 2,
		1, 2, 3
	};

	GLfloat vertices[] = {
		-1.f, -1.f, 0.f,	0.f, 0.f,	0.f, 0.f, 0.f, //bottom left
		-1.f, 1.f, 0.f,		0.f, 1.f,	0.f, 0.f, 0.f, //top left
		1.f, -1.f, 0.f,		1.f, 0.f,	0.f, 0.f, 0.f, //bottom right
		1.f, 1.f, 0.f,		1.f, 1.f,	0.f, 0.f, 0.f //top right
	};

	glGenVertexArrays(1, &bigWindowVAO);
	glBindVertexArray(bigWindowVAO);
	glGenBuffers(1, &bigWindowIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bigWindowIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glGenBuffers(1, &bigWindowVBO);
	glBindBuffer(GL_ARRAY_BUFFER, bigWindowVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 3));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);



	glGenVertexArrays(1, &windowVAO);
	glBindVertexArray(windowVAO);
	glGenBuffers(1, &windowIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, windowIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glGenBuffers(1, &windowVBO);
	glBindBuffer(GL_ARRAY_BUFFER, windowVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 3));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenFramebuffers(1, &windowFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, windowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Framebuffer not complete. Status:" << glCheckFramebufferStatus(GL_FRAMEBUFFER) << "\n Error: " << (GLenum)glGetError() << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	std::array<GLuint, 4> clearColor{ 0, 0, 0, 0 };
	glClearBufferuiv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, clearColor.data());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//hi, i love you <3

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

		DirectionalShadowMapPass(&mainLight);
		for (size_t i = 0; i < pointLightCount; i++) {
			OmniShadowMapPass(&pointLights[i]);
		}
		for (size_t i = 0; i < spotLightCount; i++) {
			OmniShadowMapPass(&spotLights[i]);
		}
		RenderPass(camera->calculateViewMatrix(), projection);

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
