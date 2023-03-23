#pragma once
#include "RenderObject.h"
#include "Transform.h"

namespace NCL {
	namespace CSC8508 {

		class HUDOnLoad : public RenderObject {
		public:
			HUDOnLoad(Transform* transform, MeshGeometry* mesh, ShaderBase* shader) : RenderObject(NULL, mesh, shader) {
				this->transform = transform;
			}
			void Update(float dt){
				timer += dt;
				transform->SetOrientation(Quaternion::AxisAngleToQuaterion(Vector3(0, 0, -1), 45 * timer));
			};
			float timer = 0;
		};
	}
}
