#pragma once
#include "PushdownState.h"
#include "SinglePlayerScreen.h"
#include "../../../../PaintingGame/PaintingGame.h"

namespace NCL {
	class MenuHandler;
	namespace CSC8508 {

		class PauseScreen : public PushdownState
		{
		public:
			PauseScreen(PaintingGame* paintingGame);
			~PauseScreen();
			PushdownResult OnUpdate(float dt, PushdownState** newState) override;
			void OnAwake() override;
		protected:
			bool selectExitOption;
			PaintingGame* paintingGame;
			MenuHandler* menuHandler;
		};

	}
}
