#include "DirectionalLight.h"
DirectionalLight::DirectionalLight() : Light() {
	direction = glm::vec3(0, -1.0f, 0);
}

DirectionalLight::DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat directionX, GLfloat directionY, GLfloat directionZ, GLfloat diffuseIntensity) : Light(red, green, blue, aIntensity, diffuseIntensity) {
	direction = glm::vec3(directionX, directionY, directionZ);
}


void DirectionalLight::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation, GLfloat diffuseIntensityLocation, GLfloat directionLocation) {
	glUniform3f(ambientColorLocation, color.x, color.y, color.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}

DirectionalLight::~DirectionalLight() {

}