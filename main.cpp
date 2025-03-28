#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "ShaderClass.hpp"
#include "VBO.hpp"
#include "VAO.hpp"
#include "EBO.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

//Sets triangles points on screen
GLfloat vertices[] = {
   -0.5f,  0.5f, 0.0f,		0.0f, 1.0f, //UPPER LEFT
   -0.5f, -0.5f, 0.0f,		0.0f, 0.0f, //BOTTOM LEFT
	0.5f, -0.5f, 0.0f,		1.0f, 0.0f, //BOTTOM RIGHT
	0.5f,  0.5f, 0.0f,		1.0f, 1.0f  //UPPER RIGHT
};

GLuint indices[] = {
	0, 1, 2,
	0, 2, 3
};

int main() {
	//INITIALIZE OPENGL VERSION 4.6 
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Set windows to a 800x800 square
	GLFWwindow* window = glfwCreateWindow(800, 800, "Pong", NULL, NULL);

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Checks if window was created
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Loads and checks for proper loading of GLAD
	gladLoadGL();
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//Generates the shader object
	Shader ShaderProgram("default.vert", "default.frag");

	//Generates Vertex Array Object and binds it
	VAO VAO_1;
	VAO_1.Bind();

	//Generates Vertex Buffer Object and binds it
	VBO VBO_1(vertices, sizeof(vertices));

	//Generates Element Buffer Object and binds it
	EBO EBO_1(indices, sizeof(indices));

	VAO_1.LinkVBO(VBO_1, 0);

	//Unbind all previous objects
	VAO_1.Unbind();
	VBO_1.Unbind();
	EBO_1.Unbind();

	//Texture

	int width_img, height_img, col_channels;
	unsigned char* bytes = stbi_load("perry.jpg", &width_img, &height_img, &col_channels, 0);

	GLuint texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_img, height_img, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);

	stbi_image_free(bytes);
	glBindTexture(GL_TEXTURE_2D, 0);
	glGenerateMipmap(GL_TEXTURE_2D);

	//Main program loop
	while (!glfwWindowShouldClose(window)) {
		//INPUT
		processInput(window);

		//RENDERING
		glClearColor(1.0f, 0.95f, 0.95f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		float redValue = (sin(timeValue) / 2.0f) + 0.5f;

		int vertexColorLocation = glGetUniformLocation(ShaderProgram.ID, "triangleColor");

		//Sets the shader program to the previous one made
		ShaderProgram.Activate();

		glUniform4f(vertexColorLocation, redValue, greenValue, 0.0f, 1.0f);

		VAO_1.Bind();

		//Draws the triangle
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(VAO_1.ID);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//Check and call events & swap buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	//Properly deletes stuff in memory and exits program
	VAO_1.Delete();
	VBO_1.Delete();
	EBO_1.Delete();
	ShaderProgram.Delete();

	glDeleteTextures(1, &texture);

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
}