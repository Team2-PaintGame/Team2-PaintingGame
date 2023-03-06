#pragma once
#include "PaintingGame.h"
#include <PlayerController.h>

namespace NCL {
	namespace CSC8508 {

		class SinglePlayerGame : public PaintingGame {
		public:
			SinglePlayerGame(GameAssets* assets);
			virtual ~SinglePlayerGame();
			void Update(float dt) override;
		protected:
			Player* AddPlayer(Vector3 position) override;
			Player* player = NULL;
			PlayerController* playerController;
		};
	}
}
