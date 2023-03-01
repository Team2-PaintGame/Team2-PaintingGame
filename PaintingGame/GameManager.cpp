#pragma once
#include "GameManager.h"
#include "SplashScreen.h"
#include "MainMenuScreen.h"

using namespace NCL;
using namespace CSC8508;

bool GameManager::RunGame(float dt) {
	renderer->Render();
	return gameMachine->Update(dt);
}
