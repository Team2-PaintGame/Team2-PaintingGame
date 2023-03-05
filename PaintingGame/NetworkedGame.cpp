#include "NetworkedGame.h"
#include "NetworkPlayer.h"
#include <GameClient.h>
#include <GameServer.h>
#include "reactphysics3d/reactphysics3d.h"
#include "Utils.h"
#include "PlayerBase.h"
#include "GameManager.h"
#include "Win32Window.h"

#define COLLISION_MSG 30

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
			/*StringPacket serverPacket = "Server says hello!" + std::to_string(1);
			thisServer->SendGlobalPacket(serverPacket);*/
			std::cout << "Server: sending packet" << std::endl;
			UpdateAsServer(dt);
		}
		else if (thisClient) {
			/*StringPacket clientPacket = "Client says hello!" + std::to_string(1);
			thisClient->SendPacket(clientPacket);*/
			std::cout << "Client: sending packet" << std::endl;
			UpdateAsClient(dt);
		}
		timeToNextPacket += 1.0f / 60.0f; //60hz server/client update
	}

	if ((thisClient && connected) || thisServer)
	{
		//ServerPlayer->GetCamera()->UpdateCamera(dt);
		playerController->Update(dt);
		PaintingGame::Update(dt);
	}
}

void NetworkedGame::UpdateAsServer(float dt) {
	//packetsToSnapshot--;
	//if (packetsToSnapshot < 0) {
	//	BroadcastSnapshot(false);
	//	packetsToSnapshot = 5;
	//}
	//else {
	//	BroadcastSnapshot(true);
	//}

	ServerPacket packet;
	packet.orientation = ServerPlayer->GetTransform().GetOrientation();
	packet.position = ServerPlayer->GetTransform().GetPosition();
	packet.playerID = ServerPlayerID;
	thisServer->SendGlobalPacket(packet);
	thisServer->UpdateServer();
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
		thisClient->SendPacket(packet);
	}
	thisClient->UpdateClient();
}

void NetworkedGame::ServerCreateClientPlayer(SpawnPacket* payload)
{
	// Server create client player and send packet
	// back to client to create server character
	ClientPlayer = CreatePlayer(payload->position);
	ClientPlayer->GetTransform().SetPosition(payload->position);
	ClientPlayerID = payload->playerID;
	SpawnPacket packet;
	packet.playerID = ServerPlayerID;
	packet.position = ServerPlayer->GetTransform().GetPosition();
	thisServer->SendGlobalPacket(packet);
	thisServer->UpdateServer();
}

void NetworkedGame::ClientCreateServerPlayer(SpawnPacket* payload)
{
	// client creates server player
	ServerPlayer = CreatePlayer(payload->position);
	ServerPlayerID = payload->playerID;
}

void NetworkedGame::EnactClientUpdatesOnServer(ClientPacket* payload)
{
	// any changes to server character or other objects to update client
	if (ClientPlayer) {
		ClientPlayer->GetTransform().SetOrientation(payload->orientation);
		ClientPlayer->GetTransform().SetPosition(payload->position);

		reactphysics3d::Transform newRBTransform = reactphysics3d::Transform(~payload->position, ~payload->orientation);
		ClientPlayer->GetRigidBody()->setTransform(newRBTransform);
	}
}

void NetworkedGame::EnactServerUpdatesOnClient(ServerPacket* payload)
{
	// any changes from client to update server
	if (ServerPlayer) {
		ServerPlayer->GetTransform().SetOrientation(payload->orientation);
		ServerPlayer->GetTransform().SetPosition(payload->position);

		reactphysics3d::Transform newRBTransform = reactphysics3d::Transform(~payload->position, ~payload->orientation);
		ServerPlayer->GetRigidBody()->setTransform(newRBTransform);
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

NCL::PlayerBase* NetworkedGame::SpawnPlayer() {
	if (thisServer) {
		ServerPlayer = CreatePlayer(Vector3(5.0f, 15.0f, 5.0f));
		world->AddGameObject(ServerPlayer);
		ServerPlayerID = 1;
		return ServerPlayer;
	}
	if (thisClient) {
		// send to server that player has been spawned
		ClientPlayer = CreatePlayer(Vector3(-5.0f, 10.0f, -5.0f));
		world->AddGameObject(ClientPlayer);
		ClientPlayerID = 2;
		SpawnPacket packet;
		packet.position = ClientPlayer->GetTransform().GetPosition();
		packet.playerID = ClientPlayerID;
		thisClient->SendPacket(packet);
		return ClientPlayer;
	}
	return nullptr;
}

NCL::PlayerBase* NetworkedGame::AddPlayer(Vector3 position) {
	return nullptr;
}

void NetworkedGame::StartLevel() {

	InitWorld();

	PlayerBase* player = SpawnPlayer();
	//InitCamera(*world->GetMainCamera(), *player, 1.0f );

	playerController = GameManager::sConfig.playerControllerFactory->createPlayerController(player);
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
			std::cout << "SPAWN Packet recieved ))))))))))))))";
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
