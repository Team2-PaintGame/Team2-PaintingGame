/*
#include "SoundEmitter.h"

SoundEmitter::SoundEmitter(void) {
	Reset();
}
SoundEmitter::SoundEmitter(Sound * s) {
	Reset();
	SetSound(s);
}

void SoundEmitter::Reset() {
	priority = SOUNDPRIORTY_LOW;
	volume = 1.0f;
	radius = 500.0f;
	timeLeft = 0.0f;
	isLooping = true;
	currentSource = NULL;
	sound = NULL;
}

SoundEmitter ::~SoundEmitter(void) {
	DetachSource();
}

bool SoundEmitter::CompareNodesByPriority(SoundEmitter* a, SoundEmitter * b) {
	return (a -> priority > b -> priority) ? true : false;
}

void SoundEmitter::SetSound(Sound* s) {
	sound = s;
	DetachSource();
	if (s) {
		timeLeft = s -> GetLength();
	}

}

void SoundEmitter::AttachSource(OALSource* s) {
	currentSource = s;
	
	if (!currentSource) {
		return;
		
	}
	currentSource -> inUse = true;
	
	alSourcef(currentSource -> source, AL_MAX_DISTANCE, radius);
	alSourcef(currentSource -> source, AL_REFERENCE_DISTANCE, radius * 0.2f);
	alSourcei(currentSource -> source, AL_BUFFER, sound -> GetBuffer());
	alSourcef(currentSource -> source, AL_SEC_OFFSET,(sound -> GetLength() / 1000.0) - (timeLeft / 1000.0));
	alSourcePlay(currentSource -> source);
}

void SoundEmitter::DetachSource() {
	if (!currentSource) {
		return;
	}
	alSourcef(currentSource-> source, AL_GAIN, 0.0f);
	alSourceStop(currentSource-> source);
	alSourcei(currentSource-> source, AL_BUFFER, 0);
	
	currentSource-> inUse = false;
	currentSource = NULL;
	
}

void SoundEmitter::Update(float msec) {
	timeLeft -= msec;
	
	while (isLooping && timeLeft < 0.0f) {
		timeLeft += sound -> GetLength();
		
	}

	if (currentSource) {
		Vector3 pos;
		
		if (target) {
			pos = target->GetTransform().GetPosition();
			
		}
		else {
			pos = this -> position;
			
		}
		
		alSourcefv(currentSource-> source, AL_POSITION, (float*)&pos);
		
		alSourcef(currentSource-> source, AL_GAIN, volume);
		alSourcei(currentSource-> source, AL_LOOPING, isLooping ? 1 : 0);
		alSourcef(currentSource-> source, AL_MAX_DISTANCE, radius);
		alSourcef(currentSource-> source, AL_REFERENCE_DISTANCE, radius * 0.2f);
		
	}

}

*/