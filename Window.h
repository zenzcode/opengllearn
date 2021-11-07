#pragma once
#include <iostream>
#include <glew.h>
#include <glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	inline GLfloat GetBufferWidth() {
		return bufferWidth;
	}
	inline GLfloat GetBufferHeight() {
		return bufferHeight;
	}
	int Initialise();

	inline bool GetShouldClose() { if (!mainWindow) return false;  return glfwWindowShouldClose(mainWindow); }

	inline bool* getKeys() {
		return keys;
	}

	GLfloat GetChangeX();
	GLfloat GetChangeY();

	inline GLfloat GetWidth() {
		return width;
	}

	inline GLfloat GetHeight() {
		return height;
	}

	inline void SwapBuffers() {
		if (!mainWindow) return;
		glfwSwapBuffers(mainWindow);
	}

	~Window();
private:
	GLFWwindow* mainWindow;
	GLint width, height;
	GLint bufferWidth, bufferHeight;
	bool keys[1024];
	void CreateCallBacks();
	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void handleMouse(GLFWwindow* window, double newX, double newY);
	GLfloat lastMouseX;
	GLfloat lastMouseY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMove;
};

