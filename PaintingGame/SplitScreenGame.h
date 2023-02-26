#pragma once
#include "PaintingGame.h"

namespace NCL {
	namespace CSC8508 {
		
		class SplitScreenGame : public PaintingGame {
		public:
			SplitScreenGame(GameTechRenderer* render, GameWorld* world, reactphysics3d::PhysicsCommon* physicsCommon, MenuHandler* menu);
			virtual ~SplitScreenGame();
			virtual PlayerBase* AddPlayer();

		protected:
			//contains an array of players (2) only
			std::vector<PlayerBase*> players;
			const int maxPlayers = 2;
		};
	}
}

