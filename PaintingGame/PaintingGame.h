#pragma once
#include "Player.h"
#include "GameAssets.h"
#include <Camera.h>
#include <GameWorld.h>
#include <DirectionalLight.h>
#include <SpotLight.h>
#include <PointLight.h>
#include "SceneNode.h"

class Gamepad;
class AnimationController;

namespace NCL {
	class MenuHandler;
	namespace CSC8508 {
		class PaintingGame : public SceneNode {
		public:
			PaintingGame(GameAssets* assets);
			~PaintingGame();
			virtual void Update(float dt);
			void Restart() { InitWorld(); }
			virtual GameWorld* GetWorld() const { return world; }
		protected:
			//virtual void InitCamera(Camera& camera, PlayerBase& focus, float aspect_multiplier = 1.0f);
			virtual void InitWorld();

			virtual Player* CreatePlayer(Vector3 position);
			virtual Player* AddPlayer(Vector3 position) { return nullptr; };

			GameWorld* world;

			bool useGravity = true;
			bool useFog = true;
		
			//Create a physics world 
			reactphysics3d::PhysicsCommon physicsCommon;
			reactphysics3d::PhysicsWorld* physicsWorld = NULL;
			GameAssets* assets;

			//containers for lights
			std::vector<DirectionalLight> directionalLights;
			std::vector<PointLight> pointLights;
			std::vector<SpotLight> spotLights;
		};
	}
}

