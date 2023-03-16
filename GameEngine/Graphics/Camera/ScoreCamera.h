#include <Camera.h>

namespace NCL::CSC8508 {
	class ScoreCamera : public Camera {
	public:
		ScoreCamera();
		~ScoreCamera() {}

	protected:
		float zoomFactor = 5.0f;
		float distanceFromFocus = 300.0f;
	};
}