#pragma once
#include "Transform.h"
#include "Layer.h"
#include <reactphysics3d/reactphysics3d.h>
#include "SceneNode.h"
#include "Vector3.h"


using std::vector;

namespace NCL::CSC8508 {
	class NetworkObject;
	class RenderObject;
	class PhysicsObject;

	class GameObject :public SceneNode	{
	public:
		GameObject(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, std::string name = "");
		GameObject(std::string name = "");
		virtual ~GameObject();

		bool IsActive() const {
			return isActive;
		}

		void SetActive(bool state) {
			isActive = state;
		}

		Transform& GetTransform() {
			return transform;
		}

		reactphysics3d::RigidBody* GetRigidBody() {
			return rigidBody;
		}

		NetworkObject* GetNetworkObject() const {
			return networkObject;
		}

		void SetLayer(Layer l) {
			layer = l;
		}

		Layer GetLayer() const {
			return layer;
		}

		const std::string& GetName() const {
			return name;
		}

		virtual void OnCollisionBegin(GameObject* otherObject) {
			//std::cout << "OnCollisionBegin event occured!\n";
		}

		virtual void OnCollisionEnd(GameObject* otherObject) {
			//std::cout << "OnCollisionEnd event occured!\n";
		}


		void SetWorldID(int newID) {
			worldID = newID;
		}

		int		GetWorldID() const {
			return worldID;
		}

		void UpdateTransform();

		NCL::Maths::Vector3 collisionPoint;

	protected:
		/// Reference to the physics common object
		reactphysics3d::PhysicsCommon& physicsCommon;
		reactphysics3d::PhysicsWorld* physicsWorld;

		/// Body used to simulate the dynamics of the box
		reactphysics3d::RigidBody* rigidBody;
		rp3d::CollisionShape* boundingVolume;

		Transform			transform;

		NetworkObject*		networkObject;

		bool		isActive;
		int			worldID;
		std::string	name;

		Vector3 broadphaseAABB;
		Layer	layer;
	};
}

