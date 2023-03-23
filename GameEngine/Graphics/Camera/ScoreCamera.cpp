#include "ScoreCamera.h"
#include "Window.h"

NCL::CSC8508::ScoreCamera::ScoreCamera()
{
	int width = 1280;
	int height = 720;


	znear = -1.0f;
	zfar = 10000.0f;

	left = -width / 2.0f;
	right = width / 2.0f;
	top = height / 2.0f;
	bottom = -height / 2.0f;

	left = left / zoomFactor;
	right = right / zoomFactor;
	top = top / zoomFactor;
	bottom = bottom / zoomFactor;


	pitch = -90.0f;
	yaw = 0.0f;

	camType = CameraType::Orthographic;

	position = Vector3(17.0f, distanceFromFocus, -20.0f);

}