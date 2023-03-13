#pragma once
#include <stack>

namespace NCL {
	namespace CSC8508 {
		class PushdownState;

		class PushdownMachine
		{
		public:
			PushdownMachine(PushdownState* initialState);
			~PushdownMachine();
			void Reset();
			bool Update(float dt);
			PushdownState* GetActiveState() const { return activeState; }
		protected:
			PushdownState* activeState = NULL;
			PushdownState* initialState = NULL;

			std::stack<PushdownState*> stateStack;
		};
	}
}

