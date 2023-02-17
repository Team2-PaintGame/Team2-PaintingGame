#include "AnimationController.h"
#include "MeshAnimation.h"
#include "RenderObject.h"

void AnimationController::SetAnimation(NCL::MeshAnimation* meshAnim)
{
	//meshAnimation = meshAnim;
	renderObject->SetRigged(true);
	renderObject->animation = meshAnim;
}

void AnimationController::UpdateAnimations(float dt)
{
	if (renderObject == nullptr && !renderObject->IsRigged()) return;

	renderObject->frameTime -= dt;
	while (renderObject->frameTime < 0.0f) {
		renderObject->currentFrame = (renderObject->currentFrame + 1) % renderObject->animation->GetFrameCount();
		renderObject->frameTime += 1.0f / renderObject->animation->GetFrameRate();
	}
}
