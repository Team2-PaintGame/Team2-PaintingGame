#pragma once
#include "PushdownState.h"
#include <Window.h>
#include "../../../../PaintingGame/PaintingGame.h"

namespace NCL {
	namespace CSC8508 {

		class LanScreen : public PushdownState
		{
		public:
			LanScreen(Window* window, PaintingGame* paintingGame);
			~LanScreen();
			PushdownResult OnUpdate(float dt, PushdownState** newState) override;
			void OnAwake() override;
		protected:
			bool isPlayingGame;
			Window* window;
			PaintingGame* paintingGame;
		};

	}
}