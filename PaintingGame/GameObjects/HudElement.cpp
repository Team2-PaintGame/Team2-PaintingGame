#include "HudElement.h"

float timer = 0;

void NCL::LoadingHud::Update(float dt)
{
	timer += dt;
	quat = Quaternion::AxisAngleToQuaterion(Vector3(0, 0, 1), 45 * timer);
	transform.SetOrientation(quat);
}
