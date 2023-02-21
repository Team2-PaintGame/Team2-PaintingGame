#include "SecurityGuard.h"
#include <fstream>
#include "Assets.h"
#include <cmath>

namespace NCL::CSC8508 {
	SecurityGuard::SecurityGuard(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, std::string objectName, Vector3 position, MeshGeometry* mesh,
		TextureBase* texture, ShaderBase* shader, Vector3 size, GameObject* playerOne, GameObject* playerTwo)
		: GameObject(physicsCommon, physicsWorld, objectName)
	{

		this->playerOne = playerOne;
		this->playerTwo = playerTwo;
		this->physicsWorld = physicsWorld;
		chasedPlayer = nullptr;
		transform.SetScale(size).SetPosition(position);
		renderObject = new RenderObject(&transform, mesh, shader);
		renderObject->AddTexture(texture);
		renderObject->SetColour(Debug::RED);
		boundingVolume = physicsCommon.createBoxShape(~transform.GetScale() / 2.0f);
		reactphysics3d::Transform rp3d_transform(~position, rp3d::Quaternion::identity());

		// Create a rigid body in the physics world
		rigidBody = physicsWorld->createRigidBody(rp3d_transform);
		rigidBody->addCollider(boundingVolume, rp3d::Transform::identity()); //collider
		rigidBody->updateMassPropertiesFromColliders();
		rigidBody->setLinearDamping(1.5f);
		rigidBody->setAngularDamping(1.5f);
		callbackPlayerOne = new SecurityCallbackClass(playerOne);
		callbackPlayerTwo = new SecurityCallbackClass(playerTwo);

		FindNavigableNodes("GameMaze3.txt", navigableNodes);
		navigationGrid = new NavigationGrid("GameMaze3.txt");
		navigationPath = new NavigationPath();

		InitBehaviorTree();



	}
	SecurityGuard::~SecurityGuard()
	{
		delete rootSelector;
		delete patrolSequence;
		delete chaseSequence;

		delete chooseDestination;
		delete goToDestination;

		delete lookForPlayer;
		delete chaseThePlayer;
		delete attackThePlayer;

		delete callbackPlayerOne;
		delete callbackPlayerTwo;

		delete navigationGrid;
		delete navigationPath;

		// delete renderObject // Is this deleted here or elsewhere?

		physicsCommon.destroyBoxShape(boundingVolume);

	}
	void SecurityGuard::Update(float dt)
	{
		DisplayPathfinding();
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

	}
	void SecurityGuard::InitBehaviorTree()
	{
		state = Ongoing;

		InitChooseDestination();
		InitGoToDesination();

		InitLookForPlayer();
		InitChaseThePlayer();
		InitAttackThePlayer();

		InitPatrolSequence();
		InitChaseSequence();
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
		rootSelector->AddChild(chaseSequence);
		rootSelector->AddChild(patrolSequence);
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
				securityPosition = FindClosestNode(securityPosition);
				bool foundPath = navigationGrid->FindPath(securityPosition, destination, *navigationPath);
				
				//std::cout << "Choose Destination - Found Path\n";
				return Success;
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
				
				Vector3 direction = navigationPath->waypoints.back() - this->GetTransform().GetPosition();
				Vector3 velocity = rigidBody->getLinearVelocity();
								
				if (LookForPlayers() != nullptr) {
					navigationPath->Clear();
					std::cout << " Going to the destination - Can see Player\n";
					return Success;
				}
				if (velocity.Length() < 0.1) {
					//std::cout << "Velocity = " << velocity.Length() << "\n";
					timeAccumulator += dt;
					if (timeAccumulator > 5.0) {
						timeAccumulator = 0.0f;
						navigationPath->Clear();
						return Success;
					}
				}
				if (direction.Length() <= 4 && navigationPath->waypoints.size() >= 2) {
					std::cout << " Going to the destination - Going Destination\n";
					navigationPath->waypoints.pop_back();
				}
	
				if (navigationPath->waypoints.size() == 1) {
					if (DistanceToTarget(navigationPath->waypoints.front()) <= 3.0f) {
						std::cout << " Going to the destination - Reached Destination\n";
						navigationPath->Clear();
						return Success;
					}
				}
				DetermineSpeed();
				MoveSecurityGuard(direction);
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
					std::cout << " Looking for Player - Can see Player\n";
					state = Success;
				}
				else {
					std::cout << " Looking for Player - Player not visible\n";
					state = Failure;
				}
			}
			return state;
			}
		);
	}
	void SecurityGuard::InitChaseThePlayer()
	{
		chaseThePlayer = new BehaviourAction("Chase the goat ", [&](float dt, BehaviourState state)->BehaviourState {

			if (state == Initialise) {
				Vector3 playerPosition = chasedPlayer->GetTransform().GetPosition();
				Vector3 securityPosition = this->GetTransform().GetPosition();
				playerPosition = FindClosestNode(playerPosition);
				securityPosition = FindClosestNode(securityPosition);

				navigationGrid->FindPath(securityPosition, playerPosition, *navigationPath);
					std::cout << " Chase the Player - Path found\n";
				state = Ongoing;
			}
			else if (state == Ongoing) {
				Vector3 direction = navigationPath->waypoints.back() - this->GetTransform().GetPosition();
				Vector3 nextDirection = (navigationPath->waypoints.back() - 1) - this->GetTransform().GetPosition();

				timeAccumulator += dt;
				if (timeAccumulator >= 1.5) {
					bool isPlayerVisible = LookForPlayer(chasedPlayer);
					//	std::cout << "1.5 seconds accumulated\n";
					timeAccumulator = 0.0f;
					if (isPlayerVisible) {
						navigationPath->Clear();
						return Failure;
					}

				}

				if (direction.Length() <= 4 && navigationPath->waypoints.size() >= 2) {
					navigationPath->waypoints.pop_back();
						std::cout << " Chase the Player - Chasing player\n";
				}
				if (navigationPath->waypoints.size() == 1) {
					if (DistanceToTarget(navigationPath->waypoints.back()) <= 4.0f) {
								std::cout << " Chase the Player - Reached the player\n";
						navigationPath->Clear();
						timeAccumulator = 0.0f;
						return Success;
					}
				}
				DetermineSpeed();
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
				Vector3 direction = chasedPlayer->GetTransform().GetPosition() - this->GetTransform().GetPosition();
				if (direction.Length() > 15) {
					//	std::cout << "The player is too far away..\n";
					state = Failure;
				}
				else {
					// Respawns player at position location
					reactphysics3d::Vector3 position(0, 20, 0);
					reactphysics3d::Transform rp3d_transform(position, rp3d::Quaternion::identity());
					chasedPlayer->GetRigidBody()->setTransform(rp3d_transform);
					state = Success;
				}
			}

			return state;
			}

		);
	}

	GameObject* SecurityGuard::LookForPlayers()
	{
		bool isPlayerOneVisible = LookForPlayer(playerOne);
		bool isPlayerTwoVisible = LookForPlayer(playerTwo);

		if (isPlayerOneVisible && isPlayerTwoVisible) //both players are visible
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

		if (player == playerOne) 
		{
			RaycastAgainstPlayer(playerOne, callbackPlayerOne, isPlayerVisible);
		}
		else if (player == playerTwo) 
		{
			RaycastAgainstPlayer(playerTwo, callbackPlayerTwo, isPlayerVisible);
		}
		return isPlayerVisible;
	}

	void SecurityGuard::RaycastAgainstPlayer(GameObject* player, SecurityCallbackClass* callbackClass, bool& isPlayerVisible)
	{
		Vector3 securityPosition = this->GetTransform().GetPosition();
		Vector3 playerPosition = player->GetTransform().GetPosition();
		if (player == playerOne) {
			Debug::DrawLine(securityPosition, playerPosition, Debug::BLUE, 0.1f);
		}
		else if (player == playerTwo) {
			Debug::DrawLine(securityPosition, playerPosition, Debug::RED, 0.1f);
		}
		reactphysics3d::Ray ray(~securityPosition, ~playerPosition); // '~' converts NCL Vector3 to reactphysics3d Vector3
		reactphysics3d::RaycastInfo rayCastInfo;
		physicsWorld->raycast(ray, *(&callbackClass));
		callbackClass->notifyRaycastHit(rayCastInfo);
		isPlayerVisible = callbackClass->IsPlayerVisible();
		callbackClass->ResetObjectDistances();
	}

	void SecurityGuard::FindNavigableNodes(const std::string& filename, vector<Vector3>& navigableNodes)
	{
		std::ifstream infile(Assets::DATADIR + filename);

		infile >> nodeSize;
		infile >> gridWidth;
		infile >> gridHeight;

		GridNode* allNodes = new GridNode[gridWidth * gridHeight];

		for (int y = 0; y < gridHeight; ++y) {
			for (int x = 0; x < gridWidth; ++x) {
				GridNode& n = allNodes[(gridWidth * y) + x];
				char type = 0;
				infile >> type;
				n.type = type;
				n.position = Vector3((float)(x * nodeSize) - 75, 2.5, (float)(y * nodeSize) -75);
				if (n.type == '.') {
					navigableNodes.push_back(n.position);
					//std::cout << "Navigble Node Pos: " << n.position << "\n";
				}

			}
		}
	}

	Vector3 SecurityGuard::ChooseDestination() 
	{
		int numNodes = navigableNodes.size();
		int randomNum = rand() % numNodes;
		Vector3 destination = navigableNodes[randomNum];
		//std::cout << "ChooseDestination: " << destination << "\n";
		return destination;
	}

	void SecurityGuard::DisplayPathfinding() {
		for (int i = 1; i < navigationPath->waypoints.size(); ++i) {
			Vector3 a = navigationPath->waypoints[i - 1];
			Vector3 b = navigationPath->waypoints[i];
			Debug::DrawLine(a, b, Vector4(0, 1, 0, 1), 0.05);
		}
	}

	void SecurityGuard::MoveSecurityGuard(Vector3 direction)
	{
		reactphysics3d::Vector3 forceDirection;
		forceDirection = ~direction.Normalised();
		this->GetRigidBody()->applyWorldForceAtCenterOfMass(forceDirection * force);// '~' converts NCL Vector3 to reactphysics3d Vector3
	}

	float SecurityGuard::DistanceToTarget(Vector3 destination) 
	{
		return (destination - this->GetTransform().GetPosition()).Length();
	}

	Vector3 SecurityGuard::FindClosestNode(Vector3 position)
	{
		Vector3 closestNode;
		float closestDistance = 100;
		for (auto i : navigableNodes) {
			if ((position - i).Length() < closestDistance) {
				closestNode = i;
				closestDistance = (position - i).Length();
			}
		}
		//	std::cout << "Closest Node: " << closestNode << "\n";
		return closestNode;
	}
	void SecurityGuard::DetermineSpeed()
	{
		int numWaypoints = navigationPath->waypoints.size();
		if(numWaypoints<1)
		{
			return;
		}
		if (numWaypoints == 1)
		{
			force = slowForce;
			return;
		}
		if (numWaypoints == 2 || numWaypoints == 3)
		{
			force = walkForce;
			return;
		}
		
		vector<Vector3>::iterator it = navigationPath->waypoints.end() - 1;
		if (numWaypoints >= 4)
		{
			Vector3 nextThreeWaypoints[3];
			int i = 0;
			for (it ; it != navigationPath->waypoints.end() - 4; --it)
			{
				nextThreeWaypoints[i] = *it;
				++i;
			}
			Vector3 playerPos = this->GetTransform().GetPosition();
			playerPos = FindClosestNode(playerPos);
			Vector3 firstWaypointDirection, secondWaypointDirection, thirdWaypointDirection;
			firstWaypointDirection = (nextThreeWaypoints[0] - playerPos).Normalised();
			secondWaypointDirection = (nextThreeWaypoints[1] - playerPos).Normalised();
			thirdWaypointDirection = (nextThreeWaypoints[2] - playerPos).Normalised();

			if (firstWaypointDirection == secondWaypointDirection && secondWaypointDirection == thirdWaypointDirection) //Going in a straight line
			{
				force = runForce;
				return;
			}

			if (firstWaypointDirection == secondWaypointDirection && secondWaypointDirection != thirdWaypointDirection) // third waypoint there is a turn
			{
				force = walkForce;
				return;
			}
			if (firstWaypointDirection != secondWaypointDirection && secondWaypointDirection != thirdWaypointDirection) // second waypoint there is a turn
			{
				force = slowForce;
				return;
			}
		}
	}









}



