#pragma once
#include "AnimatedObject.h"
#include "MeshGeometry.h"
#include "TextureBase.h"
#include "ShaderBase.h"
#include "MeshMaterial.h"
#include "Camera.h"
#include "Utils.h"



namespace NCL {
	using namespace Rendering;
	using namespace CSC8508;
	namespace CSC8508 {
		class RaycastManager;
	}
	

	class PlayerBase : public AnimatedObject {
	public:
		PlayerBase() = default;
		//TextureBase Constructor
		PlayerBase(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 position, 
			MeshGeometry* mesh, TextureBase* texture, ShaderBase* shader, const std::unordered_map<std::string, MeshAnimation*>& animations, 
			int size, std::string objectName = "", bool networked = false);
		//Mesh Material Constructor
		PlayerBase(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 position, 
			MeshGeometry* mesh, MeshMaterial* meshMaterial, ShaderBase* shader, const std::unordered_map<std::string, MeshAnimation*>& animations,
			int size, std::string objectName = "", bool networked = false);
		virtual ~PlayerBase();
		Camera* GetCamera() const { return camera; }
		virtual void Update(float dt);
		void SetYawPitch(float dx, float dy);
		float GetPitch() const { return pitch; }
		void SetPitch(float ptch) { pitch = std::clamp(ptch, -25.f, 25.0f); } // used for networking, sorry
		float GetYaw() const { return yaw; }
		virtual void Shoot() {}

		Vector3 GetSpawnPosition() {
			return spawnPosition;
		}

	protected:
		float	yaw = 0.0f;
		float	pitch = 0.0f;
		Vector3 spawnPosition;
		void SetMemberVariables(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 position, MeshGeometry* mesh, ShaderBase* shader, int size, bool networked);
		Camera* camera = nullptr;
		reactphysics3d::Ray ray = reactphysics3d::Ray(~Maths::Vector3(0), ~Maths::Vector3(0));
		void CameraSpring(Camera* cam);

		RaycastManager* raycastManager = NULL;
	};
}

