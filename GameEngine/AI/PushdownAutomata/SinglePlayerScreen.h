#pragma once
#include "PushdownState.h"
#include <Window.h>
#include "../../../../PaintingGame/PaintingGame.h"

namespace NCL {
	class MenuHandler;
	namespace CSC8508 {

		class SinglePlayerScreen : public PushdownState
		{
		public:
			SinglePlayerScreen(Window* window, MenuHandler* menu);
			~SinglePlayerScreen();
			PushdownResult OnUpdate(float dt, PushdownState** newState) override;
			void OnAwake() override;
		protected:
			bool isPlayingGame;
			Window* window;
			PaintingGame* paintingGame;
			MenuHandler* menuHandler;
		};
	}
}
