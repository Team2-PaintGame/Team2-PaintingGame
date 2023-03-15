#pragma once
#include<string>
#include<iostream>
#include<fstream>
#include <map>
#include<AL/al.h>

using namespace std;

struct FMTCHUNK {
	short format;
	short channels;
	unsigned long srate;
	unsigned long bps;
	short balign;
	short samp;
};

class Sound {
	friend class SoundManager;
public:
	char* GetData() { return data; }
	int GetBitRate() { return bitRate; }
	float GetFrequency() { return freqRate; }
	int GetChannels() { return channels; }
	int	GetSize() { return size; }
	ALuint GetBuffer() { return buffer; }
	//bool			IsStreaming() { return streaming; }		//Part 2
	//virtual double	StreamData(ALuint	buffer, double timeLeft) { return 0.0f; }							//Part 2

	ALenum GetOALFormat();
	double GetLength();

protected:
	Sound();
	virtual ~Sound(void);

	void			LoadFromWAV(string filename);
	void			LoadWAVChunkInfo(ifstream& file, string& name, unsigned int& size);

	char* data;
	ALuint			buffer;

	//bool			streaming;			//Part 2

	float			freqRate;
	double			length;
	unsigned int	bitRate;
	unsigned int	size;
	unsigned int	channels;

	static map<string, Sound*> sounds;
};
