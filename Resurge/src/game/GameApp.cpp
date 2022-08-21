#include "GameApp.h"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include <glm/gtc/type_ptr.hpp>
#include <engine/lighting/Light.h>

#include <engine/importer/Importer.h>


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

	ResManager::CreateModel("testImporter");
	ResManager::GetModel("testImporter")->LoadModel("src/game/res/models/envirorment/tree_model_1/scene.gltf");
	//ResManager::GetModel("testImporter")->LoadModel("src/game/res/models/testmodels/textured_cube/scene.gltf");

	ResManager::CreateShader("src/game/res/shaders/treeVS.glsl", "src/game/res/shaders/treeFS.glsl", "testImporter");
}

void GameApp::OnUserUpdate()
{
	// pass camera to renderer
	Amba::Renderer::BeginScene(AB_Cameras[0]);

	// projection matrix
	AB_Perspective = glm::perspective(glm::radians(AB_Cameras[AB_ActiveCamera].GetZoom()), (float)m_ScrWidth / (float)m_ScrHeight, 0.1f, 100.0f);
	
	Amba::VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	layout.Push<float>(2);

	ResManager::GetShader("testImporter")->Bind();
	Amba::Renderer::DrawModel(ResManager::GetModel("testImporter"), layout, ResManager::GetShader("testImporter"), AB_Perspective);

	ProcessInput();
}

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

unsigned int indices[36] = {
	0, 1, 2, 3, 4, 5, 6, 
	7, 8, 9, 10, 11, 12,
	13, 14, 15, 16, 17, 18,
	19, 20, 21, 22, 23, 24,
	25, 26, 27, 28, 29, 30,
	31, 32, 33, 34, 35
};

// Dir Light for current scene
Amba::DirLight dirLight = {
	glm::vec3(-0.2f, -1.0f, -0.3f), // position
	glm::vec4(0.1f, 0.1f, 0.1f, 1.0f), // ambient
	glm::vec4(0.4f, 0.4f, 0.4f, 1.0f), // diffuse
	glm::vec4(0.5f, 0.5f, 0.5f, 1.0f) // specular
};


void GameApp::ProcessInput()
{
	if (Amba::KeyBoard::KeyWentDown(GLFW_KEY_ESCAPE))
		SetWindowShouldClose();

	// Camera movements
	if (Amba::KeyBoard::Key(GLFW_KEY_W))
	{
		AB_Cameras[AB_ActiveCamera].UpdateCameraPos(Amba::CameraMotion::FORWARD, AB_DeltaTime);
	}

	if (Amba::KeyBoard::Key(GLFW_KEY_W))
	{
		AB_Cameras[AB_ActiveCamera].UpdateCameraPos(Amba::CameraMotion::FORWARD, AB_DeltaTime);
	}

	if (Amba::KeyBoard::Key(GLFW_KEY_S))
	{
		AB_Cameras[AB_ActiveCamera].UpdateCameraPos(Amba::CameraMotion::BACKWARD, AB_DeltaTime);
	}

	if (Amba::KeyBoard::Key(GLFW_KEY_D))
	{
		AB_Cameras[AB_ActiveCamera].UpdateCameraPos(Amba::CameraMotion::RIGHT, AB_DeltaTime);
	}

	if (Amba::KeyBoard::Key(GLFW_KEY_A))
	{
		AB_Cameras[AB_ActiveCamera].UpdateCameraPos(Amba::CameraMotion::LEFT, AB_DeltaTime);
	}

	if (Amba::KeyBoard::Key(GLFW_KEY_SPACE))
	{
		AB_Cameras[AB_ActiveCamera].UpdateCameraPos(Amba::CameraMotion::UP, AB_DeltaTime);
	}

	if (Amba::KeyBoard::Key(GLFW_KEY_LEFT_CONTROL))
	{
		AB_Cameras[AB_ActiveCamera].UpdateCameraPos(Amba::CameraMotion::DOWN, AB_DeltaTime);
	}

	// mouse movement
	double dx = Amba::Mouse::GetDX();
	double dy = Amba::Mouse::GetDY();
	if (dx != 0 || dy != 0)
	{
		AB_Cameras[AB_ActiveCamera].UpdateCameraDirection(dx, dy);
	}

	// zoom
	double scrollDy = Amba::Mouse::GetScrollDY();
	if (scrollDy != 0)
	{
		AB_Cameras[AB_ActiveCamera].UpdateCameraZoom(scrollDy);
	}
}

