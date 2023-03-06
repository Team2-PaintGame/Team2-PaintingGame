#pragma once
#include <PlayerBase.h>
#include "Window.h"

namespace NCL {
	using namespace Rendering;
	using namespace CSC8508;
	class Player : public PlayerBase {
	public:
		Player(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 position, MeshGeometry* mesh, TextureBase* texture, ShaderBase* shader, const std::unordered_map<std::string, MeshAnimation*>& animations, int size) : PlayerBase(physicsCommon, physicsWorld, position, mesh, texture, shader, size) {
			SetMemberVariables(animations);
		}
		Player(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 position, MeshGeometry* mesh, MeshMaterial* material, ShaderBase* shader, const std::unordered_map<std::string, MeshAnimation*>& animations, int size) : PlayerBase(physicsCommon, physicsWorld, position, mesh, material, shader, size) {
			SetMemberVariables(animations);
		}
		virtual ~Player() {
			delete animationController;
		}
		virtual void Update(float dt) {
			PlayerBase::Update(dt);
			animationController->Update(dt);
		}
	protected:
		void SetMemberVariables(const std::unordered_map<std::string, MeshAnimation*>& animations) {
			camera->SetBasicCameraParameters(-15.0f, 315.0f, Vector3(-60, 40, 60), 0.1f, 500.0f);
			camera->SetPerspectiveCameraParameters(Window::GetWindow()->GetScreenAspect());
			camera->SetThirdPersonCamera(&transform);

			animationController = new AnimationController(this, animations);
			renderObject->SetRigged(true);
			renderObject->SetAnimationController(animationController);
		}
		AnimationController* animationController;

	};
}