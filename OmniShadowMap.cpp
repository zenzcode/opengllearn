#include "OmniShadowMap.h"

OmniShadowMap::OmniShadowMap() : ShadowMap() {

}

bool OmniShadowMap::Init(GLuint width, GLuint height) {
	std::cout << "Omni Init" << std::endl;
	shadowWidth = width;
	shadowHeight = height;

	glGenFramebuffers(1, &FBO);
	glGenTextures(1, &shadowMapID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMapID);

	for (size_t i = 0; i < 6; ++i) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	}

	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowMapID, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Framebuffer status is " << status << std::endl;
		return false;
	}

	return true;
}

void OmniShadowMap::Read(GLenum textureUnit) {
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMapID);
}

void OmniShadowMap::Write() {
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

OmniShadowMap::~OmniShadowMap() {

}