#pragma once
#include "SceneNode.h"
#include "RenderObject.h"
#include <GameTechRenderer.h>

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
	class GameManager {
	public:
		GameManager() : gameMachine((PushdownState*)screenManager->GetScreen(ScreenType::SplashScreen)) {
		}
		~GameManager() {
			delete renderer;
			delete screenManager;
			physicsCommon.destroyPhysicsWorld(physicsWorld);
		}
		void Run(Window* window);
		//void LoadAssets(); //textures, meshes, shaders
		//setup renderer, load imgui context inside renderer
		//this class will also be responsible for making push down automata
	protected:
		GameWorld gameWorld;
		reactphysics3d::PhysicsCommon  physicsCommon;
		reactphysics3d::PhysicsWorld* physicsWorld = physicsCommon.createPhysicsWorld();
		GameTechRenderer* renderer = new GameTechRenderer(gameWorld, physicsWorld);
		ScreenManager* screenManager = new ScreenManager(renderer);
		PushdownMachine gameMachine;
	};
}