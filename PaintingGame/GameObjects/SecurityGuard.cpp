#include "SecurityGuard.h"
#include <fstream>
#include "Assets.h"
#include <cmath>
#include "Player.h"
#include <Window.h>

namespace NCL::CSC8508 {
	SecurityGuard::SecurityGuard(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, Vector3 position,
		MeshGeometry* mesh, MeshMaterial* meshMaterial, ShaderBase* shader, const std::unordered_map<std::string, MeshAnimation*>& animations,
		int size, GameObject* playerOne, GameObject* playerTwo, GameWorld* gameWorld, GameAssets* assets, uint seed, std::string objectName)
		: AnimatedObject(physicsCommon, physicsWorld, position, mesh, meshMaterial, shader, animations, size, objectName)
	
	{
		srand(seed);
		this->playerOne = playerOne;
		this->playerTwo = playerTwo;
		this->physicsWorld = physicsWorld;
		this->gameWorld = gameWorld;
		chasedPlayer = nullptr;

		transform.SetScale(size).SetPosition(position);
		renderObject = new RenderObject(&transform, mesh, shader);
		renderObject->LoadMaterialTextures(meshMaterial);

		boundingVolume = physicsCommon.createCapsuleShape(size * .35f, size);
		reactphysics3d::Transform rp3d_transform(~position, rp3d::Quaternion::identity());

		// Create a rigid body in the physics world
		rigidBody = physicsWorld->createRigidBody(rp3d_transform);
		rigidBody->addCollider(boundingVolume, rp3d::Transform::identity()); //collider
		rigidBody->updateMassPropertiesFromColliders();
		rigidBody->setLinearDamping(3.0f);
		rigidBody->setAngularDamping(3.0f);

		Vector3 lockXZAxis(0, 1, 0);
		rigidBody->setAngularLockAxisFactor(~lockXZAxis);

		AnimatedObject::SetAnimControler(animations);

		callbackPlayerOne = new SecurityCallbackClass(playerOne);
		callbackPlayerTwo = new SecurityCallbackClass(playerTwo);

		navigationMesh = new NavigationMesh("BasicLVL1.navmesh");
		navigationPath = new NavigationPath();
		ink = CreateInkStream(physicsCommon, physicsWorld, position, assets->GetMesh("sphereMesh"), Vector4(1, 1, 1, 0.25), assets->GetShader("inkShader"));
		ink->SetLayer(Layer::Bubbles);
		gameWorld->AddGameObject(ink);

		rigidBody->setUserData(this);
		layer = Layer::Enemy;
		InitBehaviorTree();
	}

	SecurityGuard::~SecurityGuard()
	{
		//delete chooseDestination;
		//delete goToDestination;

		//delete lookForPlayer;
		//delete chaseThePlayer;
		//delete attackThePlayer;

		//delete patrolSequence;
		//delete chaseSequence;
		delete rootSelector;

		delete callbackPlayerOne;
		delete callbackPlayerTwo;

		delete navigationMesh;
		delete navigationPath;
	}

	void SecurityGuard::Update(float dt)
	{
		if (isBlinded)
		{
			blindTimer += dt;
			if (blindTimer > 5.0f)
			{
				blindTimer = 0.0f;
				SetIsBlindedFalse();
			}
		}
		animationController->Update(dt);
		/*DrawNavTris();
			DisplayPathfinding();
		DrawTriRoute();*/
		if (state == Initialise) {
			state = Ongoing;
		}
		if (state == Ongoing) {
			state = rootSelector->Execute(dt);
		}
		if (state == Success) {
			rootSelector->Reset();
			state = Initialise;
		}
		if (state == Failure) {
			state = Initialise;
		}
		if (ink) {
			ink->SetLayer(Layer::Bubbles);
			ink->GetTransform().SetPosition(transform.GetPosition()).SetOrientation(transform.GetOrientation());
		}
	}

	void SecurityGuard::InitBehaviorTree()
	{
		state = Ongoing;

		InitChooseDestination();
		InitGoToDesination();

		InitLookForPlayer();
		InitChaseThePlayer();
		InitAttackThePlayer();

		InitGoToPaint();
		InitCleanPaint();

		InitPatrolSequence();
		InitChaseSequence();
		InitSuperCleanSequence();
		InitChasePatrolSelector();
		InitRootSelector();

		rootSelector->Reset();
	}
	void SecurityGuard::InitPatrolSequence()
	{
		patrolSequence = new BehaviourSequence("Patrol Sequnce ");
		patrolSequence->AddChild(chooseDestination);
		patrolSequence->AddChild(goToDestination);
	}
	void SecurityGuard::InitChaseSequence()
	{
		chaseSequence = new BehaviourSequence("Chase Sequence ");
		chaseSequence->AddChild(lookForPlayer);
		chaseSequence->AddChild(chaseThePlayer);
		chaseSequence->AddChild(attackThePlayer);
	}
	void SecurityGuard::InitRootSelector()
	{
		rootSelector = new BehaviourSelector("Root Selector ");
		rootSelector->AddChild(superCleanSequence);
		rootSelector->AddChild(chasePatrolSelector);
	}
	void SecurityGuard::InitSuperCleanSequence()
	{
		superCleanSequence = new BehaviourSequence("Super Clean ");
		superCleanSequence->AddChild(goToPaint);
		superCleanSequence->AddChild(cleanPaint);
	}
	void SecurityGuard::InitChasePatrolSelector()
	{
		chasePatrolSelector = new BehaviourSelector("Chase/Patrol ");
		chasePatrolSelector->AddChild(chaseSequence);
		chasePatrolSelector->AddChild(patrolSequence);
	}
	void SecurityGuard::InitGoToPaint()
	{
		goToPaint = new BehaviourAction("Go To Paint ", [&](float dt, BehaviourState state)->BehaviourState {
			if (state == Initialise) {
				//std::cout << "Go To Paint - Initialise\n";
				if (gameWorld->GetNumPaintedPositions() >= 900)
				{
					//std::cout << "Theres too much paint!\n";
					Vector3 paintPos = gameWorld->FindClosestPaintSplat(this->GetTransform().GetPosition());
					paintPos = navigationMesh->FindClosestPoint(paintPos);

					Vector3 securityPos = this->GetTransform().GetPosition();
					bool foundPath = navigationMesh->FindPath(securityPos, paintPos, *navigationPath);
					if (foundPath)
					{
						state = Ongoing;
					}	
				}
				else
				{
					//std::cout << "Its too clean to clean\n";
					state = Failure;
				}	
			}
			else if (state == Ongoing) 
			{
				Vector3 nextWaypoint = navigationPath->waypoints.back();
				nextWaypoint.y = this->GetTransform().GetPosition().y;
				Vector3 direction = nextWaypoint - this->GetTransform().GetPosition();

				Vector3 velocity = rigidBody->getLinearVelocity();
				if (velocity.Length() < 0.1) {
					stuckAccumulator += dt;
					if (stuckAccumulator > 5.0) {
						stuckAccumulator = 0.0f;
						navigationPath->Clear();
						return Success;
					}
				}
				if (direction.Length() <= 5 && navigationPath->waypoints.size() >= 2) {
					//std::cout << "Go To Paint - Going Destination\n";
					navigationPath->waypoints.pop_back();
				}
				if (navigationPath->waypoints.size() == 1) {
					if (DistanceToTarget(navigationPath->waypoints.front()) <= 5.0f) {
					//	std::cout << "Go To Paint - Reached Destination\n";
						navigationPath->Clear();
						return Success;
					}
				}
				if (gameWorld->CleanNearbyPaint(this->GetTransform().GetPosition(), 12))
				{
					ink->SetLayer(Layer::Bubbles);
					BubbleEmission();
				}
				DetermineChaseSpeed();
				MoveSecurityGuard(direction);
			}
			return state;

			}
		);
	}
	void SecurityGuard::InitCleanPaint()
	{
		cleanPaint = new BehaviourAction("Clean Paint ", [&](float dt, BehaviourState state)->BehaviourState {
			if (state == Initialise)
			{
				state = Ongoing;
			}
			else if (state == Ongoing)
			{
				Vector3 securityPos = this->GetTransform().GetPosition();
				if (gameWorld->CleanNearbyPaint(securityPos, 20))
				{
					ink->SetLayer(Layer::Bubbles);
					BubbleEmission();
				}
				return Success;
			}
			return state;
			}

		);

	}
	void SecurityGuard::InitChooseDestination()
	{
		chooseDestination = new BehaviourAction("Choose Destination ", [&](float dt, BehaviourState state)->BehaviourState {
			if (state == Initialise) {
				//std::cout << "Choose Destination - Initialise\n";
				state = Ongoing;
			}

			else if (state == Ongoing) {
				Vector3 destination = ChooseDestination();
				Vector3 securityPosition = this->GetTransform().GetPosition();
				bool foundPath = navigationMesh->FindPath(securityPosition, destination, *navigationPath);
				if (foundPath) {
					//std::cout << "Choose Destination - Found Path\n";
					return Success;
				}
				else
				{
					//std::cout << "Choose Destination - Found NOT Path\n";
				}
			}
			return state;
			}
		);
	}

	void SecurityGuard::InitGoToDesination()
	{
		goToDestination = new BehaviourAction("Go To Destination ", [&](float dt, BehaviourState state)->BehaviourState {
			if (state == Initialise) {
				//std::cout << " Going to the destination - Initialise \n";
				state = Ongoing;
			}
			else if (state == Ongoing) {

				Vector3 nextWaypoint = navigationPath->waypoints.back();
				nextWaypoint.y = this->GetTransform().GetPosition().y;
				Vector3 direction = nextWaypoint - this->GetTransform().GetPosition();
				Vector3 velocity = rigidBody->getLinearVelocity();
								
				if (LookForPlayers() != nullptr) {
					navigationPath->Clear();
				//	std::cout << " Going to the destination - Can see Player\n";
					return Success;
				}
				if (velocity.Length() < 0.1) {
					stuckAccumulator += dt;
					if (stuckAccumulator > 5.0) {
						stuckAccumulator = 0.0f;
						navigationPath->Clear();
						return Success;
					}
				}
				if (direction.Length() <= 5 && navigationPath->waypoints.size() >= 2) {
				//	std::cout << " Going to the destination - Going Destination\n";
					navigationPath->waypoints.pop_back();
				}
	
				if (navigationPath->waypoints.size() == 1) {
					if (DistanceToTarget(navigationPath->waypoints.front()) <= 5.0f) {
				//		std::cout << " Going to the destination - Reached Destination\n";
						this->GetRigidBody()->resetForce();
						this->GetRigidBody()->resetTorque();
						navigationPath->Clear();
						return Success;
					}
				}
				DetermineSpeed();
				MoveSecurityGuard(direction);
				if (gameWorld->CleanNearbyPaint(this->GetTransform().GetPosition(), 12))
				{
					ink->SetLayer(Layer::Bubbles);
					BubbleEmission();
				}
			}
			return state;
			}
		);
	}

	void SecurityGuard::InitLookForPlayer()
	{
		lookForPlayer = new BehaviourAction("Looking for the player ", [&](float dt, BehaviourState state)->BehaviourState {
			if (state == Initialise) {
				state = Ongoing;
			}
			else if (state == Ongoing) {

				chasedPlayer = LookForPlayers();
				if (chasedPlayer != nullptr) {
					//std::cout << " Looking for Player - Can see Player\n";
					state = Success;
				}
				else {
					//std::cout << " Looking for Player - Player not visible\n";
					state = Failure;
				}
			}
			return state;
			}
		);
	}

	void SecurityGuard::InitChaseThePlayer()
	{
		chaseThePlayer = new BehaviourAction("Chase the player ", [&](float dt, BehaviourState state)->BehaviourState {

			if (state == Initialise) {
				Vector3 playerPosition = chasedPlayer->GetTransform().GetPosition();
				Vector3 securityPosition = this->GetTransform().GetPosition();
				bool foundPath = navigationMesh->FindPath(securityPosition, playerPosition, *navigationPath);
				if (foundPath) {
					//std::cout << " Chase the Player - Path found\n";
					state = Ongoing;
				}
				else {
					//std::cout << " Chase the Player - Path NOT found\n";
					state = Failure;
				}
				
			}
			else if (state == Ongoing) {
				Vector3 nextWaypoint = navigationPath->waypoints.back();
				nextWaypoint.y = this->GetTransform().GetPosition().y;
				Vector3 direction = nextWaypoint - this->GetTransform().GetPosition();
				Vector3 velocity = rigidBody->getLinearVelocity();
				chaseAccumulator += dt;
				
				if (velocity.Length() < 0.1)
				{
					stuckAccumulator += dt;
					if (stuckAccumulator > 5)
					{
						stuckAccumulator = 0;
						navigationPath->Clear();
						rootSelector->Reset();
						return Initialise;
					}
				}
				if (chaseAccumulator >= 1.0) {
					bool isPlayerVisible = LookForPlayer(chasedPlayer);
					chaseAccumulator = 0.0f;
					if (isPlayerVisible) {
						navigationPath->Clear();
						rootSelector->Reset();
						return Initialise;
					}
				}
				if (direction.Length() <= 5 && navigationPath->waypoints.size() >= 2) {
					navigationPath->waypoints.pop_back();
					//std::cout << " Chase the Player - Chasing player\n";
				}
				if (navigationPath->waypoints.size() == 1) {
					if (DistanceToTarget(navigationPath->waypoints.back()) <= 5.0f) {
						//std::cout << " Chase the Player - Reached the player\n";
						navigationPath->Clear();
						chaseAccumulator = 0.0f;
						return Success;
					}
				}
				DetermineChaseSpeed();
				MoveSecurityGuard(direction);
			}
			return state;
			}
		);
	}

	void SecurityGuard::InitAttackThePlayer()
	{
		attackThePlayer = new BehaviourAction("Attack the Goat ", [&](float dt, BehaviourState state)->BehaviourState {
			if (state == Initialise) {
				state = Ongoing;
			}
			else if (state == Ongoing) {
				Vector3 nextWaypoint = chasedPlayer->GetTransform().GetPosition();
				nextWaypoint.y = this->GetTransform().GetPosition().y;
				Vector3 direction = nextWaypoint - this->GetTransform().GetPosition();
				DetermineChaseSpeed();
				MoveSecurityGuard(direction);
				if (hasCaughtPlayer) { // collision with player
					//std::cout << "Player Caught\n";
					hasCaughtPlayer = false;
					state = Success;
				}
				else if(direction.Length() > 20) {
				//	std::cout << "Player has escaped\n";
					state = Failure;
				}
			}
			return state;
			}
		);
	}

	void SecurityGuard::CaughtPlayer() {
		if (chasedPlayer == nullptr) { return; }

		reactphysics3d::Vector3 pos = ~((PlayerBase*)chasedPlayer)->GetSpawnPosition();

		chasedPlayer->GetRigidBody()->resetForce();
		this->GetRigidBody()->resetForce();
		reactphysics3d::Vector3 newVelocity(0, 0, 0);
		this->GetRigidBody()->setLinearVelocity(newVelocity);


		chasedPlayer->GetRigidBody()->setLinearVelocity(newVelocity);
		//reactphysics3d::Vector3 position(200, 15.0f, 200.0f);
		reactphysics3d::Transform rp3d_transform(pos, rp3d::Quaternion::identity());
		chasedPlayer->GetRigidBody()->setTransform(rp3d_transform);

		chasedPlayer->GetRigidBody()->setLinearVelocity(newVelocity);
		//reactphysics3d::Vector3 position2(200, 10.0f, 200.0f);
		reactphysics3d::Transform rp3d_transform2(pos, rp3d::Quaternion::identity());
		chasedPlayer->GetRigidBody()->setTransform(rp3d_transform2);


		chasedPlayer->GetRigidBody()->setLinearVelocity(newVelocity);
		//reactphysics3d::Vector3 position3(200, 5.0f, 200.0f);
		reactphysics3d::Transform rp3d_transform3(pos, rp3d::Quaternion::identity());
		chasedPlayer->GetRigidBody()->setTransform(rp3d_transform3);



	}

	GameObject* SecurityGuard::LookForPlayers()
	{
		bool isPlayerOneVisible = LookForPlayer(playerOne);
		if (playerTwo == nullptr) // only one player game
		{
			if (isPlayerOneVisible ) 
			{
				return playerOne;
				//return nullptr;
			}
			else
			{
				return nullptr;
			}
		}
		else					// two player game
		{
			bool isPlayerTwoVisible = LookForPlayer(playerTwo);
			if (isPlayerOneVisible && isPlayerTwoVisible)
			{
				return FindClosestPlayer();
			}
			else if (isPlayerOneVisible)
			{
				return playerOne;
			}
			else if (isPlayerTwoVisible)
			{
				return playerTwo;
			}
			else
				return nullptr;
		}
	}

	Vector3 SecurityGuard::GetForwardVector()
	{
		return this->GetTransform().GetMatrix().GetColumn(2);
	}

	GameObject* SecurityGuard::FindClosestPlayer()
	{
		Vector3 securityPosition = this->GetTransform().GetPosition();
		Vector3 playerOnePosition = playerOne->GetTransform().GetPosition();
		Vector3 playerTwoPosition = playerTwo->GetTransform().GetPosition();
		playerOnePosition -= securityPosition;
		playerTwoPosition -= securityPosition;
		GameObject* gameObject = playerOnePosition.Length() < playerTwoPosition.Length() ? playerOne : playerTwo;
		return gameObject;
	}

	bool SecurityGuard::LookForPlayer(GameObject* player)
	{
		bool isPlayerVisible = false;

		Player* play = (Player*)player;
		if (play->GetHasRespawned())
		{
			return isPlayerVisible;
		}


		if (player == playerOne)
		{
			RaycastAgainstPlayer(playerOne, callbackPlayerOne, isPlayerVisible);
		}
		else if (player == playerTwo)
		{
			RaycastAgainstPlayer(playerTwo, callbackPlayerTwo, isPlayerVisible);
		}
//		std::cout << "Is Player Visible: " << isPlayerVisible << "\n";
		return isPlayerVisible;
	}

	void SecurityGuard::RaycastAgainstPlayer(GameObject* player, SecurityCallbackClass* callbackClass, bool& isPlayerVisible)
	{
		Vector3 securityPosition = this->GetTransform().GetPosition();
		Vector3 playerPosition = player->GetTransform().GetPosition();
		bool isInFront = IsPlayerInFront(playerPosition, securityPosition);

		if (isInFront == false)
		{
			isPlayerVisible = false;
			return;
		}

		//if (player == playerOne) {
		//	Debug::DrawLine(securityPosition, playerPosition, Debug::BLUE, 0.1f);
		//}
		//else if (player == playerTwo) {
		//	Debug::DrawLine(securityPosition, playerPosition, Debug::RED, 0.1f);
		//}
		reactphysics3d::Ray ray(~securityPosition, ~playerPosition); // '~' converts NCL Vector3 to reactphysics3d Vector3
		reactphysics3d::RaycastInfo rayCastInfo;
		physicsWorld->raycast(ray, *(&callbackClass));
		callbackClass->notifyRaycastHit(rayCastInfo);
		isPlayerVisible = callbackClass->IsPlayerVisible();
		callbackClass->ResetObjectDistances();
	}

	bool SecurityGuard::IsPlayerInFront(Vector3& playerPosition, Vector3& securityPosition)
	{
		Vector3 toPlayer = playerPosition;
		toPlayer -= securityPosition;
		Vector3 right = this->GetTransform().GetMatrix().GetColumn(0);
		float triArea = Maths::FloatAreaOfTri(securityPosition, securityPosition + right, securityPosition + toPlayer);
		if (triArea < 0) { //Player in front
			bool isInFOV = IsInFieldOfView(playerPosition - securityPosition);
			return isInFOV;
		}
		else // Player Behind 
		{
			return false;
		}
	}
	bool SecurityGuard::IsInFieldOfView(Vector3 direction)
	{
		Vector3 forward = GetForwardVector();
		float angle = Vector3::Dot(forward, direction);
		angle = acos(angle / (forward.Length() * direction.Length()));
		angle = angle * 180 / PI;
		if (angle <= 180 && angle >= 130) { 
			return true;
		}
		else {
			return false;
		}
		
	}

	void SecurityGuard::MoveSecurityGuard(Vector3 direction)
	{
		reactphysics3d::Vector3 forceDirection;

		forceDirection = ~direction.Normalised();

//		Debug::DrawLine(this->GetTransform().GetPosition(), this->GetTransform().GetPosition() + forceDirection /** force*/, Debug::YELLOW, 1);
		Vector3 up(0, 1, 0);
		Vector3 negdirection(-direction.x, -direction.y, -direction.z);
		Quaternion rotation = Quaternion::LookRotation(negdirection, up);

		reactphysics3d::Transform transform = reactphysics3d::Transform(this->GetRigidBody()->getTransform().getPosition(), ~rotation);
		if (transform.isValid()) {
			this->GetRigidBody()->setTransform(transform);
		}
		this->GetRigidBody()->applyWorldForceAtCenterOfMass(forceDirection * force);// '~' converts NCL Vector3 to reactphysics3d Vector3
	}

	float SecurityGuard::DistanceToTarget(Vector3 destination) 
	{
		return (destination - this->GetTransform().GetPosition()).Length();
	}

	Vector3 SecurityGuard::ChooseRandomDestination()
	{
		int numNodes = navigationMesh->allTris.size();
		int randomNum = rand() % numNodes;
		Vector3 destination = navigationMesh->allTris[randomNum].centroid;
		return destination;
	}
	Vector3 SecurityGuard::ChooseDestination()
	{
		if (gameWorld->GetSizePaintedPositions() < 1)
		{
			//std::cout << "No Paint\n";
			return ChooseRandomDestination();
		}
		else
		{
		//	std::cout << "Going to Paint Splat\n";
			Vector3 paintPos = gameWorld->FindClosestPaintSplat(this->GetTransform().GetPosition());
			paintPos = navigationMesh->FindClosestPoint(paintPos);
			return paintPos;
		}
	}

	void SecurityGuard::DetermineChaseSpeed()
	{
		if (isBlinded)
		{
			force = 0;
			return;
		}
		int numWaypoints = navigationPath->waypoints.size();
				
		float distanceToNextWaypoint = INT_MIN;
		Vector3 nextWaypoint;
		if (numWaypoints < 1)
		{
			return;
		}
		else
		{
			nextWaypoint = navigationPath->waypoints.back();
			distanceToNextWaypoint = DistanceToTarget(nextWaypoint);
		}

		if (numWaypoints == 1) // going strait to the player
		{
			if (distanceToNextWaypoint <= 40)
			{
				force = sprintForce;
				//std::cout << "1 Waypoint, less than 40, speed: " << force << "\n";
			}
			else
			{
				force = runForce;
				//std::cout << "1 Waypoint, more than 40, speed: " << force << "\n";
			}
		}
		else if (numWaypoints > 1)
		{
			Vector3 waypointAfterNext = navigationPath->waypoints.back()[-1];
			Vector3 firstWaypointDirection = nextWaypoint - this->GetTransform().GetPosition();
			Vector3 secondWaypointDirection = waypointAfterNext - nextWaypoint;
						
			if (distanceToNextWaypoint <= 40)
			{
				float dot = Vector3::Dot(firstWaypointDirection, secondWaypointDirection);
				float cosTheta = dot / (firstWaypointDirection.Length() * secondWaypointDirection.Length());
				cosTheta = (cosTheta + 3) * 0.25f;
				force = runForce * cosTheta;
			//	std::cout << "2 Waypoints, less than 40, speed: " << force << "\n";
			}
			else
			{
			//	std::cout << "2 Waypoints, more than 40, speed: " << force << "\n";
				force = runForce;
			}
		}
	}
	void SecurityGuard::DetermineSpeed()
	{
		if (isBlinded)
		{
			force = 0;
		}
		int numWaypoints = navigationPath->waypoints.size();
		if(numWaypoints<1)
		{
			return;
		}
		float distance = navigationPath->waypoints.back().Length();

		if (distance <= 15)
		{
			force = slowForce;
			return;
		}
		else
		{
			force = walkForce;
			return;
		}
	}


	void SecurityGuard::DrawTriRoute() {


		for (int i = 0; i < navigationMesh->routeVertices.size(); i += 3)
		{
			Vector3 up(0, 5, 0);
			/*Debug::DrawLine(navigationMesh->routeVertices[i], navigationMesh->routeVertices[i] + up, Debug::CYAN, 10);
			Debug::DrawLine(navigationMesh->routeVertices[i + 1], navigationMesh->routeVertices[i + 1] + up, Debug::CYAN, 10);
			Debug::DrawLine(navigationMesh->routeVertices[i + 2], navigationMesh->routeVertices[i + 2] + up, Debug::CYAN, 10);*/

			Debug::DrawLine(navigationMesh->routeVertices[i], navigationMesh->routeVertices[i + 1], Debug::CYAN, 1);
			Debug::DrawLine(navigationMesh->routeVertices[i + 1], navigationMesh->routeVertices[i + 2] , Debug::CYAN, 1);
			Debug::DrawLine(navigationMesh->routeVertices[i + 2], navigationMesh->routeVertices[i ] , Debug::CYAN, 1);
		}
	}

	void SecurityGuard::DrawNavTris() {
		int index1 = -1;
		int index2 = -1;
		int index3 = -1;
		for (auto i : navigationMesh->allTris) {
			index1 = i.indices[0];
			index2 = i.indices[1];
			index3 = i.indices[2];
//			Debug::DrawTriangle(navigationMesh->allVerts[index1], navigationMesh->allVerts[index2], navigationMesh->allVerts[index3]);
			Debug::DrawLine(navigationMesh->allVerts[index1], navigationMesh->allVerts[index2], Debug::RED, 1);
			Debug::DrawLine(navigationMesh->allVerts[index3], navigationMesh->allVerts[index2], Debug::RED, 1);
			Debug::DrawLine(navigationMesh->allVerts[index1], navigationMesh->allVerts[index3], Debug::RED, 1);


		}
	}

	void SecurityGuard::DisplayPathfinding() {

		for (int i = 1; i < navigationPath->waypoints.size(); ++i) {
			Vector3 a = navigationPath->waypoints[i - 1];
			Vector3 b = navigationPath->waypoints[i];
			Debug::DrawLine(a, b, Debug::BLACK, 10);
		}
	}

	void SecurityGuard::OutsideNavmeshRespawn(Vector3 startPos)
	{
		float minDistance = FLT_MAX;
		float distance = FLT_MAX;
		Vector3 triPos;
		for (auto i : navigationMesh->allTris)
		{
			distance = (i.centroid - startPos).Length();
			if (distance < minDistance)
			{
				triPos = i.centroid;
				std::cout << "Outside Nav position: " << triPos << "\n";
			}
		}


		reactphysics3d::Vector3 position;
		position = ~triPos;
		reactphysics3d::Transform rp3d_transform(position, rp3d::Quaternion::identity());
		this->GetRigidBody()->setTransform(rp3d_transform);
	}



}



