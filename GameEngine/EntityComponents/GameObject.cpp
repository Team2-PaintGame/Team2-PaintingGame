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
}

GameObject::~GameObject()	{
	delete renderObject;
	delete networkObject;

	/*if (rigidBody) {
		physicsWorld->destroyRigidBody(rigidBody);
	}*/
}

void GameObject::UpdateTransform() {
	if (rigidBody) {
		transform.SetPosition(rigidBody->getTransform().getPosition());
		transform.SetOrientation(rigidBody->getTransform().getOrientation());
	}
}
