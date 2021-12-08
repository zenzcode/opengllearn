#pragma once
#include <vector>
#include <string>
#include <SOIL/SOIL.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
class Skybox
{
public:
	Skybox();
	Skybox(std::vector<std::string> faceLocations);
	void DrawSkyBox(glm::mat4 projMatrix, glm::mat4 viewMatrix);
	~Skybox();
private:
	Shader* skyShader;
	Mesh* skyMesh;

	GLuint uniformProjectionMatrix, uniformViewMatrix;
	GLuint textureId;
};

