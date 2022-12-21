#include "GameApp.h"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include <glm/gtc/type_ptr.hpp>
#include <engine/lighting/BasicLighting.h>

#include <engine/importer/Importer.h>

#include <engine/scene/Scene.h>

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
void RenderSkyBox(Amba::Camera* cam, Amba::Scene* scene);

Amba::Physics* physics;

void GameApp::OnUserCreate()
{
	// we need to initialize application - Create GLFW window
	Init("Resurge", 1920, 1080);
	AB_INFO("Window init succesfully");

	// Create a Scene
	Amba::Scene* sampleScene = ResManager::CreateScene("exampleScene", true);

	// create a physics system
	physics = new Amba::Physics();

	// Bind current scene
	BindScene(sampleScene);

	Amba::Shader* simpleShader = ResManager::CreateShader("game/res/shaders/simpleVS.glsl", "game/res/shaders/simpleFS.glsl", "simpleShader");
	Amba::Shader* pbrShader = ResManager::CreateShader("src/engine/res/shaders/pbrVS.glsl", "src/engine/res/shaders/pbrFS.glsl", "pbrLighting");

	// create main camera:
	Amba::Entity broadViewCam(sampleScene);
	broadViewCam.AddComponent<CameraComponent>();
	broadViewCam.AddComponent<TagComponent>();
	broadViewCam.GetComponent<TagComponent>()->m_Tag = "camaraPlayer";
	sampleScene->AddCameraObject(&broadViewCam, false);

	
	// basic PBR lighting
	Amba::Sphere pbrSphere(sampleScene, 2.0f, 36, 36);
	pbrSphere.Init();
	pbrSphere.AddComponent<SphereCollider>();
	pbrSphere.InitCollider();
	pbrSphere.GetComponent<TransformComponent>()->UpdatePosition(glm::vec3(10.0f, 0.0f, 10.0f));

	pbrSphere.GetComponent<MeshComponent>()->p_Shader = simpleShader;

	// set all uniforms and light positions
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

	Amba::Cube cube(sampleScene);
	cube.Init();
	cube.GetComponent<MeshComponent>()->p_Shader = pbrShader;
	cube.AddComponent<AABCollider>();
	cube.InitCollider();
	cube.GetComponent<TransformComponent>()->UpdatePosition(glm::vec3(20.0f, 0.0f, 0.0f));
	cube.AddComponent<TagComponent>();
	cube.GetComponent<TagComponent>()->m_Tag = "BaseCube";

	PhysicsComponent* cubePhysics = cube.AddComponent<PhysicsComponent>();
	cubePhysics->m_Mass = 30.0f;
	cubePhysics->ApplyForce(glm::vec3(0.0f, 0.0f, 0.0f));
	cubePhysics = nullptr;
	
	Amba::Entity cubeCopy = cube.CopyEntity();
	cubeCopy.GetComponent<TransformComponent>()->UpdatePosition(glm::vec3(15.0f, 0.0f, 0.0f));
	cubeCopy.GetComponent<PhysicsComponent>()->m_Mass = 10.0f;
	cubeCopy.GetComponent<TagComponent>()->m_Tag = "cubeCopy";

	// create walls - for now enter in object space to not mess up with transformations and scale it
	Amba::Plane wall_1(sampleScene,
						glm::vec3(0.0f, -0.5f, -0.5f), // v0
						glm::vec3(0.0f,  0.5f, -0.5f), // v1
						glm::vec3(0.0f,  0.5f,  0.5f), // v2
						glm::vec3(0.0f, -0.5f,  0.5f), // v3
						5);
	wall_1.Init();
	wall_1.GetComponent<MeshComponent>()->p_Shader = pbrShader;
	wall_1.AddComponent<AABCollider>();
	wall_1.InitCollider();
	wall_1.AddComponent<PhysicsComponent>();
	wall_1.GetComponent<PhysicsComponent>()->m_Mass = Amba::s_InfiniteMassValue;
	wall_1.GetComponent<TransformComponent>()->UpdateScale(glm::vec3(0.0f, 5.0f, 10.0f));
	wall_1.AddComponent<TagComponent>();
	wall_1.GetComponent<TagComponent>()->m_Tag = "Wall1";

	// create walls
	Amba::Plane wall_2(sampleScene,
		glm::vec3(0.0f, -0.5f, -0.5f), // v0
		glm::vec3(0.0f, 0.5f, -0.5f), // v1
		glm::vec3(0.0f, 0.5f, 0.5f), // v2
		glm::vec3(0.0f, -0.5f, 0.5f), // v3
		5);
	wall_2.Init();
	wall_2.GetComponent<MeshComponent>()->p_Shader = pbrShader;
	wall_2.AddComponent<AABCollider>();
	wall_2.InitCollider();
	wall_2.AddComponent<PhysicsComponent>();
	wall_2.GetComponent<PhysicsComponent>()->m_Mass = Amba::s_InfiniteMassValue;
	wall_2.GetComponent<TransformComponent>()->UpdatePosition(glm::vec3(30.0f, 0.0f, 0.0f));
	wall_2.GetComponent<TransformComponent>()->UpdateScale(glm::vec3(0.0f, 5.0f, 10.0f));
	wall_2.AddComponent<TagComponent>();
	wall_2.GetComponent<TagComponent>()->m_Tag = "Wall2";

	pbrSphere.Destroy();
	
	Amba::Entity player = cube.CopyEntity();
	player.GetComponent<TagComponent>()->m_Tag = "player";
	player.GetComponent<TransformComponent>()->UpdatePosition(glm::vec3(0.0f, 0.0f, 10.0f));
	
	// attach one entity to the other?

	/*
		Scene 
			needs:
				Reference of Entity class

			holds:
				Vector of entities
				component pool

				Scene 1 (introduction)

				Scene 2 (first stage of the game - )
	
				
		Physics System - shared by all scenes?  
			needs: 
				reference to scene
				reference of some sort of grid / spatial entities org

			holds:
				physics variables (gravity, etc)
				

			methods:
				applies physics to entities in a scene using components information

			when do I need entities assigned?
				when AddComponent<ColliderComponent>()
				
	
		Spatial 2D grid:
			needs:
				reference to cell


	
	*/




	ConfigureSkyBox();

}
bool first = true;
void GameApp::OnUserUpdate()
{
	Amba::Scene* activeScene = ResManager::GetScene("exampleScene");
	
	ResManager::GetShader("pbrLighting")->Bind();
	ResManager::GetShader("pbrLighting")->SetUniform3f("u_CamPos", activeScene->GetActiveCamera()->GetCamPos());

	ResManager::GetScene("exampleScene")->Update((float)AB_DeltaTime);
	
	EntityId player = activeScene->GetEntityByTag("player"); 
	EntityId camPlayer = activeScene->GetEntityByTag("camaraPlayer");

	/* simulate a sticky camera to player - this is working somewhat properly */

	// shoud all this be done in scene? in a new system? or in components?
	
	// first step would be to asign a camara follow system
	// 1) need to define offset from object is following
	if (first)
	{
		glm::vec3 offset = glm::vec3(-4.0f, 1.5f, 0.0f);

		// set first position
		activeScene->GetComponent<TransformComponent>(camPlayer)->UpdatePosition(
			activeScene->GetComponent<TransformComponent>(player)->GetPosition() +
			offset);
		first = false;
	}

	// i need to have a player controller or something like that
	// also, add camera rotation (not scaling) to work. same for entities
	// I think it issomething to do with angle not being there
	if (Amba::KeyBoard::Key(GLFW_KEY_W))
	{

		// 2) then, calculate the difference in position so the camera can follow
		// 3) were should all this be??

		// player moves
		glm::vec3 pos = activeScene->GetComponent<TransformComponent>(player)->GetPosition();
		glm::vec3 prevPos = pos;
		pos += glm::vec3(0.0f, 0.0f, 1.0f) * (float)AB_DeltaTime;
		activeScene->GetComponent<TransformComponent>(player)->UpdatePosition(pos);
	
		glm::vec3 changeInPosition = pos - prevPos;

		// camera follows
		glm::vec3 camPos = activeScene->GetComponent<TransformComponent>(camPlayer)->GetPosition();
		camPos += changeInPosition;
		activeScene->GetComponent<TransformComponent>(camPlayer)->UpdatePosition(camPos);
		
	}

	RenderSkyBox(activeScene->GetActiveCamera(), activeScene);
}

void ConfigureTerrain(Amba::Entity& terrain)
{
	ResManager::GenTexture("game/res/textures/envirorment/terrain/forest_terrain_1.png", "terrainTexture");
	ResManager::GetTexture("terrainTexture")->LoadTexture(false);
	ResManager::CreateShader("game/res/shaders/simpleVS.glsl", "game/res/shaders/simpleFS.glsl", "terrain");

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

	terrain.GetComponent<MeshComponent>()->m_Textures.push_back(ResManager::GetTexture("terrainTexture"));

	Amba::VertexBufferLayout layoutTerrain;
	layoutTerrain.Push<float>(3);
	layoutTerrain.Push<float>(3);
	layoutTerrain.Push<float>(2);
	terrain.GetComponent<MeshComponent>()->layout = layoutTerrain;
	terrain.GetComponent<TransformComponent>()->UpdateScale(glm::vec3(0.033f));

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

void RenderSkyBox(Amba::Camera* cam, Amba::Scene* scene)
{

	glm::mat4 perspective = cam->GetPerspective(scene->GetViewPortData());
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
	glm::mat4 view = glm::mat4(glm::mat3(cam->GetViewMatrix()));
	ResManager::GetShader("skybox")->SetUniform4mat("u_View", view);
	ResManager::GetShader("skybox")->SetUniform4mat("u_Projection", perspective);
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default
}