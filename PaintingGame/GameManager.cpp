#pragma once
#include "GameManager.h"
#include "SplashScreen.h"
#include "MainMenuScreen.h"

using namespace NCL;
using namespace CSC8508;

void GameManager::Run(Window* window) {
	while (window->UpdateWindow()) {
		float dt = window->GetTimer()->GetTimeDeltaSeconds();
		if (!gameMachine.Update(dt)) {
			return;
		}
	}
}
