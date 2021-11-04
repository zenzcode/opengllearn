#pragma once
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include  <glew.h>
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

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
	void SetDirectionalLight(DirectionalLight* directionalLight);
	void SetPointLight(PointLight* pLight, unsigned int lightCount);
	void SetSpotLight(SpotLight* sLight, unsigned int lightCount);
	void UseShader();
	void ClearShader();
	~Shader();
private:
	int pointLightCount;
	int spotLightCount;
	GLuint shaderID;
	GLuint uniformProjection;
	GLuint uniformModel;
	GLuint uniformView;
	GLuint uniformTextureUnit;
	GLuint uniformEyePos;
	GLuint uniformSpecularIntensity;
	GLuint uniformShininess;

	struct {
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;
		GLuint uniformDirection;
	} uniformDirectionalLight;

	struct {
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;
		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;
	} uniformPointLight[MAX_POINT_LIGHTS];

	struct {
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;
		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;
		GLuint uniformDirection;
		GLuint uniformEdge;
	} uniformSpotLight[MAX_SPOT_LIGHTS];

	GLuint uniformPointLightCount;
	GLuint uniformSpotLightCount;


	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, GLenum shaderType, const char* shaderText);
};

