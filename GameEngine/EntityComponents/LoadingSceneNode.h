#pragma once
#include <SceneNode.h>

namespace NCL {
	namespace CSC8508 {
		class LoadingSceneNode : public SceneNode {
		public:
			LoadingSceneNode(MeshGeometry* mesh,ShaderBase* shader,TextureBase* texture);

			void Update(float dt) override;
		};
	}
}
