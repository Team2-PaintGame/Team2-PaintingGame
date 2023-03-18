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

	class SecurityGuard : public AnimatedObject
	{
	public:
/*
		//TextureBase Constructor
		SecurityGuard(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 position,
			MeshGeometry* mesh, TextureBase* texture, ShaderBase* shader, const std::unordered_map<std::string, MeshAnimation*>& animations,
			int size, GameObject* playerOne, GameObject* playerTwo, std::string objectName);
*/
		//Mesh Material Constructor
		SecurityGuard(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 position,
			MeshGeometry* mesh, MeshMaterial* meshMaterial, ShaderBase* shader, const std::unordered_map<std::string, MeshAnimation*>& animations,
			int size, GameObject* playerOne, GameObject* playerTwo, GameWorld* gameWorld, std::string objectName);

		virtual	~SecurityGuard();
		virtual void Update(float dt); // should this be virutal??

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
	
		void DisplayPathfinding();
		void MoveSecurityGuard(Vector3 direction);
		float DistanceToTarget(Vector3 destination);
		GameObject* FindClosestPlayer();
		void DetermineSpeed();
		void DetermineChaseSpeed();
		void RaycastAgainstPlayer(GameObject* player, SecurityCallbackClass* callbackClass, bool& isPlayerVisible);
		void DrawNavTris();
		void DrawTriRoute();
		Vector3 GetForwardVector();
		bool IsPlayerInFront(Vector3& playerPosition, Vector3& securityPosition);
		bool IsInFieldOfView(Vector3 direction);
		void SetIsBlindedTrue() { isBlinded = true; std::cout << "isBlinded = true\n"; }
		void SetIsBlindedFalse() { isBlinded = false; std::cout << "isBlinded = false\n";}

		Vector3 ChooseRandomDestination();
		Vector3 ChooseDestination();
	protected:

		int state;

		float force = 100;
		const float slowForce = 50; // 100
		const float walkForce = 100;  //150
		const float runForce = 250;
		const float sprintForce = 350;
		bool isBlinded = false;

		float chaseAccumulator = 0.0f;
		float stuckAccumulator = 0.0f;
		float blindTimer = 0.0f;

		BehaviourSelector* rootSelector;
		BehaviourSequence* patrolSequence;
		BehaviourSequence* chaseSequence;

		BehaviourAction* chooseDestination;
		BehaviourAction* goToDestination;

		BehaviourAction* lookForPlayer;
		BehaviourAction* chaseThePlayer;
		BehaviourAction* attackThePlayer;

		NavigationPath* navigationPath;
		NavigationMesh* navigationMesh;

		GameObject* playerOne;
		GameObject* playerTwo;
		GameObject* chasedPlayer;
		SecurityCallbackClass* callbackPlayerOne;
		SecurityCallbackClass* callbackPlayerTwo;
		GameWorld* gameWorld;

	};

}