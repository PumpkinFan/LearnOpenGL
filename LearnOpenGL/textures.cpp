#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "./Shader.h"
#include "stb_image.h"

#include <iostream>
#include <cmath>

void texFramebuffer_size_callback(GLFWwindow* window, int width, int height);
void texProcessInput(GLFWwindow* window);
int texSCR_WIDTH = 1600;
int tesSCR_HEIGHT = 900;
float MIX_AMT = 0.2;

int texMain() {
	
	// defining the vertices of a rectangle composed of two triangles
	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};

	// Exercise 3
	//float vertices[] = {
	//	// positions          // colors           // texture coords
	//	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.55, 0.55f,   // top right
	//	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   0.55f, 0.45f,   // bottom right
	//	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.45f, 0.45f,   // bottom left
	//	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.45f, 0.55f    // top left 
	//};

	unsigned int indices[] = {
		0, 1, 2, // first triangle
		0, 2, 3, // second triangle
	};

	// initialize glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // these tell glfw we are using 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // version 3.3 of OpenGL
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create a window to work with
	GLFWwindow* window = glfwCreateWindow(texSCR_WIDTH, tesSCR_HEIGHT, "Hello Window", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, texFramebuffer_size_callback);

	// load GL functions
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// create textures!
	unsigned int textures[2]; // reference IDs
	glGenTextures(2, textures);

	glActiveTexture(GL_TEXTURE0); // activate texture unit (optional if only using 1 texture)
	glBindTexture(GL_TEXTURE_2D, textures[0]);

	// set behavior of textures when coordinates extend beyond (0,0) to (1,1)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // horizontal direction
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // vertical direction
	// specifying texture filtering 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // when scaling down
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // when scaling up

	stbi_set_flip_vertically_on_load(true); // due to differences in image vs OpenGL coordinates
	// load in container texture
	int width, height, nrChannels;
	unsigned char* containerData = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (!containerData) {
		std::cout << "Failed to load container texture data" << std::endl;
		return -1;
	}

	// generate texture and mipmap from loaded image
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, containerData);
	// mipmaps are collections of the same texture at different size
	// this lets OpenGL use the smaller version of the same texture when an object is further away
	// switching between mipmaps can cause artifacts - different filtering methods just like normal textures
	glGenerateMipmap(GL_TEXTURE_2D);

	// good practice to free image memory after this
	stbi_image_free(containerData);

	// load in face texture
	unsigned char* faceData = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	if (!faceData) {
		std::cout << "Failed to load face texture data" << std::endl;
		return -1;
	}
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[1]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // horizontal direction
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // vertical direction
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // when scaling down
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // when scaling up

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, faceData);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(faceData);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position pointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // stride = 6 floats because each vertex is 6 floats long
	glEnableVertexAttribArray(0);
	// color pointer
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // offset of 3 floats for the color data
	glEnableVertexAttribArray(1);
	// texture pointer
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // offset of 3 floats for the color data
	glEnableVertexAttribArray(2);

	Shader ourShader("./vertexShader.glsl", "./fragmentShader.glsl");

	while (!glfwWindowShouldClose(window)) {
		texProcessInput(window);
		glClearColor(1.0f, 0.65f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//glUseProgram(shaderProgram);
		ourShader.use();
		ourShader.setInt("texture0", 0);
		ourShader.setInt("texture1", 1);

		ourShader.setFloat("mixAmt", MIX_AMT);

		// rendering the triangle
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glBindTexture(GL_TEXTURE_2D, textures[1]);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// deallocate resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(ourShader.ID);
	glfwDestroyWindow(window);
	glfwTerminate();
}

// callback function to handle resizing of the window
void texFramebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void texProcessInput(GLFWwindow* window) {

	// glfwGetKey checks if the specified key is being pressed 
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		MIX_AMT += 0.01;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		MIX_AMT -= 0.01;
	}
}
