#pragma once
#ifdef _WIN32
#include "PaintingGame.h"
#include <NetworkBase.h>
#include <NetworkObject.h>
#include "GameClient.h"
#include "GameServer.h"
#include <array>
#include <PlayerController.h>

namespace NCL {
	namespace CSC8508 {
		class GameServer;
		class GameClient;
		class NetworkPlayer;

		class NetworkedGame : public PaintingGame, public PacketReceiver {
		public:
			NetworkedGame(GameAssets* assets);
			~NetworkedGame();

			void StartAsServer();
			void StartAsClient(char a, char b, char c, char d);

			void Update(float dt) override;

			void StartLevel();

			void ReceivePacket(int type, GamePacket* payload, int source) override;

			void OnPlayerCollision(NetworkPlayer* a, NetworkPlayer* b);

		protected:
			Player* SpawnNetworkedPlayer();
			Player* AddPlayer(Vector3 position, Team team) override;

			void UpdateAsServer(float dt);
			void UpdateAsClient(float dt);

			void ServerCreateClientPlayer(SpawnPacket* payload);

			void ClientCreateServerPlayer(SpawnPacket* payload);

			void EnactClientUpdatesOnServer(ClientPacket* payload);

			void EnactServerUpdatesOnClient(ServerPacket* payload);


			void BroadcastSnapshot(bool deltaFrame);
			void UpdateMinimumState();


		protected:
			virtual void CreateSplatOnShoot();
			std::map<int, int> stateIDs;

			GameServer* thisServer;
			GameClient* thisClient;
			float timeToNextPacket;
			int packetsToSnapshot;

			std::vector<NetworkObject*> networkObjects;

			Player* ServerPlayer;
			Player* ClientPlayer;
			int ServerPlayerID;
			int ClientPlayerID;

			PlayerController* playerController;

			bool connected = false;

			bool justShot = false;

		};
	}
}
#endif
