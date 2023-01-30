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

//bool GameObject::GetBroadphaseAABB(Vector3&outSize) const {
//	if (!boundingVolume) {
//		return false;
//	}
//	outSize = broadphaseAABB;
//	return true;
//}

//void GameObject::UpdateBroadphaseAABB() {
//	if (!boundingVolume) {
//		return;
//	}
//	if (boundingVolume->type == VolumeType::AABB) {
//		broadphaseAABB = ((AABBVolume&)*boundingVolume).GetHalfDimensions();
//	}
//	else if (boundingVolume->type == VolumeType::Sphere) {
//		float r = ((SphereVolume&)*boundingVolume).GetRadius();
//		broadphaseAABB = Vector3(r, r, r);
//	}
//	else if (boundingVolume->type == VolumeType::OBB) {
//		Matrix3 mat = Matrix3(transform.GetOrientation());
//		mat = mat.Absolute();
//		Vector3 halfSizes = ((OBBVolume&)*boundingVolume).GetHalfDimensions();
//		broadphaseAABB = mat * halfSizes;
//	}
//}

//bool GameObject::Raycast(RayCollision& closestCollision, GameObject* target) const {
//
//	std::vector<int> activeLayers;
//	int layer = 0; // start at bit index 0
//
//	int layerMask = COLLISION_LAYER_MASK.at(this->GetLayer());
//	
//	while (layerMask != 0) { // If the number is 0, no bits are set
//
//		// check if the bit at the current index 0 is set
//		if ((layerMask & 1) == 1) {
//			activeLayers.push_back(layer);
//		}
//
//		// advance to the next bit position to check
//		layerMask = layerMask >> 1; // shift all bits one position to the right
//		layer = layer + 1;              // so we are now looking at the next index.
//	}
//	
//	//checking if this object can interact with the target game object
//	if (!(std::count(activeLayers.begin(), activeLayers.end(), (int)target->GetLayer())) && layerMask) {
//		return false;
//	}
//	if (!target->GetBoundingVolume() || !this->GetBoundingVolume()) { //objects might not be collideable etc...
//		return false;
//	}
//	Ray r(transform.GetPosition(), (transform.GetPosition() - target->GetTransform().GetPosition()).Normalised());
//
//	if (CollisionDetection::RayIntersection(r, *target, closestCollision)) {
//		closestCollision.node = target;
//		Debug::DrawLine(r.GetPosition(), closestCollision.collidedAt, Vector4(0, 0, 1, 1), 10);
//		return true;
//	}
//	return false;
//}
