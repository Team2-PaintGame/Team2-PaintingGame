#include "SecurityGuard.h"
#include <fstream>
#include "Assets.h"

namespace NCL::CSC8508 {
	SecurityGuard::SecurityGuard(reactphysics3d::PhysicsCommon& physicsCommon, reactphysics3d::PhysicsWorld* physicsWorld, std::string objectName, Vector3 position, MeshGeometry* mesh, TextureBase* texture, ShaderBase* shader, Vector3 size, GameObject* player)
		: GameObject(physicsCommon, physicsWorld, objectName)
	{

		this->player = player;
		this->physicsWorld = physicsWorld;
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
		callbackObject = new SecurityCallbackClass(player);

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

		delete callbackObject;

		delete navigationGrid;
		delete navigationPath;

		physicsCommon.destroyBoxShape(boundingVolume);

	}
	void SecurityGuard::Update(float dt)
	{
		DisplayPathfinding();
		if (state == Initialise) {
			state = Ongoing;
		}
		if (state == Ongoing) {
			//	std::cout << "root Execution !\n";
			state = rootSelector->Execute(dt);
		}
		if (state == Success) {
			rootSelector->Reset();
			state = Initialise;
			//	std::cout << "What a successful adventure !\n";
		}
		if (state == Failure) {
			state = Initialise;
			//	std::cout << "What a waste of time !\n";
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
				std::cout << "Choose Destination - Initialise\n";
				state = Ongoing;
			}

			else if (state == Ongoing) {
				Vector3 destination = ChooseDestination();
				Vector3 securityPosition = this->GetTransform().GetPosition();
				securityPosition = FindClosestNode(securityPosition);

				bool foundPath = navigationGrid->FindPath(securityPosition, destination, *navigationPath);
				std::cout << "Choose Destination - Found Path\n";

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
				std::cout << " Going to the destination - Initialise \n";
				state = Ongoing;
			}
			else if (state == Ongoing) {

				Vector3 direction = navigationPath->waypoints.back() - this->GetTransform().GetPosition();
				Vector3 velocity = rigidBody->getLinearVelocity();
				std::cout << "Velocity = " << velocity.Length() << "\n";
				if (LookForPlayer()) {
					navigationPath->Clear();
					std::cout << " Going to the destination - Can see Player\n";
					return Success;
				}
				if (direction.Length() <= 3 && navigationPath->waypoints.size() >= 2) {
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

				bool isPLayerVisible = LookForPlayer();
				if (isPLayerVisible) {
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
				Vector3 playerPosition = player->GetTransform().GetPosition();
				Vector3 securityPosition = this->GetTransform().GetPosition();
				playerPosition = FindClosestNode(playerPosition);
				securityPosition = FindClosestNode(securityPosition);

				navigationGrid->FindPath(securityPosition, playerPosition, *navigationPath);
				std::cout << " Chase the Player - Path found\n";
				state = Ongoing;
			}
			else if (state == Ongoing) {
				Vector3 direction = navigationPath->waypoints.back() - this->GetTransform().GetPosition();
				if (direction.Length() <= 4 && navigationPath->waypoints.size() >= 2) {
					navigationPath->waypoints.pop_back();
					std::cout << " Chase the Player - Chasing player\n";
				}
				if ( navigationPath->waypoints.size() == 1) {
					if (DistanceToTarget(navigationPath->waypoints.back()) <= 4.0f) {
						std::cout << " Chase the Player - Reached the player\n";
						navigationPath->Clear();
						return Success;
					}
				}
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
				Vector3 direction = player->GetTransform().GetPosition() - this->GetTransform().GetPosition();
				if (direction.Length() > 15) {
					//			std::cout << "The goat is too far away..\n";
					state = Failure;
				}
				else {
					//std::cout << "Spitting at goat..\n";
					/*player->GetRenderObject()->SetColour(Debug::RED);
					player->SetHealth();
					player->GetRenderObject()->SetColour(Debug::GREEN);*/

					state = Success;
				}
			}

			return state;
			}

		);
	}


	bool SecurityGuard::LookForPlayer()
	{
		Vector3 securityPosition = this->GetTransform().GetPosition();
		Vector3 playerPosition = player->GetTransform().GetPosition();
		Debug::DrawLine(securityPosition, playerPosition, Debug::BLUE, 0.1f);
		reactphysics3d::Ray ray(~securityPosition, ~playerPosition); // '~' converts NCL Vector3 to reactphysics3d Vector3
		reactphysics3d::RaycastInfo rayCastInfo;

		physicsWorld->raycast(ray, *(&callbackObject));
		callbackObject->notifyRaycastHit(rayCastInfo);

		bool isPlayerVisible = callbackObject->IsPlayerVisible();
		callbackObject->ResetObjectDistances();
		return isPlayerVisible;

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
		std::cout << "ChooseDestination: " << destination << "\n";
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
		float force = 100;
		reactphysics3d::Vector3 forceDirection;
		forceDirection = ~direction.Normalised();
		this->GetRigidBody()->applyWorldForceAtCenterOfMass(forceDirection*force);// '~' converts NCL Vector3 to reactphysics3d Vector3
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









}



