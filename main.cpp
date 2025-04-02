#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>	


#include "ShaderClass.hpp"
#include "VBO.hpp"
#include "VAO.hpp"
#include "EBO.hpp"
#include "Texture.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

GLuint SCREEN_WIDTH = 800;
GLuint SCREEN_HEIGHT = 800;

//Sets triangles points on screen
GLfloat vertices[] = {
   -0.5f,   0.0f,	0.5f,	0.0f, 0.0f, //FRONT LEFT
	0.5f,   0.0f,	0.5f,	1.0f, 0.0f, //FRONT RIGHT
   -0.5f,   0.0f,  -0.5f,	1.0f, 0.0f, //BACK LEFT
	0.5f,	0.0f,  -0.5f,	0.0f, 0.0f, //BACK RIGHT
	0.0f,	0.8f,   0.0f,	0.5f, 1.0f  //TOP
};

GLuint indices[] = {
	0, 1, 2,
	1, 3, 2,
	2, 0, 4,
	2, 3, 4,
	0, 1, 4,
	1, 3, 4
};

int main() {
	//INITIALIZE OPENGL VERSION 4.6 
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Set windows to a 800x800 square
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pong", NULL, NULL);

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

	VAO_1.LinkAttrib(VBO_1, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
	VAO_1.LinkAttrib(VBO_1, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	//Unbind all previous objects
	VAO_1.Unbind();
	VBO_1.Unbind();
	EBO_1.Unbind();

	//Texture
	Texture PerryTexture("perry.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
	PerryTexture.textureUnit(ShaderProgram, "tex0", 0);


	float rotation = 0.0f;
	double previous_time = glfwGetTime();
	
	glEnable(GL_DEPTH_TEST);

	//Main program loop
	while (!glfwWindowShouldClose(window)) {
		//INPUT
		processInput(window);

		//RENDERING
		glClearColor(1.0f, 0.95f, 0.95f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		float redValue = (sin(timeValue) / 2.0f) + 0.5f;

		int vertexColorLocation = glGetUniformLocation(ShaderProgram.ID, "triangleColor");

		//Sets the shader program to the previous one made
		ShaderProgram.Activate();

		double current_time = glfwGetTime();
		if (current_time - previous_time >= 0.1f / 60.0f) {
			rotation += 0.5f;
			previous_time = current_time;
		}

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));
		projection = glm::perspective(glm::radians(45.0f), (float)(SCREEN_WIDTH / SCREEN_HEIGHT), 0.1f, 100.0f);

		int model_location = glGetUniformLocation(ShaderProgram.ID, "model");
		glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));

		int view_location = glGetUniformLocation(ShaderProgram.ID, "view");
		glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));
		
		int projection_location = glGetUniformLocation(ShaderProgram.ID, "projection");
		glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));

		glUniform4f(vertexColorLocation, redValue, greenValue, 0.0f, 1.0f);

		VAO_1.Bind();

		//Draws the triangle
		PerryTexture.Bind();
		glBindVertexArray(VAO_1.ID);
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);

		//Check and call events & swap buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	//Properly deletes stuff in memory and exits program
	VAO_1.Delete();
	VBO_1.Delete();
	EBO_1.Delete();
	ShaderProgram.Delete();

	PerryTexture.Delete();

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