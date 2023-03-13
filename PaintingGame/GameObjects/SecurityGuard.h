#pragma once
#include "GameObject.h"
#include "PlayerBase.h"
#include "BehaviourSelector.h"
#include "BehaviourSequence.h"
#include "BehaviourAction.h"
#include "SecurityGuard.h"
#include "MeshGeometry.h"
#include "TextureBase.h"
#include "ShaderBase.h"
#include "RenderObject.h"
#include "Debug.h"
#include "Utils.h"
#include "SecurityCallbackClass.h"
#include "NavigationGrid.h"
#include "NavigationPath.h"
#include "NavigationMesh.h"
#include"AnimationController.h"

namespace NCL::CSC8508 {

	class SecurityGuard : public /*GameObject*/ PlayerBase
	{
	public:
		SecurityGuard(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, std::string objectName, Vector3 position, MeshGeometry* mesh, 
			TextureBase* texture, ShaderBase* shader, Vector3 size, GameObject* playerOne, GameObject* playerTwo);
		SecurityGuard(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, std::string objectName, Vector3 position, MeshGeometry* mesh,
			MeshMaterial* material, ShaderBase* shader, const std::unordered_map<std::string, MeshAnimation*>& animations, int size, GameObject* playerOne, GameObject* playerTwo);
		~SecurityGuard();
		void Update(float dt);

		void InitChooseDestination();
		void InitGoToDesination();

		void InitLookForPlayer();
		void InitChaseThePlayer();
		void InitAttackThePlayer();

		void InitPatrolSequence();
		void InitChaseSequence();
		void InitRootSelector();
		void InitBehaviorTree();

		bool LookForPlayer(GameObject* player);
		GameObject* LookForPlayers();
		Vector3 ChooseDestination();
		void DisplayPathfinding();
		void MoveSecurityGuard(Vector3 direction);
		float DistanceToTarget(Vector3 destination);
		GameObject* FindClosestPlayer();
		void DetermineSpeed();
		void RaycastAgainstPlayer(GameObject* player, SecurityCallbackClass* callbackClass, bool& isPlayerVisible);
		void DrawNavTris();
		void DrawTriRoute();

	protected:

		int state;
		int nodeSize;
		int gridWidth;
		int gridHeight;

		float force = 100;
		const float slowForce = 300; // 100
		const float walkForce = 300;  //150
		const float runForce = 300;

		float timeAccumulator = 0.0f;

		BehaviourSelector* rootSelector;
		BehaviourSequence* patrolSequence;
		BehaviourSequence* chaseSequence;

		BehaviourAction* chooseDestination;
		BehaviourAction* goToDestination;

		BehaviourAction* lookForPlayer;
		BehaviourAction* chaseThePlayer;
		BehaviourAction* attackThePlayer;

		//rp3d::BoxShape* boundingVolume;
		rp3d::CollisionShape* boundingVolume;

		reactphysics3d::PhysicsWorld* physicsWorld;

		vector <Vector3> navigableNodes;
		
//		NavigationGrid* navigationGrid;
		NavigationPath* navigationPath;
		NavigationMesh* navigationMesh;

		GameObject* playerOne;
		GameObject* playerTwo;
		GameObject* chasedPlayer;
		SecurityCallbackClass* callbackPlayerOne;
		SecurityCallbackClass* callbackPlayerTwo;

		AnimationController* animationController = NULL;
	};

}