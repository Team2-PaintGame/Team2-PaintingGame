#pragma once
#include "GameObject.h"
#include "MeshGeometry.h"
#include "MeshMaterial.h"
#include "ShaderBase.h"
#include "RenderObject.h"
#include "Utils.h"
#include "Ink.h"

namespace NCL {
	using namespace Rendering;
	using namespace CSC8508;
	class Gun : public GameObject {
	public:
		Gun(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 position, MeshGeometry* mesh, MeshMaterial* material, ShaderBase* shader, int size, Vector4 color, Ink* ink) : GameObject(physicsCommon, physicsWorld, "Gun") {
			transform
				.SetScale(Vector3(size))
				.SetPosition(position);

			renderObject = new RenderObject(&transform, mesh, shader);
			renderObject->LoadMaterialTextures(material);
			renderObject->SetColour(color);

			this->ink = ink;
		}
		virtual void Update(float dt) {
			if (ink) {
				ink->GetTransform()
					.SetPosition(transform.GetPosition() + (transform.GetOrientation() * inkOffset))
					.SetOrientation(transform.GetOrientation());
			}
		}
	protected:
		Ink* ink;
		const Vector3 inkOffset = Vector3(0, 0.5, -2);
	};
}
 
