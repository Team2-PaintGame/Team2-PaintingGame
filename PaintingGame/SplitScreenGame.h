#pragma once
#include "PaintingGame.h"
#include <PlayerController.h>

namespace NCL {
	namespace CSC8508 {
		
		class SplitScreenGame : public PaintingGame {
		public:
			SplitScreenGame(GameAssets* assets);
			virtual ~SplitScreenGame();
			PlayerBase* AddPlayer(Camera* camera, Vector3 position, Gamepad* gamepad = nullptr) override;
			void Update(float dt) override;

		protected:
			//void InitWorld() override;
			void InitPlayers();

		protected:
			//contains an array of players (2) only
			const int maxPlayers = 2;
			std::vector<PlayerBase*> players;
			std::vector<PlayerController*> playerControllers;
		};
	}
}

