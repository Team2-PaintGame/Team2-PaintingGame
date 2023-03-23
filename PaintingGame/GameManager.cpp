#include "GameManager.h"
#include "SplashScreen.h"
#include "MainMenuScreen.h"
#include "LoadingScreen.h"
#include <chrono>

using namespace NCL;
using namespace CSC8508;

PlatformConfigurations GameManager::sConfig = PlatformConfigurations();

GameManager* GameManager::gameManager = nullptr;
bool GameManager::loadingFlag = false;

GameManager::GameManager(Window* window) {
	renderer = sConfig.rendererFactory->createRenderer(*window);
	assetLoader = sConfig.assetLoaderFactory->createAssetLoader();
	gameAssets = new GameAssets(assetLoader);
	screenManager = new ScreenManager(gameAssets);
	gameManager = this;

#ifdef _WIN32
	HGLRC context = ((OGLRenderer*)renderer)->CreateAnotherContext();
	//((OGLRenderer*)renderer)->ResetContext();
	((LoadingScreen*)screenManager->GetScreen(ScreenType::LoadingScreen))->SetThread(new std::thread(LoadAssets, gameAssets, (OGLRenderer*)renderer, context));
#else
	renderer->BindDebugShader(gameAssets->GetShader("debugShader"));
#endif
}

GameManager::~GameManager() {
	delete renderer;
	delete screenManager;
	delete assetLoader;
	delete gameAssets;
}

bool GameManager::RunGame(float dt) {
	bool isRunning =  screenManager->Update(dt);

	//gameAssets->ReloadDebug();
	//renderer->BindDebugShader(gameAssets->GetShader("debugShader"));
	//((OGLRenderer*)renderer)->ResetDefaultContext();
	renderer->BindScreen(screenManager->GetActiveScreen());
	renderer->Render();
	calculateRenderingTime();
	Debug::UpdateRenderables(dt);
	return isRunning;
}

void GameManager::calculateRenderingTime()
{
	std::chrono::microseconds duration = std::chrono::duration_cast<std::chrono::microseconds>(renderer->rendererEndTime - renderer->rendererStartTime);
	Debug::ShowRenderTime(duration.count());
}

#ifdef _WIN32
//Static function to start the thread
void GameManager::LoadAssets(GameAssets* assets, OGLRenderer* renderer, HGLRC context)
{
	loadingFlag = false;
	renderer->MakeCurrent(context);
	assets->Load();
	loadingFlag = true;
}

void GameManager::FinishLoadingCallback()
{
	gameManager->FinishLoading();
}

void GameManager::FinishLoading()
{
	((OGLRenderer*)renderer)->ResetDefaultContext();
	gameAssets->ReloadShaders();
	gameAssets->ReloadMeshes();
	screenManager->LoadScreens();
	renderer->BindDebugShader(gameAssets->GetShader("debugShader"));

	((OGLPaintingGameRenderer*)renderer)->ResetDebugRenderers();
}
#endif
