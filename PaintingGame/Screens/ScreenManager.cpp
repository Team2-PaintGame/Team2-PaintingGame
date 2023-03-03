#pragma once
#include "ScreenManager.h"
#include "SplashScreen.h"
#include "MainMenuScreen.h"
#include "GameScreen.h"

using namespace NCL;
using namespace CSC8508;


ScreenManager::ScreenManager(GameAssets* assets) {
	this->assets = assets;
	LoadScreens();
	machine = std::make_unique<PushdownMachine>((PushdownState*)GetScreen(ScreenType::SplashScreen));
}

void ScreenManager::LoadScreens() {
	screenSceneNodes.insert(std::make_pair(ScreenType::SplashScreen, SceneNode(assets->GetMesh("quadMesh"), assets->GetShader("screenShader"), assets->GetTexture("splashScreenTex"))));
	screenSceneNodes.insert(std::make_pair(ScreenType::MainMenuScreen, SceneNode(assets->GetMesh("quadMesh"), assets->GetShader("screenShader"), assets->GetTexture("mainMenuScreenTex"))));

	screens.insert(std::make_pair(ScreenType::SplashScreen, std::make_unique<SplashScreen>(this, &screenSceneNodes.at(ScreenType::SplashScreen))));
	screens.insert(std::make_pair(ScreenType::MainMenuScreen, std::make_unique<MainMenuScreen>(this, &screenSceneNodes.at(ScreenType::MainMenuScreen))));
	screens.insert(std::make_pair(ScreenType::GameScreen, std::make_unique<GameScreen>(this)));
}

BaseScreen* NCL::CSC8508::ScreenManager::GetScreen(ScreenType screenType) const {
	return screens.count(screenType) ? screens.at(screenType).get() : nullptr;
}

BaseScreen* ScreenManager::GetActiveScreen() const {
	return (BaseScreen*)machine->GetActiveState();
}

bool ScreenManager::Update(float dt) {
	return machine->Update(dt);
}

PushdownState::PushdownResult BaseScreen::OnUpdate(float dt, PushdownState** newState) {
	if (!isMenuDisplayed) {
		sceneNode->Update(dt);
	}
	return onStateChange(newState);
}

void BaseScreen::RenderMenu() {
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (isMenuDisplayed) {
		MenuFrame();
	}

	ImGui::EndFrame();

	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
