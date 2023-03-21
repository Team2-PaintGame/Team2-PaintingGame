#include "LoadingSceneNode.h"


void NCL::CSC8508::LoadingSceneNode::Update(float dt)
{
	 timer += dt;
	 quat = Quaternion::AxisAngleToQuaterion(Vector3(0, 0, 1), 45 * timer);
	 transform->SetOrientation(quat);
}
