#include "Skybox.h"
Skybox::Skybox() {

}

Skybox::Skybox(std::vector<std::string> faceLocations) {
	skyShader = new Shader();
	skyShader->CreateFromFiles("Shaders/SkyShader.vert", "Shaders/SkyShader.frag");

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

	uniformProjectionMatrix = skyShader->GetProjectionLocation();
	uniformViewMatrix = skyShader->GetViewLocation();

	int width, height, bitDepth;

	for (size_t i = 0; i < 6; ++i) {
		unsigned char* texData = SOIL_load_image(faceLocations[i].c_str(), &width, &height, &bitDepth, SOIL_LOAD_AUTO);
		if (!texData) {
			std::cout << "Could not find file at location " << faceLocations[i] << std::endl;
			return;
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);

		SOIL_free_image_data(texData);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	unsigned int indices[] = {
		0, 1, 3,
		0, 3, 2,
		4, 7, 6,
		4, 5, 7,
		0, 4, 6,
		0, 6, 3,
		2, 7, 5,
		2, 3, 7,
		4, 0, 5,
		0, 2, 5,
		1, 7, 6,
		3, 6, 7
	};

	GLfloat vertices[] = {
		-1.f, 1.f, -1.f,	0.f, 0.f,	0.f,0.f,0.f, // 0
		-1.f, -1.f, -1.f,	0.f, 0.f,	0.f,0.f,0.f, // 1
		1.f, 1.f, -1.f,		0.f, 0.f,	0.f,0.f,0.f, // 2
		1.f, -1.f, -1.f,	0.f, 0.f,	0.f,0.f,0.f, // 3


		-1.f, 1.f, 1.f,		0.f, 0.f,	0.f,0.f,0.f, // 4
		1.f, 1.f, 1.f,		0.f, 0.f,	0.f,0.f,0.f, // 5
		-1.f, -1.f, 1.f,	0.f, 0.f,	0.f,0.f,0.f, // 6
		1.f, -1.f, 1.f,		0.f, 0.f,	0.f,0.f,0.f // 7
	};
	skyMesh = new Mesh();
	skyMesh->CreateMesh(vertices, indices, 64, 36);
}

void Skybox::DrawSkyBox(glm::mat4 projMatrix, glm::mat4 viewMatrix) {
	viewMatrix = glm::mat4(glm::mat3(viewMatrix));

	glDepthMask(GL_FALSE);
	skyShader->UseShader();
	glUniformMatrix4fv(uniformViewMatrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(uniformProjectionMatrix, 1, GL_FALSE, glm::value_ptr(projMatrix));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
	skyShader->Validate();
	skyMesh->RenderMesh();
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glActiveTexture(0);
	glDepthMask(GL_TRUE);
}

Skybox::~Skybox() {

}