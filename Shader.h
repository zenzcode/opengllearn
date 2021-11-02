#pragma once
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <glew.h>

class Shader
{
public:
	Shader();
	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);
	std::string ReadFile(const char* fileLocation);
	GLuint GetProjectionLocation();
	GLuint GetViewLocation();
	GLuint GetModelLocation();
	GLuint GetTextureUnitLocation();
	GLuint GetAmbientIntensityLocation();
	GLuint GetAmbientColorLocation();
	GLuint GetUniformDiffuseIntensity();
	GLuint GetUniformDirection();
	GLuint GetUniformEyePosition();
	GLuint GetUniformSpecularIntensity();
	GLuint GetUniformShininess();
	void UseShader();
	void ClearShader();
	~Shader();
private:
	GLuint shaderID;
	GLuint uniformProjection;
	GLuint uniformModel;
	GLuint uniformView;
	GLuint uniformTextureUnit;
	GLuint uniformAmbientIntensity;
	GLuint uniformAmbientColor;
	GLuint uniformDiffuseIntensity;
	GLuint uniformDirection;
	GLuint uniformEyePos;
	GLuint uniformSpecularIntensity;
	GLuint uniformShininess;

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, GLenum shaderType, const char* shaderText);
};

