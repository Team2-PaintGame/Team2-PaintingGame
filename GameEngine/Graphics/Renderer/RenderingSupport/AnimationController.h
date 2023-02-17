#pragma once

namespace NCL {
	class MeshAnimation;
	namespace CSC8508 {
		class RenderObject;
	}
}

class AnimationController {
public:
	AnimationController();
	void SetIdleAnimation(NCL::MeshAnimation* meshAnim);
	void SetTauntAnimation(NCL::MeshAnimation* meshAnim);
	void SetRunAnimation(NCL::MeshAnimation* meshAnim);
	
	void SetRenderer(NCL::CSC8508::RenderObject* renderObj);
	void UpdateAnimations(float dt);
protected:
	NCL::MeshAnimation* IdleAnimation;
	NCL::MeshAnimation* tauntAnimation;
	NCL::MeshAnimation* runAnimation;
	NCL::CSC8508::RenderObject* renderObject;
	void SetCurrentAnimation(NCL::MeshAnimation* meshAnim);
};