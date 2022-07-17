#include "GameApp.h"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform float green;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.5f, green, 0.2f, 1.0f);\n"
"}\0";

unsigned int shaderProgram;
unsigned int VBO;

int success;
char infoLog[512];

float positions[8] = {
	-0.5f, -0.5f, // 0
	 0.5f, -0.5f, // 1
	 0.5f,  0.5f, // 2
	-0.5f, 0.5f // 3
};

unsigned int indices[6] = {
	0, 1, 2,
	2, 3, 0
};

void GameApp::OnUserCreate()
{
	// Create GLFW window
	Init("Resurge", 1280, 760);
	AB_INFO("Window init succesfully");

	ResManager::GenVA("testVA");
	ResManager::GenVBO(positions, 4 * 2 * sizeof(float), "testVBO");

	VertexBufferLayout layout;
	layout.Push<float>(2);

	ResManager::rm_VertexArrays["testVA"]->AddBuffer(ResManager::GetVBO("testVBO"), layout);

	ResManager::GenIBO(indices, 6, "testIndices");

	// shaders
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	AB_ASSERT(success, "Vertex Shader compilation failed");

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	AB_ASSERT(success, "Fragment Shader compilation failed");

	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);
	
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		AB_ERROR("SHADER PROGRAM FAILED {0}", infoLog);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	AB_INFO("VBO AND SHADERS CREATED -- CHECK ERRORS");

}


void GameApp::OnUserUpdate()
{
	float greenValue = (glm::sin(AB_timeElapsed) / 2.0f) + 0.5f;
	int vertexColorLocation = glGetUniformLocation(shaderProgram, "green");
 
	glUseProgram(shaderProgram);
	
	glUniform1f(vertexColorLocation, greenValue);

	ResManager::GetVA("testVA")->Bind();
	ResManager::GetIBO("testIndices")->Bind();

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	ProcessInput();
}

void GameApp::ProcessInput()
{

	if (KeyBoard::KeyWentDown(GLFW_KEY_ESCAPE))
		SetWindowShouldClose();

	if (Mouse::ButtonChanged(GLFW_MOUSE_BUTTON_1))
		std::cout << "Shoot!\n";

	if (KeyBoard::KeyChanged(GLFW_KEY_W))
		std::cout << "Forward!\n";

}