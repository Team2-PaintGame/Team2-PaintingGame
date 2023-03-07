#pragma once
#include "GameObject.h"
#include "MeshGeometry.h"
#include "TextureBase.h"
#include "ShaderBase.h"
#include "MeshMaterial.h"
#include "AnimationController.h"
#include "Camera.h"

namespace NCL {
	using namespace Rendering;
	using namespace CSC8508;

	class PlayerBase : public GameObject {
	public:
		PlayerBase() = default;
		PlayerBase(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 position, MeshGeometry* mesh, TextureBase* texture, ShaderBase* shader, int size);
		PlayerBase(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 position, MeshGeometry* mesh, MeshMaterial* meshMaterial, ShaderBase* shader, int size);
		virtual ~PlayerBase();
		Camera* GetCamera() const { return camera; }
		virtual void Update(float dt);
		void SetYawPitch(float dx, float dy);
		float GetPitch() const { return pitch; }
		float GetYaw() const { return yaw; }
		virtual void Shoot() {}
	protected:
		float	yaw = 0.0f;
		float	pitch = 0.0f;
		void SetMemberVariables(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 position, MeshGeometry* mesh, ShaderBase* shader, int size);
		Camera* camera;
		AnimationController* animationController;
	};
}

