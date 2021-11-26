#include "DirectionalLight.h"
DirectionalLight::DirectionalLight() : Light() {
	direction = glm::vec3(0, -1.0f, 0);
	lightProj = glm::ortho(-5.f, 5.f, -5.f, 5.f, 0.1f, 20.0f);
}

DirectionalLight::DirectionalLight(GLfloat shadowWidth, GLfloat shadowHeight, GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat directionX, GLfloat directionY, GLfloat directionZ, GLfloat diffuseIntensity) : Light(shadowWidth, shadowHeight, red, green, blue, aIntensity, diffuseIntensity) {
	direction = glm::vec3(directionX, directionY, directionZ);
	lightProj = glm::ortho(-20.f, 20.f, -20.f, 20.f, 0.1f, 100.0f);
}


void DirectionalLight::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation, GLfloat diffuseIntensityLocation, GLfloat directionLocation) {
	glUniform3f(ambientColorLocation, color.x, color.y, color.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}

glm::mat4 DirectionalLight::CalculateLightTransform() {		
	return lightProj * glm::lookAt(-direction, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

DirectionalLight::~DirectionalLight() {

}