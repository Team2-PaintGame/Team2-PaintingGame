#pragma once
#include <SceneNode.h>
#include "Transform.h"
#include "Quaternion.h"

namespace NCL {
	namespace CSC8508 {

			class LoadingSceneNode : public SceneNode {
			public:
				LoadingSceneNode(MeshGeometry* mesh, ShaderBase* shader, TextureBase* texture) : SceneNode(mesh, shader, texture) {
					renderObj = GetRenderObject();
					transform = renderObj->GetTransform();
				}

				void Update(float dt) override;

			protected:
				RenderObject* renderObj;
				Quaternion quat;
				float timer = 0;
				Transform* transform;
			};
	}
}
