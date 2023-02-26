#pragma once
#include "PaintingGame.h"

namespace NCL {
	namespace CSC8508 {

		class SinglePlayerGame : public PaintingGame {
		public:
			SinglePlayerGame(GameTechRenderer* render, GameWorld* world, reactphysics3d::PhysicsCommon* physicsCommon, MenuHandler* menu);
			virtual ~SinglePlayerGame();
			virtual PlayerBase* AddPlayer();

		protected:
			PlayerBase* player;
		};
	}
}
