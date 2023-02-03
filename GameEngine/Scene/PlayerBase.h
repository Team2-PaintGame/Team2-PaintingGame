#pragma once
#include "GameObject.h"
#include "MeshGeometry.h"
#include "TextureBase.h"
#include "ShaderBase.h"

namespace NCL {
	using namespace Rendering;
	using namespace CSC8508;
	class PlayerBase : public GameObject {
	public:
		PlayerBase() = default;
		PlayerBase(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 position, MeshGeometry* mesh, TextureBase* texture, ShaderBase* shader, int size);
		virtual void Update(float dt);
		virtual ~PlayerBase();
		//void ShootBullet();
	protected:
		float runSpeed = 50;	// unit: m/s
		float turnSpeed = 100.0f; // unit: degrees/s
		float currentRunSpeed = 0.0f;
		float currentTurnSpeed = 0.0f;
		float timeSinceLastShot;
		rp3d::BoxShape* boundingVolume;
		//rp3d::Collider* collider;
	};
}

