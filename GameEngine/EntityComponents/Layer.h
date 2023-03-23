#pragma once
#include <vector>
#include <map>

enum class Layer {
	Collectibles,
	Player,
	Enemy,
	StaticObjects,
	Paint,
	Bubbles
};


static std::map<Layer, int> COLLISION_LAYER_MASK = {
	//std::make_pair(Layer::Camera, (1 << (int)Layer::Spheres)),
	//std::make_pair(Layer::Spheres, (1 << (int)Layer::Cubes) | (1 << (int)Layer::OtherObjects)),
	//std::make_pair(Layer::Cubes, (1 << (int)Layer::Spheres) | (1 << (int)Layer::OtherObjects)),
	//std::make_pair(Layer::OtherObjects, 1 << (int)Layer::Spheres) // ~(1 << (int)Layer::Camera)),
	std::make_pair(Layer::Player, (1 << (int)Layer::StaticObjects) | (1 << (int)Layer::Collectibles) | (1 << (int)Layer::Enemy)),
	std::make_pair(Layer::Enemy, (1 << (int)Layer::StaticObjects) | (1 << (int)Layer::Player)),
	std::make_pair(Layer::StaticObjects, (1 << (int)Layer::Player) | (1 << (int)Layer::Enemy)),
	std::make_pair(Layer::Collectibles, (1 << (int)Layer::Player)),
};