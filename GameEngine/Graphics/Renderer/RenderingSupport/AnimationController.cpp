#include "AnimationController.h"
#include "MeshAnimation.h"
#include "RenderObject.h"
#include "Transform.h"

AnimationController::AnimationController()
{
	
}

void AnimationController::SetIdleAnimation(NCL::MeshAnimation* meshAnim)
{
	IdleAnimation = meshAnim;
	//renderObject->SetRigged(true);
	//renderObject->animation = meshAnim;
}

void AnimationController::SetTauntAnimation(NCL::MeshAnimation* meshAnim)
{
	tauntAnimation = meshAnim;
	//renderObject->SetRigged(true);
	//renderObject->animation = meshAnim;
}

void AnimationController::SetRunAnimation(NCL::MeshAnimation* meshAnim)
{
	runAnimation = meshAnim;
	//renderObject->SetRigged(true);
	//renderObject->animation = meshAnim;
}

void AnimationController::SetCurrentAnimation(NCL::MeshAnimation* meshAnim)
{
	renderObject->SetRigged(true);
	renderObject->animation = meshAnim;
}

void AnimationController::SetRenderer(NCL::CSC8508::RenderObject* renderObj)
{
	this->renderObject = renderObj;
}

void AnimationController::UpdateAnimations(float dt)
{
	if (renderObject->GetTransform()->GetPosition().x > 0) {
		SetCurrentAnimation(runAnimation);
	}
	else {
		SetCurrentAnimation(IdleAnimation);
	}

	if (renderObject == nullptr && !renderObject->IsRigged()) return;

	renderObject->frameTime -= dt;
	while (renderObject->frameTime < 0.0f) {
		renderObject->currentFrame = (renderObject->currentFrame + 1) % renderObject->animation->GetFrameCount();
		renderObject->frameTime += 1.0f / renderObject->animation->GetFrameRate();
	}
	
	
}


