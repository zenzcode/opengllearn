#include "Camera.h"

Camera::Camera(glm::vec3 initialPosition, glm::vec3 up, GLfloat yaw, GLfloat pitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed) : position(initialPosition), worldUp(up), yaw(yaw), pitch(pitch), movementSpeed(startMoveSpeed), turnSpeed(startTurnSpeed) {
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	update();
}

Camera::~Camera() {

}

void Camera::update() {
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}


void Camera::mouseControl(GLfloat xChange, GLfloat yChange) {
	xChange *= turnSpeed;
	yChange *= turnSpeed;
	yaw += -xChange;
	pitch += -yChange;

	pitch = glm::clamp(pitch, -89.f, 89.f);
	update();
}

glm::vec3 Camera::GetCameraPosition() {
	return position;
}

void Camera::keyControl(bool* keys, GLfloat deltaTime) {
	if (keys[GLFW_KEY_W]) {
		position += (front * movementSpeed) * deltaTime;
	}

	if (keys[GLFW_KEY_S]) {
		position -= (front * movementSpeed) * deltaTime;
	}

	if (keys[GLFW_KEY_D]) {
		position += (right * movementSpeed) * deltaTime;
	}

	if (keys[GLFW_KEY_A]) {
		position -= (right * movementSpeed) * deltaTime;
	}
}

glm::mat4 Camera::calculateViewMatrix() {
	return glm::lookAt(position, position + front, worldUp);
}