#pragma once
#include <random>
#include <functional>
#include <GameObject.h>
#include <tuple>

namespace NCL {
	namespace CSC8508 {
		class GameObject;
		class Constraint;
		class GameEventListener;

		struct PaintSplat {
			Vector3 position;
			Vector4 colour;
			bool sent = false;

			PaintSplat(Vector3 position, Vector4 colour, bool sent = false) {
				this->position = position;
				this->colour = colour;
				this->sent = sent;
			}
		};

		typedef std::function<void(GameObject*)> GameObjectFunc;
		typedef std::function<void(int, Vector3&, Vector4&)> Vector3and4Func;
		typedef std::function<void(int, PaintSplat&)> PaintSplatFunc;

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
			void OperateOnPaintedPositions(Vector3and4Func f);


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
			size_t GetNumberOfGameObjects() const { return gameObjects.size(); }
			void AddPaintedPosition(const Vector3& position, Vector4 team);
			size_t GetNumPaintedPositions() const { return paintedPositions.size(); }

			void CalculateNewScores() 
			{
				int team1Score = 0;
				int team2Score = 0;
				for (auto& element : paintedPositions) 
				{
					if (element.colour == Vector4(1, 0, 0, 1)) {
						team2Score += 10;
					}
					else if (element.colour == Vector4(0, 0, 1, 1)) {
						team1Score += 10;
					}
				}
				SetTeamOneScore(team1Score);
				SetTeamTwoScore(team2Score);
			}

			int CalculateWinningTeam() 
			{
				if (teamOneScore > teamTwoScore) {
					return 1;
				}
				else if (teamTwoScore > teamOneScore) {
					return 2;
				}
				else { // draw
					return 3;
				}
			}

			void SetTeamOneScore(int score) { teamOneScore = score; }

			int GetTeamOneScore() { return teamOneScore; }

			void SetTeamTwoScore(int score) { teamTwoScore = score; }

			int GetTeamTwoScore() { return teamTwoScore; }

			bool CleanNearbyPaint(Vector3 SecurityPos, float range);
			Vector3 FindClosestPaintSplat(Vector3 position);
			int GetSizePaintedPositions();

			std::vector<PaintSplat>& GetSplats() { return paintedPositions; }

			std::vector<std::tuple<int, Vector4>> splatsToChangeColour;
			std::vector<std::tuple<int, Vector4>> cleanedSplats;
			std::vector<PaintSplat> paintedPositions;

		protected:
			RaycastManager* raycastManager;
			reactphysics3d::PhysicsWorld* physicsWorld = NULL;
			
			GameEventListener* eventListener;
			std::vector<GameObject*> gameObjects;
			std::vector<Constraint*> constraints;
			bool shuffleConstraints;
			bool shuffleObjects;
			int	worldIDCounter;
			int	worldStateCounter;
			int teamOneScore = 0;
			int teamTwoScore = 0;

			Vector4 RedTeamColour = {1,0,0,1};
			Vector4 BlueTeamColour = {0,0,1,1};
			
		};
	}
}

