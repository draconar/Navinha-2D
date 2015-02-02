#include "AudioManager.h"

AudioManager AudioManager::audioManager;
AudioManager* AudioManager::getAudioManager ()
{
	return &audioManager;

}

void AudioResource::load()
{
	
/*	//initializes the position and velocity of the sound source
	for (int i = 0; i < 3; i++) {
		SourcePos [i] = 0.0;
		SourceVel[i] = 0.0;
	}

	ALenum format;
	ALsizei size;
	ALvoid* data;
	ALsizei freq;
	ALboolean loop;

	// Load wav data into a buffer.

	alGenBuffers(1, &Buffer);

	if(alGetError() != AL_NO_ERROR)
		//return AL_FALSE;
		printf ("ERROR: Generating Audio Buffers\n");

	alutLoadWAVFile("gfx/FancyPants.wav", &format, &data, &size, &freq, &loop);
	alBufferData(Buffer, format, data, size, freq);
	alutUnloadWAV(format, data, size, freq);

	// Bind the buffer with the source.

	alGenSources(1, &Source);

	if(alGetError() != AL_NO_ERROR)
		//return AL_FALSE;
		printf ("ERROR: Generating Audio Sources\n");

	alSourcei (Source, AL_BUFFER,   Buffer   );
	alSourcef (Source, AL_PITCH,    1.0      );
	alSourcef (Source, AL_GAIN,     1.0      );
	alSourcefv(Source, AL_POSITION, SourcePos);
	alSourcefv(Source, AL_VELOCITY, SourceVel);
	alSourcei (Source, AL_LOOPING,  loop     );

	// Do another error check and return.

	if(alGetError() == AL_NO_ERROR)
		//return AL_TRUE;
		printf ("ERROR: Settiong up audio source parameters\n");

	//return AL_FALSE;

	/*BASS if (audioType == AUDIO_TYPE_SAMPLE)
	{

		sampleData = BASS_SampleLoad (FALSE, fileName.c_str(), 0, 0, 1, 0);
		channelData = BASS_SampleGetChannel (sampleData, false);

	}

	else
	{

		streamData = BASS_StreamCreateFile (FALSE, fileName.c_str(), 0, 0, 0);
		channelData = streamData;

	}*/

}

void AudioResource::unload()
{
	
	/*alDeleteBuffers (1, &Buffer);
	alDeleteSources (1, &Source);
	alutExit ();*/


	/*if (audioType == AUDIO_TYPE_SAMPLE) BASS_SampleFree (sampleData);

	else BASS_StreamFree (streamData);

	sampleData = streamData = NULL;*/

}

void AudioManager::SetListenerValues()
{
    /*alListenerfv(AL_POSITION,    ListenerPos);
    alListenerfv(AL_VELOCITY,    ListenerVel);
    alListenerfv(AL_ORIENTATION, ListenerOri);*/
}

bool AudioManager::init (int device, DWORD SampleRate, DWORD flags, HWND win)
{
	/*
	// Initialize OpenAL and clear the error bit.
    alutInit(NULL,0);
    alGetError();

	//initializes both the position and velocity of the listener
	for (int i = 0; i < 3; i++) {
		ListenerPos [i] = 0.0;
		ListenerVel[i] = 0.0;
	}
	
	// Orientation of the Listener. (first 3 elements are "at", second 3 are "up")
	// Also note that these should be units of '1'.
	ListenerOri[0] = 0.0;
	ListenerOri[1] = 0.0;
	ListenerOri[2] = -1.0;
	ListenerOri[3] = 0.0;
	ListenerOri[4] = 1.0;
	ListenerOri[5] = 0.0;
	//ListenerOri[] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };

    SetListenerValues();
	
	//fazer verificação de erros

	return true;

	//BASS

	/*BOOL bassActive = BASS_Init (device, SampleRate, flags, win, NULL);

	if (bassActive)
	{

		//Get device info
		std::stringstream deviceStringStream;

		if (BASS_GetDeviceInfo (device, &deviceInfo))
		{

			deviceStringStream << "Audio Device Info. Name: " << deviceInfo.name << " Driver: " << deviceInfo.driver;
			deviceInfoString = deviceStringStream.str();

		}

	}

	return bassActive;*/

	return true;

}

void AudioManager::free ()
{

	//BASS_Free ();

}

void AudioManager::playFromResource (AudioResource* audioResource)
{
	
	/*if (!audioResource) return;

	alSourcePlay (audioResource->Source);
	/*if (!audioResource) return;
	
	BASS_ChannelPlay (audioResource->channelData, false);*/

}

void AudioManager::addAudioPlayer(AudioPlayer *player)
{
	/*
	players.push_back (player);
	BASS_ChannelPlay (player->audioResource->channelData, false);
	player->repeatCount = 0;
*/
}

void AudioManager::update()
{
	
	/*std::list<AudioPlayer*>::iterator list_itr;

	for (list_itr = players.begin(); list_itr != players.end(); list_itr++)
	{

		if (BASS_ChannelIsActive ((*list_itr)->audioResource->channelData) == BASS_ACTIVE_STOPPED)
		{

			if ((*list_itr)->repeatCount >= (*list_itr)->numRepeats)
			{

				(*list_itr)->onMediaComplete();
				players.erase(list_itr);
				return;
			
			}

			else
			{

				(*list_itr)->repeatCount++;
				BASS_ChannelPlay ((*list_itr)->audioResource->channelData, false);

			}

		}

	}*/

}

Resource* AudioManager::loadResourceFromXML (TiXmlElement* element)
{
/*	
	if (element)
	{
		
		AudioResource* resource = new AudioResource();

		for (TiXmlAttribute* elementAttrib = element->FirstAttribute(); elementAttrib; elementAttrib = elementAttrib->Next())
		{

			std::string attribName = elementAttrib->Name();
			std::string attribValue = elementAttrib->Value();

			if (attribName == "UID")
			{

				resource->resourceID = atoi (attribValue.c_str());

			}

			if (attribName == "filename")
			{

				resource->fileName = attribValue;

			}

			if (attribName == "scenescope")
			{

				resource->scope = atoi (attribValue.c_str());

			}

			if (attribName == "audio_type")
			{

				if (attribValue == "sample") resource->audioType = AUDIO_TYPE_SAMPLE;

				else resource->audioType = AUDIO_TYPE_STREAM;

			}

		}

		//probe if file exists then load resource
		if( !ResourceManager::probeIfFileExists(resource->fileName) )
			return NULL;
		else {
			resource->load();
			/*resource->streamData = BASS_StreamCreateFile (FALSE, resource->fileName.c_str(), 0, 0, 0);
			resource->channelData = resource->streamData;*/
	/*	}

		return resource;

	}

	return NULL;*/

	return NULL;

}