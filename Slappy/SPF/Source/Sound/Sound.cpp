/*////////////////////////////////////////////////////////////////////////
//File:	CSound.cpp
//
//Author:	Dan Manning
//
//Creation Date:	july 30, 2004
//
//Purpose:	Function definitions for CSound class
//NOTE:	This module needs to have alut.lib and openal32.lib added
//to the project 
//
/////////////////////////////////////////////////////////////////////////*/

#include "sound.h"
#include "..\memmanager\memmanager.h"
#include "..\LevelManager\LevelManager.h"

#include <memory.h>
#include <cstring>
#include <al\al.h>
#include <al\alc.h>
#include <al\alut.h>

CSound *CSound::m_pInstance = 0;

CSound::CSound(void) {}
CSound::CSound(const CSound &CSoundRef) {}
CSound &CSound::operator=(const CSound &CSoundRef) { return *this; }
CSound::~CSound(void) {}

//cheat, make the sound volume global so it doesn't change when the sound module is shut down
static float m_fVolume = 1.0f;
static float m_fMusicVolume = 0.7f;

/*////////////////////////////////////////////////////////////////////////
//Function:	GetInstance
//
//Return:	
CSound*		pointer to the only instance of CSound
//	
//Purpose:
Provide a way to access the singleton functionality of the CSound class
//
/////////////////////////////////////////////////////////////////////////*/
CSound* CSound::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = (CSound*)MemMalloc(sizeof(CSound));
		memset(m_pInstance, 0, sizeof(CSound));
		m_pInstance->CSound::CSound();
	}

	return m_pInstance;
}

/*////////////////////////////////////////////////////////////////////////
//Function:	DeleteInstance
//
//Purpose:	Clean up after the CSound singleton
//
/////////////////////////////////////////////////////////////////////////*/
void CSound::DeleteInstance()
{
	if (m_pInstance)
	{
		m_pInstance->ShutdownSound();
		m_pInstance->CSound::~CSound();
		MemFree(m_pInstance);
		m_pInstance = 0;
	}
}

/*////////////////////////////////////////////////////////////////////////
//Function:	InitSound
//
//Purpose:	This function sets up openal for use
//
/////////////////////////////////////////////////////////////////////////*/
bool CSound::InitSound()
{
	m_SoundLogger.Init("SoundLogs.txt");

	alutInit(NULL, 0);
	alGetError();

	//First, initialize the music and let it have whatever resources it needs
    alGenBuffers(2, m_OggBuffers);
    ErrorCheck();
    alGenSources(1, &m_OggSource);
    ErrorCheck();

	//Set buffers and sources to 0
	m_Buffers = (OALBufferT*)MemMalloc(sizeof(OALBufferT) * MAX_BUFFERS);
	m_Sources = (OALSourceT*)MemMalloc(sizeof(OALSourceT) * MAX_SOURCES);
	memset(m_Buffers, 0, (sizeof(OALBufferT) * MAX_BUFFERS));
	memset(m_Sources, 0, (sizeof(OALSourceT) * MAX_SOURCES));

	//generate openal buffers and sources
	for (int i = 0; i < MAX_BUFFERS; i++)
	{
		alGenBuffers(1, &m_Buffers[i].uBuffer);
		ErrorCheck();
		m_Buffers[i].bFree = true;
	}

	//start generating sources until openal throws an error
	for (int i = 0; i < MAX_SOURCES; i++)
	{
		alGenSources(1, &m_Sources[i].uSource);

		ALenum SourceTest = alGetError();
		if (SourceTest == AL_NO_ERROR)
		{
			this->m_iNumSources++;
			m_Sources[i].bFree = true;
		}
		else
		{
			break;
		}
	}

	//log the number of sources created
	m_SoundLogger << "Number of sources created: " << m_iNumSources << '\n';

	//Set listener to 0s
	memset(&m_Listener, 0, sizeof(SoundListenerT));
	m_Listener.ListenerOrientation[2] = -1.0f;
	m_Listener.ListenerOrientation[4] = 1.0f;

	//pass the listener to openal
	//SetListenerParams(CDisplayDonkey::GetInstance()->getCameraPos());
	alListenerfv(AL_POSITION, m_Listener.ListenerPos);
	ErrorCheck();
	alListenerfv(AL_VELOCITY, m_Listener.ListenerVelocity);
	ErrorCheck();
	alListenerfv(AL_ORIENTATION, m_Listener.ListenerOrientation);
	ErrorCheck();

	//Set initial volume to 1.0f (max volume)
	//m_fVolume = 1.0f;

	//set the music flag to false
	m_bMusicLoaded = false;

	m_SoundLogger << "CSound initialized successfully\n";

	return true;
}

/*////////////////////////////////////////////////////////////////////////
//Function:	ShutdownSound
//
//Purpose:
//This function shuts down and cleans up the sound module					
//
/////////////////////////////////////////////////////////////////////////*/
void CSound::ShutdownSound()
{
	//delete sources
	for (int i = 0; i < m_iNumSources; i++)
	{
		if (!m_Sources[i].bFree)
		{
			alSourceStop(m_Sources[i].uSource);
			ErrorCheck();
		}
		alDeleteSources(1, &m_Sources[i].uSource);
		ErrorCheck();
	}

	//delete buffers
	for (int i = 0; i < MAX_BUFFERS; i++)
	{
		alDeleteBuffers(1, &m_Buffers[i].uBuffer);
		ErrorCheck();
	}

	//free buffers and sources
	MemFree(m_Buffers);
	MemFree(m_Sources);

	ReleaseMusic();

	alutExit();

	m_SoundLogger << "CSound shutdown successfully\n";
	m_SoundLogger.Shutdown();
}

/*////////////////////////////////////////////////////////////////////////
//Function:	LoadSound
//
//Input:
//strFileName:	sound file to load, must be a .wav file
//
//Return:
//int:			id of the loaded sound
//
//Purpose:		This function loads a sound file into a buffer for use
//				by openal and gives the user an id to that sound buffer
//				This function returns -1 if errors occur
//
/////////////////////////////////////////////////////////////////////////*/
int CSound::LoadSound(char *strFileName)
{
	//check if this sound has been loaded yet
	for (int i = 0; i < MAX_BUFFERS; i++)
	{
		if (!m_Buffers[i].bFree)
		{
			//do a string compare on filename
			if (strcmp(m_Buffers[i].strFileName, strFileName) == 0)
			{
				//Aha! this file has already been loaded
				return i;
			}
		}
	}

	//if it gets here, this file has NOT been loaded
	//get the next available buffer
	int iIndex = NextAvailableBuffer();

	//variables to load data into
	ALenum format;
	ALsizei size;
	ALvoid *data;
	ALsizei freq;
	ALboolean loop;

	//load file
	alutLoadWAVFile(strFileName, &format, &data, &size, &freq, &loop);
	ErrorCheck();
	alBufferData(m_Buffers[iIndex].uBuffer, format, data, size, freq);
	ErrorCheck();
	alutUnloadWAV(format, data, size, freq);
	ErrorCheck();

	// Do another error check and return.
	if(alGetError() == AL_NO_ERROR)
	{
		m_Buffers[iIndex].bFree = false;
		m_SoundLogger << "Load Successful: " << strFileName << " was loaded successfully\n";
		//set filename
		memset(m_Buffers[iIndex].strFileName, 0, (sizeof(char) * MAX_SOUND_FILENAME));
		strcpy(m_Buffers[iIndex].strFileName, strFileName);
		return iIndex;
	}

	m_SoundLogger << "LOAD ERROR: " << strFileName << " load was unsuccessful\n";

	return -1;
}

/*////////////////////////////////////////////////////////////////////////
//Function:	SetListenerParams
//
//Input:
//tPosition:	the new position of the listener
//
//
//Purpose:		This function is used to set the params for the listener.
//				Use the default values to use a static listener at the origin,
//				looking down the -z and pointing up
//
/////////////////////////////////////////////////////////////////////////*/
void CSound::SetListenerParams(Vec4DT tPosition)
{
	//move the listener to the new position
	m_Listener.ListenerPos[0] = tPosition.tfX;
	m_Listener.ListenerPos[1] = tPosition.tfY;
	//m_Listener.ListenerPos[2] = tPosition.tfZ;
	m_Listener.ListenerPos[2] = 0.0f;

	alListenerfv(AL_POSITION, m_Listener.ListenerPos);
	ErrorCheck();
}

/*////////////////////////////////////////////////////////////////////////
//Function:	PlaySound
//
//Input:
//iBuffer:		the id of the buffer to play
//Position:		the location to play the sound at
//				NOTE**NOTE**NOTE**NOTE
//				pass in a zero vector for this
//bLoop:		whether or not this sound effect should be looped
//
//Return Type:
//int:			a unique id to the sound source created.
//				This can be used to stop, pause, or change volume 
//				of a sound.
//				-1 if an error occurred
//
//Purpose:		This function is used to play a sound. The sound will be
//				attached to the specified source.
//				
/////////////////////////////////////////////////////////////////////////*/
int CSound::PlaySoundEffect(int iBufferID, Vec4DT Position, bool bLoop)
{
	//find the next unused sound source
	int iSourceIndex = NextAvailableSource();

	if (iSourceIndex < 0)
	{
		return 0;
	}

	float fSoundVolume = m_fVolume;

	//play the source in openal
	alSourcei(m_Sources[iSourceIndex].uSource, AL_BUFFER, m_Buffers[iBufferID].uBuffer);
	ErrorCheck();
	alSourcef(m_Sources[iSourceIndex].uSource, AL_PITCH, 1.0f);
	ErrorCheck();
	////alSourcefv(m_Sources[iSourceIndex].uSource, AL_POSITION, fPosition);
	//ErrorCheck();
	////alSourcefv(m_Sources[iSourceIndex].uSource, AL_VELOCITY, velocity);
	//ErrorCheck();
	alSourcef(m_Sources[iSourceIndex].uSource, AL_GAIN, fSoundVolume);
	ErrorCheck();
	alSourcei(m_Sources[iSourceIndex].uSource, AL_LOOPING, bLoop);
	ErrorCheck();
	alSourcePlay(m_Sources[iSourceIndex].uSource);
	ErrorCheck();
	
	ErrorCheck();

	//set source to unavailable
	m_Sources[iSourceIndex].bFree = false;

	return iSourceIndex;
}

/*////////////////////////////////////////////////////////////////////////
//Function:	UpdateSound
//
//Purpose:
//This function updates the sound module.  It goes through the list of sources
//and checks if the sound is done playing.  If it is, the source is deleted.  
//When a source is deleted, the numReferences of its emitter is decremented. 
//If the DeleteMe flag is set AND the numReferences hits 0, the emitter is
//deleted.
//
/////////////////////////////////////////////////////////////////////////*/
void CSound::UpdateSound()
{
	//Iterate through list of sources
	for (int i = 0; i < MAX_SOURCES; i++)
	{
		//check if this is an active source
		if (m_Sources[i].bFree)
		{
			continue;
		}

		//check if source is still playing
		//if no, set to inactive
		if (!IsPlaying(i) && !IsPaused(i))
		{
			m_Sources[i].bFree = true;
		}
	}

	//update the music
	UpdateMusic();
}

/*////////////////////////////////////////////////////////////////////////
//Function:	IsPlaying
//
//Input:
//SourceID:	the id of the source to check
//
//Return:
//	true:	the source is playing
//	false:	the source is not playing
//
//Purpose:	Function to check if an openal source is playing or not
//
/////////////////////////////////////////////////////////////////////////*/
bool CSound::IsPlaying(int iSourceID)
{
	ALenum state;
	alGetSourcei(m_Sources[iSourceID].uSource, AL_SOURCE_STATE, &state);
	ErrorCheck();
	return (state == AL_PLAYING);
}

/*////////////////////////////////////////////////////////////////////////
//Function:	IsPaused
//
//Input:
//	SourceID:	the is of the source to check
//
//Return:
//	true:	the source is paused
//	false:	the source is not paused
//
//Purpose:	Function to check if an openal source is paused or not
//			Used by the togglepause function
//
/////////////////////////////////////////////////////////////////////////*/
bool CSound::IsPaused(int iSourceID)
{
	ALenum state;
	alGetSourcei(m_Sources[iSourceID].uSource, AL_SOURCE_STATE, &state);
	ErrorCheck();
	return (state == AL_PAUSED);
}

/*////////////////////////////////////////////////////////////////////////
//Function:	ChangeSFXVolume & ChangeMusicVolume
//
//Input:	
//	fVolume:	the volume to change to
//
//Purpose:	Provide functionality for the menu manager to change the volume
//			of the sound effects and music in the game
//
/////////////////////////////////////////////////////////////////////////*/
void CSound::ChangeSFXVolume(float fVolume)
{
	//set new volume
	m_fVolume = fVolume;

	float fSoundVolume = 0.0f;
	fSoundVolume = m_fVolume;

	//iterate sound effects and change volume of currently playing sounds
	for (int i = 0; i < MAX_SOURCES; i++)
	{
		if (!m_Sources[i].bFree)
		{
			alSourcef(m_Sources[i].uSource, AL_GAIN, fSoundVolume);
			ErrorCheck();
		}
	}
}

void CSound::ChangeMusicVolume(float fMusicVolume)
{
	//change music volume
	m_fMusicVolume = fMusicVolume;
	if (m_fMusicVolume < 0.0f)
	{
		m_fMusicVolume = 0.0f;
	}
	alSourcef(m_OggSource, AL_GAIN, m_fMusicVolume);
}

/*////////////////////////////////////////////////////////////////////////
//Function:	ClearBuffers
//
//Purpose:	This function will clear all the currently loaded sounds
//
/////////////////////////////////////////////////////////////////////////*/
void CSound::ClearBuffers()
{
	//make sure all the sounds are stopped
	StopAllSounds();

	//set all buffers to free
	for (int i = 0; i < MAX_BUFFERS; i++)
	{
		if (!m_Buffers[i].bFree)
		{
			m_SoundLogger << "Unload successful: " << m_Buffers[i].strFileName << '\n';
		}
		m_Buffers[i].bFree = true;
	}
}

/*////////////////////////////////////////////////////////////////////////
//Function:	StopAllSounds
//
//Purpose:	This function will stop all sounds from playing
//
/////////////////////////////////////////////////////////////////////////*/
void CSound::StopAllSounds()
{
	//loop through currently playing sources and stop them
	for (int i = 0; i < MAX_SOURCES; i++)
	{
		alSourceStop(m_Sources[i].uSource);
		ErrorCheck();
		m_Sources[i].bFree = true;
	}

	//Iterate through list of sources
	for (int i = 0; i < MAX_SOURCES; i++)
	{
		//check if this is an active source
		if (m_Sources[i].bFree)
		{
			continue;
		}

		//check if source is still playing
		//if no, set to inactive
		if (!IsPlaying(i) && !IsPaused(i))
		{
			m_Sources[i].bFree = true;
		}
	}
}

/*////////////////////////////////////////////////////////////////////////
//Function:	TogglePause
//
//Input:	
//bPaused:	true to pause all playing sounds, false to unpause 
//			all playing sounds
//
//Purpose:	Provide functionality to pause and unpause the sound module
//
/////////////////////////////////////////////////////////////////////////*/
void CSound::TogglePause(bool bPaused)
{
	//loop through currently playing sources and pause them
	//or unpause them
	if (bPaused)
	{
		for (int i = 0; i < MAX_SOURCES; i++)
		{
			if (!m_Sources[i].bFree)
			{
				alSourcePause(m_Sources[i].uSource);
				ErrorCheck();
			}
		}
	}
	else
	{
		for (int i = 0; i < MAX_SOURCES; i++)
		{
			if (IsPaused(i))
			{
				if (!m_Sources[i].bFree)
				{
					alSourcePlay(m_Sources[i].uSource);
					ErrorCheck();
				}
			}
		}
	}
}

/*////////////////////////////////////////////////////////////////////////
//Function:	ErrorLog
//
//Purpose:	Check if OpenAL returned any errors
//
/////////////////////////////////////////////////////////////////////////*/
void CSound::Error_Check(int linenum)
{
	ALenum DebugTest = alGetError();
	if (DebugTest != AL_NO_ERROR)
	{
		m_SoundLogger << "ERROR: OpenAL returned an error at line " << linenum << '\n' << '\t';
		switch (DebugTest)
		{
			case AL_NO_ERROR:
			{
				m_SoundLogger << "error was : AL_NO_ERROR\n";
				break;
			}
			case AL_INVALID_ENUM:
			{
				m_SoundLogger << "error was : AL_INVALID_ENUM\n";
				break;
			}
			case AL_INVALID_VALUE:
			{
				m_SoundLogger << "error was : AL_INVALID_VALUE\n";
				break;
			}
			case AL_INVALID_OPERATION:
			{
				m_SoundLogger << "error was : AL_INVALID_OPERATION\n";
				break;
			}
			case AL_OUT_OF_MEMORY:
			{
				m_SoundLogger << "error was : AL_OUT_OF_MEMORY\n";
				break;
			}
			default:
			{
				m_SoundLogger << "Holy shit, OpenAL passed a weird error number: " << DebugTest << '\n';
				break;
			}
		}
	}
}

/*////////////////////////////////////////////////////////////////////////
//Function:	LoadMusic
//
//Input:	
//strFileName:	file name of ogg file to load
//
//Purpose:	Load a ogg file for playing
//
/////////////////////////////////////////////////////////////////////////*/
void CSound::LoadMusic(char *strFileName)
{
	//make sure music isnt playing
	ReleaseMusic();

	//used for error checking
	int result;

	//open file
	m_OggFile = fopen(strFileName, "rb");
	if (!m_OggFile)
	{
		m_SoundLogger << "ERROR: Could not open OggVorbis file: " << strFileName << '\n';
	}

	result = ov_open(m_OggFile, &m_OggStream, NULL, 0);
	LogOggError(result);

    m_VorbisInfo = ov_info(&m_OggStream, -1);
    m_VorbisComment = ov_comment(&m_OggStream, -1);

    if(m_VorbisInfo->channels == 1)
	{
        m_OggFormat = AL_FORMAT_MONO16;
	}
    else
	{
        m_OggFormat = AL_FORMAT_STEREO16;
	}

    alGenBuffers(2, m_OggBuffers);
    ErrorCheck();
    alGenSources(1, &m_OggSource);
    ErrorCheck();
    
    alSource3f(m_OggSource, AL_POSITION,        0.0, 0.0, 0.0);
    alSource3f(m_OggSource, AL_VELOCITY,        0.0, 0.0, 0.0);
    alSource3f(m_OggSource, AL_DIRECTION,       0.0, 0.0, 0.0);
    alSourcef (m_OggSource, AL_ROLLOFF_FACTOR,  0.0          );
    alSourcei (m_OggSource, AL_SOURCE_RELATIVE, AL_TRUE      );

	//set the music volume, make sure it doesn't drop below a certain threshold
	float fMusicVol = m_fMusicVolume;
	if (fMusicVol < 0.0f)
	{
		fMusicVol = 0.0f;
	}
	alSourcef(m_OggSource, AL_GAIN, fMusicVol);
	
	m_bMusicLoaded = true;

	ov_raw_seek(&m_OggStream, 0);
	PlayMusic();
}

/*////////////////////////////////////////////////////////////////////////
//Function:	ReleaseMusic
//
//Purpose:	Clean up memory and variables for music
//
/////////////////////////////////////////////////////////////////////////*/
void CSound::ReleaseMusic()
{
	StopMusic();

	//clear up memory
	alDeleteSources(1, &m_OggSource);
    ErrorCheck();
    alDeleteBuffers(1, m_OggBuffers);
    ErrorCheck();

    ov_clear(&m_OggStream);

	m_bMusicLoaded = false;
}

/*////////////////////////////////////////////////////////////////////////
//Function:	PlayMusic
//
//Purpose:	Playback the currently loaded ogg vorbis stream
//
/////////////////////////////////////////////////////////////////////////*/
void CSound::PlayMusic()
{
	if (!StreamMusic(m_OggBuffers[0]))
	{
		m_SoundLogger << "ERROR: OggVorbis refused to play\n";
	}

	if (!StreamMusic(m_OggBuffers[1]))
	{
		m_SoundLogger << "ERROR: OggVorbis refused to play\n";
	}

	alSourceQueueBuffers(m_OggSource, 2, m_OggBuffers);
	alSourcePlay(m_OggSource);
}

/*////////////////////////////////////////////////////////////////////////
//Function:	UpdateMusic
//
//Purpose:	Process the queued buffers and make sure the music keeps playing
//
/////////////////////////////////////////////////////////////////////////*/
void CSound::UpdateMusic()
{
	if (!m_bMusicLoaded)
	{
		return;
	}

    int processed;

    alGetSourcei(m_OggSource, AL_BUFFERS_PROCESSED, &processed);

    while(processed)
    {
        ALuint buffer;
        
		//pop the next buffer and play it
        alSourceUnqueueBuffers(m_OggSource, 1, &buffer);
        ErrorCheck();
        StreamMusic(buffer);
        alSourceQueueBuffers(m_OggSource, 1, &buffer);
        ErrorCheck();
		processed--;
    }

	//check if the song is over
	ALenum state;
	alGetSourcei(m_OggSource, AL_SOURCE_STATE, &state);
	ErrorCheck();

	//restart the music when it ends
	double dCurMusicPos = ov_time_tell(&m_OggStream);
	double dTotalMusicLength = ov_time_total(&m_OggStream, -1);
	if (dCurMusicPos >= dTotalMusicLength)
	{
		StopMusic();
		ov_raw_seek(&m_OggStream, 0);
		PlayMusic();
	}
}

/*////////////////////////////////////////////////////////////////////////
//Function:	StreamMusic
//
//Input:
//	uBuffer:	the buffer to set up for streaming
//
//Return:
//	true:	buffer successfully queued
//
//Purpose:	Set up a buffer for streaming
//
/////////////////////////////////////////////////////////////////////////*/
bool CSound::StreamMusic(ALuint uBuffer)
{
    char pcm[BUFFER_SIZE];
    int  size = 0;
    int  section;
    int  result;

    while(size < BUFFER_SIZE)
    {
        result = ov_read(&m_OggStream, pcm + size, BUFFER_SIZE - size, 0, 2, 1, &section);
    
        if(result > 0)
		{
            size += result;
		}
        else if(result < 0)
		{
            LogOggError(result);
		}
        else
		{
            break;
		}
    }
    
    if(size == 0)
	{
		return false;
	}
        
    alBufferData(uBuffer, m_OggFormat, pcm, size, m_VorbisInfo->rate);
    ErrorCheck();
	return true;
}

/*////////////////////////////////////////////////////////////////////////
//Function:	LogOggError
//
//Input:	
//	iErrorCode:	the error code returned from ogg vorbis
//
//Purpose:	Output an ogg vorbis error code to the log file
//
/////////////////////////////////////////////////////////////////////////*/
void CSound::LogOggError(int iErrorCode)
{
    switch(iErrorCode)
    {
        case OV_EREAD:
			{
				m_SoundLogger << "ERROR: Ogg Error: Read from media\n";
			}
			break;
        case OV_ENOTVORBIS:
			{
				m_SoundLogger << "ERROR: Ogg Error: Not Vorbis data\n";
			}
			break;
        case OV_EVERSION:
			{
				m_SoundLogger << "ERROR: Ogg Error: Vorbis version mismatch\n";
			}
			break;
        case OV_EBADHEADER:
			{
				m_SoundLogger << "ERROR: Ogg Error: Invalid Vorbis header\n";
			}
			break;
        case OV_EFAULT:
			{
				m_SoundLogger << "ERROR: Ogg Error: Internal logic fault (bug or heap/stack corruption)\n";
			}
			break;
        default:
			{
				m_SoundLogger << "ERROR: Ogg Error: Unknown Ogg error: " << iErrorCode << '\n';
			}
			break;
    }
}

/*////////////////////////////////////////////////////////////////////////
//Function:	StopMusic
//
//Purpose:	Unqueue any buffers from the music source
//
/////////////////////////////////////////////////////////////////////////*/
void CSound::StopMusic()
{
	//make sure it isn't playing
	alSourceStop(m_OggSource);

	//clear out all queued sources
	int queued = 0;
	alGetSourcei(m_OggSource, AL_BUFFERS_QUEUED, &queued);
    
    while(queued)
    {
        ALuint tempbuffer;
        alSourceUnqueueBuffers(m_OggSource, 1, &tempbuffer);
        ErrorCheck();
		queued--;
    }
}

/*////////////////////////////////////////////////////////////////////////
//Function:	GetVolume & GetMusicVolume
//
//Purpose:	Provide access to the volume and music volume of the game
//
/////////////////////////////////////////////////////////////////////////*/
float CSound::GetVolume() { return m_fVolume; }
float CSound::GetMusicVolume() { return m_fMusicVolume; }