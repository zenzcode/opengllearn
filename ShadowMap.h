#pragma once

#include <iostream>
#include <glew.h>

class ShadowMap
{
public:
	ShadowMap();
	virtual bool Init(GLfloat width, GLfloat height);
	virtual void Write();	
	virtual void Read(GLenum textureUnit);	
	inline GLuint GetShadowWidth() { return shadowWidth; }
    inline GLuint GetShadowHeight() { return shadowHeight; }
	~ShadowMap();
protected:
	GLuint FBO;
	GLuint shadowMapID;
	GLuint shadowWidth, shadowHeight;
};

