#pragma once
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include  <glew.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
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
	void CreateFromFiles(const char* vertexLocation, const char* geometryShader, const char* fragmentLocation);
	void Validate();
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
	GLuint GetUniformDirectionalLightTransform();
	GLuint GetUniformDirectionalShadowMap();
	GLuint GetUniformTexture();
	GLuint GetOmniLightPos();
	GLuint GetFarPlanePos();
	inline GLuint GetShaderID() {
		return shaderID;
	}
	void SetDirectionalLight(DirectionalLight* directionalLight);
	void SetPointLight(PointLight* pLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset);
	void SetSpotLight(SpotLight* sLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset);
	void UseShader();
	void ClearShader();
	void SetTexture(GLuint textureUnit);
	void SetDirectionalShadowMap(GLuint textureUnit);
	void SetDirectionalLightTransform(glm::mat4* lTransform);
	void SetOmniLightMatrices(std::vector<glm::mat4> lightMatrices);
	~Shader();
private:
	int pointLightCount;
	int spotLightCount;
	GLuint shaderID = 0;
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformTextureUnit = 0;
	GLuint uniformEyePos = 0;
	GLuint uniformSpecularIntensity = 0;
	GLuint uniformShininess = 0;
	GLuint uniformDirectionalLightTransform = 0;
	GLuint uniformDirectionalShadowMap = 0;
	GLuint uniformTexture = 0;
	GLuint uniformOmniLightPos = 0;
	GLuint uniformFarPlane = 0;
	GLuint uniformLightMatrices[6] = { 0 };

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

	struct {
		GLuint shadowMap;
		GLuint farPlane;
	} uniformOmniShadowMaps[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];

	GLuint uniformPointLightCount = 0;
	GLuint uniformSpotLightCount = 0;


	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void CompileShader(const char* vertexCode, const char* geometryCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, GLenum shaderType, const char* shaderText);

	void CompileProgram();
};

