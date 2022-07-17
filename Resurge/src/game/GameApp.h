#pragma once

#include <application/Application.h>

// set different game states
enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};


class GameApp : public Amba::Application
{
public:

	GameState State;

	void OnUserCreate() override;
	void OnUserUpdate() override;

	void ProcessInput();

private:

};

