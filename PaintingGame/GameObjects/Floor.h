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
	class Floor : public GameObject {
	public:
		Floor() = default;
		Floor(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 position, MeshGeometry* mesh, TextureBase* texture, ShaderBase* shader, TextureBase* noiseTex, int size) : GameObject(physicsCommon, physicsWorld, "Floor") {
			transform
				.SetScale(Vector3(size, 1, size))
				.SetPosition(position);

			/*renderObject = new RenderObject(&transform, mesh, shader);
			renderObject->AddTexture(texture);*/
			renderObject = new RenderObject(&transform, mesh, texture, shader, noiseTex);

			boundingVolume = physicsCommon.createBoxShape(~transform.GetScale()/2.0f);
			reactphysics3d::Transform rp3d_transform(~position, rp3d::Quaternion::identity());

			// Create a rigid body in the physics world
			rigidBody = physicsWorld->createRigidBody(rp3d_transform);
			rigidBody->addCollider(boundingVolume, reactphysics3d::Transform::identity()); //collider
			rigidBody->setType(reactphysics3d::BodyType::STATIC);
			rigidBody->updateMassPropertiesFromColliders();
		}

		virtual ~Floor() {
			physicsCommon.destroyBoxShape(boundingVolume);
		}
	protected:
		rp3d::BoxShape* boundingVolume;
	};
}

