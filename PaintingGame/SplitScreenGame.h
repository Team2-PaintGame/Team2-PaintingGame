#pragma once
#include "PaintingGame.h"

namespace NCL {
	namespace CSC8508 {
		
		class SplitScreenGame : public PaintingGame {
		public:
			SplitScreenGame(GameTechRenderer* render, GameWorld* world, reactphysics3d::PhysicsCommon* physicsCommon, MenuHandler* menu);
			virtual ~SplitScreenGame();
			PlayerBase* AddPlayer(Camera* camera, Vector3 position, Gamepad* gamepad = nullptr) override;
			void UpdateGame(float dt) override;

		protected:
			//void InitWorld() override;
			void InitPlayers();

		protected:
			//contains an array of players (2) only
			const int maxPlayers = 2;
			std::vector<PlayerBase*> players;
			std::vector<PlayerController*> playerControllers;
			Gamepad* gamepad;
		};
	}
}

