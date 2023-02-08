#pragma once
#include "PushdownState.h"
#include <Window.h>
#include "../../../../PaintingGame/PaintingGame.h"

namespace NCL {
	namespace CSC8508 {

		class SplitScreen : public PushdownState
		{
		public:
			SplitScreen(Window* window, PaintingGame* paintingGame);
			~SplitScreen();
			PushdownResult OnUpdate(float dt, PushdownState** newState) override;
			void OnAwake() override;
		protected:
			Window* window;
			PaintingGame* paintingGame;
			bool isPlayingGame;
		};

	}
}
