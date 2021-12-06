#include "SpotLight.h"

SpotLight::SpotLight() : PointLight(){
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	edge = 0.0f;
}

SpotLight::SpotLight(GLfloat shadowWidth, GLfloat shadowHeight, GLfloat near, GLfloat far, GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat posX, GLfloat posY,
	GLfloat posZ, GLfloat dirX, GLfloat dirY, GLfloat dirZ, GLfloat diffuseIntensity,
	GLfloat con, GLfloat lin, GLfloat exp, GLfloat edge) : PointLight(shadowWidth, shadowHeight, near, far, red, green, blue, aIntensity, posX, posY, posZ, diffuseIntensity, con, lin, exp)
{
	this->direction = glm::normalize(glm::vec3(dirX, dirY, dirZ));
	this->edge = edge;
	this->procEdge = cosf(glm::radians(edge));
}

void SpotLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation, GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint constantLocation, 
	GLuint linearLocation, GLuint exponentialLocation, GLuint directionLocation, GLuint edgeLocation) {
	glUniform3f(ambientColorLocation, color.x, color.y, color.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);

	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentialLocation, exponential);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(edgeLocation, procEdge);
}

void SpotLight::SetFlash(glm::vec3 pos, glm::vec3 dir) {
	position = pos;
	direction = dir;
}

SpotLight::~SpotLight() {

}