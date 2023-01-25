#include "StateGameObject.h"
#include "StateTransition.h"
#include "StateMachine.h"
#include "State.h"
#include "PhysicsObject.h"

using namespace NCL;
using namespace CSC8508;

StateGameObject::StateGameObject() {
	counter = 2.0f;
	stateMachine = new StateMachine();
}

StateGameObject::~StateGameObject() {
	delete stateMachine;
}

void StateGameObject::Update(float dt) {
	stateMachine->Update(dt);
}

void StateGameObject::MoveLeft(float dt) {
	GetPhysicsObject()->AddForce({ -100, 0, 0 });
	counter -= dt;
}

void StateGameObject::MoveRight(float dt) {
	GetPhysicsObject()->AddForce({ 100, 0, 0 });
	counter -= dt;
}

void StateGameObject::MoveUp(float dt) {
	GetPhysicsObject()->AddForce({ 0, 0, -100 });
	counter -= dt;
}

void StateGameObject::MoveDown(float dt) {
	GetPhysicsObject()->AddForce({ 0, 0, 100 });
	counter -= dt;
}