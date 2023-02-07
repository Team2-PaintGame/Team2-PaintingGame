#pragma once
#include "PushdownState.h"

namespace NCL {
	namespace CSC8508 {

		class IntroScreen : public PushdownState
		{
		public:
			IntroScreen();
			~IntroScreen();
			PushdownResult OnUpdate(float dt, PushdownState** newState) override;
			void OnAwake() override;
		protected:
			bool isSinglePlayer, isSplitScreen, isLanScreen, isExit;
		};

	}
}