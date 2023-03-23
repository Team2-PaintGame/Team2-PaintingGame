#include "NetworkedGame.h"
#include "NetworkPlayer.h"
#include <GameClient.h>
#include <GameServer.h>
#include "reactphysics3d/reactphysics3d.h"
#include "Utils.h"
#include "Player.h"
#include "GameManager.h"
#include "PlayerControllers.h"

#define COLLISION_MSG 30

using namespace NCL;
using namespace NCL::CSC8508;

struct MessagePacket : public GamePacket {
	short playerID;
	short messageID;

	MessagePacket() {
		type = Message;
		size = sizeof(short) * 2;
	}
};

NetworkedGame::NetworkedGame(GameAssets* assets) : PaintingGame(assets) {
	thisServer = nullptr;
	thisClient = nullptr;

	NetworkBase::Initialise();
	timeToNextPacket  = 0.0f;
	packetsToSnapshot = 0;

	if (!GameManager::sConfig.playerControllerFactory) {
		GameManager::sConfig.playerControllerFactory = new Win32PlayerControllerFactory();
	}
}

NetworkedGame::~NetworkedGame()	{
	delete thisServer;
	delete thisClient;

	delete playerController;
}

void NetworkedGame::StartAsServer() {
	thisServer = new GameServer(NetworkBase::GetDefaultPort(), 4);

	thisServer->RegisterPacketHandler(Received_State, this);
	thisServer->RegisterPacketHandler(Client_Update, this);
	thisServer->RegisterPacketHandler(Spawn_Player, this);
	thisServer->RegisterPacketHandler(String_Message, this);

	StartLevel();
}

void NetworkedGame::StartAsClient(char a, char b, char c, char d) {
	thisClient = new GameClient();
	thisClient->Connect(a, b, c, d, NetworkBase::GetDefaultPort());

	thisClient->RegisterPacketHandler(Delta_State, this);
	thisClient->RegisterPacketHandler(Full_State, this);
	thisClient->RegisterPacketHandler(Player_Connected, this);
	thisClient->RegisterPacketHandler(Player_Disconnected, this);
	thisClient->RegisterPacketHandler(Spawn_Player, this);

	thisClient->RegisterPacketHandler(Confirm_Spawn, this);
	thisClient->RegisterPacketHandler(Server_Update, this);
	thisClient->RegisterPacketHandler(String_Message, this);

}

void NetworkedGame::Update(float dt) {
	timeToNextPacket -= dt;
	if (timeToNextPacket < 0) {
		if (thisServer) {
			std::cout << "Server: sending packet" << std::endl;
			UpdateAsServer(dt);
			justShot = false;
		}
		else if (thisClient) {
			std::cout << "Client: sending packet" << std::endl;
			UpdateAsClient(dt);
			justShot = false;
		}
		timeToNextPacket += 1.0f / 60.0f; //60hz server/client update
	}

	if ((thisClient && connected) || thisServer)
	{
		playerController->Update(dt);

		if (thisServer)
		{
			justShot = ServerPlayer->GetGun()->didJustShoot() ? true : justShot;
		}
		else if (thisClient)
		{
			justShot = ClientPlayer->GetGun()->didJustShoot() ? true : justShot;
		}

		PaintingGame::Update(dt);
	}
}

void NetworkedGame::UpdateAsServer(float dt) {
	ServerPacket packet;
	packet.orientation = ServerPlayer->GetTransform().GetOrientation();
	packet.position = ServerPlayer->GetTransform().GetPosition();
	packet.playerID = ServerPlayerID;
	packet.startedShooting = justShot;
	packet.gunPitch = ServerPlayer->GetPitch();
	thisServer->SendGlobalPacket(packet);
	thisServer->UpdateServer();
	Window::GetWindow()->LockMouseToWindow(false);
}

void NetworkedGame::UpdateAsClient(float dt) {
	if (thisClient->okToSpawn)
	{
		StartLevel();
		thisClient->okToSpawn = false;
		connected = true;
	}
	else if(connected) {
		ClientPacket packet;
		packet.orientation = ClientPlayer->GetTransform().GetOrientation();
		packet.position = ClientPlayer->GetTransform().GetPosition();
		packet.playerID = ClientPlayerID;
		packet.startedShooting = justShot;
		packet.gunPitch = ClientPlayer->GetPitch();
		thisClient->SendPacket(packet);
	}
	thisClient->UpdateClient();
	Window::GetWindow()->LockMouseToWindow(false);
}

void NetworkedGame::ServerCreateClientPlayer(SpawnPacket* payload)
{
	// Server create client player and send packet
	// back to client to create server character
	ClientPlayer = CreatePlayer(payload->position, Team::Blue, true);
	ClientPlayer->GetTransform().SetPosition(payload->position);
	ClientPlayerID = payload->playerID;
	SpawnPacket packet;
	packet.playerID = ServerPlayerID;
	packet.position = ServerPlayer->GetTransform().GetPosition();
	packet.seed = seed;
	thisServer->SendGlobalPacket(packet);
	thisServer->UpdateServer();
	AddSecurityAI(Vector3(100, 5, 100), ClientPlayer, ServerPlayer, packet.seed);
}

void NetworkedGame::ClientCreateServerPlayer(SpawnPacket* payload)
{
	// client creates server player
	ServerPlayer = CreatePlayer(payload->position, Team::Red, true);
	ServerPlayerID = payload->playerID;
	AddSecurityAI(Vector3(100, 5, 100), ClientPlayer, ServerPlayer, payload->seed);
}

void NetworkedGame::EnactClientUpdatesOnServer(ClientPacket* payload)
{
	// any changes to server character or other objects to update client
	if (ClientPlayer) {
		ClientPlayer->GetTransform().SetOrientation(payload->orientation);
		ClientPlayer->GetTransform().SetPosition(payload->position);
		ClientPlayer->SetPitch(payload->gunPitch);

		reactphysics3d::Transform newRBTransform = reactphysics3d::Transform(~payload->position, ~payload->orientation);
		ClientPlayer->GetRigidBody()->setTransform(newRBTransform);
		if (payload->startedShooting)
		{
			ClientPlayer->Shoot();
		}
	}
}

void NetworkedGame::EnactServerUpdatesOnClient(ServerPacket* payload)
{
	// any changes from client to update server
	if (ServerPlayer) {
		ServerPlayer->GetTransform().SetOrientation(payload->orientation);
		ServerPlayer->GetTransform().SetPosition(payload->position);
		ServerPlayer->SetPitch( payload->gunPitch);

		reactphysics3d::Transform newRBTransform = reactphysics3d::Transform(~payload->position, ~payload->orientation);
		ServerPlayer->GetRigidBody()->setTransform(newRBTransform);

		if (payload->startedShooting)
		{
			ServerPlayer->Shoot();
		}
	}
}

void NetworkedGame::BroadcastSnapshot(bool deltaFrame) {
	std::vector<GameObject*>::const_iterator first;
	std::vector<GameObject*>::const_iterator last;

	world->GetObjectIterators(first, last);

	for (auto i = first; i != last; ++i) {
		NetworkObject* o = (*i)->GetNetworkObject();
		if (!o) {
			continue;
		}

		int playerState = 0;
		GamePacket* newPacket = nullptr;
		if (o->WritePacket(&newPacket, deltaFrame, playerState)) {
			thisServer->SendGlobalPacket(*newPacket);
			delete newPacket;
		}
	}
}

void NetworkedGame::UpdateMinimumState() {
	//Periodically remove old data from the server
	int minID = INT_MAX;
	int maxID = 0; //we could use this to see if a player is lagging behind?

	for (auto i : stateIDs) {
		minID = std::min(minID, i.second);
		maxID = std::max(maxID, i.second);
	}
	//every client has acknowledged reaching at least state minID
	//so we can get rid of any old states!
	std::vector<GameObject*>::const_iterator first;
	std::vector<GameObject*>::const_iterator last;
	world->GetObjectIterators(first, last);

	for (auto i = first; i != last; ++i) {
		NetworkObject* o = (*i)->GetNetworkObject();
		if (!o) {
			continue;
		}
		o->UpdateStateHistory(minID); //clear out old states so they arent taking up memory...
	}
}

void NetworkedGame::CreateSplatOnShoot() {
	//if (thisServer) {
	//	if (playerController->Shoot()) {
	//		SceneContactPoint* closestCollision = world->Raycast(ServerPlayer->GetShootRay());
	//		if (closestCollision->isHit) {
	//			world->AddPaintedPosition(closestCollision->hitPos);

	//			//Broadcast Paint Position
	//		}
	//	}
	//}
	//if (thisClient) {
	//	if (playerController->Shoot()) {
	//		SceneContactPoint* closestCollision = world->Raycast(ClientPlayer->GetShootRay());
	//		if (closestCollision->isHit) {
	//			world->AddPaintedPosition(closestCollision->hitPos);
	//			//Broadcast Paint Position
	//		}
	//	}
	//}
}

NCL::Player* NetworkedGame::SpawnNetworkedPlayer() {
	if (thisServer) {
		ServerPlayer = AddPlayer(Vector3(15.0f, 15.0f, 15.0f), Team::Red);
		ServerPlayerID = 1;
		return ServerPlayer;
	}
	if (thisClient) {
		// send to server that player has been spawned
		ClientPlayer = AddPlayer(Vector3(115.0f, 15.0f, 55.0f), Team::Blue);
	//	AddSecurityAI(Vector3(100, 5, 100), ServerPlayer, ClientPlayer);
		ClientPlayerID = 2;
		SpawnPacket packet;
		packet.position = ClientPlayer->GetTransform().GetPosition();
		packet.playerID = ClientPlayerID;
		thisClient->SendPacket(packet);
		return ClientPlayer;
	}
	return nullptr;
}

Player* NetworkedGame::AddPlayer(Vector3 position, Team team) {
	Player* player = CreatePlayer(position, team);
	activeCameras.push_back(player->GetCamera());

	return player;
}

void NetworkedGame::StartLevel() {

	InitWorld();
	SpawnNetworkedPlayer();
	if (thisServer)
	{
		playerController = GameManager::sConfig.playerControllerFactory->createPlayerController(ServerPlayer);
	}
	else if (thisClient)
	{
		playerController = GameManager::sConfig.playerControllerFactory->createPlayerController(ClientPlayer);
	}
}

void NetworkedGame::ReceivePacket(int type, GamePacket* payload, int source) {
	if (type == String_Message) {
		StringPacket* realPacket = (StringPacket*)payload;

		string msg = realPacket->GetStringFromData();

		std::cout << " received message : " << msg << std::endl;
		return;
	}
	if (thisServer) {
		std::cout << "Server: " << payload->type << std::endl;
		switch (payload->type) {
		case Spawn_Player:
			ServerCreateClientPlayer((SpawnPacket*) payload);
			std::cout << "SPAWN Packet recieved";
			break;
		case Client_Update:
			EnactClientUpdatesOnServer((ClientPacket*) payload);
			break;
		}
	}
	else if (thisClient) {
		switch (payload->type) {
		case Spawn_Player:
			ClientCreateServerPlayer((SpawnPacket*) payload);
			break;
		case Server_Update:
			EnactServerUpdatesOnClient((ServerPacket*) payload);
			break;
		case Full_State:
			// something here
			break;
		}
	}
}

void NetworkedGame::OnPlayerCollision(NetworkPlayer* a, NetworkPlayer* b) {
	if (thisServer) { //detected a collision between players!
		MessagePacket newPacket;
		newPacket.messageID = COLLISION_MSG;
		newPacket.playerID  = a->GetPlayerNum();

		thisClient->SendPacket(newPacket);

		newPacket.playerID = b->GetPlayerNum();
		thisClient->SendPacket(newPacket);
	}
}
