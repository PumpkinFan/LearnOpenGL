#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void _framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 900;

int _main() {

	glfwInit();

	// configuring glfw window settings
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // these tell glfw we are using 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // version 3.3 of OpenGL
	// OpenGL profile is the collection of functions we use
	// only two options 'CORE' (only modern functions) and 'COMPAT' (modern + old functions)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// creating a new window
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello Window", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// we have to tell glfw we want to use this window 
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, _framebuffer_size_callback);

	// check glad loaded all OpenGL functions
	// make sure to create context before doing this!
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		// rendering commands
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents(); // checks for events (keyboard input, mouse movement, etc.)
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window) {

	// glfwGetKey checks if the specified key is being pressed 
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

// callback function to handle resizing of the window
void _framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
