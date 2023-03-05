#include "AnimationController.h"
#include "MeshAnimation.h"
#include "GameObject.h"

using namespace NCL;
using namespace NCL::CSC8508;

AnimationController::AnimationController(GameObject* gameObject, std::unordered_map<std::string, MeshAnimation*> animations) {
	this->gameObject = gameObject;
	State* idleState = new State([&](float dt)->void {
		currentAnimation = animations.at("idleAnimation");
		});
	State* moveState = new State([&](float dt)->void {
		currentAnimation = animations.at("moveAnimation");
		});

	StateTransition* idleToMoveStateTransition = new StateTransition(idleState, moveState, [&](void)->bool {

		if (gameObject->GetRigidBody()->getLinearVelocity().length() > 5.f) {
			currentFrame = 0;
			return true;
		}
		return false;
	});

	StateTransition* moveToIdleStateTransition = new StateTransition(moveState, idleState, [&](void)->bool {

		if (gameObject->GetRigidBody()->getLinearVelocity().length() < 5.f) {
			currentFrame = 0;
			return true;
		}
		return false;
	});

	this->AddState(idleState);
	this->AddState(moveState);
	this->AddTransition(idleToMoveStateTransition);
	this->AddTransition(moveToIdleStateTransition);
}
void AnimationController::Update(float dt) {
	StateMachine::Update(dt);

	frameTime -= dt;
	while (frameTime < 0.0f) {
		currentFrame = (currentFrame + 1) % currentAnimation->GetFrameCount();
		frameTime += 1.0f / currentAnimation->GetFrameRate();
	}
}
