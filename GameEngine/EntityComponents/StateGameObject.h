#pragma once
#include "GameObject.h"

namespace NCL {
    namespace CSC8508 {
        class StateMachine;
        class StateGameObject : public GameObject  {
        public:
            StateGameObject(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld);
            ~StateGameObject();

            virtual void Update(float dt);

        protected:
            void MoveLeft(float dt);
            void MoveRight(float dt);
            void MoveUp(float dt);
            void MoveDown(float dt);

            StateMachine* stateMachine;
            float counter;
        };
    }
}
