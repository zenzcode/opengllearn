#pragma once
#include <glew.h>
#include <glm.hpp>
#include "ShadowMap.h"
class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat diffuseIntensity);

	~Light();
protected:
	glm::vec3 color;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;
	glm::mat4 lightProj;
	ShadowMap* shadowMap;
};

