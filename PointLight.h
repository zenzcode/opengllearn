#pragma once
#include "Light.h"
class PointLight :
    public Light
{
public:
    PointLight();
    PointLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat diffuseIntensity, GLfloat con, GLfloat lin, GLfloat exp);
    void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation, GLfloat diffuseIntensityLocation, GLfloat positionLocation, GLfloat constantLocation, GLfloat linearLocation, GLfloat exponentialLocation);
    ~PointLight();
private:
    glm::vec3 position;

    GLfloat linear, exponential, constant;
};

