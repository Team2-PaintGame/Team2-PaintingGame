#pragma once
#include "PaintingGame.h"
#include <PlayerController.h>

namespace NCL {
	namespace CSC8508 {

		class SinglePlayerGame : public PaintingGame {
		public:
			SinglePlayerGame(GameAssets* assets);
			virtual ~SinglePlayerGame();
			Player* AddPlayer(Vector3 position) override;
			void Update(float dt) override;

		protected:
			//void InitWorld() override;

		protected:
			Player* player = NULL;
			PlayerController* playerController;
		};
	}
}
