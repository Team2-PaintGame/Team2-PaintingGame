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

class Gamepad;

namespace NCL {
	class MenuHandler;
	namespace CSC8508 {
		class PaintingGame {
		public:
#ifdef USEVULKAN
			PaintingGame(GameTechVulkanRenderer* render, GameWorld* world, reactphysics3d::PhysicsWorld* physicsWorld, reactphysics3d::PhysicsCommon* physicsCommon, MenuHandler* menu, bool online = false);
#else
			
			PaintingGame(GameTechRenderer* render, GameWorld* world, reactphysics3d::PhysicsCommon* physicsCommon, MenuHandler* menu, bool online = false);
#endif
			
			~PaintingGame();
			virtual void UpdateGame(float dt);
			GameTechRenderer* GetGameTechRenderer();
			PlayerBase* InitSecondPlayer();
			void InitSecondCamera();
			void DestroySecondPlayer();

		protected:
			void InitialiseAssets();
			void InitCamera();
			void InitWorld();
			PlayerBase* InitiliazePlayer();
			PlayerBase* InitialiseNetworkPlayer();
#ifdef USEVULKAN
			GameTechVulkanRenderer* renderer;
#else
			GameTechRenderer* renderer;
#endif
			GameWorld* world;

			bool useGravity = true;
			bool useFog = true;
			bool useSplitScreen = false;
			bool thirdPersonCamera;

			bool is_Networked;

			float		forceMagnitude;

			std::map<std::string, MeshGeometry*> meshes;
			std::map<std::string, MeshMaterial*> meshMaterials;
			std::map<std::string, MeshAnimation*> meshAnimations;

			std::map<std::string, TextureBase*> textures;
			std::map<std::string, ShaderBase*> shaders;

			//Coursework Additional functionality	
			PlayerBase* players[2] = { NULL };
			PlayerBase* netPlayer = NULL;
			PlayerController* playerControllers[2] = {NULL};
			
			//Create a physics world 
			reactphysics3d::PhysicsCommon* physicsCommon;
			reactphysics3d::PhysicsWorld* physicsWorld = NULL; 

			int numberOfPlayerControllers = 1;

			Gamepad* gamepad = NULL;
			bool wasConnected = true;

			//UI
			MenuHandler* menuHandler;
		};
	}
}

