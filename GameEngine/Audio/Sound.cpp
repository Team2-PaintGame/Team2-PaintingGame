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
	delete data;
	//alDeleteBuffers(1, &buffer);
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