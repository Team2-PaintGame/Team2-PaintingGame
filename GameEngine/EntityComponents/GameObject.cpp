#include "GameObject.h"
#include "RenderObject.h"
#include "NetworkObject.h"
#include <Debug.h>

using namespace NCL::CSC8508;

GameObject::GameObject(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, string objectName): physicsCommon(physicsCommon), physicsWorld(physicsWorld) {
	name			= objectName;
	worldID			= -1;
	isActive		= true;
	renderObject	= nullptr;
	networkObject	= nullptr;
	rigidBody		= nullptr;
}

GameObject::~GameObject()	{
	delete networkObject;
}

void GameObject::UpdateTransform() {
	if (rigidBody) {
		transform.SetPosition(rigidBody->getTransform().getPosition());
		transform.SetOrientation(rigidBody->getTransform().getOrientation());
	}
}

//void NCL::CSC8508::GameObjectListener::onTrigger(const reactphysics3d::OverlapCallback::CallbackData& callbackData)
//{
//	for (int p = 0; p < callbackData.getNbOverlappingPairs(); p++) 
//	{
//		GameObjectIterator first;
//		GameObjectIterator last;
//		world->GetObjectIterators(first, last);
//
//		reactphysics3d::OverlapCallback::OverlapPair overlapPair = callbackData.getOverlappingPair(p);
//		
//		for (auto iter = first; iter < last; iter++) 
//		{
//			GameObject* obj = *iter;
//
//			if (obj->GetRigidBody() == overlapPair.getBody2()) {}
//		}
//
//	}
//}
