/*////////////////////////////////////////////////////////////////////////
//File:	Profiler.cpp
//
//Author:	Dan Manning
//
//Creation Date:	8-28-4
//
//Purpose:	Function definitions for profiler module
//
/////////////////////////////////////////////////////////////////////////*/
#include "Profiler.h"
//#include "..\\GameTimer\\GameTimer.h"
//#include "..\\Logger\\Logger.h"
#include <fstream>
using namespace std;
#include <windows.h>

#if _RUNPROFILER

//max length of a profiler tag
#define MAX_TAG_LENGTH 64

//These are the tags that will be outputted to the profiler.txt file at the end
//of the program.  Each must appear in this array according to the corresponding
//profiler in the enumeration.
static char ProfilerTags[Num_Profilers][MAX_TAG_LENGTH] = 
{
	"TimerUpdate",
	"CSound_Update",
	"CInputInterface_UpdateInput",
	"AI_Update",
	"CPhysicsModule_UpdatePhysics",
	"CObjectManager_Update",
	"Animation_Update",
	"ParticleEngine_Update",
	"CDisplayDonkey_drawWorld",
	"CHUD_UpdateHUD",
	"CDisplayDonkey_Render",
	"CCamera_profiler",
	"UpdatePlayers_profiler",
};

/*
	TimerUpdate_profiler,
	CSound_Update_profiler,
	CInputInterface_UpdateInput_profiler,
	AI_Update_profiler,
	CPhysicsModule_UpdatePhysics_profiler,
	CObjectManager_Update_profiler,
	Animation_Update_profiler,
	ParticleEngine_Update_profiler,
	CDisplayDonkey_drawWorld_profiler,
	CHUD_UpdateHUD_profiler,
	CDisplayDonkey_Render_profiler,
	CCamera_profiler,
	UpdatePlayers_profiler,
*/

//array to hold lengths of time each profiler runs during the program
static float fLengthProfilers[Num_Profilers];

//The current profiler that is running
//Will be set to -1 if no profiler is running
static int iCurrentProfiler;

//Holds the last profiler
//Used to run previous profiler
static int iPreviousProfiler;

//time the current profiler started
static float fTimeStart;

//This variable holds the time of the system time.  
//This number will be compared with the result of 
//QueryPerformanceCounter to find the change in time.
static LARGE_INTEGER iOldSystemTime = {0,0};

//The current time according to the performance counter
static LARGE_INTEGER iCurrentSystemTime = {0,0};

//number of cpu ticks per second
static LARGE_INTEGER iTicksPerSecond = {0,0};

/*////////////////////////////////////////////////////////////////////////
//Function:	InitializeProfiler
//
//Purpose:	Resets all profiler data to 0
//
/////////////////////////////////////////////////////////////////////////*/
void _InitializeProfiler()
{
	memset(&fLengthProfilers, 0, (sizeof(float) * Num_Profilers));
	iCurrentProfiler = -1;
	iPreviousProfiler = -1;
	fTimeStart = 0.0f;

	//find the ticks per second
	QueryPerformanceFrequency(&iTicksPerSecond);

	//set timers
	QueryPerformanceCounter(&iCurrentSystemTime);
	iOldSystemTime = iCurrentSystemTime;
}

/*////////////////////////////////////////////////////////////////////////
//Function:	_UpdateProfiler
//
//Purpose:	This function is called at the beginning of each frame to set
//	the internal clock of the profiler
//
/////////////////////////////////////////////////////////////////////////*/
void _UpdateProfiler()
{
	//fTimeStart = GetGameTimer();
}

/*////////////////////////////////////////////////////////////////////////
//Function:	_StartProfiler
//
//Input:	
//	iProfilerID:	the id of the profiler to start, as enumerated in profiler.h
//
//Purpose:	Start profiling a function
//
/////////////////////////////////////////////////////////////////////////*/
void _StartProfiler(int iProfilerID)
{
	//end current profiler
	if (iCurrentProfiler >= 0)
	{
		LARGE_INTEGER iDifference = {0,0};
		float fDifference = 0.0f;

		//find current time
		QueryPerformanceCounter(&iCurrentSystemTime);

		//subtract old time from current time
		iDifference.QuadPart = iCurrentSystemTime.QuadPart - iOldSystemTime.QuadPart;
		iOldSystemTime = iCurrentSystemTime;

		//divide by ticks per second
		fDifference = (float)iDifference.QuadPart / (float)iTicksPerSecond.QuadPart;

		//add to gameclock
		fLengthProfilers[iCurrentProfiler] += fDifference;
	}

	iPreviousProfiler = iCurrentProfiler;
	iCurrentProfiler = iProfilerID;
}

/*////////////////////////////////////////////////////////////////////////
//Function:	StopProfiler
//
//Purpose:	Stop the profiler module at the end of a frame
//
/////////////////////////////////////////////////////////////////////////*/
void _StopProfiler()
{
	//end current profiler
	if (iCurrentProfiler >= 0)
	{
		StartProfiler(-1);
	}

	//iCurrentProfiler = -1;
	fTimeStart = 0.0f;
}

/*////////////////////////////////////////////////////////////////////////
//Function:	ProfilerDump
//
//Purpose:	Output the contents of the profiler to profiler.txt
//
/////////////////////////////////////////////////////////////////////////*/
void _ProfilerDump()
{
	ofstream fout;
	fout.open("LogFiles\\GameProfile.txt");

	//sum of all game time
	float fTotalGameTime = 0.0f;

	//sum up game time
	for (int i = 0; i < Num_Profilers; i++)
	{
		fTotalGameTime += fLengthProfilers[i];
	}

	//output profiler info to file
	for (int i = 0; i < Num_Profilers; i++)
	{
		fout << ProfilerTags[i] << '\n'
			<< '\t' << fLengthProfilers[i] << '\n' 
			<< '\t' << ((fLengthProfilers[i] / fTotalGameTime) * 100.0f) << '\n';
	}

	fout.close();
}

/*////////////////////////////////////////////////////////////////////////
//Function:	StartPreviousProfiler
//
//Purpose:	This function starts the previous profiler.  If you call StartProfiler
//			outside of the GameManager, you NEED to call this function when you 
//			want to stop profiling your function!!!!!!!!
//
/////////////////////////////////////////////////////////////////////////*/
void _StartPreviousProfiler()
{
	StartProfiler(iPreviousProfiler);
}

#endif