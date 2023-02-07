#pragma once
#include "PushdownState.h"

namespace NCL {
	namespace CSC8508 {

		class PauseScreen : public PushdownState
		{
		public:
			PauseScreen();
			~PauseScreen();
			PushdownResult OnUpdate(float dt, PushdownState** newState) override;
			void OnAwake() override;
		};

	}
}
