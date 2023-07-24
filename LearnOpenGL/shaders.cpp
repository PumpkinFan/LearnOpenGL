#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "./Shader.h"

#include <iostream>
#include <cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
int SCR_WIDTH = 1600;
int SCR_HEIGHT = 900;

// GLSL source code for vertex shader
// very simple shader - just outputs the vertices with no transformation
//const char* vertexShaderSource = "#version 330 core\n"
//	"layout (location = 0) in vec3 aPos;\n" // give vertex positions the attribute position 0
//	"layout (location = 1) in vec3 aColor;\n" // gives vertex colors attribute position 1
//	"out vec3 ourColor;\n" // output ourColor to the fragment shader
//	"void main()\n"
//	"{\n"
//	"   gl_Position = vec4(aPos, 1.0);\n" // we can directly plug a vec3 (aPos here) into a vec4 constructor
//	"	ourColor = aColor;\n"
//	"}\0";

// GLSL source code for fragment shader
//const char* fragmentShaderSource = "#version 330 core\n"
//	"out vec4 FragColor;\n"
//	"in vec3 ourColor;\n"
//	//"uniform vec4 ourColor;\n" // use a uniform to access values from cpp code 
//	"void main()\n"
//	"{\n"
//	"   FragColor = vec4(ourColor, 1.0);\n"
//	"}\n\0";

int main() {

	/*
	float triangle[] = {
		0.0f, 0.5f, 0.0f,
		-0.5f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f,
	};
	*/

	// adding color data to vertices
	float triangle[] = {
		// positions        // colors
		0.0f,  0.5f, 0.0f,  0.0f, 1.0f, 1.0f,
	   -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 0.0f,
	};

	// initialize glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // these tell glfw we are using 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // version 3.3 of OpenGL
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create a window to work with
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello Window", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// load GL functions
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

	// position pointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // stride =  6 floats because each vertex is now 6 floats long
	glEnableVertexAttribArray(0);
	// color pointer
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // offset of 3 floats for the color data
	glEnableVertexAttribArray(1);

	/*
	// compiling the vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER); // creates shader object "behind-the-scenes" and returns id for later use
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// checking for compile-time errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// compiling the fragment shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	*/
	Shader ourShader("./vertexShader.vs", "./fragmentShader.fs");

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.5f, 0.5f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//glUseProgram(shaderProgram);
		ourShader.use();

		// updating uniform value
		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0 + 0.5);
		float blueValue = (cos(timeValue) / 2.0 + 0.5);
		//int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor"); // "find" the uniform in our shader
		//glUniform4f(vertexColorLocation, 0.0f, greenValue, blueValue, 1.0f); // set the value of the uniform vec4 of floats

		// rendering the triangle
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// deallocate resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteProgram(shaderProgram);
	glDeleteProgram(ourShader.ID);
	glfwDestroyWindow(window);
	glfwTerminate();
}

// callback function to handle resizing of the window
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
