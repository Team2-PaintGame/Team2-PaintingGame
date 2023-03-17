#pragma once
#include "GameManager.h"
#include "SplashScreen.h"
#include "MainMenuScreen.h"
#include "LoadingScreen.h"

using namespace NCL;
using namespace CSC8508;

PlatformConfigurations GameManager::sConfig = PlatformConfigurations();

GameManager::GameManager(Window* window) {
	renderer = sConfig.rendererFactory->createRenderer(*window);
	assetLoader = sConfig.assetLoaderFactory->createAssetLoader();
	gameAssets = new GameAssets(assetLoader);
	screenManager = new ScreenManager(gameAssets);

	renderer->BindDebugShader(gameAssets->GetShader("debugShader"));
	//renderer->BindScreen(screenManager->GetScreen(ScreenType::LoadingScreen));

	//assetThread = std::thread(LoadAssets, gameAssets);
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

//Static function to start the thread
void GameManager::LoadAssets(GameAssets* assets)
{
	assets->Load();
}
