#pragma once

#include <application/Application.h>

// set different game states
enum class GameState {
	NONE = 0,
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};


class GameApp : public Amba::Application
{
public:
	GameApp();
	~GameApp();


	void OnUserCreate() override;
	void OnUserUpdate() override;

	void ProcessInput();

private:

	GameState AB_State;

	unsigned int AB_ActiveCamera;

	// array of cameras
	Camera AB_Cameras[5];

	// projection mat
	glm::mat4 AB_Projection;

};

