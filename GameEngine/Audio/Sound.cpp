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



/*
#include "Sound.h"

map<string, Sound*> Sound::sounds;

Sound::Sound() {
	//streaming = false;		//Part 2
	bitRate = 0;
	freqRate = 0;
	length = 0;
	data = NULL;
	buffer = 0;
}

Sound::~Sound(void) {
	//alDeleteBuffers(1, &buffer);
	delete data;
	
}

ALenum Sound::GetOALFormat() {
	if (GetBitRate() == 16) {
		return GetChannels() == 2 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
	}
	else if (GetBitRate() == 8) {
		return GetChannels() == 2 ? AL_FORMAT_STEREO8 : AL_FORMAT_MONO8;
	}
	return AL_FORMAT_MONO8;
}

double Sound::GetLength() {
	return length;
}

void Sound::LoadFromWAV(string filename) {
	ifstream	file(filename.c_str(), ios::in | ios::binary);

	if (!file) {
		cout << "Failed to load WAV file '" << filename << "'!" << endl;
		return;
	}

	string		 chunkName;
	unsigned int chunkSize;

	while (!file.eof()) {
		LoadWAVChunkInfo(file, chunkName, chunkSize);

		if (chunkName == "RIFF") {
			file.seekg(4, ios_base::cur);
			//char waveString[4];
			//file.read((char*)&waveString,4);
		}
		else if (chunkName == "fmt ") {
			FMTCHUNK fmt;

			file.read((char*)&fmt, sizeof(FMTCHUNK));

			bitRate = fmt.samp;
			freqRate = (float)fmt.srate;
			channels = fmt.channels;
		}
		else if (chunkName == "data") {
			size = chunkSize;
			data = new char[size];
			file.read((char*)data, chunkSize);
			break;
			/*
			In release mode, ifstream and / or something else were combining
			to make this function see another 'data' chunk, filled with
			nonsense data, breaking WAV loading. Easiest way to get around it
			was to simply break after loading the data chunk. This *should*
			be fine for any WAV file you find / use. Not fun to debug.
	       */
/*
		}
		else {
			file.seekg(chunkSize, ios_base::cur);
		}
	}

	length = (float)size / (channels * freqRate * (bitRate / 8.0f)) * 1000.0f;

	file.close();
}

void Sound::LoadWAVChunkInfo(ifstream& file, string& name, unsigned int& size) {
	char chunk[4];
	file.read((char*)&chunk, 4);
	file.read((char*)&size, 4);

	name = string(chunk, 4);
}

void Sound::AddSound(string name) {
	if (!GetSound(name)) {
		Sound* s = new Sound();

		string extension = name.substr(name.length() - 3, 3);

		if (extension == "wav") {
			s->LoadFromWAV(name);

			alGenBuffers(1, &s->buffer);

			alBufferData(s->buffer, s->GetOALFormat(), s->GetData(), s->GetSize(), (ALsizei)s->GetFrequency());

		}
		else {
			cout << " Invalid extension ’" << extension << " ’!" << endl;

		}
		sounds.insert(make_pair(name, s));

	}

}

Sound* Sound::GetSound(string name) {
	map < string, Sound* >::iterator s = sounds.find(name);
	return (s != sounds.end() ? s->second : NULL);
}

void Sound::DeleteSounds() {
	for (map < string, Sound* >::iterator i = sounds.begin();
		i != sounds.end(); ++i) {
		alDeleteBuffers(1, &((*i).second->buffer));
		delete i->second;
	}
	sounds.clear();
}
*/