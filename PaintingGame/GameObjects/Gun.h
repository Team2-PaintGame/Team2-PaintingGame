#pragma once
#include "GameObject.h"
#include "MeshGeometry.h"
#include "MeshMaterial.h"
#include "ShaderBase.h"
#include "RenderObject.h"
#include "Utils.h"
#include "Ink.h"
#include "HudElement.h"

namespace NCL {
	using namespace Rendering;
	using namespace CSC8508;
	class Gun : public GameObject {
	public:
		Gun(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 position, MeshGeometry* mesh, MeshMaterial* material, ShaderBase* shader, int size, Vector4 color, Ink* ink, FocusPoint* reticle) : GameObject(physicsCommon, physicsWorld, "Gun") {
			transform
				.SetScale(Vector3(size))
				.SetPosition(position);

			renderObject = new CSC8508::RenderObject(&transform, mesh, shader);
			renderObject->LoadMaterialTextures(material);
			renderObject->SetColour(color);

			this->ink = ink;
			ink->SetLayer(Layer::Paint);
			this->reticle = reticle;
		}
		virtual void Update(float dt) {
			if (ink) {
				ink->GetTransform()
					.SetPosition(transform.GetPosition() + (transform.GetOrientation() * inkOffset))
					.SetOrientation(transform.GetOrientation());
			}
		}
		void Shoot() {
			ink->StartEmission();
		}

		bool didJustShoot() {
			return ink->isJustStarting();
		}

		bool isShooting() {
			return ink->isEmitting();
		}
	protected:
		Ink* ink = NULL;
		const Vector3 inkOffset = Vector3(0, 0.5, -2);
		FocusPoint* reticle = NULL;
	};
}
 
