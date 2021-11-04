#include "PointLight.h"

PointLight::PointLight() : Light(){
	position = glm::vec3(0, 0, 0);
	constant = 1.f;
	exponential = 0.f;
	linear = 0.f;
}

PointLight::PointLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat diffuseIntensity, GLfloat con, GLfloat lin, GLfloat exp) :Light(red, green, blue, aIntensity, diffuseIntensity) {
	position = glm::vec3(posX, posY, posZ);
	constant = con;
	linear = lin;
	exponential = exp;
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

PointLight::~PointLight() {

}