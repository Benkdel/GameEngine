#include "GameApp.h"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include <glm/gtc/type_ptr.hpp>
#include <engine/lighting/Light.h>

#include <engine/importer/Importer.h>

#define TESTING 1

#if TESTING

GameApp::GameApp() {}

GameApp::~GameApp() {}

void GameApp::OnUserCreate()
{
	// Create GLFW window
	Init("Resurge", 1280, 760);
	AB_INFO("Window init succesfully");
	
	ABImp::Importer importer;

	importer.LoadData("src/game/res/models/envirorment/tree_model_1/scene.gltf");

	std::cout << "================== Meshes ======================= \n";
	for (auto& mesh : importer.m_Meshes)
	{
		std::cout << "Positions: " << mesh.attributes.Positions << "\n";
		std::cout << "Normals: "<< mesh.attributes.Normals << "\n";
		std::cout << "Texcoords: "<< mesh.attributes.Texcoords << "\n";
		std::cout << "Indices: " << mesh.indices << "\n";
		std::cout << "Materials: " << mesh.material << "\n";
		std::cout << "Mode: " << mesh.mode << "\n";

		std::cout << "------------ NEXT MESH --------------- \n";
	}

	std::cout << "\n\n================== Accesors ======================= \n";
	for (auto& accesor : importer.m_Accessors)
	{
		std::cout << "BufferView: " << accesor.bufferView << "\n";
		std::cout << "ByteOffset: " << accesor.byteOffset << "\n";
		std::cout << "ChunkSize: " << accesor.chunkSize << "\n";
		std::cout << "ComponentType: " << accesor.componentType << "\n";
		std::cout << "Count: " << accesor.count << "\n";
		std::cout << "Type: " << accesor.type << "\n";

		std::cout << "\n ------------ NEXT ACCESOR --------------- \n";
	}

	std::cout << "\n\n================== Buffer Views ======================= \n";
	for (auto& buffView : importer.m_BufferViews)
	{
		std::cout << "Buffer: " << buffView.buffer << "\n";
		std::cout << "ByteLength: " << buffView.byteLength << "\n";
		std::cout << "ByteOFfset: " << buffView.byteOffset << "\n";
		std::cout << "ByteStride: " << buffView.byteStride << "\n";
		std::cout << "Name: " << buffView.name << "\n";
		std::cout << "Target: " << buffView.target << "\n";

		std::cout << "\n ------------ NEXT BUFFER VIEW --------------- \n";
	}

	std::cout << "\n\n================== Buffers ======================= \n";
	for (auto& buffer : importer.m_Buffers)
	{
		std::cout << "ByteLength: " << buffer.byteLength << "\n";
		std::cout << "Uri: " << buffer.uri << "\n";

		std::cout << "\n ------------ NEXT BUFFER --------------- \n";
	}
}

void GameApp::OnUserUpdate()
{
	ProcessInput();
}


#else

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

	Amba::VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(2);

	ResManager::rm_VertexArrays["testVA"]->AddBuffer(ResManager::GetVBO("testVBO"), layout);

	ResManager::GenIBO(indices, 36, "testIndices");

	// shaders
	ResManager::CreateShader("src/game/res/shaders/testVS.glsl", "src/game/res/shaders/testFS.glsl", "testShader");
	ResManager::GenTexture("src/game/res/textures/wall.jpg", "testTexture");
	ResManager::GetTexture("testTexture")->LoadTexture(true);

	// create camera
	AB_Cameras[0] = Amba::Camera(glm::vec3(0.0f, 0.0f, 0.0f));
	AB_ActiveCamera = 0;
	
	// model testing
	ResManager::CreateModel("testModel");
	ResManager::GetModel("testModel")->LoadModel("src/game/res/models/envirorment/tree_model_1/scene.gltf");

	// model shaders
	ResManager::CreateShader("src/game/res/shaders/treeVS.glsl", "src/game/res/shaders/treeFS.glsl", "treeShader");

}

void GameApp::OnUserUpdate()
{
	ResManager::GetShader("testShader")->Bind();
	ResManager::GetTexture("testTexture")->Bind();
	ResManager::GetShader("testShader")->SetUniform1i("u_Texture", 0);
	
	// pass camera to renderer
	Amba::Renderer::BeginScene(AB_Cameras[0]);

	// projection matrix
	AB_Perspective = glm::perspective(glm::radians(AB_Cameras[AB_ActiveCamera].GetZoom()), (float)m_ScrWidth / (float)m_ScrHeight, 0.1f, 100.0f);

	glm::mat4 modelScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.001f));

	Amba::VertexBufferLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	layout.Push<float>(2);

	glm::vec3 pos = AB_Cameras[AB_ActiveCamera].GetCamFront();
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * modelScale;
	ResManager::GetShader("treeShader")->Bind();
	dirLight.render(ResManager::GetShader("treeShader"));
	ResManager::GetShader("treeShader")->SetUniform3f("viewPos", AB_Cameras[AB_ActiveCamera].GetCamPos());
	Amba::Renderer::DrawModel(ResManager::GetModel("testModel"), layout, ResManager::GetShader("treeShader"), AB_Perspective, transform);

	glm::mat4 cubeScale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
	transform = glm::translate(glm::mat4(1.0f), pos + glm::vec3(3.5f, 0.0f, 0.0f)) * cubeScale;
	ResManager::GetTexture("testTexture")->Bind();
	Amba::Renderer::Draw(ResManager::GetVA("testVA"), ResManager::GetIBO("testIndices"), ResManager::GetShader("testShader"), AB_Perspective, transform);

	ProcessInput();
}

#endif

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

