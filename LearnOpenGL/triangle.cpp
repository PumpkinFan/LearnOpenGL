#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// OpenGL requires us to define a vertex and fragment shader ourselves
// GLSL source code for vertex shader
const char* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";
// GLSL source code for fragment shader
const char* fragmentShaderSourceMagenta= "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);\n"
	"}\0";

const char* fragmentShaderSourceYellow = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"}\0";


int main() {

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // these tell glfw we are using 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // version 3.3 of OpenGL
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello Triangle", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// defining the vertices of a triangle
	// in OpenGL all coordinates are in 3D 
	float first_triangle[] = {
		0.0f, 0.0f, 0.0f,
		-0.5f, 0.0f, 0.0f,
		-0.25f, 0.5f, 0.0f,
	};
	float second_triangle[] = {
		0.0f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f,
		0.25f, 0.5f, 0.0f,
	}; 
	

	// defining the vertices of a rectangle composed of two triangles
	/*
	float vertices[] = {
		-0.5f, 0.5f, 0.0f, // top left
		0.5f, 0.5f, 0.0f, // top right
		0.5f, -0.5f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, // bottom left
	};
	*/
	unsigned int indices[] = {
		0, 1, 2, // first triangle
		0, 2, 3, // second triangle
	};

	// the vertex array object (VAO) stores vertex array pointer configurations
	// makes it easier to do the same object rendering more than once
	unsigned int VAOs[2];
	glGenVertexArrays(2, VAOs);

	// we manage vertex memory via vertex buffer objects (VBO)
	// these objects send many vertices to the GPU at once and 
	// store them there in memory (if possible)
	unsigned int VBOs[2]; // buffer id
	glGenBuffers(2, VBOs); // create buffer

	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]); // bind buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(first_triangle), first_triangle, GL_STATIC_DRAW); // copy vertices data to bound buffer memory
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(second_triangle), second_triangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// using an element buffer object (EBO) to store rendering objects
	// as a collection of vertex indices
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// we need to tell OpenGL how to interpret vertex data
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // takes data from VBO currently bound to GL_ARRAY_BUFFER
	//glEnableVertexAttribArray(0);

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
	unsigned int fragmentShaderMagenta;
	fragmentShaderMagenta= glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderMagenta, 1, &fragmentShaderSourceMagenta, NULL);
	glCompileShader(fragmentShaderMagenta);

	glGetShaderiv(fragmentShaderMagenta, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShaderMagenta, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	unsigned int fragmentShaderYellow;
	fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderYellow, 1, &fragmentShaderSourceYellow, NULL);
	glCompileShader(fragmentShaderYellow);

	glGetShaderiv(fragmentShaderYellow, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShaderYellow, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// we need to create a shader program which links the individual shaders
	// into a graphics pipeline
	unsigned int shaderProgram0;
	shaderProgram0 = glCreateProgram(); // initialize and return reference id
	glAttachShader(shaderProgram0, vertexShader);
	glAttachShader(shaderProgram0, fragmentShaderMagenta);
	glLinkProgram(shaderProgram0);

	// check for linking errors
	glGetProgramiv(shaderProgram0, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram0, 512, NULL, infoLog);
		std::cout << "ERROR: Failed to link shader program\n" << infoLog << std::endl;
	}

	unsigned int shaderProgram1;
	shaderProgram1 = glCreateProgram(); // initialize and return reference id
	glAttachShader(shaderProgram1, vertexShader);
	glAttachShader(shaderProgram1, fragmentShaderYellow);
	glLinkProgram(shaderProgram1);

	// check for linking errors
	glGetProgramiv(shaderProgram1, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram1, 512, NULL, infoLog);
		std::cout << "ERROR: Failed to link shader program\n" << infoLog << std::endl;
	}

	// we can now delete the shaders since they are no longer needed
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShaderMagenta);
	glDeleteShader(fragmentShaderYellow);

	while (!glfwWindowShouldClose(window)) {

		// render loop
		glClearColor(0.2f, 1.0f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram0);
		glBindVertexArray(VAOs[0]);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(shaderProgram1);
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents(); // checks for events (keyboard input, mouse movement, etc.)
	}

	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
	glDeleteProgram(shaderProgram0);
	glDeleteProgram(shaderProgram1);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
