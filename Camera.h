#pragma once
#include <glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <glfw3.h>

class Camera
{
public:
	Camera(glm::vec3 initialPosition, glm::vec3 up, GLfloat yaw, GLfloat pitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);
	void keyControl(bool* keys, GLfloat deltaTime);
	void mouseControl(GLfloat xChange, GLfloat yChange);
	glm::mat4 calculateViewMatrix();
	glm::vec3 GetCameraPosition();
	glm::vec3 GetCameraDirection();
	~Camera();

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;

	GLfloat movementSpeed;
	GLfloat turnSpeed;

	void update();
};

