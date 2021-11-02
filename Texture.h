#pragma once
#include <glew.h>
#include <SOIL/SOIL.h>
#include <iostream>
#include <string>
class Texture
{
public:
	Texture();
	Texture(char* fileLocation);

	void LoadTexture();
	void UseTexture();
	void ClearTexture();

	~Texture();
private:
	GLuint textureID;
	int width, height, bitDepth;
	char* fileLocation;
};

