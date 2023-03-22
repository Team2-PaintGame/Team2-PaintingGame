#include "SoundEmitter.h"

SoundEmitter::SoundEmitter(void) {
	Reset();
}
SoundEmitter::SoundEmitter(Sound* s) {
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
	pos = 0;
	global = false;
	for (unsigned int i = 0; i < NUM_BUFFERS; ++i) {
		buffers[i] = 0;
	}
}

SoundEmitter ::~SoundEmitter(void) {
	DetachSource();
}

bool SoundEmitter::CompareNodesByPriority(SoundEmitter* a, SoundEmitter* b) {
	return (a->priority > b->priority) ? true : false;
}

void SoundEmitter::SetSound(Sound* s) {
	sound = s;
	DetachSource();
	if (s) {
		timeLeft = s->GetLength();
		if (s->IsStreaming()) {
			alGenBuffers(NUM_BUFFERS, buffers);
		}
		else {
			alDeleteBuffers(NUM_BUFFERS, buffers);
		}
	}

}

void SoundEmitter::AttachSource(OALSource* s) {
	if (!s) return;
	currentSource = s;

	if (!currentSource) {
		return;

	}
	currentSource->inUse = true;

	alSourceStop(currentSource->source);
	alSourcef(currentSource->source, AL_MAX_DISTANCE, radius);
	alSourcef(currentSource->source, AL_REFERENCE_DISTANCE, radius * 0.2f);

	if (sound->IsStreaming()) {
		pos = timeLeft;
		int numBuffered = 0;
		while (numBuffered < NUM_BUFFERS) {
			double streamed = sound->StreamData(buffers[numBuffered], pos);
			if (streamed) {
				pos -= streamed;
				++numBuffered;

			}
			else {
				break;
			}
		}

		alSourceQueueBuffers(currentSource->source, numBuffered, &buffers[0]);
	}
	else {
		alSourcei(currentSource->source, AL_BUFFER, sound->GetBuffer());
		alSourcef(currentSource->source, AL_SEC_OFFSET, (sound->GetLength() / 1000.0) - (timeLeft / 1000.0));

	}

	alSourcePlay(currentSource->source);




	//alSourcei(currentSource -> source, AL_BUFFER, sound -> GetBuffer());
	//alSourcef(currentSource -> source, AL_SEC_OFFSET,(sound -> GetLength() / 1000.0) - (timeLeft / 1000.0));
	//alSourcePlay(currentSource -> source);
}

void SoundEmitter::DetachSource() {
	if (!currentSource) {
		return;
	}
	alSourcef(currentSource->source, AL_GAIN, 0.0f);
	alSourceStop(currentSource->source);
	if (sound && sound->IsStreaming()) {
		int numProcessed = 0;
		ALuint tempBuffer;
		alGetSourcei(currentSource->source, AL_BUFFERS_PROCESSED, &numProcessed);
		while (numProcessed--) {
			alSourceUnqueueBuffers(currentSource->source, 1, &tempBuffer);
		}
	}

	alSourcei(currentSource->source, AL_BUFFER, 0);
	//alSourcei(currentSource-> source, AL_BUFFER, 0);

	currentSource->inUse = false;
	currentSource = NULL;

}

void SoundEmitter::Update(float msec) {

	if (!currentSource) {
		return;
	}
	timeLeft -= msec;
	Vector3 soundPos;
	if (GetGlobal()) {
		soundPos = SoundSystem::GetSoundSystem()->GetListenerTransform().GetPositionVector();

	}
	else {
		if (target) {
			soundPos = target->GetTransform().GetPosition();
		}
		else {
			soundPos = this->position;
		}
	}

	alSourcefv(currentSource->source, AL_POSITION, (float*)&soundPos);

	if (sound->IsStreaming()) {
		int numProcessed;
		alGetSourcei(currentSource->source, AL_BUFFERS_PROCESSED, &numProcessed);
		alSourcei(currentSource->source, AL_LOOPING, 0);

		while (numProcessed--) {
			ALuint freeBuffer;
			alSourceUnqueueBuffers(currentSource->source, 1, &freeBuffer);

			pos -= sound->StreamData(freeBuffer, pos);
			alSourceQueueBuffers(currentSource->source, 1, &freeBuffer);

			if (pos < 0 && isLooping) {
				pos += sound->GetLength();
			}

		}
	}
	else {
		alSourcei(currentSource->source, AL_LOOPING, isLooping ? 1 : 0);
	}


	while (isLooping && timeLeft < 0.0f) {
		timeLeft += sound->GetLength();

	}

	if (currentSource) {
		Vector3 pos;

		if (target) {
			pos = target->GetTransform().GetPosition();

		}
		else {
			pos = this->position;

		}

		alSourcefv(currentSource->source, AL_POSITION, (float*)&pos);

		alSourcef(currentSource->source, AL_GAIN, volume);
		alSourcei(currentSource->source, AL_LOOPING, isLooping ? 1 : 0);
		alSourcef(currentSource->source, AL_MAX_DISTANCE, radius);
		alSourcef(currentSource->source, AL_REFERENCE_DISTANCE, radius * 0.2f);

	}

}