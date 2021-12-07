#include "PointLight.h"

PointLight::PointLight() : Light(){
	position = glm::vec3(0, 0, 0);
	constant = 1.f;
	exponential = 0.f;
	linear = 0.f;
}

PointLight::PointLight(GLfloat shadowWidth, GLfloat shadowHeight, GLfloat near, GLfloat far, GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat diffuseIntensity, GLfloat con, GLfloat lin, GLfloat exp) :Light(shadowWidth, shadowHeight, red, green, blue, aIntensity, diffuseIntensity) {
	position = glm::vec3(posX, posY, posZ);
	constant = con;
	linear = lin;
	exponential = exp;

	farPlane = far;

	float aspect = shadowWidth / shadowHeight;
	lightProj = glm::perspective(glm::radians(90.f), aspect, near, far);

	shadowMap = new OmniShadowMap();
	shadowMap->Init(shadowWidth, shadowHeight);
}

void PointLight::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation, GLfloat diffuseIntensityLocation, GLfloat positionLocation, GLfloat constantLocation, GLfloat linearLocation, GLfloat exponentialLocation) {
	glUniform3f(ambientColorLocation, color.x, color.y, color.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);

	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentialLocation, exponential);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}

std::vector<glm::mat4> PointLight::CalculateLightTransform() {
	std::vector<glm::mat4> lightTransform;
	//+x-x
	lightTransform.push_back(lightProj * glm::lookAt(position, position + glm::vec3(1.0f, 0.f, 0.f), glm::vec3(0.f, -1.f, 0.f)));
	lightTransform.push_back(lightProj * glm::lookAt(position, position + glm::vec3(-1.0f, 0.f, 0.f), glm::vec3(0.f, -1.f, 0.f)));
	//+y-y
	lightTransform.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0f, 1.f, 0.f), glm::vec3(0.f, 0.f, 1.f)));
	lightTransform.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0f, -1.f, 0.f), glm::vec3(0.f, 0.f, -1.f)));
	//+z-z
	lightTransform.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0f, 0.f, 1.f), glm::vec3(0.f, -1.f, 0.f)));
	lightTransform.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0f, 0.f, -1.f), glm::vec3(0.f, -1.f, 0.f)));
	return lightTransform;
}


glm::vec3 PointLight::GetPosition() {
	return position;
}

GLfloat PointLight::GetFarPlane() {
	return farPlane;
}

PointLight::~PointLight() {

}