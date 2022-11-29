#include "GameApp.h"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include <glm/gtc/type_ptr.hpp>
#include <engine/lighting/BasicLighting.h>

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

void ConfigureTerrain(Amba::Entity& ent);
void ConfigureSkyBox();
void RenderSkyBox(Amba::Camera& cam, glm::mat4 perspective);

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


	Amba::Entity terrain(ResManager::GetScene("exampleScene"));
	ConfigureTerrain(terrain);

	
	ResManager::CreateShader("src/engine/res/shaders/pbrVS.glsl", "src/engine/res/shaders/pbrFS.glsl", "pbrLighting");


	// basic PBR lighting
	Amba::Sphere pbrSphere(ResManager::GetScene("exampleScene"), 2.0f, 36, 36);
	pbrSphere.Init();
	pbrSphere.AddComponent<SphereCollider>();
	pbrSphere.InitCollider();
	pbrSphere.GetComponent<TransformComponent>()->m_Position = glm::vec3(10.0f, 10.0f, 20.0f);

	pbrSphere.GetComponent<MeshComponent>()->p_Shader = ResManager::GetShader("pbrLighting");

	// set all uniforms and light positions
	Amba::Shader* pbrShader = ResManager::GetShader("pbrLighting");
	
	pbrShader->Bind();
	pbrShader->SetUniform3f("u_Albedo", glm::vec3(0.0f, 0.5f, 0.0f));
	pbrShader->SetUniform1f("u_Ao", 1.0f);
	pbrShader->SetUniform1f("u_Metallic", 0.4f);
	pbrShader->SetUniform1f("u_Roughness", 0.25f);
	pbrShader->SetUniform1i("u_texturedPBR", 0);
	pbrShader->SetUniform1i("u_NumOfLights", 4);

	glm::vec3 lightPositions[] = {
		glm::vec3(-10.0f,  10.0f, 10.0f),
		glm::vec3(10.0f,  10.0f, 10.0f),
		glm::vec3(-10.0f, -10.0f, 10.0f),
		glm::vec3(10.0f, -10.0f, 10.0f),
	};
	glm::vec3 lightColors[] = {
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f)
	};

	for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
	{
		glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
		newPos = lightPositions[i];
		pbrShader->SetUniform3f("u_LightPositions[" + std::to_string(i) + "]", newPos);
		pbrShader->SetUniform3f("u_LightColors[" + std::to_string(i) + "]", lightColors[i]);
	}
	pbrShader = nullptr;

	Amba::Cube cube(ResManager::GetScene("exampleScene"));
	cube.Init();
	cube.GetComponent<MeshComponent>()->p_Shader = ResManager::GetShader("pbrLighting");
	cube.AddComponent<AABCollider>();
	cube.InitCollider();
	cube.GetComponent<TransformComponent>()->m_Position = glm::vec3(10.0f, 10.0f, 10.0f);
	 
	PhysicsComponent* cubePhysics = cube.AddComponent<PhysicsComponent>();
	cubePhysics->m_Mass = 100.0f;
	cubePhysics->ApplyForce(glm::vec3(0.0f, 0.0f, 0.0f));
	cubePhysics = nullptr;

	PhysicsComponent* spherePhysics = pbrSphere.AddComponent<PhysicsComponent>();
	spherePhysics->m_Mass = 100.0f;
	spherePhysics->ApplyForce(glm::vec3(0.0f, 0.0f, 0.0f));
	spherePhysics = nullptr;

	// fow now. add physics component to plane
	PhysicsComponent* terrainPhysics = terrain.AddComponent<PhysicsComponent>();
	terrainPhysics->m_Mass = 1000000000.0f;
	terrainPhysics->ToggleEntGravity(false);
	terrainPhysics = nullptr;

	terrain.Destroy();
	pbrSphere.Destroy();

	EntityId copy = ResManager::GetScene("exampleScene")->CopyEntity(cube.GetEntId());
	ResManager::GetScene("exampleScene")->
		GetComponent<TransformComponent>(copy)->m_Position +=
		glm::vec3(15.0f, 0.0f, 0.0f);

	ResManager::GetScene("exampleScene")->GetComponent<PhysicsComponent>(copy)->m_Mass = 50.0f;


	/*EntityId entities[10];
	for (int i = 0; i < 10; i++)
	{
		entities[i] = ResManager::GetScene("exampleScene")->CopyEntity(cube.GetEntId());
		ResManager::GetScene("exampleScene")->
					GetComponent<TransformComponent>(entities[i])->m_Position = 
																		glm::vec3(5.0f * (float)i, 10.0f, 10.0f * (float)(i / 5));
	}*/

	// How I envision my gameObject API
	// I want to create a character
	/*
	*   
	*	(return a reference)
		GameObject player1 = ResManager::CreateGameObject("player1");

		player1.LoadModel("path"); (will have multiple entities)

		or 

		
		
		player1.Getcomponent<MeshComponent>();

		player1.AddMaterial(

	
	
	*/

	ConfigureSkyBox();

}

void GameApp::OnUserUpdate()
{
	// pass camera and Scene to renderer
	Amba::Renderer::BeginScene(AB_Cameras[0], ResManager::GetScene("exampleScene"));

	// for now setting cam
	ResManager::GetShader("pbrLighting")->Bind();
	ResManager::GetShader("pbrLighting")->SetUniform3f("u_CamPos", AB_Cameras[0].GetCamPos());

	Amba::Renderer::DrawActiveScene(ResManager::GetShader("simpleShader"), AB_Perspective);


	ResManager::GetScene("exampleScene")->Update(AB_DeltaTime);
	


	RenderSkyBox(AB_Cameras[0], AB_Perspective);
	
}

void ConfigureTerrain(Amba::Entity& terrain)
{
	terrain.AddComponent<MeshComponent>();
	terrain.AddComponent<TransformComponent>();

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

			terrain.GetComponent<MeshComponent>()->m_Vertices.push_back({ positions, normals, texCoords });
		}
	}

	// indices calculation
	std::vector<unsigned int>* indices = &terrain.GetComponent<MeshComponent>()->m_Indices;

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

	terrain.GetComponent<MeshComponent>()->p_Texture = ResManager::GetTexture("terrainTexture");

	Amba::VertexBufferLayout layoutTerrain;
	layoutTerrain.Push<float>(3);
	layoutTerrain.Push<float>(3);
	layoutTerrain.Push<float>(2);
	terrain.GetComponent<MeshComponent>()->layout = layoutTerrain;
	terrain.GetComponent<TransformComponent>()->m_Size = 0.033f;

	terrain.AddComponent<PlaneCollider>();
	terrain.InitCollider();
}

void ConfigureSkyBox()
{
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

}

void RenderSkyBox(Amba::Camera& cam, glm::mat4 perspective)
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
	glm::mat4 view = glm::mat4(glm::mat3(cam.GetViewMatrix()));
	ResManager::GetShader("skybox")->SetUniform4mat("u_View", view);
	ResManager::GetShader("skybox")->SetUniform4mat("u_Projection", perspective);
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default
}