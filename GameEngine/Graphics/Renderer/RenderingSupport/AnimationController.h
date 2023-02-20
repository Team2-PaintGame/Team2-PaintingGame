#pragma once

namespace NCL {
	class MeshAnimation;
	namespace CSC8508 {
		class RenderObject;
		class GameObject;
	}
}

class AnimationController {
public:
	AnimationController();
	void SetGameObject(NCL::CSC8508::GameObject* gameObj);
	void SetIdleAnimation(NCL::MeshAnimation* meshAnim);
	void SetTauntAnimation(NCL::MeshAnimation* meshAnim);
	void SetRunAnimation(NCL::MeshAnimation* meshAnim);
	
	void SetRenderer(NCL::CSC8508::RenderObject* renderObj);
	void UpdateAnimations(float dt);
protected:
	NCL::MeshAnimation* IdleAnimation;
	NCL::MeshAnimation* tauntAnimation;
	NCL::MeshAnimation* movingAnimation;
	//NCL::CSC8508::RenderObject* renderObject;
	NCL::CSC8508::GameObject* gameObject;
	void SetCurrentAnimation(NCL::MeshAnimation* meshAnim);
};