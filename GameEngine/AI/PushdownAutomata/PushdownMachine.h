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
			bool Update(float dt);
			PushdownState* GetActiveState() const { return activeState; }
		protected:
			void Reset(uint32_t minsize = 1);
			PushdownState* activeState = NULL;
			PushdownState* initialState = NULL;

			std::stack<PushdownState*> stateStack;
		};
	}
}

