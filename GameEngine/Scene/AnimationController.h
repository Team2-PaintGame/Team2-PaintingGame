#pragma once
#include "StateMachine.h"
#include "State.h"
#include "StateTransition.h"

#include <map>
#include <unordered_map>
#include <string>

namespace NCL {
	class MeshAnimation;
	class PlayerBase;
	class AnimatedObject;
	namespace CSC8508 {
		class AnimationController : public StateMachine {
		public:
			AnimationController(/*PlayerBase**/AnimatedObject* gameObject, const std::unordered_map<std::string, MeshAnimation*>& animations);
			virtual void Update(float dt);
			MeshAnimation* GetCurrentAnimation() const { return currentAnimation; }
			int GetCurrentFrame() const { return currentFrame; }
		protected:
			MeshAnimation* currentAnimation;
			/*PlayerBase**/
			AnimatedObject* gameObject;
			std::unordered_map<std::string, MeshAnimation*> animations;
			int currentFrame = 0;
			float frameTime = 0.0f;
		};
	}
}