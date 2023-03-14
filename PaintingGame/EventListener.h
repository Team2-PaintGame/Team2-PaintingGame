#pragma once

#include <reactphysics3d/reactphysics3d.h>

namespace NCL::CSC8508 {

	class GameWorld;
	class GameEventListener : public reactphysics3d::EventListener {
	public:
		GameEventListener(reactphysics3d::PhysicsWorld* physicsWorld, GameWorld* gameWorld);
		~GameEventListener() {};


		virtual void onContact(const CollisionCallback::CallbackData& callbackData) override;
		virtual void onTrigger(const reactphysics3d::OverlapCallback::CallbackData& callbackData) override;
	protected:
		reactphysics3d::PhysicsWorld* physicsWorld;
		GameWorld* gameWorld;
	};
}