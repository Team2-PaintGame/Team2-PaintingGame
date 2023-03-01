#pragma once
#include "ScreenManager.h"
#include "SplashScreen.h"
#include "MainMenuScreen.h"

using namespace NCL;
using namespace CSC8508;


ScreenManager::ScreenManager(GameAssets* assets) {
	screenSceneNodes.insert(std::make_pair(ScreenType::SplashScreen, SceneNode(assets->GetMesh("quadMesh"), assets->GetShader("screenShader"), assets->GetTexture("splashScreenTex"))));
	screenSceneNodes.insert(std::make_pair(ScreenType::MainMenuScreen, SceneNode(assets->GetMesh("quadMesh"), assets->GetShader("screenShader"), assets->GetTexture("splashScreenTex"))));

	screens.insert(std::make_pair(ScreenType::SplashScreen, std::make_unique<SplashScreen>(this, &screenSceneNodes.at(ScreenType::SplashScreen))));
	screens.insert(std::make_pair(ScreenType::MainMenuScreen, std::make_unique<MainMenuScreen>(this, &screenSceneNodes.at(ScreenType::MainMenuScreen))));
}

BaseScreen* NCL::CSC8508::ScreenManager::GetScreen(ScreenType screenType) const {
	return screens.count(screenType) ? screens.at(screenType).get() : nullptr;
}


PushdownState::PushdownResult BaseScreen::OnUpdate(float dt, PushdownState** newState) {
	if (!isMenuDisplayed) {
		sceneNode->Update(dt);
	}
	return onStateChange(newState);
}

//void BaseScreen::RenderMenu() {
//	// Start the Dear ImGui frame
//	ImGui_ImplOpenGL3_NewFrame();
//	ImGui_ImplWin32_NewFrame();
//	ImGui::NewFrame();
//
//	if (isMenuDisplayed) {
//		MenuFrame();
//	}
//
//	ImGui::EndFrame();
//}
