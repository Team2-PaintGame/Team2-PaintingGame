#include "GameWorld.h"
#include "GameObject.h"
#include "Constraint.h"
#include "CollisionDetection.h"
#include "Camera.h"
#include <chrono>

using namespace NCL;
using namespace NCL::CSC8508;

GameWorld::GameWorld()	{
	mainCamera = new Camera();

	shuffleConstraints	= false;
	shuffleObjects		= false;
	worldIDCounter		= 0;
	worldStateCounter	= 0;
}

GameWorld::~GameWorld()	{
}

void GameWorld::Clear() {
	gameObjects.clear();
	constraints.clear();
	worldIDCounter		= 0;
	worldStateCounter	= 0;
}

void GameWorld::ClearAndErase() {
	for (auto& i : gameObjects) {
		delete i;
	}
	for (auto& i : constraints) {
		delete i;
	}
	Clear();
}

void GameWorld::AddGameObject(GameObject* o) {
	gameObjects.emplace_back(o);
	o->SetWorldID(worldIDCounter++);
	worldStateCounter++;
}

void GameWorld::RemoveGameObject(GameObject* o, bool andDelete) {
	gameObjects.erase(std::remove(gameObjects.begin(), gameObjects.end(), o), gameObjects.end());
	if (andDelete) {
		delete o;
	}
	worldStateCounter++;
}

void GameWorld::GetObjectIterators(
	GameObjectIterator& first,
	GameObjectIterator& last) const {

	first	= gameObjects.begin();
	last	= gameObjects.end();
}

void GameWorld::OperateOnContents(GameObjectFunc f) {
	for (GameObject* g : gameObjects) {
		f(g);
	}
}

void GameWorld::UpdateWorld(float dt) {
	auto rng = std::default_random_engine{};

	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine e(seed);

	if (shuffleObjects) {
		std::shuffle(gameObjects.begin(), gameObjects.end(), e);
	}

	if (shuffleConstraints) {
		std::shuffle(constraints.begin(), constraints.end(), e);
	}
	for (const auto& object : gameObjects) {
		object->Update(dt);
	}
}

bool GameWorld::Raycast(Ray& r, RayCollision& closestCollision, bool closestObject, GameObject* ignoreThis, int layerMask) const {

	std::vector<int> activeLayers;
	int layer = 0; // start at bit index 0

	while (layerMask != 0) { // If the number is 0, no bits are set

		// check if the bit at the current index 0 is set
		if ((layerMask & 1) == 1) {
			activeLayers.push_back(layer);
		}

		// advance to the next bit position to check
		layerMask = layerMask >> 1; // shift all bits one position to the right
		layer = layer + 1;              // so we are now looking at the next index.
	}

	//The simplest raycast just goes through each object and sees if there's a collision
	RayCollision collision;

	for (auto& i : gameObjects) {

		//checking if the ray can interact with this game object
		if (!(std::count(activeLayers.begin(), activeLayers.end(), (int)i->GetLayer())) && layerMask) {
			continue;
		}
		if (!i->GetBoundingVolume()) { //objects might not be collideable etc...
			continue;
		}
		if (i == ignoreThis) {
			continue;
		}
		RayCollision thisCollision;
		if (CollisionDetection::RayIntersection(r, *i, thisCollision)) {
				
			if (!closestObject) {	
				closestCollision		= collision;
				closestCollision.node = i;
				return true;
			}
			else {
				if (thisCollision.rayDistance < collision.rayDistance) {
					thisCollision.node = i;
					collision = thisCollision;
				}
			}
		}
	}
	if (collision.node) {
		closestCollision		= collision;
		closestCollision.node	= collision.node;
		return true;
	}
	return false;
}


/*
Constraint Tutorial Stuff
*/

void GameWorld::AddConstraint(Constraint* c) {
	constraints.emplace_back(c);
}

void GameWorld::RemoveConstraint(Constraint* c, bool andDelete) {
	constraints.erase(std::remove(constraints.begin(), constraints.end(), c), constraints.end());
	if (andDelete) {
		delete c;
	}
}

void GameWorld::GetConstraintIterators(
	std::vector<Constraint*>::const_iterator& first,
	std::vector<Constraint*>::const_iterator& last) const {
	first	= constraints.begin();
	last	= constraints.end();
}