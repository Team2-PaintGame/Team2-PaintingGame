#include <Window.h>
#include "Debug.h"

#include "PaintingGame.h"
#include "NetworkedGame.h"

using namespace NCL;
using namespace CSC8508;

//class TestPacketReceiver : public PacketReceiver {
//public:
//	TestPacketReceiver(string name) {
//		this -> name = name;
//	}
//
//	void ReceivePacket(int type, GamePacket* payload, int source) {
//		if (type == String_Message) {
//			StringPacket* realPacket = (StringPacket*)payload;
//
//			string msg = realPacket -> GetStringFromData();
//
//			std::cout << name << " received message : " << msg << std::endl;
//		}
//	}
//protected:
//	string name;
//};
//
//
//void TestNetworking() {
//	NetworkBase::Initialise();
//
//	TestPacketReceiver serverReceiver(" Server ");
//	TestPacketReceiver clientReceiver(" Client ");
//
//	int port = NetworkBase::GetDefaultPort();
//
//	GameServer* server = new GameServer(port, 1);
//	GameClient* client = new GameClient();
//
//	server -> RegisterPacketHandler(String_Message, &serverReceiver);
//	client -> RegisterPacketHandler(String_Message, &clientReceiver);
//
//	bool canConnect = client -> Connect(127, 0, 0, 1, port);
//	
//	for (int i = 0; i < 100; ++i) {
//		StringPacket serverPacket = "Server says hello!" + std::to_string(i);
//		StringPacket clientPacket = "Client says hello!" + std::to_string(i);
//
//		server->SendGlobalPacket(serverPacket);
//
//		client -> SendPacket(clientPacket);
//
//		server -> UpdateServer();
//		client -> UpdateClient();
//
//		std::this_thread::sleep_for(std::chrono::milliseconds(10));
//	}
//
//	NetworkBase::Destroy();
//}



int main() {
	Window* w = Window::CreateGameWindow("CSC8508 Game technology!", 1280, 720);


	if (!w->HasInitialised()) {
		return -1;
	}

	w->ShowOSPointer(true);
	w->LockMouseToWindow(true);
	w->GetTimer()->GetTimeDeltaSeconds(); //Clear the timer so we don't get a larget first dt!

	bool started = false;
	NetworkedGame g;
	while (w->UpdateWindow() && !started) {
		if (w->GetKeyboard()->KeyPressed(KeyboardKeys::NUM1)) {
			g.StartAsClient(127, 0, 0, 1);
			started = true;
		}
		if (w->GetKeyboard()->KeyPressed(KeyboardKeys::NUM3)) {
			g.StartAsServer();
			started = true;
		}
	}

	while (w->UpdateWindow() && !Window::GetKeyboard()->KeyDown(KeyboardKeys::ESCAPE)) {
		float time = w->GetTimer()->GetTotalTimeSeconds();
		float dt = w->GetTimer()->GetTimeDeltaSeconds();
		if (dt > 0.1f) {
			std::cout << "Skipping large time delta" << std::endl;
			continue; //must have hit a breakpoint or something to have a 1 second frame time!
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::PRIOR)) {
			w->ShowConsole(true);
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::NEXT)) {
			w->ShowConsole(false);
		}

		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::T)) {
			w->SetWindowPosition(0, 0);
		}

		w->SetTitle("Gametech frame time:" + std::to_string(1000.0f * dt));

		g.UpdateGame(dt);
		//g.menuSystem.Update(dt);
	}
	Window::DestroyGameWindow();
}