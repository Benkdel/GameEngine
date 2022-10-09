#include "game/GameApp.h"

int main(int argc, char** argv)
{
	GameApp *game = new GameApp();

	game->Run();

	delete game;
}