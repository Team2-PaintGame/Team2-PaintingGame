#pragma once
#include "GameManager.h"
#include "SplashScreen.h"
#include "MainMenuScreen.h"

using namespace NCL;
using namespace CSC8508;

GameManager::GameManager(Window* window) {
	renderer = config.rendererFactory->createRenderer(*window);
	assetLoader = config.assetLoaderFactory->createAssetLoader();
	gameAssets = new GameAssets(assetLoader);
	screenManager = new ScreenManager(gameAssets);
}

GameManager::~GameManager() {
	delete renderer;
	delete screenManager;
	delete assetLoader;
	delete gameAssets;
}

bool GameManager::RunGame(float dt) {
	bool isRunning =  screenManager->Update(dt);
	renderer->BindScreen(screenManager->GetActiveScreen());
	renderer->Render();
	return isRunning;
}
