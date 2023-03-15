#include"SoundManager.h"

void SoundManager::AddSound(string name) {
	if (!GetSound(name)) {
		Sound * s = new Sound();
		
		string extension = name.substr(name.length() - 3, 3);
		
		if (extension == "wav ") {
			s -> LoadFromWAV(name);
		
			alGenBuffers(1, &s -> buffer);
			
			alBufferData(s -> buffer, s -> GetOALFormat(), s -> GetData(),
			s -> GetSize(), (ALsizei)s -> GetFrequency());
			
		}
		else {
			cout << " Invalid extension ’" << extension << " ’!" << endl;
			
		}
		sounds.insert(make_pair(name, s));
		
	}
	
}

Sound * SoundManager::GetSound(string name) {
	map < string, Sound* >::iterator s = sounds.find(name);
	return (s != sounds.end() ? s -> second : NULL);
}

void SoundManager::DeleteSounds() {
	for (map < string, Sound* >::iterator i = sounds.begin();
		i != sounds.end(); ++i) {
		delete i -> second;
	}
}

