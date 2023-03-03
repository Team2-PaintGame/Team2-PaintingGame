#include "AnimationController.h"
#include "MeshAnimation.h"
#include "RenderObject.h"
#include "Transform.h"
#include "GameObject.h"
#include "StateMachine.h"
#include "State.h"
#include "StateTransition.h"

using namespace NCL;
using namespace NCL::CSC8508;

AnimationController::AnimationController()
{
	
}

void AnimationController::InitStateMachine()
{
	animStateMachine = new StateMachine();

	idleState = new State([&](float dt)->void {
		SetCurrentAnimation(IdleAnimation);
		});
	moveState = new State([&](float dt)->void {
		SetCurrentAnimation(movingAnimation);
		});

	idleToMoveStateTransition = new StateTransition(idleState, moveState, [&](void)->bool {

			if (gameObject->GetRigidBody()->getLinearVelocity().length() > 5.f)
			{
				gameObject->GetRenderObject()->currentFrame = 0;
				return true;
			}
			return false;
		});

	moveToIdleStateTransition = new StateTransition(moveState, idleState, [&](void)->bool {

			if (gameObject->GetRigidBody()->getLinearVelocity().length() < 5.f)
			{
				gameObject->GetRenderObject()->currentFrame = 0;
				return true;
			}
			return false;
		});

	animStateMachine->AddState(idleState);
	animStateMachine->AddState(moveState);
	animStateMachine->AddTransition(idleToMoveStateTransition);
	animStateMachine->AddTransition(moveToIdleStateTransition);
}

void AnimationController::SetGameObject(GameObject* gameObj)
{
	this->gameObject = gameObj;
}

void AnimationController::SetIdleAnimation(NCL::MeshAnimation* meshAnim)
{
	IdleAnimation = meshAnim;
}

void AnimationController::SetTauntAnimation(NCL::MeshAnimation* meshAnim)
{
	tauntAnimation = meshAnim;
}

void AnimationController::SetRunAnimation(NCL::MeshAnimation* meshAnim)
{
	movingAnimation = meshAnim;
}

void AnimationController::SetCurrentAnimation(NCL::MeshAnimation* meshAnim)
{
	gameObject->GetRenderObject()->SetRigged(true);
	gameObject->GetRenderObject()->SetMeshAnimation(meshAnim);
}

void AnimationController::SetRenderer(NCL::CSC8508::RenderObject* renderObj)
{
	this->gameObject->SetRenderObject(renderObj);
}

void AnimationController::UpdateAnimations(float dt)
{
	//std::cout << gameObject->GetRigidBody()->getLinearVelocity().length() << "\n";

	/*if(gameObject->GetRigidBody()->getLinearVelocity().length() > 5.f) {
		SetCurrentAnimation(movingAnimation);
	}
	else{
		SetCurrentAnimation(IdleAnimation);
	}*/

	if(gameObject->GetRenderObject()->GetMeshAnimation() == nullptr && animStateMachine == nullptr && !gameObject->GetRenderObject()->IsRigged()) return;

	animStateMachine->Update(dt);

	gameObject->GetRenderObject()->frameTime -= dt;

	while (gameObject->GetRenderObject()->frameTime < 0.0f) {
		gameObject->GetRenderObject()->currentFrame = (gameObject->GetRenderObject()->currentFrame + 1) % gameObject->GetRenderObject()->GetMeshAnimation()->GetFrameCount();
		gameObject->GetRenderObject()->frameTime += 1.0f / gameObject->GetRenderObject()->GetMeshAnimation()->GetFrameRate();
	}
}


