#pragma once
#include "ScreenManager.h"
#include "SplashScreen.h"
#include "MainMenuScreen.h"

using namespace NCL;
using namespace CSC8508;


ScreenManager::ScreenManager(GameTechRenderer* renderer) {
	LoadAssets(renderer);
	screenSceneNodes.insert(std::make_pair(ScreenType::SplashScreen, SceneNode(quadMesh, screenShader, screenTextures.at(ScreenType::SplashScreen))));
	screenSceneNodes.insert(std::make_pair(ScreenType::MainMenuScreen, SceneNode(quadMesh, screenShader, screenTextures.at(ScreenType::SplashScreen))));

	screens.insert(std::make_pair(ScreenType::SplashScreen, new SplashScreen(this, &screenSceneNodes.at(ScreenType::SplashScreen))));
	screens.insert(std::make_pair(ScreenType::MainMenuScreen, new MainMenuScreen(this, &screenSceneNodes.at(ScreenType::MainMenuScreen))));

}

ScreenManager::~ScreenManager() {
	for (auto const& [key, val] : screens) {
		delete val;
	}

	for (auto const& [key, val] : screenTextures) {
		delete val;
	}

	delete screenShader;
	delete quadMesh;
}

BaseScreen* NCL::CSC8508::ScreenManager::GetScreen(ScreenType screenType) const {
	return screens.count(screenType) ? screens.at(screenType) : nullptr;
}

void ScreenManager::LoadAssets(GameTechRenderer* renderer) {
	screenTextures.insert(std::make_pair(ScreenType::SplashScreen, renderer->LoadTexture("Screens/bg.jpg")));
	
	quadMesh = renderer->LoadQuadMesh();
	screenShader = renderer->LoadShader("screen.vert", "screen.frag");
}
