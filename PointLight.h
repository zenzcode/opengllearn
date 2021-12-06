#pragma once
#include "Light.h"
#include "OmniShadowMap.h"
class PointLight :
    public Light
{
public:
    PointLight();
    PointLight(GLfloat shadowWidth, GLfloat shadowHeight, GLfloat near, GLfloat far, GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat diffuseIntensity, GLfloat con, GLfloat lin, GLfloat exp);
    void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation, GLfloat diffuseIntensityLocation, GLfloat positionLocation, GLfloat constantLocation, GLfloat linearLocation, GLfloat exponentialLocation);
    std::vector<glm::mat4> CalculateLightTransform();
    GLfloat GetFarPlane();
    glm::vec3 GetPosition();
    ~PointLight();
protected:
    glm::vec3 position;

    GLfloat linear, exponential, constant;

    GLfloat farPlane;
};

