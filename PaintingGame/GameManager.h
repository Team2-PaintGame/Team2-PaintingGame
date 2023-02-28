#pragma once
#include "ScreenManager.h"
#include "PlatformConfigurations.h"

namespace NCL::CSC8508 {
	class GameManager {
	public:
		GameManager(Window* window) : gameMachine((PushdownState*)screenManager->GetScreen(ScreenType::SplashScreen)) {
			renderer2 = config.rendererFactory->createRenderer(*window);
		}
		~GameManager() {
			delete renderer;
			delete screenManager;
			delete renderer2;
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
		PlatformConfigurations config;
		RendererBase* renderer2;
		GameTechRenderer* renderer = new GameTechRenderer(gameWorld, physicsWorld);
		//PaintingGameRenderer rendererTemp;
		ScreenManager* screenManager = new ScreenManager(renderer);
		PushdownMachine gameMachine;
	};
}