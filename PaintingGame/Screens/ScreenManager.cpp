#include "ScreenManager.h"
#include "SplashScreen.h"
#include "LoadingScreen.h"
#include "MainMenuScreen.h"
#include "GameScreen.h"
#include "GameOverScreen.h"

using namespace NCL;
using namespace CSC8508;


ScreenManager::ScreenManager(GameAssets* assets) {
	this->assets = assets;
#ifdef _WIN32
	LoadLoadingScreen();
	machine = std::make_unique<PushdownMachine>((PushdownState*)GetScreen(ScreenType::LoadingScreen));
#endif
#ifdef __ORBIS__
	LoadScreens();
	machine = std::make_unique<PushdownMachine>((PushdownState*)GetScreen(ScreenType::SplashScreen));
#endif
}

void ScreenManager::LoadScreens() {
	screenSceneNodes.emplace(std::make_pair(ScreenType::SplashScreen, std::make_unique<SceneNode>(assets->GetMesh("quadMesh"), assets->GetShader("screenShader"), assets->GetTexture("splashScreenTex"))));
	screenSceneNodes.emplace(std::make_pair(ScreenType::MainMenuScreen, std::make_unique<SceneNode>(assets->GetMesh("quadMesh"), assets->GetShader("screenShader"), assets->GetTexture("mainMenuScreenTex"))));
	screenSceneNodes.emplace(std::make_pair(ScreenType::GameOverScreen, std::make_unique<SceneNode>(assets->GetMesh("quadMesh"), assets->GetShader("screenShader"), assets->GetTexture("gameOverScreenTex"))));

	screens.insert(std::make_pair(ScreenType::SplashScreen, std::make_unique<SplashScreen>(this, screenSceneNodes.at(ScreenType::SplashScreen).get())));
	screens.insert(std::make_pair(ScreenType::MainMenuScreen, std::make_unique<MainMenuScreen>(this, screenSceneNodes.at(ScreenType::MainMenuScreen).get())));
	screens.insert(std::make_pair(ScreenType::GameOverScreen, std::make_unique<GameOverScreen>(this, screenSceneNodes.at(ScreenType::GameOverScreen).get())));
	screens.insert(std::make_pair(ScreenType::GameScreen, std::make_unique<GameScreen>(this)));
}

void ScreenManager::LoadLoadingScreen() {
	//screenSceneNodes.emplace(std::make_pair(ScreenType::LoadingScreen, std::make_unique<SceneNode>(assets->GetMesh("quadMesh"), assets->GetShader("screenShader"), assets->GetTexture("splashScreenTex"))));
#ifdef _WIN32
	screens.insert(std::make_pair(ScreenType::LoadingScreen, std::make_unique<LoadingScreen>(this, nullptr)));
#endif
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
	if (!isMenuDisplayed || bisNetworkedGame) {
		sceneNode->Update(dt);
	}
	TransitionTimer(dt);
	return onStateChange(newState);
}

void BaseScreen::OnSleep() {
	command = ScreenCommand::None;
}

void BaseScreen::RenderMenu() {
#ifdef _WIN32
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (isMenuDisplayed) {
		MenuFrame();
	}
	if (isDebugDisplayed) {
		DebugWindow();
	}

	ImGui::EndFrame();

	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif
}
