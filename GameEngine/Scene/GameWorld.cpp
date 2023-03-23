#include "GameWorld.h"
#include "GameObject.h"
#include "Camera.h"
#include <chrono>
using namespace NCL;
using namespace NCL::CSC8508;

rp3d::decimal RaycastManager::notifyRaycastHit(const rp3d::RaycastInfo& raycastInfo) {
	if (ignoreBody == raycastInfo.body) { return -1; }

	rp3d::Vector3 n = raycastInfo.worldNormal;
	rp3d::Vector3 hitPos = raycastInfo.worldPoint;

	SceneContactPoint* collision = new SceneContactPoint();
	collision->isHit = true;
	collision->hitFraction = raycastInfo.hitFraction;
	collision->hitPos = hitPos;
	collision->normal = n;
	collision->body = raycastInfo.body;

	hitPoints.push_back(collision);
	
	//Debug::DrawLine(hitPos, hitPos + n, Vector4(1, 1, 0, 1), 4.0f);
	return raycastInfo.hitFraction;
}

GameWorld::GameWorld(reactphysics3d::PhysicsWorld* physicsWorld)	{
	this->physicsWorld = physicsWorld;
	raycastManager = new RaycastManager();
	shuffleConstraints	= false;
	shuffleObjects		= false;
	worldIDCounter		= 0;
	worldStateCounter	= 0;
	paintedPositions.reserve(1000);
}

GameWorld::~GameWorld()	{
	ClearAndErase();
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

void GameWorld::OperateOnPaintedPositions(Vector3and4Func f) {
	int index = 0;
	for (auto& splat : paintedPositions) {
		f(index++, splat.position, splat.colour);
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
		object->UpdateTransform();
	}
}

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

SceneContactPoint* GameWorld::Raycast(const reactphysics3d::Ray& r, GameObject* ignoreThis) const {
	raycastManager->clear();
	if (ignoreThis) { raycastManager->setIgnore(ignoreThis->GetRigidBody()); }

	physicsWorld->raycast(r, raycastManager);
	SceneContactPoint* dummy = new SceneContactPoint();
	dummy->isHit = false;
	if (!raycastManager->isHit()) { return dummy; }

	SceneContactPoint* closestHit = raycastManager->getHit();
	rp3d::Vector3 n = closestHit->normal;
	rp3d::Vector3 hitPos = closestHit->hitPos;
	//Debug::DrawLine(hitPos, hitPos + (2 * n), Vector4(1, 0, 0, 1), 4.0f);

	for (auto& i : gameObjects) {
		if (i->GetRigidBody() == closestHit->body) {
			closestHit->object = i;
		}
	}

	return closestHit;
}

double GetDistance(Vector3 vec1, Vector3 vec2)
{
	return std::sqrt(std::pow((vec1.x - vec2.x), 2) + std::pow((vec1.y - vec2.y), 2) + std::pow((vec1.z - vec2.z), 2));
}

void GameWorld::AddPaintedPosition(const Vector3& position, Vector4 team) {
	Vector4 colour;
	if (team == RedTeamColour) {
		colour = RedTeamColour;
	} 
	else if (team == BlueTeamColour) {
		colour = BlueTeamColour;
	}
	for (int i = 0; i < GetNumPaintedPositions(); i++)
	{
		PaintSplat element = paintedPositions[i];
		if ((element.colour == colour && element.position == position)) {
			return;
		}
		else if (element.colour == colour && GetDistance(element.position, position) < 2) {
			return;
		}
		else if (element.colour != colour && GetDistance(element.position, position) < 5) {
			if (colour == RedTeamColour) {
				element.colour = RedTeamColour;
			}
			else {
				element.colour = BlueTeamColour;
			}
			splatsToChangeColour.push_back({ i, element.colour });
			return;
		}
	}
	paintedPositions.push_back(PaintSplat(position, colour));
}

bool GameWorld::CleanNearbyPaint(Vector3 SecurityPos, float range)
{
	bool hasCleaned = false;
	for (auto paintPos = paintedPositions.begin(); paintPos != paintedPositions.end();)
	{
		float distance = (SecurityPos - paintPos->position).Length();
		if (distance < range)
		{
			paintPos = paintedPositions.erase(paintPos);
			hasCleaned = true;
		}
		else
		{
			paintPos++;
		}

	}
	return hasCleaned;
}

Vector3 GameWorld::FindClosestPaintSplat(Vector3 position)
{
	Vector3 paintPos;
	float min = FLT_MAX;
	for (auto i : paintedPositions)
	{
		float distance = (i.position - position).Length();
		if (distance < min && i.position.y < 10)
		{
			paintPos = i.position;
		}
	}
	return paintPos;
}

int GameWorld::GetSizePaintedPositions()
{
	return paintedPositions.size();
}