#include "Shader.h"

Shader::Shader() {
	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;
	uniformView = 0;
	pointLightCount = 0;
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode) {
	CompileShader(vertexCode, fragmentCode);
}


void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation) {
	std::string vertexString = ReadFile(vertexLocation);
	std::string fragmentString = ReadFile(fragmentLocation);
	CompileShader(vertexString.c_str(), fragmentString.c_str());
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

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode) {
	shaderID = glCreateProgram();

	AddShader(shaderID, GL_VERTEX_SHADER, vertexCode);
	AddShader(shaderID, GL_FRAGMENT_SHADER, fragmentCode);

	GLint result = 0;
	GLchar message[1024] = { 0 };

	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderID, sizeof(message), NULL, message);
		printf("Linking error %s", message);
		return;
	}

	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderID, sizeof(message), NULL, message);
		printf("Couldnt validate program. %s", message);
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

	std::cout << "Programm compiled and Linked successfully ID:" << shaderID << std::endl;
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


void Shader::SetPointLight(PointLight* pLight, unsigned int lightCount) {
	if (lightCount > MAX_POINT_LIGHTS) lightCount = MAX_POINT_LIGHTS;
	glUniform1i(uniformPointLightCount, lightCount);
	pointLightCount = lightCount;
	for (size_t i = 0; i < lightCount; ++i) {
		pLight[i].UseLight(uniformPointLight[i].uniformAmbientIntensity, uniformPointLight[i].uniformColor, uniformPointLight[i].uniformDiffuseIntensity, uniformPointLight[i].uniformPosition, uniformPointLight[i].uniformConstant, uniformPointLight[i].uniformLinear, uniformPointLight[i].uniformExponent);
	}
}

void Shader::SetDirectionalLight(DirectionalLight* directionalLight) {
	directionalLight->UseLight(uniformDirectionalLight.uniformAmbientIntensity, uniformDirectionalLight.uniformColor, uniformDirectionalLight.uniformDiffuseIntensity, uniformDirectionalLight.uniformDirection);
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

Shader::~Shader() {
	ClearShader();
}