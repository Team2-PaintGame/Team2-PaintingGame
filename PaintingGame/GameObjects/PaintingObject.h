#pragma once
#include "GameObject.h"
#include "MeshGeometry.h"
#include "TextureBase.h"
#include "ShaderBase.h"
#include "RenderObject.h"
#include "Utils.h"
#include "MeshMaterial.h"

namespace NCL {
	using namespace Rendering;
	using namespace CSC8508;
	class PaintingObject : public GameObject {
	public:
		PaintingObject() = default;
		PaintingObject(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 position,Quaternion orientation, MeshGeometry* mesh,
			MeshMaterial* meshMat, ShaderBase* shader, int size, string objectName) : GameObject(physicsCommon, physicsWorld, objectName) {
			transform
				.SetScale(Vector3(size, size, size / 10))
				.SetPosition(position)
				.SetOrientation(orientation);
				

			renderObject = new RenderObject(&transform, mesh, shader);
			renderObject->LoadMaterialTextures(meshMat);

			boundingVolume = physicsCommon.createBoxShape(~transform.GetScale() / 2.0f);
		//	reactphysics3d::Transform rp3d_transform(~position, rp3d::Quaternion::identity());
			reactphysics3d::Transform rp3d_transform(~position, ~orientation);

			// Create a rigid body in the physics world
			rigidBody = physicsWorld->createRigidBody(rp3d_transform);
			rigidBody->addCollider(boundingVolume, rp3d::Transform::identity()); //collider
			rigidBody->setMass(0);
			rigidBody->setAngularLockAxisFactor({ 0,0,0 });
			rigidBody->setLinearLockAxisFactor({ 0,0,0 });
			//rigidBody->updateMassPropertiesFromColliders();
		}
		
		virtual ~PaintingObject() {
			if (rigidBody) {
				physicsWorld->destroyRigidBody(rigidBody);
			}
			physicsCommon.destroyBoxShape((rp3d::BoxShape*)(boundingVolume));
		}
	protected:
	};
}
 

