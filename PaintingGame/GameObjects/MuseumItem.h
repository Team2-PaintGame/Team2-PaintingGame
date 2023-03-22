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


	class MuseumItem : public GameObject {
	public:

		MuseumItem(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 position, MeshGeometry* mesh, TextureBase* texture, ShaderBase* shader, Vector3 size, std::string name)
			: GameObject(physicsCommon, physicsWorld, name)
		{
			transform.SetScale(size).SetPosition(position);
			renderObject = new RenderObject(&transform, mesh, shader);
			//renderObject->AddTexture(texture);

			int subMeshes = mesh->GetSubMeshCount();
			for (int index = 0; index < subMeshes; ++index) {
				renderObject->AddTexture(texture, "mainTex", index);
			}

			boundingVolume = physicsCommon.createBoxShape(~transform.GetScale() / 2.0f);
			reactphysics3d::Transform rp3d_transform(~position, rp3d::Quaternion::identity());

			// Create a rigid body in the physics world
			rigidBody = physicsWorld->createRigidBody(rp3d_transform);
			rigidBody->addCollider(boundingVolume, reactphysics3d::Transform::identity()); //collider
			rigidBody->setType(reactphysics3d::BodyType::STATIC);
			rigidBody->updateMassPropertiesFromColliders();
		}	

	protected:
		rp3d::BoxShape* boundingVolume;

	};
}