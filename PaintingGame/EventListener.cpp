#include "EventListener.h"
#include "GameWorld.h"


using namespace NCL::CSC8508;

GameEventListener::GameEventListener(reactphysics3d::PhysicsWorld* physicsWorld, GameWorld* gameWorld) :
	physicsWorld(physicsWorld), gameWorld(gameWorld){
	physicsWorld->setEventListener(this);
}

void GameEventListener::onContact(const CollisionCallback::CallbackData& callbackData)
{
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
		if (void* userData = overlapPair.getBody1()->getUserData())
		{
			obj = (GameObject*) userData;
			cb = overlapPair.getBody1();
		}
		else if (userData = overlapPair.getBody2()->getUserData())
		{
			cb = overlapPair.getBody2();
			obj = (GameObject*)userData;
		}

		if (obj && obj->GetLayer() == Layer::Paint)
		{
			gameWorld->AddPaintedPosition(obj->GetTransform().GetPosition());
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

