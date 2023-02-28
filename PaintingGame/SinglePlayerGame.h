#pragma once
#include "PaintingGame.h"

namespace NCL {
	namespace CSC8508 {

		class SinglePlayerGame : public PaintingGame {
		public:
			SinglePlayerGame(GameTechRenderer* render, GameWorld* world, reactphysics3d::PhysicsCommon* physicsCommon, MenuHandler* menu);
			virtual ~SinglePlayerGame();
			PlayerBase* AddPlayer(Camera* camera, Vector3 position, Gamepad* gamepad = nullptr) override;
			void UpdateGame(float dt) override;

		protected:
			//void InitWorld() override;

		protected:
			PlayerBase* player = NULL;
			PlayerController* playerController;
			Gamepad* gamepad;
		};
	}
}
