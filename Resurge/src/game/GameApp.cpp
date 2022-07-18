#include "GameApp.h"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include <glm/gtc/type_ptr.hpp>

float positions[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

unsigned int indices[] = {
	0, 1, 2, 3, 4, 5, 6, 
	7, 8, 9, 10, 11, 12,
	13, 14, 15, 16, 17, 18,
	19, 20, 21, 22, 23, 24,
	25, 26, 27, 28, 29, 30,
	31, 32, 33, 34, 35
};

GameApp::GameApp()
	: AB_ActiveCamera(0), AB_State(GameState::NONE)
{
}

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
	ResManager::GenVBO(positions, sizeof(positions), "testVBO");

	VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(2);

	ResManager::rm_VertexArrays["testVA"]->AddBuffer(ResManager::GetVBO("testVBO"), layout);

	ResManager::GenIBO(indices, 36, "testIndices");

	// shaders
	ResManager::CreateShader("src/game/res/shaders/testVS.glsl", "src/game/res/shaders/testFS.glsl", "testShader");
	ResManager::LoadTexture("src/game/res/textures/wall.jpg", "testTexture");

	// create camera
	AB_Cameras[0] = Camera(glm::vec3(0.0f, 0.0f, 0.0f));

	// projection matrix
	AB_Projection = glm::perspective(glm::radians(AB_Cameras[0].GetZoom()), (float)m_ScrWidth / (float)m_ScrHeight, 0.1f, 100.0f);
	
}

void GameApp::OnUserUpdate()
{
	ResManager::GetShader("testShader")->Bind();
	ResManager::GetTexture("testTexture")->Bind();
	ResManager::GetShader("testShader")->SetUniform1i("u_Texture", 0);
	
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));

	glm::mat4 MVP = AB_Projection * AB_Cameras[0].GetViewMatrix() * model;

	ResManager::GetShader("testShader")->SetUniform4mat("u_MVP", MVP);

	Renderer::Draw(ResManager::GetVA("testVA"), ResManager::GetIBO("testIndices"), ResManager::GetShader("testShader"));
	
	ProcessInput();
}

void GameApp::ProcessInput()
{
	if (KeyBoard::KeyWentDown(GLFW_KEY_ESCAPE))
		SetWindowShouldClose();

	// Camera movements
	if (KeyBoard::Key(GLFW_KEY_W))
	{
		AB_Cameras[AB_ActiveCamera].UpdateCameraPos(CameraMotion::FORWARD, AB_DeltaTime);
	}

	if (KeyBoard::Key(GLFW_KEY_W))
	{
		AB_Cameras[AB_ActiveCamera].UpdateCameraPos(CameraMotion::FORWARD, AB_DeltaTime);
	}

	if (KeyBoard::Key(GLFW_KEY_S))
	{
		AB_Cameras[AB_ActiveCamera].UpdateCameraPos(CameraMotion::BACKWARD, AB_DeltaTime);
	}

	if (KeyBoard::Key(GLFW_KEY_D))
	{
		AB_Cameras[AB_ActiveCamera].UpdateCameraPos(CameraMotion::RIGHT, AB_DeltaTime);
	}

	if (KeyBoard::Key(GLFW_KEY_A))
	{
		AB_Cameras[AB_ActiveCamera].UpdateCameraPos(CameraMotion::LEFT, AB_DeltaTime);
	}

	if (KeyBoard::Key(GLFW_KEY_SPACE))
	{
		AB_Cameras[AB_ActiveCamera].UpdateCameraPos(CameraMotion::FORWARD, AB_DeltaTime);
	}

	// mouse movement
	double dx = Mouse::GetDX();
	double dy = Mouse::GetDY();
	if (dx != 0 || dy != 0)
	{
		AB_Cameras[AB_ActiveCamera].UpdateCameraDirection(dx, dy);
	}

	// zoom
	double scrollDy = Mouse::GetScrollDY();
	if (scrollDy != 0)
	{
		AB_Cameras[AB_ActiveCamera].UpdateCameraZoom(scrollDy);
	}
}
