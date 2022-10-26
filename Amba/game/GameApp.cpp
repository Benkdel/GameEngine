#include "GameApp.h"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include <glm/gtc/type_ptr.hpp>
#include <engine/lighting/Light.h>

#include <engine/importer/Importer.h>


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

float terrain[32 * 32 * 5];


GameApp::GameApp()
	: AB_State(GameState::NONE)
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

	ResManager::CreateModel("tree");
	ResManager::GetModel("tree")->LoadModel("game/res/models/envirorment/tree_model_1/scene.gltf");

	ResManager::CreateShader("game/res/shaders/simpleVS.glsl", "game/res/shaders/simpleFS.glsl", "tree");

	ResManager::GenTexture("game/res/textures/envirorment/wall.jpg", "terrainTexture");

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

	/* next steps:
	* 
	*  player? --> camera, maybe a gun. for now
	*  envirorment:
	*			1) skybox:
	*           2) terrain: incl collision box, rigid body, and stuff.
	*			3) entities: entity component system
	*			4) physics engine
	*           5) complete importer incl animations, AI
	*			
	* 
	*/
	Amba::Vertex vertex;

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

	ResManager::GetShader("tree")->Bind();
	Amba::Renderer::DrawModel(ResManager::GetModel("tree"), layout, ResManager::GetShader("tree"), AB_Perspective);

	glBindVertexArray(0);

	// skybox
	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), &positions, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//Amba::VertexArray va;
	//Amba::VertexBuffer vbo(&positions, sizeof(positions));

	//Amba::VertexBufferLayout layout2;
	//layout.Push<float>(2);
	//va.AddBuffer(&vbo, layout2);
	//va.Bind();
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


	Amba::VertexArray va;
	Amba::VertexBuffer vbo(&terrain, sizeof(terrain));

	Amba::VertexBufferLayout layoutTerrain;
	layoutTerrain.Push<float>(3);
	layoutTerrain.Push<float>(2);

	va.AddBuffer(&vbo, layoutTerrain);
	ResManager::GetTexture("terrainTexture")->Bind(0);
	ResManager::GetShader("tree")->SetUniform1i("u_Texture", 0);
	Amba::Renderer::DrawTriangles(&va, 32 * 32 * 5, ResManager::GetShader("tree"), AB_Perspective);

	
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
