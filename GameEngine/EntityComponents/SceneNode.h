#pragma once

namespace NCL::CSC8508 {
	class SceneNode {
	public:
		SceneNode() = default;
		virtual void Update(float dt) {};
	protected:
		RenderObject* renderObject = NULL;
	};
}