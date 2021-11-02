#pragma once
#include <glew.h>

class Material
{
public:
	Material();
	Material(GLfloat sIntensity, GLfloat shine);
	void UseMaterial(GLuint specularIntensityUniform, GLuint shininessUniform);
	~Material();
private:
	GLfloat specularIntensity;
	GLfloat shininess;
};

