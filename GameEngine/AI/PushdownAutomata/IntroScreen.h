#pragma once
#include "PushdownState.h"
#include <Window.h>
#include "../../../../PaintingGame/PaintingGame.h"



namespace NCL {

	class MenuHandler;

	namespace CSC8508 {

		class IntroScreen : public PushdownState
		{
		public:
			IntroScreen(Window* window);
			~IntroScreen();
			PushdownResult OnUpdate(float dt, PushdownState** newState) override;
			void OnAwake() override;
			void OnSleep() override;
		protected:
			Window* window;
			PaintingGame* paintingGame;
			MenuHandler* menuHandler;

			GameTechRenderer* renderer;
			GameWorld* gameWorld;
			reactphysics3d::PhysicsCommon* physicsCommon;
		};

	}
}