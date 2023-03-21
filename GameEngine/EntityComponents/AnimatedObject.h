#pragma once
#include "GameObject.h"
#include "AnimationController.h"

namespace NCL {
	using namespace Rendering;
	using namespace CSC8508;

//	namespace CSC8508 { class GameObject; }
	class AnimatedObject : public GameObject {
	public:
		//Mesh Material Constructor
		AnimatedObject(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 position, 
			MeshGeometry* mesh, MeshMaterial* meshMaterial, ShaderBase* shader, const std::unordered_map<std::string, MeshAnimation*>& animations,
			int size, std::string objectName = "");
		//TextureBase Constructor
		AnimatedObject(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 position,
			MeshGeometry* mesh, TextureBase* texture, ShaderBase* shader, const std::unordered_map<std::string, MeshAnimation*>& animations,
			int size, std::string objectName = "");

		virtual ~AnimatedObject();
		virtual void Update(float dt);
		void SetIsMoving(bool val) { isMoving = val; }
		bool GetIsMoving() const { return isMoving; }

		void SetAnimControler(const std::unordered_map<std::string, MeshAnimation*>& animations);

	protected:
		AnimationController* animationController = NULL;
		bool isMoving = false;

	};









}
