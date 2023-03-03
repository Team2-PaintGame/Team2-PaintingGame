#pragma once
#include "RenderObject.h"
#include "GameWorld.h"

namespace NCL::CSC8508 {
	class SceneNode {
	public:
		SceneNode() = default;
		SceneNode(MeshGeometry* mesh, ShaderBase* shader, TextureBase* texture) { SetRenderObject(mesh, shader, texture); }
		RenderObject* GetRenderObject() const { return renderObject; }
		void SetRenderObject(RenderObject* newObject) {	renderObject = newObject; }
		void SetRenderObject(MeshGeometry* mesh, ShaderBase* shader, TextureBase* texture) {
			renderObject = new RenderObject(NULL, mesh, shader);
			renderObject->SetDefaultTexture(texture);
		}
		virtual void Update(float dt) {}
		virtual GameWorld* GetWorld() const { return nullptr; }
	protected:
		RenderObject* renderObject = NULL;
	};
}