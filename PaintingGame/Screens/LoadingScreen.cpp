#include "LoadingScreen.h"
#ifdef _WIN32
#include "GameScreen.h"
#include "HUDOnLoad.h"
#include "OGLTexture.h"
#include "OGLAssetLoader.h"
#include <thread>
#include "GameManager.h"

using namespace NCL;
using namespace CSC8508;

NCL::CSC8508::LoadingScreen::LoadingScreen(ScreenManager* screenManager, SceneNode* sceneNode) : BaseScreen(screenManager, sceneNode) {
	NCL::Assets::OGLAssetLoader loader;
	ShaderBase* loadScreenShader = loader.LoadShader("hud.vert", "hud.frag");

	BaseScreen::sceneNode = new SceneNode(loader.LoadMesh(NCL::Assets::MeshType::Quad), loadScreenShader, TextureLoader::LoadAPITexture("Screens/bg1.jpg"));

	screenType = ScreenType::LoadingScreen;
	renderObj = this->sceneNode->GetRenderObject();
	transform = renderObj->GetTransform();

	hUDOnLoad = new HUDOnLoad(new Transform(), loader.LoadMesh(NCL::Assets::MeshType::Quad), loadScreenShader);
	hUDOnLoad->SetDefaultTexture(TextureLoader::LoadAPITexture("loadingSprites.png"));
	hUDOnLoad->GetTransform()->SetScale(Vector3(0.075f));
	hUDOnLoad->GetTransform()->SetPosition(Vector3(0.8f, -0.8f, 1));
}

void NCL::CSC8508::LoadingScreen::OnAwake()
{
	
}

PushdownState::PushdownResult LoadingScreen::OnUpdate(float dt, PushdownState** newState)
{
	hUDOnLoad->Update(dt);
	return onStateChange(newState);
}

PushdownState::PushdownResult LoadingScreen::onStateChange(PushdownState** newState)
{
	if (bLoaded) return PushdownResult::Pop;
	if (!threadToWait || GameManager::GetLoadingFlag())
	{
		if (threadToWait)
		{
			threadToWait->join();
			delete threadToWait;
			threadToWait = nullptr;
			GameManager::FinishLoadingCallback();
		}
		bLoaded = true;
		*newState = screenManager->GetScreen(ScreenType::SplashScreen);

		return PushdownResult::Push;
	}
	else
	{
		return PushdownResult::NoChange;
	}
}
#endif