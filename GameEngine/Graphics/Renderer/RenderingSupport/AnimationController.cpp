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
	animStateMachine = new StateMachine();

	
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
	gameObject->GetRenderObject()->currentFrame = 0;
	gameObject->GetRenderObject()->SetRigged(true);
	gameObject->GetRenderObject()->animation = meshAnim;
}

void AnimationController::SetRenderer(NCL::CSC8508::RenderObject* renderObj)
{
	this->gameObject->SetRenderObject(renderObj);
}

void AnimationController::UpdateAnimations(float dt)
{
	std::cout << gameObject->GetRigidBody()->getLinearVelocity().length() << "\n";

	if(gameObject->GetRigidBody()->getLinearVelocity().length() > 5.f) {
		SetCurrentAnimation(movingAnimation);
	}
	else{
		SetCurrentAnimation(IdleAnimation);
	}

	if(gameObject->GetRenderObject()->animation == nullptr && !gameObject->GetRenderObject()->IsRigged()) return;

	gameObject->GetRenderObject()->frameTime -= dt;

	while (gameObject->GetRenderObject()->frameTime < 0.0f) {
		gameObject->GetRenderObject()->currentFrame = (gameObject->GetRenderObject()->currentFrame + 1) % gameObject->GetRenderObject()->animation->GetFrameCount();
		gameObject->GetRenderObject()->frameTime += 1.0f / gameObject->GetRenderObject()->animation->GetFrameRate();
	}
}


