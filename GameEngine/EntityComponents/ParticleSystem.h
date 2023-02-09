#pragma once
#include "Vector3.h"
#include "GameObject.h"
#include "RenderObject.h"
#include "Utils.h"

namespace NCL {
	using namespace Maths;
	using namespace CSC8508;

	class Particle : public GameObject {
	public:
		Particle(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, int size) : GameObject(physicsCommon, physicsWorld) {
			Vector3 position = Vector3(rand() % 50, 10, rand() % 50);

			transform
				.SetPosition(position)
				.SetScale(Vector3(size));
			
			boundingVolume = physicsCommon.createBoxShape(~transform.GetScale() / 2.0f);
			reactphysics3d::Transform rp3d_transform(~position, rp3d::Quaternion::identity());

			rigidBody = physicsWorld->createRigidBody(rp3d_transform);
			rigidBody->addCollider(boundingVolume, rp3d::Transform::identity()); //collider
			rigidBody->updateMassPropertiesFromColliders();
		}

		void Update(float dt) {
			//need to call this here because particles itself are not part of game world
			GameObject::UpdateTransform();

			elapsedTime += dt;
		}

		virtual ~Particle() {
			physicsCommon.destroyBoxShape(boundingVolume);
		}
	protected:
		Vector3 velocity;
		float lifeSpan = 3.0f;
		float elapsedTime = 0.0f;
		rp3d::BoxShape* boundingVolume;
	};
	class ParticleSystem : public GameObject {
	public:
		ParticleSystem(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, MeshGeometry* mesh, TextureBase* texture, ShaderBase* shader, int size = 1, std::string name = "Particle System");
		
		void Update(float dt) {
			for (auto& p : particles) {
				p->Update(dt);
			}
		}
		
		virtual ~ParticleSystem();
		
	protected:
		unsigned int numParticles = 100;
		std::vector<Particle*> particles;
		std::vector<Transform*> transforms;
	};
}