#pragma once
#include "PaintingGame.h"
#include <PlayerController.h>

class ScoreManager;
namespace NCL {
	namespace CSC8508 {

		class SinglePlayerGame : public PaintingGame {
		public:
			SinglePlayerGame(GameAssets* assets);
			virtual ~SinglePlayerGame();
			void Update(float dt) override;
		protected:
			virtual void CreateSplatOnShoot();
			Player* AddPlayer(Vector3 position, PlayerBase::Team team) override;
			Player* player = NULL;
			PlayerController* playerController;
			ScoreManager* scoreManager = nullptr;
		};
	}
}
