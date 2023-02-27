#pragma once

namespace NCL::CSC8508 {
	class SceneNode {
	public:
		SceneNode() = default;
		virtual void Update(float dt) {};
		RenderObject* GetRenderObject() const {
			return renderObject;
		}
		void SetRenderObject(RenderObject* newObject) {
			renderObject = newObject;
		}
	protected:
		RenderObject* renderObject = NULL;
	};
}