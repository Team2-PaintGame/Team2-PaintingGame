#pragma once
#include "PushdownState.h"
#include <Window.h>
#include "../../../../PaintingGame/PaintingGame.h"

namespace NCL {
	namespace CSC8508 {

		class SinglePlayerScreen : public PushdownState
		{
		public:
			SinglePlayerScreen(Window* window, PaintingGame* paintingGame);
			~SinglePlayerScreen();
			PushdownResult OnUpdate(float dt, PushdownState** newState) override;
			void OnAwake() override;
			void OnSleep() override;
		protected:
			bool isPlayingGame;
			Window* window;
			PaintingGame* paintingGame;
		};
	}
}
