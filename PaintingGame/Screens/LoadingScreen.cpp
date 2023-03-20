#include "LoadingScreen.h"
#include "GameScreen.h"
#include "HUDOnLoad.h"
#include "OGLTexture.h"
#include "OGLAssetLoader.h"
#include <thread>
#include "GameManager.h"

using namespace NCL;
using namespace CSC8508;

void NCL::CSC8508::LoadingScreen::OnAwake()
{
	NCL::Assets::OGLAssetLoader loader;
	//hUDOnLoad = new HUDOnLoad(new Transform(), loader.LoadMesh(NCL::Assets::MeshType::Quad), loader.LoadShader("hud.vert", "hud.frag"));
	//hUDOnLoad->SetDefaultTexture(TextureLoader::LoadAPITexture("loadingSprites.png"));
}

PushdownState::PushdownResult LoadingScreen::OnUpdate(float dt, PushdownState** newState)
{
	//hUDOnLoad->Update(dt);

	return onStateChange(newState);
}

PushdownState::PushdownResult LoadingScreen::onStateChange(PushdownState** newState)
{
	if (!threadToWait || threadToWait->joinable()) // if thread joinable
	{
		if (threadToWait)
		{
			threadToWait->join();
			delete threadToWait;
			threadToWait = nullptr;
			GameManager::FinishLoadingCallback();
		}

		*newState = screenManager->GetScreen(ScreenType::SplashScreen);

		return PushdownResult::Push;
	}
	else
	{
		return PushdownResult::NoChange;
	}
}
