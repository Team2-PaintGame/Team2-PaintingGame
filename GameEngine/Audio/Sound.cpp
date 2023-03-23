#ifdef _WIN32

#include "Sound.h"
#include <stdexcept>

Sound::Sound(const std::string& fileName) {
	device = alcOpenDevice(NULL);
	context = alcCreateContext(device, NULL);
	alcMakeContextCurrent(context);

	initOpenAL();

	FILE* file = fopen(fileName.c_str(), "rb");
	if(!file) {
		throw std::runtime_error("Failed to open file: " + fileName);
	}
	fseek(file, 0, SEEK_END);
	long fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);
	char* buffer = new char[fileSize];
	fread(buffer, 1, fileSize, file);
	fclose(file);

	alGenBuffers(1, &bufferID);
	alBufferData(bufferID, AL_FORMAT_MONO16, buffer, fileSize, 44100);

	alGenSources(1, &sourceID);
	alSourcei(sourceID, AL_BUFFER, bufferID);

	delete[] buffer;

}

Sound::~Sound() {
	alDeleteSources(1, &sourceID);
	alDeleteBuffers(1, &bufferID);

	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
}

void Sound::Play() {
	alSourcePlay(sourceID);
}

void Sound::Stop() {
	alSourceStop(sourceID);
}

bool Sound::initOpenAL() {
	device = alcOpenDevice(NULL);
	if (!device) {
		return false;
	}

	context = alcCreateContext(device, NULL);
	if (!context) {
		return false;
	}

	alcMakeContextCurrent(context);

	return true;
}

#endif



