#include "AnimationController.h"
#include "MeshAnimation.h"
#include "RenderObject.h"
#include "Transform.h"
#include "GameObject.h"

AnimationController::AnimationController()
{
	
}

void AnimationController::SetGameObject(NCL::CSC8508::GameObject* gameObj)
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
	gameObject->GetRenderObject()->animation = nullptr;
	gameObject->GetRenderObject()->animation = meshAnim;
}

void AnimationController::SetRenderer(NCL::CSC8508::RenderObject* renderObj)
{
	this->gameObject->SetRenderObject(renderObj);
}

void AnimationController::UpdateAnimations(float dt)
{
	std::cout << gameObject->GetRigidBody()->getLinearVelocity().length() << "\n";

	if (gameObject->GetRigidBody()->getLinearVelocity().length() > 5.f) {
		SetCurrentAnimation(movingAnimation);
	}
	else{
		SetCurrentAnimation(IdleAnimation);
	}

	if (gameObject->GetRenderObject() == nullptr && !gameObject->GetRenderObject()->IsRigged()) return;

	gameObject->GetRenderObject()->frameTime -= dt;

	while (gameObject->GetRenderObject()->frameTime < 0.0f) {
		gameObject->GetRenderObject()->currentFrame = (gameObject->GetRenderObject()->currentFrame + 1) % gameObject->GetRenderObject()->animation->GetFrameCount();
		gameObject->GetRenderObject()->frameTime += 1.0f / gameObject->GetRenderObject()->animation->GetFrameRate();
	}
	
	
}


