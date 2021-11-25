#pragma once
#include "Light.h"

class DirectionalLight :
    public Light
{
public:
    DirectionalLight();
    DirectionalLight(GLfloat shadowWith, GLfloat shadowHeight, GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat directionX, GLfloat directionY, GLfloat directionZ, GLfloat diffuseIntensity);
    void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation, GLfloat diffuseIntensityLocation, GLfloat directionLocation);
    glm::mat4 CalculateLightTransform();
    ~DirectionalLight();
private:
    glm::vec3 direction;
};

