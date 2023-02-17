#pragma once



namespace NCL {
	class MeshAnimation;
	namespace CSC8508 {
		class RenderObject;
	}
}

class AnimationController {
public:
	void SetAnimation(NCL::MeshAnimation* meshAnim);
	void SetRenderer(NCL::CSC8508::RenderObject* renderObj) { this->renderObject = renderObj; };
	void UpdateAnimations(float dt);
protected:
	//NCL::MeshAnimation* meshAnimation;
	NCL::CSC8508::RenderObject* renderObject;
};