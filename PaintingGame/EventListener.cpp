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
        
        GameObjectIterator first;
        GameObjectIterator last;
        gameWorld->GetObjectIterators(first, last);

        for (auto iter = first; iter < last; iter++) 
        {
        	GameObject* obj = *iter;
            
        	if (obj->GetWorldID() == 23 /*21 for particles*/ && obj->GetRigidBody() == overlapPair.getBody1())
            {
                gameWorld->AddPaintedPosition(obj->GetTransform().GetPosition());
                std::cout << "TRIGGER HitSphere collisions" << std::endl;
            }
        }
    }
}

