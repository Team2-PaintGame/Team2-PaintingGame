#include "NetworkedGame.h"
#include "NetworkPlayer.h"
#include <GameClient.h>
#include <GameServer.h>

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

NetworkedGame::NetworkedGame() {
	thisServer = nullptr;
	thisClient = nullptr;

	is_Networked = true;
	thirdPersonCamera = true;

	NetworkBase::Initialise();
	timeToNextPacket  = 0.0f;
	packetsToSnapshot = 0;
}

NetworkedGame::~NetworkedGame()	{
	delete thisServer;
	delete thisClient;
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

	thisClient->RegisterPacketHandler(Confirm_Spawn, this);
	thisClient->RegisterPacketHandler(Server_Update, this);
	thisClient->RegisterPacketHandler(String_Message, this);

	// Send packet to Server to create character

	StartLevel();
}

void NetworkedGame::UpdateGame(float dt) {
	timeToNextPacket -= dt;
	if (timeToNextPacket < 0) {
		if (thisServer) {
			StringPacket serverPacket = "Server says hello!" + std::to_string(1);
			thisServer->SendGlobalPacket(serverPacket);
			std::cout << "Server: sending packet" << std::endl;
			UpdateAsServer(dt);
		}
		else if (thisClient) {
			StringPacket clientPacket = "Client says hello!" + std::to_string(1);
			thisClient->SendPacket(clientPacket);
			std::cout << "Client: sending packet" << std::endl;
			UpdateAsClient(dt);
		}
		timeToNextPacket += 1.0f / 60.0f; //60hz server/client update
	}

	PaintingGame::UpdateGame(dt);
}

void NetworkedGame::UpdateAsServer(float dt) {
	packetsToSnapshot--;
	if (packetsToSnapshot < 0) {
		BroadcastSnapshot(false);
		packetsToSnapshot = 5;
	}
	else {
		BroadcastSnapshot(true);
	}
	thisServer->UpdateServer();
}

void NetworkedGame::UpdateAsClient(float dt) {

	//if (ClientPlayerID != -1)
	//{
		ClientPacket newPacket(ClientPlayerID, ClientPlayer->GetTransform().GetPosition());
		thisClient->SendPacket(newPacket);
	//}

	thisClient->UpdateClient();
}

void NetworkedGame::ServerCreateClientPlayer()
{
	// Server create client player and send packet
	// back to client to create server character
}

void NetworkedGame::ClientCreateServerPlayer(ConfSpawnPacket* payload)
{
	// client creates server player
}

void NetworkedGame::EnactClientUpdatesOnServer()
{
	// any changes to server character or other objects to update client
}

void NetworkedGame::EnactServerUpdatesOnClient()
{
	// any changes from client to update server
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

void NetworkedGame::SpawnPlayer() {
	if (thisServer) {
		ServerPlayer = InitiliazePlayer();
		ServerPlayerID = 1;
	}
	if (thisClient) {
		// send to server that player has been spawned
		ClientPlayer = InitiliazePlayer();
		ClientPlayerID = 2;
	}
}

void NetworkedGame::StartLevel() {
	//InitialiseAssets();
	SpawnPlayer();
	if (thisServer) {
		world->GetMainCamera()->SetThirdPersonCamera(ServerPlayer);
	}
	else if (thisClient) {
		world->GetMainCamera()->SetThirdPersonCamera(ClientPlayer);
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
		switch (payload->type) {
		case Spawn_Player:
			ServerCreateClientPlayer();
			break;
		case Client_Update:
			int id = ((ClientPacket*)payload)->GetPlayerID();
			Vector3 pos = ((ClientPacket*)payload)->GetPosition();
			EnactClientUpdatesOnServer();
			break;
		}
	}
	else if (thisClient) {
		switch (payload->type) {
		case Confirm_Spawn:
			ClientCreateServerPlayer((ConfSpawnPacket*) payload);
			break;
		case Server_Update:
			EnactServerUpdatesOnClient();
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