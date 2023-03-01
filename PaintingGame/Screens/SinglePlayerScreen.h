#pragma once
#include "PushdownState.h"
#include <Window.h>
#include "../../../../PaintingGame/PaintingGame.h"

namespace NCL {
	class MenuHandler;
	namespace CSC8508 {

		class SinglePlayerScreen : public PushdownState
		{
		public:
			SinglePlayerScreen(Window* window, GameTechRenderer* rend, GameWorld* gameWorld, reactphysics3d::PhysicsCommon* physicsCommon, MenuHandler* menu);
			//SinglePlayerScreen(PaintingGame* paintingGame, MenuHandler* menu);
			~SinglePlayerScreen();
			PushdownResult OnUpdate(float dt, PushdownState** newState) override;
			void OnAwake() override;
			void OnSleep() override;
		protected:
			bool isPlayingGame;
			Window* window;
			PaintingGame* paintingGame;
			MenuHandler* menuHandler;

			GameTechRenderer* renderer;
			GameWorld* gameWorld;
			reactphysics3d::PhysicsCommon* physicsCommon;
		};
	}
}
