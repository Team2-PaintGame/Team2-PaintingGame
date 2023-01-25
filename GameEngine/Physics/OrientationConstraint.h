#pragma once
#include "Constraint.h"

namespace NCL {
	namespace CSC8508 {
		class GameObject;

		class OrientationConstraint : public Constraint
		{
		public:
			OrientationConstraint(GameObject* a, GameObject* b);
			~OrientationConstraint();

			void UpdateConstraint(float dt) override;

		protected:
			GameObject* objectA;
			GameObject* objectB;

			float distance;
		};
	}
}

