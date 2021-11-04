#pragma once
#include "PointLight.h"
class SpotLight :
    public PointLight
{
public:
    SpotLight();
    SpotLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat dirX, GLfloat dirY, GLfloat dirZ, GLfloat diffuseIntensity, GLfloat con, GLfloat lin, GLfloat exp, GLfloat edge);
    void UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation, GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint constantLocation, GLuint linearLocation, GLuint exponentialLocation, GLuint directionLocation, GLuint edgeLocation);
    void SetFlash(glm::vec3 pos, glm::vec3 dir);
    ~SpotLight();
private:
    glm::vec3 direction;

    GLfloat edge, procEdge;
};

