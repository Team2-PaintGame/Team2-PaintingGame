#pragma once
#include "RenderObject.h"

namespace NCL::CSC8508 {
	class GameWorld;
	class SceneNode {
	public:
		SceneNode() = default;
		SceneNode(MeshGeometry* mesh, ShaderBase* shader, TextureBase* texture);
		RenderObject* GetRenderObject() const { return renderObject; }
		void SetRenderObject(RenderObject* newObject);
		void SetRenderObject(MeshGeometry* mesh, ShaderBase* shader, TextureBase* texture);
		virtual void Update(float dt) {}
		virtual GameWorld* GetWorld() const { return nullptr; }
	protected:
		RenderObject* renderObject = NULL;
	};
}