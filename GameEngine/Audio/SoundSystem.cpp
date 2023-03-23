/*
#include "SoundSystem.h"

SoundSystem * SoundSystem::instance = NULL;

SoundSystem::SoundSystem(unsigned int channels) {
	listener = NULL;
	masterVolume = 1.0f;
	
	cout << " Creating SoundSystem !" << endl;
	
	cout << " Found the following devices : " << alcGetString(NULL, ALC_DEVICE_SPECIFIER) << endl;
	
	device = alcOpenDevice(NULL); // Open the ’best ’ device
	
	if (!device) {
		cout << " SoundSystem creation failed ! No valid device !" << endl;
		return;
		
	}

	cout << " SoundSystem created with device : " << alcGetString(device, ALC_DEVICE_SPECIFIER) << endl;
	
	context = alcCreateContext(device, NULL);
	alcMakeContextCurrent(context);
	
	alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED);

	for (unsigned int i = 0; i < channels; ++i) {
		ALuint source;
		
		alGenSources(1, &source);
		ALenum error = alGetError();
		
		if (error == AL_NO_ERROR) {
			sources.push_back(new OALSource(source));
			
		}
		else {
			break;
			
		}
		
	}
	cout << " SoundSystem has " << sources.size() << " channels available !" << endl;
	
}

SoundSystem ::~SoundSystem(void) {
	
	for (vector < SoundEmitter* >::iterator i = emitters.begin();
		i != emitters.end(); ++i) {
		delete (*i);
	}

	for (vector < OALSource* >::iterator i = sources.begin();
		i != sources.end(); ++i) {
		alDeleteSources(1, &(*i) -> source);
		delete (*i);
		
	}
	emitters.clear();
	sources.clear();
	Sound::DeleteSounds();
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
	
}

void SoundSystem::SetMasterVolume(float value) {
	value = max(0.0f, value);
	value = min(1.0f, value);
	masterVolume = value;
	alListenerf(AL_GAIN, masterVolume);
	
}

void SoundSystem::UpdateListener() {
	if (listener) {
		Vector3 worldPos = listener->GetTransform().GetPosition();
		
		Vector3 dirup[2];
		// forward
		dirup[0].x = -listener->GetTransform().GetMatrix().array[0][2];
		dirup[0].y = -listener->GetTransform().GetMatrix().array[1][2];
		dirup[0].z = -listener->GetTransform().GetMatrix().array[2][2];
		// Up
		dirup[1].x = listener->GetTransform().GetMatrix().array[0][1];
		dirup[1].y = listener->GetTransform().GetMatrix().array[1][1];
		dirup[1].z = listener->GetTransform().GetMatrix().array[2][1];
		
		alListenerfv(AL_POSITION, (float*)&worldPos);
		alListenerfv(AL_ORIENTATION, (float*)&dirup);
		
	}
	else {
		Vector3 worldPos = Vector3(0, 0, 0);
		alListenerfv(AL_POSITION, (float*)&worldPos);
		
	}
	
}

void SoundSystem::Update(float msec) {
	UpdateListener();
	
	for (vector < SoundEmitter* >::iterator i = emitters.begin(); i != emitters.end(); ++i) {
		frameEmitters.push_back((*i));
		(*i) -> Update(msec);
		
	}
	
	CullNodes();

	if (frameEmitters.size() > sources.size()) {
		std::sort(frameEmitters.begin(), frameEmitters.end(), SoundEmitter::CompareNodesByPriority);

		DetachSources(frameEmitters.begin() + (sources.size() + 1), frameEmitters.end());

		AttachSources(frameEmitters.begin(), frameEmitters.begin() + sources.size());
		
	}
	else{
		AttachSources(frameEmitters.begin(), frameEmitters.end());
		
	}
	
	frameEmitters.clear();
	
}

void SoundSystem::CullNodes() {
	for (vector < SoundEmitter* >::iterator i = frameEmitters.begin(); i != frameEmitters.end(); ) {
		SoundEmitter * e = (*i);
		
		float length;
		
		if (e -> GetTarget()) {
			if (listener)
				length = (listener->GetTransform().GetMatrix().GetPositionVector() - e->GetTarget()->GetTransform().GetPosition()).Length();

			else
				length = (Vector3(0, 0, 0) - e->GetPosition()).Length();
			
		}
		else {
			if (listener)
				length = (listener->GetTransform().GetMatrix().GetPositionVector() - e -> GetPosition()).Length();
			else
				length = (Vector3(0, 0, 0) - e->GetPosition()).Length();
			
		}
		
		if (length > e -> GetRadius() || !e -> GetSound() || e -> GetTimeLeft() < 0) {
			e -> DetachSource();
			i = frameEmitters.erase(i);
			
		}
		else {
			++i;
			
		}
		
	}
	
}

void SoundSystem::DetachSources(vector < SoundEmitter* >::iterator from, vector < SoundEmitter* >::iterator to) {
	for (vector < SoundEmitter* >::iterator i = from; i != to; ++i) {
		(*i) -> DetachSource();
	
	}
	
}

void SoundSystem::AttachSources(vector < SoundEmitter* >::iterator from, vector < SoundEmitter* >::iterator to) {
	for (vector < SoundEmitter* >::iterator i = from; i != to; ++i) {
		if (!(*i) -> GetSource()) {
			(*i) -> AttachSource(GetSource());
			
		}
		
	}
	
}

OALSource* SoundSystem::GetSource() {
	for (vector < OALSource* >::iterator i = sources.begin(); i != sources.end(); ++i) {
		OALSource * s = *i;
		if (!s -> inUse) {
			return s;
			
		}
		
	}
	return NULL;
	
}

*/



