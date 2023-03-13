#pragma once
#include "PaintingGame.h"
#include <PlayerController.h>

namespace NCL {
	namespace CSC8508 {
		
		class SplitScreenGame : public PaintingGame {
		public:
			SplitScreenGame(GameAssets* assets);
			virtual ~SplitScreenGame();
			void Update(float dt) override;

		protected:
			Player* AddPlayer(Vector3 position, Team team) override;
			void InitPlayers();
			virtual void CreateSplatOnShoot();
		protected:
			//contains an array of players (2) only
			const int maxPlayers = 2;
			std::vector<Player*> players;
			std::vector<PlayerController*> playerControllers;
			PlayerControllerFactory* secondPlayerControllerFactory = NULL;
		};
	}
}

