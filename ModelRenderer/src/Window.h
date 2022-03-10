#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>


#define MAX_KEYS 1024
#define MAX_BUTTONS 32

class Window {
public:
	Window(int windowWidth, int windowHeight, const char* title);
	~Window();
public:
	int Initialize();
	void WindowCallBacks();

	int getBufferWidth() { return bufferWidth; }
	int getBufferHeight() { return bufferHeight; }
	bool getShouldClose() { return glfwWindowShouldClose(window); }
	void swapBuffers() { glfwSwapBuffers(window); }

	void getMousePosition(double& x, double& y) const;
	bool* getKeys() { return m_Keys; }
	GLfloat getXChange();
	GLfloat getYChange();

private:
	static void HandleKeys(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void HandleButtons(GLFWwindow* window, int button, int action, int mods);
	static void HandleMousePositions(GLFWwindow* window, double xpos, double ypos);
	static void HandleWindowError(int error, const char* description);

private:
	GLFWwindow* window;
	int windowWidth, windowHeight;
	int bufferWidth, bufferHeight;
	const char* title;

	bool m_Keys[MAX_KEYS];

	float lastX, lastY;
	float xChange, yChange;
	bool isFirstMove;
	bool m_Buttons[MAX_BUTTONS];
};