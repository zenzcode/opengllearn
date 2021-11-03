#include "Window.h"
#include "glfw3.h"

Window::Window() {
	width = 800;
	height = 600;
	xChange = 0.f;
	yChange = 0.f;

	for (size_t i = 0; i < 1024; ++i) {
		keys[i] = false;
	}
}

Window::Window(GLint windowWidth, GLint windowHeight) {
	width = windowWidth;
	height = windowHeight;
	xChange = 0;
	yChange = 0;

	for (size_t i = 0; i < 1024; ++i) {
		keys[i] = false;
	}
}

int Window::Initialise() {
	if (!glfwInit()) {
		std::cerr << "GLFW could not be initialized. Aborting..." << std::endl;
		glfwTerminate();
		return -1;
	}

	mainWindow = glfwCreateWindow(width, height, "OpenGL Tests", NULL, NULL);

	if (!mainWindow) {
		std::cerr << "Window could not be created. Aborting..." << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwWindowHint(GLFW_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
	glViewport(0, 0, bufferWidth, bufferHeight);
	glfwMakeContextCurrent(mainWindow);
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);


	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cerr << "Error initializing GLEW. Aborting..." << std::endl;
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return -1;
	}

	glfwSetWindowUserPointer(mainWindow, this);
	CreateCallBacks();

	return 0;
}

GLfloat Window::GetChangeX() {
	auto theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::GetChangeY() {
	auto theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

Window::~Window() {
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}

void Window::CreateCallBacks() {
	glfwSetKeyCallback(mainWindow, handleKeys);
	glfwSetCursorPosCallback(mainWindow, handleMouse);
}

void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode) {
	Window* w = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) {
			w->keys[key] = true;
		}
		else if (action == GLFW_RELEASE) {
			w->keys[key] = false;
		}
	}
}

void Window::handleMouse(GLFWwindow* window, double newX, double newY){
	Window* w = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (w->mouseFirstMove) {
		w->lastMouseX = newX;
		w->lastMouseY = newY;
		w->mouseFirstMove = false;
		w->xChange = 0;
		w->yChange = 0;
	}

	w->xChange = w->lastMouseX - newX;
	w->yChange = newY - w->lastMouseY;
	w->lastMouseX = newX;
	w->lastMouseY = newY;
}