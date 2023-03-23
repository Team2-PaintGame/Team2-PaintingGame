/*
#pragma once
#include <vector>
#include <algorithm>

#include"Matrix4.h"
#include"GameObject.h"

#include "Sound.h"
#include "SoundEmitter.h"
#include <AL/al.h>
#include <AL/alc.h>

using std::vector;

class SoundEmitter;

using namespace NCL::Maths;
using namespace NCL::CSC8508;
struct OALSource {
	ALuint source;
	bool inUse;
	
	OALSource(ALuint src) {
		source = src;
		inUse = false;
		
	}

	
};

class SoundSystem {
public:
	static void Initialise(unsigned int channels = 32) {
		if (!instance) { 
			instance = new SoundSystem(channels); 
		}
		
	}
	
	static void Destroy() { delete instance; }
	
	inline static SoundSystem * GetSoundSystem() { return instance; }

	void SetListener(GameObject* listener) {
		this->listener = listener;
	}
	//Matrix4 GetListenerTransform() {
	//	return listenerTransform;
	//	
	//}

	void AddSoundEmitter(SoundEmitter * s) { emitters.push_back(s); }
	void RemoveSoundEmitter(SoundEmitter * s);
	
	void Update(float msec);
	
	void SetMasterVolume(float value);

protected:
	SoundSystem(unsigned int channels = 32);
	~SoundSystem(void);
	
	void UpdateListener();
	void UpdateTemporaryEmitters(float msec);
	
	void DetachSources(vector < SoundEmitter* >::iterator from, vector < SoundEmitter* >::iterator to);
	void AttachSources(vector < SoundEmitter* >::iterator from, vector < SoundEmitter* >::iterator to);
	
	void CullNodes();
	OALSource * GetSource();

	//Matrix4 listenerTransform;
	float masterVolume;
	ALCcontext * context;
	ALCdevice * device;
	GameObject* listener;
	
	vector < OALSource* > sources;
	vector < SoundEmitter* > emitters;
	vector < SoundEmitter* > frameEmitters;
	
	static SoundSystem * instance;
	
};
*/