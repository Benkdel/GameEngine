#include "GameApp.h"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include <glm/gtc/type_ptr.hpp>
#include <engine/lighting/Light.h>

#include <engine/importer/Importer.h>

#include <engine/ecs/Scene.h>

unsigned int cubemapTexture;

// used for the skymap
float positions[] = {
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};


GameApp::GameApp()
	: AB_State(GameState::NONE)
{
}

GameApp::~GameApp()
{
	ResManager::cleanup();
}

Amba::Cube* cube;

void GameApp::OnUserCreate()
{
	// we need to initialize application - Create GLFW window
	Init("Resurge", 1280, 760);
	AB_INFO("Window init succesfully");

	// Create a Scene
	ResManager::CreateScene("exampleScene");

	// Bind current scene
	BindScene(ResManager::GetScene("exampleScene"));

	ResManager::CreateShader("game/res/shaders/simpleVS.glsl", "game/res/shaders/simpleFS.glsl", "simpleShader");

	ResManager::GenTexture("game/res/textures/envirorment/terrain/forest_terrain_1.png", "terrainTexture");
	ResManager::GetTexture("terrainTexture")->LoadTexture(false);

	ResManager::GenTexture("game/res/textures/wall.jpg", "wall");
	ResManager::GetTexture("wall")->LoadTexture(false);

	ResManager::CreateShader("game/res/shaders/simpleVS.glsl", "game/res/shaders/simpleFS.glsl", "terrain");

	// load sykbox
	std::vector<std::string> faces
	{
		    "game/res/textures/skybox/daytime_sea_bluesky/right.jpg",
			"game/res/textures/skybox/daytime_sea_bluesky/left.jpg",
			"game/res/textures/skybox/daytime_sea_bluesky/top.jpg",
			"game/res/textures/skybox/daytime_sea_bluesky/bottom.jpg",
			"game/res/textures/skybox/daytime_sea_bluesky/front.jpg",
			"game/res/textures/skybox/daytime_sea_bluesky/back.jpg"
	};
	cubemapTexture = Amba::LoadCubemap(faces);

	ResManager::CreateShader("game/res/shaders/skyboxVS.glsl", "game/res/shaders/skyboxFS.glsl", "skybox");

	EntityId terrain = ResManager::GetScene("exampleScene")->CreateEntity();

	ResManager::GetScene("exampleScene")->AddComponent<MeshComponent>(terrain);
	ResManager::GetScene("exampleScene")->AddComponent<TransformComponent>(terrain);

	unsigned int vertexCount = 128;
	unsigned int size = 1280;

	// we are using same normal data for every vertice
	glm::vec3 normals = glm::vec3(0.0f, 1.0f, 0.0f);

	// make terrain and populate mesh
	for (unsigned int i = 0; i < vertexCount; i++)
	{
		for (unsigned int j = 0; j < vertexCount; j++)
		{
			// populate here the vector data
			glm::vec3 positions = glm::vec3(
				(float)j / ((float)vertexCount - 1) * size,
				0.0f,
				(float)i / ((float)vertexCount - 1) * size);

			glm::vec2 texCoords = glm::vec2(
				(float)j / ((float)vertexCount - 1),
				(float)i / ((float)vertexCount - 1));

			ResManager::GetScene("exampleScene")->GetComponent<MeshComponent>(terrain)->m_Vertices.push_back({ positions, normals, texCoords });
		}
	}

	// indices calculation
	std::vector<unsigned int>* indices = &ResManager::GetScene("exampleScene")->GetComponent<MeshComponent>(terrain)->m_Indices;

	for (unsigned int i = 0; i < vertexCount - 1; i++) {
		for (unsigned int j = 0; j < vertexCount - 1; j++) {
			int topLeft = (i * vertexCount) + j;
			int topRight = topLeft + 1;
			int bottomLeft = ((i + 1) * vertexCount) + j;
			int bottomRight = bottomLeft + 1;
			indices->push_back(topLeft);
			indices->push_back(bottomLeft);
			indices->push_back(topRight);
			indices->push_back(topRight);
			indices->push_back(bottomLeft);
			indices->push_back(bottomRight);
		}
	}
	indices = nullptr;

	ResManager::GetScene("exampleScene")->GetComponent<MeshComponent>(terrain)->p_Texture = ResManager::GetTexture("terrainTexture");

	Amba::VertexBufferLayout layoutTerrain;
	layoutTerrain.Push<float>(3);
	layoutTerrain.Push<float>(3);
	layoutTerrain.Push<float>(2);
	ResManager::GetScene("exampleScene")->GetComponent<MeshComponent>(terrain)->layout = layoutTerrain;

	
	ResManager::GetScene("exampleScene")->GetComponent<TransformComponent>(terrain)->m_Size = 0.033f;


	cube = new Amba::Cube(ResManager::GetScene("exampleScene"));
	ResManager::GetScene("exampleScene")->GetComponent<MeshComponent>(cube->m_EntityId)->p_Texture = ResManager::GetTexture("wall");

	
	EntityId enemy_1 = ResManager::GetScene("exampleScene")->CopyEntity(cube->m_EntityId);
	EntityId enemy_2 = ResManager::GetScene("exampleScene")->CopyEntity(cube->m_EntityId);
	EntityId enemy_3 = ResManager::GetScene("exampleScene")->CopyEntity(cube->m_EntityId);

	ResManager::GetScene("exampleScene")->GetComponent<TransformComponent>(enemy_1)->m_Position = glm::vec3(45.0f, 0.0f, 25.0f);
	ResManager::GetScene("exampleScene")->GetComponent<TransformComponent>(enemy_2)->m_Position = glm::vec3(25.0f, 0.0f,  5.0f);
	ResManager::GetScene("exampleScene")->GetComponent<TransformComponent>(enemy_3)->m_Position = glm::vec3(25.0f, 0.0f, 50.0f);

	// delete terrain from entities to avoid rendering for now
	ResManager::GetScene("exampleScene")->DestroyEntity(terrain);

}

void GameApp::OnUserUpdate()
{
	// pass camera and Scene to renderer
	Amba::Renderer::BeginScene(AB_Cameras[0], ResManager::GetScene("exampleScene"));

	Amba::Renderer::DrawActiveScene(ResManager::GetShader("simpleShader"), AB_Perspective);
	
	// for debugging
	Amba::Renderer::DrawGrid(ResManager::GetScene("exampleScene"), AB_Perspective);

	ResManager::GetScene("exampleScene")->ApplyPhysics();
	
	ResManager::GetScene("exampleScene")->FindNearEntities(cube->m_EntityId, ResManager::GetScene("exampleScene")->GetComponent<TransformComponent>(cube->m_EntityId)->m_Position);

	bool skybox = false;

	if (skybox)
	{
		// skybox - remember to always draw this last!!
		glBindVertexArray(0);
		unsigned int skyboxVAO, skyboxVBO;
		glGenVertexArrays(1, &skyboxVAO);
		glGenBuffers(1, &skyboxVBO);
		glBindVertexArray(skyboxVAO);
		glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(positions), &positions, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		ResManager::GetShader("skybox")->Bind();
		glm::mat4 view = glm::mat4(glm::mat3(AB_Cameras[AB_ActiveCamera].GetViewMatrix()));
		ResManager::GetShader("skybox")->SetUniform4mat("u_View", view);
		ResManager::GetShader("skybox")->SetUniform4mat("u_Projection", AB_Perspective);
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // set depth function back to default
	}
}


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
