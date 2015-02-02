#ifndef AUDIOMANAGER_H_INCLUDED
#define AUDIOMANAGER_H_INCLUDED

#include "ResourceManager.h"
#include "Bass.h"
#include <string>
#include <sstream>
#include <list>
#include <conio.h>
#include <stdlib.h>
#include <al.h>
#include <alc.h>
#include <alut.h>

typedef enum {AUDIO_TYPE_SAMPLE = 0, AUDIO_TYPE_STREAM = 1} AUDIO_TYPE;

//class AudioResource encapsulates an audio resource as loaded by the BASS
class AudioResource : public Resource
{
private:
	
protected:

public:
	
	// Buffers to hold sound data.
	ALuint Buffer;
	// Sources are points of emitting sound.
	ALuint Source;
	// Position of the source sound.
	ALfloat SourcePos[3];
	// Velocity of the source sound.
	ALfloat SourceVel[3];

	HSAMPLE sampleData;
	HSTREAM streamData;
	HCHANNEL channelData;

	AUDIO_TYPE audioType;

	void load();
	void unload();

};

/*class AudioPlayer corresponds to an audio object in a scene.

 Abstract base class intend to be overridden with a handled

 onMediaCompllete method. This class is passed to the audio manager
 and specifies how a file is to be played (Num of repeats, etc).

 onMediaComplete is called by the audio manager to notify the class
 when media playback has completed.									*/

class AudioPlayer
{
private:

protected:

public:
	AudioResource* audioResource;
	int numRepeats;
	int repeatCount;

	AudioPlayer ()
	{
		audioResource = NULL;
		numRepeats = repeatCount = 0;
	}

	virtual void onMediaComplete () = 0;

};

class AudioManager
{
private:
	void SetListenerValues ();

protected:
	AudioManager() {};
	
	static AudioManager audioManager;
	std::list<AudioPlayer*> players;

public:
	// Position of the Listener.
	ALfloat ListenerPos[3];
	// Velocity of the Listener.
	ALfloat ListenerVel[3];
	// Orientation of the Listener. (first 3 elements are "at", second 3 are "up")
	// Also note that these should be units of '1'.
	ALfloat ListenerOri[6];

	BASS_DEVICEINFO deviceInfo;
	std::string deviceInfoString;
	static AudioManager* getAudioManager();
	bool init (int device = -1, DWORD sampleRate = 44100, DWORD flags = 0, HWND win = 0);
	void free();
	void setVolume (float volume) {BASS_SetVolume (volume);}
	void pause () {BASS_Pause();}
	void start () {BASS_Start();}
	Resource* loadResourceFromXML (TiXmlElement* element);

	//Plays audio resource once
	void playFromResource (AudioResource* audioResource);
	void addAudioPlayer (AudioPlayer* player);

	void update();

};

#endif //AUDIOMANAGER_H_INCLUDED