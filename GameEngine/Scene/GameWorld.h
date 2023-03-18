#pragma once
#include <random>
#include <functional>
#include <GameObject.h>

namespace NCL {
	namespace CSC8508 {
		class GameObject;
		class Constraint;
		class GameEventListener;

		typedef std::function<void(GameObject*)> GameObjectFunc;
		typedef std::function<void(int, Vector3&)> Vector3Func;

		typedef std::vector<GameObject*>::const_iterator GameObjectIterator;


		struct SceneContactPoint {
			bool isHit;
			float hitFraction;
			reactphysics3d::Vector3 hitPos;
			reactphysics3d::Vector3 normal;
			reactphysics3d::CollisionBody* body;
			GameObject* object;
		};

		class RaycastManager : public reactphysics3d::RaycastCallback {
		public:

			RaycastManager() { ignoreBody = nullptr; }
			~RaycastManager() {}

			virtual rp3d::decimal notifyRaycastHit(const rp3d::RaycastInfo& raycastInfo) override;

			bool isHit() {
				return !hitPoints.empty();
			}

			void setIgnore(reactphysics3d::RigidBody* ignoreThis) {
				ignoreBody = ignoreThis;
			}

			SceneContactPoint* getHit() {
				SceneContactPoint* nearest = new SceneContactPoint();
				nearest->hitFraction = 1.1f;

				for (auto i : hitPoints) {
					if (i->hitFraction < nearest->hitFraction) {
						nearest = i;
					}
				}

				return nearest;
			}

			void clear() {
				hitPoints.clear();
				ignoreBody = nullptr;
			}
		private:
			std::vector<SceneContactPoint*> hitPoints;
			reactphysics3d::RigidBody* ignoreBody;
		};

		class GameWorld	{
		public:
			GameWorld(reactphysics3d::PhysicsWorld* physicsWorld);
			~GameWorld();

			void Clear();
			void ClearAndErase();

			void AddGameObject(GameObject* o);
			void RemoveGameObject(GameObject* o, bool andDelete = false);

			void AddEventListener(GameEventListener* eventListener) { this->eventListener = eventListener; }
			GameEventListener* GetEventListener() const { return eventListener; }

			void AddConstraint(Constraint* c);
			void RemoveConstraint(Constraint* c, bool andDelete = false);

			void ShuffleConstraints(bool state) {
				shuffleConstraints = state;
			}

			void ShuffleObjects(bool state) {
				shuffleObjects = state;
			}

			virtual void UpdateWorld(float dt);

			void OperateOnContents(GameObjectFunc f);
			void OperateOnPaintedPositions(Vector3Func f);


			void GetObjectIterators(
				GameObjectIterator& first,
				GameObjectIterator& last) const;

			void GetConstraintIterators(
				std::vector<Constraint*>::const_iterator& first,
				std::vector<Constraint*>::const_iterator& last) const;

			int GetWorldStateID() const {
				return worldStateCounter;
			}

			reactphysics3d::PhysicsWorld& GetPhysicsWorld() const { return *physicsWorld; }

			SceneContactPoint* Raycast(const reactphysics3d::Ray& r, GameObject* ignore = nullptr) const;

			void AddPaintedPosition(const Vector3& position);
			size_t GetNumPaintedPositions() const { return paintedPositions.size();}
			size_t GetNumberOfGameObjects() const { return gameObjects.size();}

			void CleanNearbyPaint(Vector3 SecurityPos);
			Vector3 FindClosestPaintSplat(Vector3 position);
			int GetSizePaintedPositions();
		protected:
			RaycastManager* raycastManager;
			reactphysics3d::PhysicsWorld* physicsWorld = NULL;
			GameEventListener* eventListener;
			std::vector<Vector3> paintedPositions;
			std::vector<GameObject*> gameObjects;
			std::vector<Constraint*> constraints;
			bool shuffleConstraints;
			bool shuffleObjects;
			int		worldIDCounter;
			int		worldStateCounter;
		};
	}
}

