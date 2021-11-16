#pragma once

#include <iostream>
#include <glew.h>

class ShadowMap
{
public:
	ShadowMap();
	virtual bool Init(GLuint width, GLuint height);
	virtual void Write();	
	virtual void Read(GLenum textureUnit);	
	inline GLuint GetShadowWidth() { return shadowWidth; }
	~ShadowMap();
protected:
	GLuint FBO;
	GLuint shadowMapID;
	GLuint shadowWidth, shadowHeight;
};

