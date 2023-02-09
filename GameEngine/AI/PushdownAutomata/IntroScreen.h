#pragma once
#include "PushdownState.h"
#include <Window.h>
#include "../../../../PaintingGame/PaintingGame.h"

namespace NCL {
	namespace CSC8508 {

		class IntroScreen : public PushdownState
		{
		public:
			IntroScreen(Window* window, PaintingGame* paintingGame);
			IntroScreen(Window* window);
			~IntroScreen();
			PushdownResult OnUpdate(float dt, PushdownState** newState) override;
			void OnAwake() override;
		protected:
			bool isLanScreen;
			Window* window;
			PaintingGame* paintingGame;
		};

	}
}