#pragma once
#include "PushdownState.h"

namespace NCL {
	namespace CSC8508 {

		class LanScreen : public PushdownState
		{
		public:
			LanScreen();
			~LanScreen();
			PushdownResult OnUpdate(float dt, PushdownState** newState) override;
			void OnAwake() override;
		};

	}
}