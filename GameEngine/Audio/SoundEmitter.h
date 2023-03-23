/*
#pragma once

#include "Sound.h"
#include "SoundSystem.h"
#include "GameObject.h"

using namespace NCL::CSC8508;
enum SoundPriority {
	SOUNDPRIORTY_LOW,
	SOUNDPRIORITY_MEDIUM,
	SOUNDPRIORITY_HIGH,
	SOUNDPRIORITY_ALWAYS
	
};

struct OALSource;

class SoundEmitter {
public:
	SoundEmitter(void);
	SoundEmitter(Sound * s);
	~SoundEmitter(void);
	
	void Reset();

	void SetSound(Sound* s);
	inline Sound * GetSound() { return sound; }
	
	inline void SetPriority(SoundPriority p) { priority = p; }
	inline SoundPriority GetPriority() { return priority; }
	
	inline void SetVolume(float volume) {
		volume = min(1.0f, max(0.0f, volume));
		
	}
	inline float GetVolume() { return volume; }
	
	inline void SetLooping(bool state) { isLooping = state; }
	inline bool GetLooping() { return isLooping; }
	
	inline void SetRadius(float value) {
		radius = max(0.0f, value);
		
	}
	inline float GetRadius() { return radius; }
	
	inline float GetTimeLeft() { return timeLeft; }
	
	inline OALSource * GetSource() { return currentSource; }
	
	void SetTarget(GameObject * s) { target = s; }
	GameObject* GetTarget() { return target; }

	void AttachSource(OALSource* s);
	void DetachSource();
	
	static bool CompareNodesByPriority(SoundEmitter * a, SoundEmitter * b);
	
	virtual void Update(float msec);
	void SetPosition(Vector3 pos) { position = pos; }
	Vector3 GetPosition() { return position; }
	
protected:
	Sound * sound;
	OALSource * currentSource;
	SoundPriority priority;
	Vector3 position;

	GameObject* target;

	float volume;
	float radius;
	bool isLooping;
	float timeLeft;
	
};

*/