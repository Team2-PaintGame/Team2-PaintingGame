#pragma once
#include "GameObject.h"
#include "MeshGeometry.h"
#include "TextureBase.h"
#include "ShaderBase.h"
#include "RenderObject.h"

namespace NCL {
	using namespace Rendering;
	using namespace CSC8508;
	class Floor : public GameObject {
	public:
		Floor() = default;
		Floor(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 position, MeshGeometry* mesh, TextureBase* texture, ShaderBase* shader, int size) : GameObject(physicsCommon, physicsWorld) {
			name = "Floor";
			transform
				.SetScale(Vector3(size, 1, size))
				.SetPosition(position);

			renderObject = new RenderObject(&transform, mesh, shader);
			renderObject->AddTexture(0, texture, "mainTex");

			boundingVolume = physicsCommon.createBoxShape(rp3d::Vector3(size / 2.0f, 1 / 2.0f, size / 2.0f));
			reactphysics3d::Transform rp3d_transform(rp3d::Vector3(position.x, position.y, position.z), rp3d::Quaternion::identity());

			// Create a rigid body in the physics world
			collisionBody = physicsWorld->createRigidBody(rp3d_transform);
			collisionBody->addCollider(boundingVolume, reactphysics3d::Transform::identity()); //collider
			dynamic_cast<rp3d::RigidBody*>(collisionBody)->setType(reactphysics3d::BodyType::STATIC);
			dynamic_cast<rp3d::RigidBody*>(collisionBody)->updateMassPropertiesFromColliders();
		}
		virtual ~Floor() {
			if (collisionBody) {
				physicsWorld->destroyRigidBody(dynamic_cast<rp3d::RigidBody*>(collisionBody));
			}
			physicsCommon.destroyBoxShape(boundingVolume);
		}
	protected:
		rp3d::BoxShape* boundingVolume;
	};
}

