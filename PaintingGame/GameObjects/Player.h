#pragma once
#include <PlayerBase.h>
#include "Window.h"

namespace NCL {
	using namespace Rendering;
	using namespace CSC8508;
	class Player : public PlayerBase {
	public:
		Player(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 position, MeshGeometry* mesh, TextureBase* texture, ShaderBase* shader, int size) : PlayerBase(physicsCommon, physicsWorld, position, mesh, texture, shader, size) {
			SetMemberVariables();
		}
		Player(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 position, MeshGeometry* mesh, MeshMaterial* material, ShaderBase* shader, int size) : PlayerBase(physicsCommon, physicsWorld, position, mesh, material, shader, size) {
			SetMemberVariables();
		}
		virtual ~Player() {
			delete animationController;
		}
	protected:
		void SetMemberVariables() {
			renderObject->SetRigged(true);

			camera.SetBasicCameraParameters(-15.0f, 315.0f, Vector3(-60, 40, 60), 0.1f, 500.0f);
			camera.SetPerspectiveCameraParameters(Window::GetWindow()->GetScreenAspect());
			camera.SetThirdPersonCamera(&transform);
		}
		AnimationController* animationController;

	};
}