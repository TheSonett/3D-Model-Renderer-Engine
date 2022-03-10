#include "Window.h"


Window::Window(int windowWidth, int windowHeight, const char* title)
	:windowWidth(windowWidth), windowHeight(windowHeight), title(title), bufferWidth(0), bufferHeight(0), window(nullptr), lastX(0.0f), lastY(0.0f),
	xChange(0.0f), yChange(0.0f), isFirstMove(true)
{
	for (int i = 0; i < MAX_KEYS; i++) {
		m_Keys[i] = false;
	}

	for (int i = 0; i < MAX_BUTTONS; i++) {
		m_Buttons[i] = false;
	}
}

int Window::Initialize() {
	if (glfwInit() == GLFW_FALSE) {
		std::cout << "Failed to initialize GLFW!" << std::endl;
		glfwTerminate();
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	window = glfwCreateWindow(windowWidth, windowHeight, title, NULL, NULL);
	if (!window)
	{
		printf("Error creating GLFW window!");
		glfwTerminate();
		return 1;
	}

	glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glfwSwapInterval(1);

	// ImGui initialization
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));

	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW!" << std::endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, bufferWidth, bufferHeight);

	// For Input Events
	glfwSetWindowUserPointer(window, this);

	std::cout << glGetString(GL_VERSION) << std::endl;
	return 0;
}
// Handling Keyboard + Mouse + Error Events + Call Backs

GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

void Window::HandleKeys(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < MAX_KEYS) {
		if (action == GLFW_PRESS) {
			win->m_Keys[key] = true;
			//std::cout << "Pressed: " << key << std::endl;
		}
		else if (action == GLFW_RELEASE) {
			win->m_Keys[key] = false;
			//std::cout << "Released: " << key << std::endl;
		}
	}
}

void Window::HandleButtons(GLFWwindow* window, int button, int action, int mods) {
	Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (button >= 0 && button < MAX_BUTTONS) {
		if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT) {
			win->m_Buttons[button] = true;
			std::cout << "Left Button Pressed: " << button << std::endl;
		}
		else if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_RIGHT) {
			win->m_Buttons[button] = true;
			std::cout << "Right Button Pressed: " << button << std::endl;
		}
		else if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_MIDDLE) {
			win->m_Buttons[button] = true;
			std::cout << "Middle Button Pressed: " << button << std::endl;
		}
		else {
			win->m_Buttons[button] = false;
		}
	}
}

void Window::HandleMousePositions(GLFWwindow* window, double xpos, double ypos)
{
	Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (win->isFirstMove) {
		win->lastX = xpos;
		win->lastY = ypos;
		win->isFirstMove = false;
	}

	win->xChange = xpos - win->lastX;
	win->yChange = ypos - win->lastY;

	win->lastX = xpos;
	win->lastY = ypos;

	//std::cout << "x : " << win->xChange << ", y: " << win->yChange << std::endl;
}

void Window::getMousePosition(double& x, double& y) const {
	x = xChange;
	y = yChange;
}

void Window::HandleWindowError(int error, const char* description) {
	std::cout << "Error: " << description << std::endl;
}

void Window::WindowCallBacks() {
	glfwSetErrorCallback(HandleWindowError);
	glfwSetKeyCallback(window, HandleKeys);
	glfwSetMouseButtonCallback(window, HandleButtons);
	glfwSetCursorPosCallback(window, HandleMousePositions);
}

Window::~Window() {
	glfwDestroyWindow(window);
	glfwTerminate();
}