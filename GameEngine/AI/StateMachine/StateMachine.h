#pragma once
#include <vector>
#include <map>

namespace NCL {
	namespace CSC8508 {
		class State;
		class StateTransition;

		typedef std::multimap<State*, StateTransition*> TransitionContainer;
		typedef TransitionContainer::iterator TransitionIterator;

		class StateMachine	{
		public:
			StateMachine();
			virtual ~StateMachine(); //made it virtual!

			void AddState(State* s);
			void AddTransition(StateTransition* t);

			virtual void Update(float dt); //made it virtual!
			//virtual void IsStateChanged(StateUpdateFunction func);

		protected:
			State * activeState;

			std::vector<State*> allStates;
			TransitionContainer allTransitions;

			//StateUpdateFunction func;
		};
	}
}