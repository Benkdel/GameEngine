#include "GameApp.h"


#include <iostream>


void GameApp::OnUserCreate()
{
	// Create GLFW window

	Init("Resurge", 1280, 760);
	AB_INFO("Window init succesfully");


}

void GameApp::OnUserUpdate()
{
	std::cout << AB_deltaTime << std::endl;

}