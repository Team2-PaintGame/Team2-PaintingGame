#pragma once
#include "PushdownState.h"
#include "SinglePlayerScreen.h"
#include "../../../../PaintingGame/PaintingGame.h"

namespace NCL {
	namespace CSC8508 {

		class PauseScreen : public PushdownState
		{
		public:
			PauseScreen(PaintingGame* paintingGame);
			~PauseScreen();
			PushdownResult OnUpdate(float dt, PushdownState** newState) override;
			void OnAwake() override;
			void OnSleep() override;
		protected:
			bool selectExitOption;
			PaintingGame* paintingGame;
		};

	}
}
