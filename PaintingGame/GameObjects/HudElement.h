#pragma once
#include <Vector3.h>
#include <MeshGeometry.h>
#include <TextureBase.h>
#include <ShaderBase.h>
#include <GameObject.h>
#include "RenderObject.h"

namespace NCL {
	using namespace Rendering;
	using namespace CSC8508;
	class HudElement : public GameObject {
	public:
		HudElement(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector2 position, MeshGeometry* mesh, TextureBase* texture, ShaderBase* shader, Vector2 scale) : GameObject(physicsCommon, physicsWorld, "HUD") {
			transform
				.SetScale(scale)
				.SetPosition(position);

			renderObject = new CSC8508::RenderObject(&transform, mesh, shader);
			renderObject->SetDefaultTexture(texture);
			renderObject->SetIsOccluded(false);
		}
	};

	class FocusPoint : public HudElement {
	public:
		FocusPoint(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, MeshGeometry* mesh, TextureBase* texture, ShaderBase* shader, Vector2 scale) :
			HudElement(physicsCommon, physicsWorld, Vector2(), mesh, texture, shader, scale) {
		}
	};

	class LoadingHud : public HudElement {
	public:
		LoadingHud(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, MeshGeometry* mesh, TextureBase* texture, ShaderBase* shader,Vector2 pos, Vector2 scale) :
			HudElement(physicsCommon, physicsWorld, pos, mesh, texture, shader, scale) {
			renderObject->SetIsOccluded(true);
		}
		void Update(float dt);
	private:
		Quaternion quat;
	};
}

