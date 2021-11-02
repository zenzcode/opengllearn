#include "Material.h"
Material::Material() {
	specularIntensity = 0.0f;
	shininess = 0.0f;
}

Material::Material(GLfloat specularIntensity, GLfloat shine) : specularIntensity(specularIntensity), shininess(shine) {

}

void Material::UseMaterial(GLuint specularIntensityUniform, GLuint shininessUniform) {
	glUniform1f(specularIntensityUniform, specularIntensity);
	glUniform1f(shininessUniform, shininess);
}

Material::~Material() {

}