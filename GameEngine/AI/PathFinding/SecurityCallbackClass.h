#pragma once
#include <reactphysics3d/reactphysics3d.h>
#include "GameObject.h"

namespace NCL {
    using namespace reactphysics3d;
	class SecurityCallbackClass : public RaycastCallback {
    public:
        SecurityCallbackClass(CSC8508::GameObject* player) {
            this->player = player;
        }

        virtual decimal notifyRaycastHit(const RaycastInfo& info) {

            if (info.body != NULL && info.hitFraction <= closestObject) {
                closestObject = info.hitFraction;
            }
            if (info.body == player->GetRigidBody()) {
                playerDistance = info.hitFraction;
            }
            // Return a fraction of 1.0 to gather all hits 
            return decimal(1.0);
        }
        bool IsPlayerVisible() {
            if (playerDistance == closestObject) {
                return true;
            }
            else
                return false;
        }
        void ResetObjectDistances() {
            closestObject = 1.0;
            playerDistance = 1.0;
        }
    protected:
        CSC8508::GameObject* player;
        float closestObject = 1.0;
        float playerDistance = 1.0;

	};

}