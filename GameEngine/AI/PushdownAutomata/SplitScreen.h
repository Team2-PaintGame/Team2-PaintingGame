#pragma once
#include "PushdownState.h"

namespace NCL {
	namespace CSC8508 {

		class SplitScreen : public PushdownState
		{
		public:
			SplitScreen();
			~SplitScreen();
			PushdownResult OnUpdate(float dt, PushdownState** newState) override;
			void OnAwake() override;
		};

	}
}
