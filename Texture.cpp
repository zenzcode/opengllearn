#include "Texture.h"

Texture::Texture() {
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = "";
}

Texture::Texture(const char* fileLoc) : fileLocation(fileLoc) {
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
}

bool Texture::LoadTexture() {
	unsigned char* textureData = SOIL_load_image(fileLocation, &width, &height, &bitDepth, SOIL_LOAD_AUTO);
	if (!textureData) {
		std::cerr << "Failed to load data..." << std::endl;
		return false;
	}


	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	SOIL_free_image_data(textureData);
	return true;
}

bool Texture::LoadTextureA() {
	unsigned char* textureData = SOIL_load_image(fileLocation, &width, &height, &bitDepth, SOIL_LOAD_AUTO);
	if (!textureData) {
		std::cerr << "Failed to load data..." << std::endl;
		return false;
	}


	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	SOIL_free_image_data(textureData);
	return true;
}

void Texture::UseTexture() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

}

void Texture::ClearTexture() {
	glDeleteTextures(1, &textureID);
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = (char*)"";
}

Texture::~Texture() {
	ClearTexture();
}