#include "GameObject.h"
#include "CollisionDetection.h"
#include "PhysicsObject.h"
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
}

GameObject::~GameObject()	{
	delete renderObject;
	delete networkObject;
}

void GameObject::UpdateTransform() {
	if (collisionBody) {
		transform.SetPosition(collisionBody->getTransform().getPosition());
		transform.SetOrientation(collisionBody->getTransform().getOrientation());
	}
}
