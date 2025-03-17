#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "ShaderClass.hpp"
#include "VBO.hpp"
#include "VAO.hpp"
#include "EBO.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

//Sets triangles points on screen
GLfloat vertices[] = {
	0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // TOP
	-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, //MIDDLE LEFT
	0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, //MIDDLE RIGHT
	-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, //BOTTOM LEFT
	0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f, // BOTTOM MIDDLE
	0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f //BOTTOM RIGHT
};

GLuint indices[] = {
	0, 1, 2,
	1, 3, 4,
	2, 4, 5
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

	//Main program loop
	while (!glfwWindowShouldClose(window)) {
		//INPUT
		processInput(window);

		//RENDERING
		glClearColor(1.0f, 0.95f, 0.95f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		float redValue = (cos(timeValue) / 2.0f) + 0.5f;
		//float blueValue = (sin(timeValue) / 2.0f) + 0.5f;
		float blueValue = 0.0f;

		int vertexColorLocation = glGetUniformLocation(ShaderProgram.ID, "triangleColor");

		//Sets the shader program to the previous one made
		ShaderProgram.Activate();

		glUniform4f(vertexColorLocation, redValue, greenValue, blueValue, 1.0f);

		VAO_1.Bind();

		//Draws the triangle
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

		//Check and call events & swap buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	//Properly deletes stuff in memory and exits program
	VAO_1.Delete();
	VBO_1.Delete();
	EBO_1.Delete();
	ShaderProgram.Delete();

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