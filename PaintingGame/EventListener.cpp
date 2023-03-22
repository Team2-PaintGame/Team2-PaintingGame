#include "EventListener.h"
#include "GameWorld.h"
#include "Ink.h"
#include "SecurityGuard.h"
#include "Player.h"

using namespace NCL::CSC8508;

GameEventListener::GameEventListener(reactphysics3d::PhysicsWorld* physicsWorld, GameWorld* gameWorld) :
	physicsWorld(physicsWorld), gameWorld(gameWorld){
	physicsWorld->setEventListener(this);
}

void GameEventListener::onContact(const CollisionCallback::CallbackData& callbackData)
{
	for (reactphysics3d::uint p = 0; p < callbackData.getNbContactPairs(); p++) {
		reactphysics3d::CollisionCallback::ContactPair contactPair = callbackData.getContactPair(p);
		GameObject* obj = nullptr;
		if (void* userData = contactPair.getBody1()->getUserData())
		{
			obj = (GameObject*)userData;

			if (void* userData2 = contactPair.getBody2()->getUserData())
			{
				GameObject* obj2 = (GameObject*)userData2;
				if ((obj->GetLayer() == Layer::Player && obj2->GetLayer() == Layer::Enemy) && contactPair.getEventType() == ContactPair::EventType::ContactStart)
				{
					SecurityGuard* security = (SecurityGuard*)userData2;
					Player* player = (Player*)userData;
					player->SetHasRespawnedTrue();
					security->SetHasCaughtPlayerTrue();
					security->CaughtPlayer();
				}
				else if ((obj->GetLayer() == Layer::Enemy && obj2->GetLayer() == Layer::Player) && contactPair.getEventType() == ContactPair::EventType::ContactStart)
				{
					SecurityGuard* security = (SecurityGuard*)userData;
					Player* player = (Player*)userData2;
					player->SetHasRespawnedTrue();
					security->SetHasCaughtPlayerTrue();
					security->CaughtPlayer();
				}
			}
		}
	}
}

void GameEventListener::onTrigger(const reactphysics3d::OverlapCallback::CallbackData& callbackData)
{
    for (reactphysics3d::uint p = 0; p < callbackData.getNbOverlappingPairs(); p++) {

        reactphysics3d::OverlapCallback::OverlapPair overlapPair = callbackData.getOverlappingPair(p);

		if (overlapPair.getEventType() != reactphysics3d::OverlapCallback::OverlapPair::EventType::OverlapStart)
		{
			continue;
		}
        

		reactphysics3d::CollisionBody* cb = nullptr;
		GameObject* obj = nullptr;
		bool valid = true;
		
		if (void* userData = overlapPair.getBody1()->getUserData())
		{
			obj = (GameObject*) userData;
			cb = overlapPair.getBody1();
			valid &= obj->GetLayer() == Layer::Paint;

			// Check if we're not coliding with paint or player
			if (void* userData2 = overlapPair.getBody2()->getUserData())
			{
				GameObject* obj2 = (GameObject*)userData2;
				valid &= !(obj2->GetLayer() == Layer::Paint || obj2->GetLayer() == Layer::Player);

				if ((obj->GetLayer() == Layer::Paint && obj2->GetLayer() == Layer::Enemy)  )
				{
					SecurityGuard* security = (SecurityGuard*)userData2;
					security->SetIsBlindedTrue();
				}
				else if ((obj->GetLayer() == Layer::Enemy && obj2->GetLayer() == Layer::Paint))
				{
					SecurityGuard* security = (SecurityGuard*)userData;
					security->SetIsBlindedTrue();
				}






			}


		}
		else if (userData = overlapPair.getBody2()->getUserData())
		{
			cb = overlapPair.getBody2();
			obj = (GameObject*)userData;
			valid &= obj->GetLayer() == Layer::Paint;
		}

		if (obj && valid)
		{
			gameWorld->AddPaintedPosition(obj->GetTransform().GetPosition(), ((Particle*)obj)->GetColour());
			obj->SetActive(false);
			cb->setIsActive(false);
		}


		

       /* GameObjectIterator first;
        GameObjectIterator last;
        gameWorld->GetObjectIterators(first, last);

        for (auto iter = first; iter < last; iter++) 
        {
        	GameObject* obj = *iter;
            
        	if (obj->GetLayer() == Layer::Paint && obj->GetRigidBody() == overlapPair.getBody1())
            {
                gameWorld->AddPaintedPosition(obj->GetTransform().GetPosition());
                std::cout << "TRIGGER HitSphere collisions" << std::endl;
            }
        }*/
    }
}

