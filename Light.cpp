#include "Light.h"

Light::Light() {
	color = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 1.f;
	diffuseIntensity = 0;
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity,GLfloat diffuseIntensity) {
	color = glm::vec3(red, green, blue);
	ambientIntensity = aIntensity;
	this->diffuseIntensity = diffuseIntensity;
}

Light::~Light() {

}