
#pragma once
#include "Player.h"
#include "GameAssets.h"
#include <Camera.h>
#include <GameWorld.h>
#include <DirectionalLight.h>
#include <SpotLight.h>
#include <PointLight.h>
#include "SceneNode.h"
#include "HudElement.h"
#ifdef _WIN32
#include "../Audio/SoundSystem.h"
#endif


class Gamepad;
class AnimationController;

namespace NCL {
	class MenuHandler;
	namespace CSC8508 {
		//typedef std::function<void(Camera*)> CameraFunc;
		enum class Team {
			Red,
			Blue
		};
		class PaintingGame : public SceneNode {
		public:
			PaintingGame(GameAssets* assets);
			~PaintingGame();
			virtual void Update(float dt);
			void Restart() { InitWorld(); }
			virtual GameWorld* GetWorld() const { return world; }
			bool GetTimeOver() { return gameTime <= 0.0f; }
			int GetWinner() { return world->CalculateWinningTeam(); }
			virtual reactphysics3d::PhysicsWorld* GetPhysicsWorld() const override { return physicsWorld; }
			virtual void OperateOnCameras(CameraFunc f);

		protected:
			virtual void InitWorld();
			virtual void CreateSplatOnShoot() = 0;
			Player* CreatePlayer(Vector3 position, Team team, bool networked = false);
			virtual Player* AddPlayer(Vector3 position, Team team) = 0;
			Gun* CreateGun(Vector3 position, Team team);
			virtual void AddSecurityAI(Vector3 position, PlayerBase* target1, PlayerBase* target2, uint32_t seed);
			GameWorld* world;

			bool useGravity = true;
			bool useFog = true;

			GameTimer timer;
			float gameTime = 40;

			std::string gameOverString;

			int maxSplats;
			unsigned int paintSplatSSBO;
			void SendPaintSplatData();


			//Create a physics world 
			reactphysics3d::PhysicsCommon physicsCommon;
			reactphysics3d::PhysicsWorld* physicsWorld = NULL;
			GameAssets* assets;

			//containers for lights
			std::vector<DirectionalLight> directionalLights;
			std::vector<PointLight> pointLights;
			std::vector<SpotLight> spotLights;
			Ink* ink;

			//container for cameras
			std::vector<Camera*> activeCameras;
			uint32_t seed;
		};
	}
}