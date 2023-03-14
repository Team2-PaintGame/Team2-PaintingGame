#pragma once
#include <PlayerBase.h>
#include "Window.h"
#include <Ray.h>
#include "Utils.h"
#include "Vector3.h"
#include "Gun.h"
#include "AnimationController.h"

namespace NCL {
	using namespace Rendering;
	using namespace CSC8508;
	class Player : public PlayerBase {
	public:
		Player(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 position, MeshGeometry* mesh, TextureBase* texture, ShaderBase* shader, const std::unordered_map<std::string, MeshAnimation*>& animations, int size, Gun* gun) : PlayerBase(physicsCommon, physicsWorld, position, mesh, texture, shader, size) {
			SetMemberVariables(animations, gun);
		}
		Player(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 position, MeshGeometry* mesh, MeshMaterial* material, ShaderBase* shader, const std::unordered_map<std::string, MeshAnimation*>& animations, int size, Gun* gun) : PlayerBase(physicsCommon, physicsWorld, position, mesh, material, shader, size) {
			SetMemberVariables(animations, gun);
		}
		virtual ~Player() {
			delete animationController;
		}
		Gun* GetGun() const { return gun; }
		virtual void Update(float dt);
		virtual void Shoot();
		const reactphysics3d::Ray& GetShootRay() const { return shootRay; }
		Vector2 targetPosition;
	protected:
		void SetMemberVariables(const std::unordered_map<std::string, MeshAnimation*>& animations, Gun* gun);
		AnimationController* animationController = NULL;
		reactphysics3d::Ray shootRay = reactphysics3d::Ray(~Maths::Vector3(0), ~Maths::Vector3(0));
		Gun* gun = NULL;
		const Vector3 gunOffset = Vector3(0.5, 1, -4);
	};
}