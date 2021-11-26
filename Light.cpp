#include "Light.h"

Light::Light() {
	color = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 1.f;
	diffuseIntensity = 0;
	lightProj = glm::mat4(1);
	shadowMap = new ShadowMap();
}

Light::Light(GLfloat shadowWith, GLfloat shadowHeight, GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity,GLfloat diffuseIntensity) {
	color = glm::vec3(red, green, blue);
	ambientIntensity = aIntensity;
	this->diffuseIntensity = diffuseIntensity;
	lightProj = glm::mat4(1);
	shadowMap = new ShadowMap();
	shadowMap->Init(shadowWith, shadowHeight);
}

Light::~Light() {

}