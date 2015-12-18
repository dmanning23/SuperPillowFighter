#ifndef _CSOUND_H_
#define _CSOUND_H_

/*////////////////////////////////////////////////////////////////////////
//File:	CSound.h
//
//Author:	Dan Manning
//
//Creation Date:	July 29, 2004
//
//Purpose:	Class definitions for CSound.  This class wraps OpenAL into 
//a singleton for use by a sound manager class.
//
//NOTE:
//	This class would benefit greatly from some sort of vector math library
//	This class would benefit greatly from a doubly linked list
//	These functions need to be called before any sounds can be played:
//	InitSound, SetListenerParams
//	To shut the module call these function in this order:
//	ShutDown, DeleteInstance
//
/////////////////////////////////////////////////////////////////////////*/

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define ErrorCheck() Error_Check(__LINE__)
#else
#define ErrorCheck() Error_Check(__LINE__)
#endif

#include <al/al.h>
#include <al/alc.h>
#include <al/alu.h>
#include <al/alut.h>

//include all ogg files
//#include <ogg/ogg.h>
#include <vorbis/vorbisfile.h>

#include "..\Logger\Logger.h"
#include "..\MathLibrary\SPFMath.h"

#define BUFFER_SIZE (4096 * 10)

#define MAX_BUFFERS 64
#define MAX_SOURCES 31
#define MAX_SOUND_FILENAME 64

//Struct to hold OpenAL buffers
struct OALBufferT
{
	//the buffer loaded from openal
	ALuint uBuffer;

	//whether this buffer is free or not
	bool bFree;

	//the filename of this buffer
	char strFileName[MAX_SOUND_FILENAME];
};

struct OALSourceT
{
	//This holds the openal source
	ALuint uSource;

	//whether this source is free or not
	bool bFree;
};

struct SoundListenerT
{
	//This is a pointer to an array of 3 ALfloats
	//Position of the listener
	ALfloat ListenerPos[3];

	//This is a pointer to an array of 3 ALfloats
	//Velocity of the listener
	ALfloat ListenerVelocity[3];

	//This is a pointer to an array of 6 ALfloats
	//Orientation of the listener
	//First 3 elements are "at", second 3 elements are "up"
	ALfloat ListenerOrientation[6];
};

class CSound
{
	private:

		//The number of source channels supported by the sound card
		int m_iNumSources;

		//Logger for the sound manager
		CLogger m_SoundLogger;

		//Array of all buffers loaded from OpenAL
		OALBufferT *m_Buffers;

		//Array of all sources to be used to create sounds
		OALSourceT *m_Sources;

		//functions to find the next available buffer and source
		int NextAvailableBuffer()
		{
			for (int i = 0; i < MAX_BUFFERS; i++)
				if (m_Buffers[i].bFree)	{return i;}
			return -1;
		}
		int NextAvailableSource()
		{
			for (int i = 0; i < m_iNumSources; i++)
				if (m_Sources[i].bFree)	{return i;}
			return -1;
		}

		//The only listener
		SoundListenerT m_Listener;

		//The volume to play everything at
		//ALfloat m_fVolume;

		//add singleton functionality to sound wrapper
		static CSound *m_pInstance;

		//made private to prevent anyone from creating instances
		CSound(void);
		CSound(const CSound &CSoundRef);
		CSound &operator=(const CSound &CSoundRef);
		
		/*////////////////////////////////////////////////////////////////////////
		//Function:	SetListenerParams
		//
		//Input:
		//ix, iy, iz:	the new position of the listener
		//
		//
		//Purpose:		This function is used to set the params for the listener.
		//				Use the default values to use a static listener at the origin,
		//				looking down the -z and pointing up
		//
		/////////////////////////////////////////////////////////////////////////*/
		void SetListenerParams(Vec4DT tPosition);

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
		bool IsPlaying(int iSourceID);
		
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
		bool IsPaused(int iSourceID);

		virtual ~CSound(void);

		/*////////////////////////////////////////////////////////////////////////
		//Function:	ShutdownSound
		//
		//Purpose:
		//This function shuts down and cleans up the sound module					
		//
		/////////////////////////////////////////////////////////////////////////*/
		void ShutdownSound();

		/*////////////////////////////////////////////////////////////////////////
		//Function:	ErrorLog
		//
		//Purpose:	Check if OpenAL returned any errors
		//
		/////////////////////////////////////////////////////////////////////////*/
		void Error_Check(int linenum);

	public:

		/*////////////////////////////////////////////////////////////////////////
		//Function:	GetVolume & GetMusicVolume
		//
		//Purpose:	Provide access to the volume and music volume of the game
		//
		/////////////////////////////////////////////////////////////////////////*/
		float GetVolume();
		float GetMusicVolume();

		/*////////////////////////////////////////////////////////////////////////
		//Function:	GetInstance
		//
		//Return:	
		//CSound*		pointer to the only instance of CSound
		//	
		//Purpose:
		//Provide a way to access the singleton functionality of the CSound class
		//
		/////////////////////////////////////////////////////////////////////////*/
		static CSound* GetInstance();

		/*////////////////////////////////////////////////////////////////////////
		//Function:	DeleteInstance
		//
		//Purpose:	Clean up after the CSound singleton
		//
		/////////////////////////////////////////////////////////////////////////*/
		static void DeleteInstance();
		
		/*////////////////////////////////////////////////////////////////////////
		//Function:	InitSound
		//
		//Purpose:	This function sets up openal for use
		//
		/////////////////////////////////////////////////////////////////////////*/
        bool InitSound();
		
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
		int LoadSound(char *strFileName);

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
		int PlaySoundEffect(int iBufferID, Vec4DT Position = zeroVector, bool bLoop = false);

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
		void UpdateSound();

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
		void ChangeSFXVolume(float fVolume);
		void ChangeMusicVolume(float fMusicVolume);
		
		/*////////////////////////////////////////////////////////////////////////
		//Function:	ClearBuffers
		//
		//Purpose:	This function will clear all the currently loaded sounds
		//
		/////////////////////////////////////////////////////////////////////////*/
		void ClearBuffers();
				
		/*////////////////////////////////////////////////////////////////////////
		//Function:	StopAllSounds
		//
		//Purpose:	This function will stop all sounds from playing
		//
		/////////////////////////////////////////////////////////////////////////*/
		void StopAllSounds();

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
		void TogglePause(bool bPaused);

	//Ogg Vorbis and music stuff
	private:

		//flag to tell if a song is playing
		bool m_bMusicLoaded;

		//File handle to ogg file
		FILE* m_OggFile;

		//stream handle
		OggVorbis_File m_OggStream;

		//formatting data used by ogg vorbis
		vorbis_info *m_VorbisInfo;

		//user comments?
		vorbis_comment *m_VorbisComment;

		//front and back buffers used to stream
		ALuint m_OggBuffers[2];

		//audio source for music
		ALuint m_OggSource;

		//internal format of ogg source
		ALenum m_OggFormat;

		/*////////////////////////////////////////////////////////////////////////
		//Function:	PlayMusic
		//
		//Purpose:	Playback the currently loaded ogg vorbis stream
		//
		/////////////////////////////////////////////////////////////////////////*/
		void PlayMusic();

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
		bool StreamMusic(ALuint uBuffer);

		/*////////////////////////////////////////////////////////////////////////
		//Function:	LogOggError
		//
		//Input:	
		//	iErrorCode:	the error code returned from ogg vorbis
		//
		//Purpose:	Output an ogg vorbis error code to the log file
		//
		/////////////////////////////////////////////////////////////////////////*/
		void LogOggError(int iErrorCode);

	public:

		/*////////////////////////////////////////////////////////////////////////
		//Function:	StopMusic
		//
		//Purpose:	Unqueue any buffers from the music source
		//
		/////////////////////////////////////////////////////////////////////////*/
		void StopMusic();

		/*////////////////////////////////////////////////////////////////////////
		//Function:	UpdateMusic
		//
		//Purpose:	Process the queued buffers and make sure the music keeps playing
		//
		/////////////////////////////////////////////////////////////////////////*/
		void UpdateMusic();

		/*////////////////////////////////////////////////////////////////////////
		//Function:	LoadMusic
		//
		//Input:	
		//strFileName:	file name of ogg file to load
		//
		//Purpose:	Load a ogg file for playing
		//
		/////////////////////////////////////////////////////////////////////////*/
		//void open(string path);
		void LoadMusic(char *strFileName);

		/*////////////////////////////////////////////////////////////////////////
		//Function:	ReleaseMusic
		//
		//Purpose:	Clean up memory and variables for music
		//
		/////////////////////////////////////////////////////////////////////////*/
		void ReleaseMusic();
};

#endif