#pragma once
#include "StateMachine.h"
#include "State.h"
#include "StateTransition.h"
#include <map>
#include <string>

namespace NCL {
	class MeshAnimation;
	namespace CSC8508 {
		class GameObject;
		class AnimationController : public StateMachine {
		public:
			AnimationController(GameObject* gameObject, const std::unordered_map<std::string, MeshAnimation*>& animations);
			virtual void Update(float dt);
			MeshAnimation* GetCurrentAnimation() const { return currentAnimation; }
			int GetCurrentFrame() const { return currentFrame; }
		protected:
			MeshAnimation* currentAnimation;
			GameObject* gameObject;
			std::unordered_map<std::string, MeshAnimation*> animations;
			int currentFrame = 0;
			float frameTime = 0.0f;
		};
	}
}