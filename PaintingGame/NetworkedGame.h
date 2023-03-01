#pragma once
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
			PlayerBase* SpawnPlayer();
			PlayerBase* AddPlayer(Camera* camera, Vector3 position, Gamepad* gamepad) override;

			void UpdateAsServer(float dt);
			void UpdateAsClient(float dt);

			void ServerCreateClientPlayer(SpawnPacket* payload);

			void ClientCreateServerPlayer(SpawnPacket* payload);

			void EnactClientUpdatesOnServer(ClientPacket* payload);

			void EnactServerUpdatesOnClient(ServerPacket* payload);


			void BroadcastSnapshot(bool deltaFrame);
			void UpdateMinimumState();


		protected:
			std::map<int, int> stateIDs;

			GameServer* thisServer;
			GameClient* thisClient;
			float timeToNextPacket;
			int packetsToSnapshot;

			std::vector<NetworkObject*> networkObjects;

			PlayerBase* ServerPlayer;
			PlayerBase* ClientPlayer;
			int ServerPlayerID;
			int ClientPlayerID;

			PlayerController* playerController;

			bool connected = false;

		};
	}
}

