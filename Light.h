#pragma once
#include <glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "ShadowMap.h"
class Light
{
public:
	Light();
	Light(GLfloat shadowWidth, GLfloat shadowHeight, GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat diffuseIntensity);
	inline ShadowMap* GetShadowMap() {
		return shadowMap;
	}
	~Light();
protected:
	glm::vec3 color;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;
	glm::mat4 lightProj;
	ShadowMap* shadowMap;
};

