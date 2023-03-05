#pragma once
#include "PaintingGame.h"
#include <PlayerController.h>

namespace NCL {
	namespace CSC8508 {
		
		class SplitScreenGame : public PaintingGame {
		public:
			SplitScreenGame(GameAssets* assets);
			virtual ~SplitScreenGame();
			Player* AddPlayer(Vector3 position) override;
			void Update(float dt) override;

		protected:
			//void InitWorld() override;
			void InitPlayers();

		protected:
			//contains an array of players (2) only
			const int maxPlayers = 2;
			std::vector<Player*> players;
			std::vector<PlayerController*> playerControllers;
		};
	}
}

