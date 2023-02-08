#pragma once
#include "PaintingGame.h"
#include <NetworkBase.h>
#include <NetworkObject.h>
#include "GameClient.h"
#include "GameServer.h"

namespace NCL {
	namespace CSC8508 {
		class GameServer;
		class GameClient;
		class NetworkPlayer;

		class NetworkedGame : public PaintingGame, public PacketReceiver {
		public:
			NetworkedGame();
			~NetworkedGame();

			void StartAsServer();
			void StartAsClient(char a, char b, char c, char d);

			void UpdateGame(float dt) override;

			void SpawnPlayer();

			void StartLevel();

			void ReceivePacket(int type, GamePacket* payload, int source) override;

			void OnPlayerCollision(NetworkPlayer* a, NetworkPlayer* b);

		protected:
			void UpdateAsServer(float dt);
			void UpdateAsClient(float dt);

			void ServerCreateClientPlayer(SpawnPacket* payload);

			void ClientCreateServerPlayer(SpawnPacket* payload);

			void EnactClientUpdatesOnServer(ClientPacket* payload);

			void EnactServerUpdatesOnClient(ServerPacket* payload);


			void BroadcastSnapshot(bool deltaFrame);
			void UpdateMinimumState();
			std::map<int, int> stateIDs;

			GameServer* thisServer;
			GameClient* thisClient;
			float timeToNextPacket;
			int packetsToSnapshot;

			std::vector<NetworkObject*> networkObjects;

			std::map<int, GameObject*> serverPlayers;
			PlayerBase* ServerPlayer;
			PlayerBase* ClientPlayer;
			int ServerPlayerID;
			int ClientPlayerID;

			bool connected = false;
		};
	}
}

