#include "GameApp.h"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>


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

GameApp::~GameApp()
{
	ResManager::cleanup();
}

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
	ResManager::CreateShader("src/game/res/shaders/testVS.glsl", "src/game/res/shaders/testFS.glsl", "testShader");

	ResManager::GetVA("testVA")->Unbind();
	ResManager::GetShader("testShader")->Unbind();
}


void GameApp::OnUserUpdate()
{
	float greenValue = (glm::sin(AB_timeElapsed) / 2.0f) + 0.5f;
 
	ResManager::GetShader("testShader")->Bind();
	
	ResManager::GetVA("testVA")->Bind();
	ResManager::GetIBO("testIndices")->Bind();

	ResManager::GetShader("testShader")->SetUniform1f("u_Green", greenValue);
	
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