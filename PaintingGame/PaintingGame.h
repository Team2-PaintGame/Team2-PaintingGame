#pragma once

#include <GameObject.h>
#include <GameTechRenderer.h>
#include <GameWorld.h>
#include <map>
#include <MeshMaterial.h>
#include <NavigationGrid.h>
#include <MeshAnimation.h>
#include <Vector4.h>
#include <Vector3.h>
#include <RenderObject.h>
#include <PlayerBase.h>
#include <PlayerController.h>
#include <reactphysics3d/reactphysics3d.h>
#include "InputController.h"
#include "AnimationController.h"

namespace NCL {
	class MenuHandler;
	namespace CSC8508 {
		class PaintingGame : public SceneNode {
		public:
#ifdef USEVULKAN
			PaintingGame(GameTechVulkanRenderer* render, GameWorld* world, reactphysics3d::PhysicsWorld* physicsWorld, reactphysics3d::PhysicsCommon* physicsCommon, MenuHandler* menu, bool online = false);
#else
			PaintingGame(GameTechRenderer* render, GameWorld* world, reactphysics3d::PhysicsCommon* physicsCommon, MenuHandler* menu, bool online = false);
#endif
			
			~PaintingGame();
			virtual void UpdateGame(float dt);
			GameTechRenderer* GetGameTechRenderer();

			void Restart() { InitWorld(); }

		protected:
			void InitialiseAssets();

			virtual void InitCamera(Camera& camera, PlayerBase& focus, float aspect_multiplier = 1.0f);
			virtual void InitWorld();

			virtual PlayerBase* CreatePlayer(Vector3 position);
			virtual PlayerBase* AddPlayer(Camera* camera, Vector3 position, Gamepad* gamepad = nullptr) { return nullptr; };
#ifdef USEVULKAN
			GameTechVulkanRenderer* renderer;
#else
			GameTechRenderer* renderer;
#endif
			GameWorld* world;

			bool useGravity = true;
			bool useFog = true;

			float		forceMagnitude;

			std::map<std::string, MeshGeometry*> meshes;
			std::map<std::string, MeshMaterial*> meshMaterials;
			std::map<std::string, MeshAnimation*> meshAnimations;

			std::map<std::string, TextureBase*> textures;
			std::map<std::string, ShaderBase*> shaders;

			//Coursework Additional functionality	
			
			//Create a physics world 
			reactphysics3d::PhysicsCommon* physicsCommon;
			reactphysics3d::PhysicsWorld* physicsWorld = NULL;

			//UI
			MenuHandler* menuHandler;

			AnimationController* animController;

			reactphysics3d::ConcaveMeshShape* arenaConcaveMeshCollision;
			reactphysics3d::ConcaveMeshShape* CreateConcaveCollision(std::string meshName);
		};
	}
}

