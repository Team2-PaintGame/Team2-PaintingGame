#pragma once
#include "GameObject.h"
#include "NetworkBase.h"
#include "NetworkState.h"

namespace NCL::CSC8508 {
	class GameObject;

	struct FullPacket : public GamePacket {
		int		objectID = -1;
		NetworkState fullState;

		FullPacket() {
			type = Full_State;
			size = sizeof(FullPacket) - sizeof(GamePacket);
		}
	};

	struct DeltaPacket : public GamePacket {
		int		fullID		= -1;
		int		objectID	= -1;
		char	pos[3];
		char	orientation[4];

		DeltaPacket() {
			type = Delta_State;
			size = sizeof(DeltaPacket) - sizeof(GamePacket);
		}
	};

	struct ClientPacket : public GamePacket {
		int		lastID;
		int playerID;
		Vector3	position;

		ClientPacket(int id, Vector3 pos) {
			playerID = id;
			position = pos;
			type = Client_Update;
			size = sizeof(ClientPacket);
		}

		Vector3 GetPosition() {
			return position;
		}

		int GetPlayerID() {
			return playerID;
		}
	};

	struct ConfSpawnPacket : public GamePacket {
		int lastID;
		int playerID;
		Vector3	position;

		std::vector<int> existingPlayers;

		ConfSpawnPacket(int id) {
			playerID = id;
			type = Confirm_Spawn;
			size = sizeof(ConfSpawnPacket);
		}

		int GetID() {
			return playerID;
		}
	};

	struct ServerPacket : public GamePacket {
		int lastID;
		int playerID;
		Vector3 playerPos;

		ServerPacket() {
			type = Server_Update;
			size = sizeof(ServerPacket);
		}
	};

	class NetworkObject		{
	public:
		NetworkObject(GameObject& o, int id);
		virtual ~NetworkObject();

		//Called by clients
		virtual bool ReadPacket(GamePacket& p);
		//Called by servers
		virtual bool WritePacket(GamePacket** p, bool deltaFrame, int stateID);

		void UpdateStateHistory(int minID);

	protected:

		NetworkState& GetLatestNetworkState();

		bool GetNetworkState(int frameID, NetworkState& state);

		virtual bool ReadDeltaPacket(DeltaPacket &p);
		virtual bool ReadFullPacket(FullPacket &p);

		virtual bool WriteDeltaPacket(GamePacket**p, int stateID);
		virtual bool WriteFullPacket(GamePacket**p);

		GameObject& object;

		NetworkState lastFullState;

		std::vector<NetworkState> stateHistory;

		int deltaErrors;
		int fullErrors;

		int networkID;
	};
}