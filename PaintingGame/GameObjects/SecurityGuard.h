#pragma once
#include "GameObject.h"
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


namespace NCL::CSC8508 {

	class SecurityGuard : public GameObject
	{
	public:
		SecurityGuard(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, std::string objectName, Vector3 position, MeshGeometry* mesh, 
			TextureBase* texture, ShaderBase* shader, Vector3 size, GameObject* playerOne, GameObject* playerTwo);
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
		void FindNavigableNodes(const std::string& filename, vector<Vector3>& navigableNodes);
		Vector3 ChooseDestination();
		void DisplayPathfinding();
		void MoveSecurityGuard(Vector3 direction);
		float DistanceToTarget(Vector3 destination);
		Vector3 FindClosestNode(Vector3 position);
		GameObject* FindClosestPlayer();
		void DetermineSpeed();
		void RaycastAgainstPlayer(GameObject* player, SecurityCallbackClass* callbackClass, bool& isPlayerVisible);

		

	protected:

		int state;
		int nodeSize;
		int gridWidth;
		int gridHeight;

		float force = 100;
		const float slowForce = 100;
		const float walkForce = 150;
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

		rp3d::BoxShape* boundingVolume;

		reactphysics3d::PhysicsWorld* physicsWorld;

		vector <Vector3> navigableNodes;
		
		NavigationGrid* navigationGrid;
		NavigationPath* navigationPath;

		GameObject* playerOne;
		GameObject* playerTwo;
		GameObject* chasedPlayer;
		SecurityCallbackClass* callbackPlayerOne;
		SecurityCallbackClass* callbackPlayerTwo;
	};

}