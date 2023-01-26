#pragma once

#include <GameObject.h>
#include <GameTechRenderer.h>
#include <PhysicsSystem.h>
#include <GameWorld.h>
#include <map>
#include <MeshMaterial.h>
#include <NavigationGrid.h>
#include <MeshAnimation.h>
#include <Vector4.h>
#include <Vector3.h>
#include <RenderObject.h>
#include <PhysicsObject.h>
#include <PlayerBase.h>

namespace NCL {
	namespace CSC8508 {
		class PaintingGame {
		public:
			PaintingGame();
			~PaintingGame();
			virtual void UpdateGame(float dt);
		protected:
			void InitialiseAssets();
			void InitCamera();
			void InitWorld();
			void InitiliazePlayer();
#ifdef USEVULKAN
			GameTechVulkanRenderer* renderer;
#else
			GameTechRenderer* renderer;
#endif
			PhysicsSystem* physics;
			GameWorld* world;

			bool useGravity = false;
			bool useFog = true;

			float		forceMagnitude;

			std::map<std::string, MeshGeometry*> meshes;
			std::map<std::string, MeshMaterial*> meshMaterials;
			std::map<std::string, MeshAnimation*> meshAnimations;

			std::map<std::string, TextureBase*> textures;
			std::map<std::string, ShaderBase*> shaders;

			//Coursework Additional functionality	
			float remainingTime = 30;
			PlayerBase* player = NULL;
		};
	}
}

