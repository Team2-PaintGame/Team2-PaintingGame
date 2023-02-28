#pragma once
#include <GameTechRenderer.h>

//change it from game tech renderer to renderer base
namespace NCL::CSC8508 {
	class BaseScreen;
	enum class ScreenType {
		None,
		SplashScreen,
		MainMenuScreen,
		GameScreen,
		GameWinScreen,
		GameOverScreen,
	};
	class ScreenManager {
	public:
		ScreenManager(GameTechRenderer* renderer);
		~ScreenManager();
		BaseScreen* GetScreen(ScreenType screenType) const;
	protected:
		void LoadAssets(GameTechRenderer* renderer);
		std::map<ScreenType, BaseScreen*> screens;
		std::map<ScreenType, TextureBase*> screenTextures;
		std::map<ScreenType, SceneNode> screenSceneNodes;
		MeshGeometry* quadMesh;
		ShaderBase* screenShader;
	};
}