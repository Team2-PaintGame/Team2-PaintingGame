
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
		//typedef std::function<void(Camera*)> CameraFunc;
		enum class Team {
			Red,
			Blue,
			None
		};
		class PaintingGame : public SceneNode {
		public:
			PaintingGame(GameAssets* assets);
			~PaintingGame();
			virtual void Update(float dt);
			void Restart() { InitWorld(); }
			virtual GameWorld* GetWorld() const { return world; }
			virtual void OperateOnCameras(CameraFunc f);
		protected:
			virtual void InitWorld();
			virtual void CreateSplatOnShoot() = 0;
			virtual Player* CreatePlayer(Vector3 position, Team team);
			virtual Player* AddPlayer(Vector3 position, Team team) = 0;

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

			//container for cameras
			std::vector<Camera*> activeCameras;
			
		};
	}
}