#pragma once
#include "GameObject.h"
#include "MeshGeometry.h"
#include "TextureBase.h"
#include "ShaderBase.h"
#include "RenderObject.h"
#include "Utils.h"

namespace NCL {
	using namespace Rendering;
	using namespace CSC8508;
	class Box : public GameObject {
	public:
		Box() = default;
		Box(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 position, MeshGeometry* mesh, TextureBase* texture, ShaderBase* shader, int size) : GameObject(physicsCommon, physicsWorld, "Box") {
			transform
				.SetScale(Vector3(size))
				.SetPosition(position);

			renderObject = new RenderObject(&transform, mesh, shader);
			renderObject->SetDefaultTexture(texture);

			boundingVolume = physicsCommon.createBoxShape(~transform.GetScale() / 2.0f);
			reactphysics3d::Transform rp3d_transform(~position, rp3d::Quaternion::identity());

			// Create a rigid body in the physics world
			rigidBody = physicsWorld->createRigidBody(rp3d_transform);
			rigidBody->addCollider(boundingVolume, rp3d::Transform::identity()); //collider
			rigidBody->updateMassPropertiesFromColliders();
		}
		
		virtual ~Box() {
			if (rigidBody) {
				physicsWorld->destroyRigidBody(rigidBody);
			}
			physicsCommon.destroyBoxShape((rp3d::BoxShape*)(boundingVolume));
		}
	protected:
	};
}
 
