#include "Shader.h"

Shader::Shader() {
	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;
	uniformView = 0;
	pointLightCount = 0;
	spotLightCount = 0;
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode) {
	CompileShader(vertexCode, fragmentCode);
}


void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation) {
	std::string vertexString = ReadFile(vertexLocation);
	std::string fragmentString = ReadFile(fragmentLocation);
	CompileShader(vertexString.c_str(), fragmentString.c_str());
}


void Shader::CreateFromFiles(const char* vertexLocation, const char* geometryShader, const char* fragmentLocation) {
	std::string vertexString = ReadFile(vertexLocation);
	std::string geometryString = ReadFile(geometryShader);
	std::string fragmentString = ReadFile(fragmentLocation);
	CompileShader(vertexString.c_str(), geometryString.c_str(), fragmentString.c_str());
}

std::string Shader::ReadFile(const char* fileLocation) {
	std::string contentFile;
	std::ifstream fileStream(fileLocation, std::ios::in);

	if (!fileStream.is_open()) return "File could not be loaded.";
	
	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		contentFile.append(line + "\n");
	}
	fileStream.close();
	return contentFile;
}


void Shader::CompileShader(const char* vertexCode, const char* geometryCode, const char* fragmentCode) {
	shaderID = glCreateProgram();

	AddShader(shaderID, GL_VERTEX_SHADER, vertexCode);
	AddShader(shaderID, GL_GEOMETRY_SHADER, geometryCode);
	AddShader(shaderID, GL_FRAGMENT_SHADER, fragmentCode);

	CompileProgram();
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode) {
	shaderID = glCreateProgram();

	AddShader(shaderID, GL_VERTEX_SHADER, vertexCode);
	AddShader(shaderID, GL_FRAGMENT_SHADER, fragmentCode);

	CompileProgram();
}

void Shader::Validate() {
	GLint result = 0;
	GLchar message[1024] = { 0 };
	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderID, sizeof(message), NULL, message);
		printf("Couldnt validate program. %s", message);
		return;
	}
}

void Shader::CompileProgram() {
	GLint result = 0;
	GLchar message[1024] = { 0 };

	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderID, sizeof(message), NULL, message);
		printf("Linking error %s", message);
		return;
	}

	uniformModel = glGetUniformLocation(shaderID, "model");
	uniformProjection = glGetUniformLocation(shaderID, "projection");
	uniformView = glGetUniformLocation(shaderID, "view");
	uniformTextureUnit = glGetUniformLocation(shaderID, "textureIn");
	uniformDirectionalLight.uniformColor = glGetUniformLocation(shaderID, "directionalLight.base.color");
	uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.base.ambientIntensity");
	uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.base.diffuseIntensity");
	uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");
	uniformEyePos = glGetUniformLocation(shaderID, "eyePosition");
	uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
	uniformShininess = glGetUniformLocation(shaderID, "material.shininess");
	uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");
	uniformSpotLightCount = glGetUniformLocation(shaderID, "spotLightCount");

	for (size_t i = 0; i < MAX_POINT_LIGHTS; ++i) {
		char locBuf[100] = { '\0' };

		snprintf(locBuf, sizeof(locBuf), "pointLights[%d].base.color", i);
		uniformPointLight[i].uniformColor = glGetUniformLocation(shaderID, locBuf);

		snprintf(locBuf, sizeof(locBuf), "pointLights[%d].base.ambientIntensity", i);
		uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuf);

		snprintf(locBuf, sizeof(locBuf), "pointLights[%d].base.diffuseIntensity", i);
		uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuf);

		snprintf(locBuf, sizeof(locBuf), "pointLights[%d].position", i);
		uniformPointLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuf);

		snprintf(locBuf, sizeof(locBuf), "pointLights[%d].constant", i);
		uniformPointLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuf);

		snprintf(locBuf, sizeof(locBuf), "pointLights[%d].linear", i);
		uniformPointLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuf);

		snprintf(locBuf, sizeof(locBuf), "pointLights[%d].exponent", i);
		uniformPointLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuf);
	}

	for (size_t i = 0; i < MAX_SPOT_LIGHTS; ++i) {
		char locBuf[100] = { "\0" };

		snprintf(locBuf, sizeof(locBuf), "spotLights[%d].base.base.color", i);
		uniformSpotLight[i].uniformColor = glGetUniformLocation(shaderID, locBuf);

		snprintf(locBuf, sizeof(locBuf), "spotLights[%d].base.base.ambientIntensity", i);
		uniformSpotLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuf);

		snprintf(locBuf, sizeof(locBuf), "spotLights[%d].base.base.diffuseIntensity", i);
		uniformSpotLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuf);

		snprintf(locBuf, sizeof(locBuf), "spotLights[%d].base.position", i);
		uniformSpotLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuf);

		snprintf(locBuf, sizeof(locBuf), "spotLights[%d].base.constant", i);
		uniformSpotLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuf);

		snprintf(locBuf, sizeof(locBuf), "spotLights[%d].base.linear", i);
		uniformSpotLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuf);

		snprintf(locBuf, sizeof(locBuf), "spotLights[%d].base.exponent", i);
		uniformSpotLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuf);

		snprintf(locBuf, sizeof(locBuf), "spotLights[%d].direction", i);
		uniformSpotLight[i].uniformDirection = glGetUniformLocation(shaderID, locBuf);

		snprintf(locBuf, sizeof(locBuf), "spotLights[%d].edge", i);
		uniformSpotLight[i].uniformEdge = glGetUniformLocation(shaderID, locBuf);
	}

	uniformTexture = glGetUniformLocation(shaderID, "textureIn");
	uniformDirectionalLightTransform = glGetUniformLocation(shaderID, "directionalLightSpaceTransform");
	uniformDirectionalShadowMap = glGetUniformLocation(shaderID, "directionalShadowMap");

	uniformOmniLightPos = glGetUniformLocation(shaderID, "lightPos");
	uniformFarPlane = glGetUniformLocation(shaderID, "farPlane");

	for (size_t i = 0; i < 6; ++i) {
		char locBuf[100] = { '\0' };

		snprintf(locBuf, sizeof(locBuf), "lightMatrices[%d]", i);
		uniformLightMatrices[i] = glGetUniformLocation(shaderID, locBuf);
	}

	for (size_t i = 0; i < MAX_SPOT_LIGHTS + MAX_POINT_LIGHTS; ++i) {
		char locBuf[100] = { '\0' };

		snprintf(locBuf, sizeof(locBuf), "omniShadowMaps[%d].shadowMap", i);
		uniformOmniShadowMaps[i].shadowMap = glGetUniformLocation(shaderID, locBuf);

		snprintf(locBuf, sizeof(locBuf), "omniShadowMaps[%d].farPlane", i);
		uniformOmniShadowMaps[i].farPlane = glGetUniformLocation(shaderID, locBuf);
	}

	std::cout << "Programm compiled and Linked successfully ID:" << shaderID << std::endl;
}


void Shader::SetSpotLight(SpotLight* sLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset) {
	if (lightCount > MAX_SPOT_LIGHTS) lightCount = MAX_SPOT_LIGHTS;
	glUniform1i(uniformSpotLightCount, lightCount);
	for (size_t i = 0; i < lightCount; ++i) {
		sLight[i].UseLight(uniformSpotLight[i].uniformAmbientIntensity, uniformSpotLight[i].uniformColor, uniformSpotLight[i].uniformDiffuseIntensity,
			uniformSpotLight[i].uniformPosition, uniformSpotLight[i].uniformConstant, uniformSpotLight[i].uniformLinear, uniformSpotLight[i].uniformExponent,
			uniformSpotLight[i].uniformDirection, uniformSpotLight[i].uniformEdge);
		sLight[i].GetShadowMap()->Read(GL_TEXTURE0 + textureUnit + i);
		glUniform1i(uniformOmniShadowMaps[i + offset].shadowMap, textureUnit + i);
		glUniform1f(uniformOmniShadowMaps[i + offset].farPlane, sLight[i].GetFarPlane());
	}
}

void Shader::AddShader(GLuint theProgram, GLenum shaderType, const char* shaderText) {
	GLuint shader = glCreateShader(shaderType);

	GLint shaderTextLength[] = {
		strlen(shaderText)
	};

	glShaderSource(shader, 1, &shaderText, shaderTextLength);

	GLint result = 0;
	GLchar message[1024] = { 0 };
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(shader, sizeof(message), NULL, message);
		printf("Shader Compile Error %s", message);
		return;
	}

	glAttachShader(theProgram, shader);

	std::cout << "Shader compiled and Linked successfully TYPE:" << shaderType << std::endl;
}

void Shader::UseShader() {
	glUseProgram(shaderID);
}

void Shader::ClearShader() {
	if (shaderID != 0) {
		glDeleteProgram(shaderID);
	}

	uniformModel = 0;
	uniformProjection = 0;
}


void Shader::SetPointLight(PointLight* pLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset) {
	if (lightCount > MAX_POINT_LIGHTS) lightCount = MAX_POINT_LIGHTS;
	glUniform1i(uniformPointLightCount, lightCount);
	for (size_t i = 0; i < lightCount; ++i) {
		pLight[i].UseLight(uniformPointLight[i].uniformAmbientIntensity, uniformPointLight[i].uniformColor, uniformPointLight[i].uniformDiffuseIntensity, uniformPointLight[i].uniformPosition, uniformPointLight[i].uniformConstant, uniformPointLight[i].uniformLinear, uniformPointLight[i].uniformExponent);
		pLight[i].GetShadowMap()->Read(GL_TEXTURE0 + textureUnit + i);
		glUniform1i(uniformOmniShadowMaps[i + offset].shadowMap, textureUnit + i);
		glUniform1f(uniformOmniShadowMaps[i + offset].farPlane, pLight[i].GetFarPlane());
	}
}

void Shader::SetDirectionalLight(DirectionalLight* directionalLight) {
	directionalLight->UseLight(uniformDirectionalLight.uniformAmbientIntensity, uniformDirectionalLight.uniformColor, uniformDirectionalLight.uniformDiffuseIntensity, uniformDirectionalLight.uniformDirection);
}

void Shader::SetTexture(GLuint textureUnit) {
	glUniform1i(uniformTexture, textureUnit);
}

void Shader::SetDirectionalShadowMap(GLuint textureUnit) {
	glUniform1i(uniformDirectionalShadowMap, textureUnit);
}

void Shader::SetDirectionalLightTransform(glm::mat4* transform) {
	glUniformMatrix4fv(uniformDirectionalLightTransform, 1, GL_FALSE, glm::value_ptr(*transform));
}

void Shader::SetOmniLightMatrices(std::vector<glm::mat4> lightMatrices) {
	for (size_t i = 0; i < lightMatrices.size(); ++i) {
		glUniformMatrix4fv(uniformLightMatrices[i], 1, GL_FALSE, glm::value_ptr(lightMatrices[i]));
	}
}

GLuint Shader::GetModelLocation() {
	return uniformModel;
}

GLuint Shader::GetTextureUnitLocation() {
	return uniformTextureUnit;
}

GLuint Shader::GetProjectionLocation() {
	return uniformProjection;
}

GLuint Shader::GetViewLocation() {
	return uniformView;
}

GLuint Shader::GetAmbientIntensityLocation() {
	return uniformDirectionalLight.uniformAmbientIntensity;
}

GLuint Shader::GetAmbientColorLocation() {
	return uniformDirectionalLight.uniformColor;
}


GLuint Shader::GetOmniLightPos() {
	return uniformOmniLightPos;
}
GLuint Shader::GetFarPlanePos() {
	return uniformFarPlane;
}
GLuint* Shader::GetUniformLightMatrices() {
	return uniformLightMatrices;
}


GLuint Shader::GetUniformDiffuseIntensity() {
	return uniformDirectionalLight.uniformDiffuseIntensity;
}
GLuint Shader::GetUniformDirection() {
	return uniformDirectionalLight.uniformDirection;
}

GLuint Shader::GetUniformEyePosition() {
	return uniformEyePos;
}

GLuint Shader::GetUniformSpecularIntensity() {
	return uniformSpecularIntensity;
}

GLuint Shader::GetUniformShininess() {
	return uniformShininess;
}

GLuint Shader::GetUniformDirectionalLightTransform() {
	return uniformDirectionalLightTransform;
}

GLuint Shader::GetUniformDirectionalShadowMap() {
	return uniformDirectionalShadowMap;
}

GLuint Shader::GetUniformTexture() {
	return uniformTexture;
}

Shader::~Shader() {
	ClearShader();
}