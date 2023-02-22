#pragma once
#include "PushdownState.h"
#include <Window.h>
#include "../../../../PaintingGame/NetworkedGame.h"

namespace NCL {
	class MenuHandler;
	namespace CSC8508 {

		class LanScreen : public PushdownState
		{
		public:
			LanScreen(Window* window, GameTechRenderer* rend, GameWorld* gameWorld, reactphysics3d::PhysicsCommon* physicsCommon, MenuHandler* menu);
			~LanScreen();
			PushdownResult OnUpdate(float dt, PushdownState** newState) override;
			void OnAwake() override;
		protected:
			bool isPlayingGame;
			Window* window;
			NetworkedGame* paintingGame;
			MenuHandler* menuHandler;

			GameTechRenderer* renderer;
			GameWorld* gameWorld;
			reactphysics3d::PhysicsCommon* physicsCommon;
		};

	}
}