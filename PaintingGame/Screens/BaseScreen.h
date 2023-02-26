#pragma once
#include "PushdownState.h"

namespace NCL {
	namespace CSC8508 {

		class BaseScreen : public PushdownState
		{
		public:
			BaseScreen();
			~BaseScreen();
			PushdownResult OnUpdate(float dt, PushdownState** newState) override;
			void OnAwake() override;
		protected:
			
		};

	}
}
