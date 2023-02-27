#pragma once

namespace NCL::CSC8508 {
	class GameManager {
	public:
		GameManager() = default;
		void LoadScreens();
		void LoadAssets(); //textures, meshes, shaders
		//load imgui context, setup renderer
		//this class will also be responsible for making push down automata
	protected:

	};
}