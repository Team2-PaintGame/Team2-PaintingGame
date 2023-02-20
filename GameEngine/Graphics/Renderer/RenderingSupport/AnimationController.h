#pragma once

namespace NCL {
	class MeshAnimation;
	namespace CSC8508 {
		class RenderObject;
		class GameObject;
		class StateTransition;
		class State;
		class StateMachine;
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
	NCL::CSC8508::State* idleState = nullptr;
	NCL::CSC8508::State* moveState = nullptr;
	NCL::CSC8508::State* tauntState = nullptr;
	NCL::CSC8508::StateMachine* animStateMachine = nullptr;
	NCL::CSC8508::StateTransition* idleToMoveStateTransition = nullptr;
	NCL::CSC8508::StateTransition* MoveToIdleStateTransition = nullptr;

	NCL::MeshAnimation* IdleAnimation = nullptr;
	NCL::MeshAnimation* tauntAnimation = nullptr;
	NCL::MeshAnimation* movingAnimation = nullptr;
	//NCL::CSC8508::RenderObject* renderObject;
	NCL::CSC8508::GameObject* gameObject = nullptr;
	void SetCurrentAnimation(NCL::MeshAnimation* meshAnim);
};