#pragma once
#include <glew.h>
#include <SOIL/SOIL.h>
#include <iostream>
#include <string>
class Texture
{
public:
	Texture();
	Texture(const char* fileLocation);

	bool LoadTexture();
	bool LoadTextureA();
	void UseTexture();
	void ClearTexture();

	~Texture();
private:
	GLuint textureID;
	int width, height, bitDepth;
	const char* fileLocation;
};

