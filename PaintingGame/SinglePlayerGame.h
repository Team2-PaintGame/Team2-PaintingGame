#pragma once
#include "PaintingGame.h"
#include <PlayerController.h>

namespace NCL {
	namespace CSC8508 {

		class SinglePlayerGame : public PaintingGame {
		public:
			SinglePlayerGame(GameAssets* assets);
			virtual ~SinglePlayerGame();
			PlayerBase* AddPlayer(Camera* camera, Vector3 position, Gamepad* gamepad = nullptr) override;
			void Update(float dt) override;

		protected:
			//void InitWorld() override;

		protected:
			PlayerBase* player = NULL;
			PlayerController* playerController;
		};
	}
}
