#pragma once
#include "PushdownState.h"
#include <Window.h>
#include "../../../../PaintingGame/PaintingGame.h"

namespace NCL {
	class MenuHandler;
	namespace CSC8508 {

		class SplitScreen : public PushdownState
		{
		public:
			SplitScreen(Window* window, MenuHandler * menu);
			~SplitScreen();
			PushdownResult OnUpdate(float dt, PushdownState** newState) override;
			void OnAwake() override;
			void OnSleep() override;
		protected:
			Window* window;
			PaintingGame* paintingGame;
			bool isPlayingGame;
			MenuHandler* menuHandler;
		};

	}
}