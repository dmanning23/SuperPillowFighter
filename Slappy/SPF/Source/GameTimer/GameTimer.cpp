/*////////////////////////////////////////////////////////////////////////
//File:	GameTimer.c
//
//Author:	Dan Manning
//
//Creation Date:	8-20-4
//
//Purpose:	Function definitions for the Game timer module
//
/////////////////////////////////////////////////////////////////////////*/
#include <windows.h>
#include "GameTimer.h"

//the current game time
static float fGameClock = 0;

//This variable holds the time of the system time.  
//This number will be compared with the result of 
//QueryPerformanceCounter to find the change in time.
static LARGE_INTEGER iOldSystemTime = {0,0};

//The current time according to the performance counter
static LARGE_INTEGER iCurrentSystemTime = {0,0};

//tells whether the game is pause or unpaused
static BOOL bPaused = FALSE;

//number of cpu ticks per second
static LARGE_INTEGER iTicksPerSecond = {0,0};

static LARGE_INTEGER iDifference = {0,0};
static float fDifference = 0.0f;

/*////////////////////////////////////////////////////////////////////////
//Function:	TimerStart
//
//Purpose:	This function starts the GameTimer.  
//The GameClock is set to 0, and the OldSystemTime is set to the current time.
//
/////////////////////////////////////////////////////////////////////////*/
void TimerStart()
{
	//find the ticks per second
	QueryPerformanceFrequency(&iTicksPerSecond);

	//set timers
	QueryPerformanceCounter(&iCurrentSystemTime);
	iOldSystemTime = iCurrentSystemTime;

	fGameClock = 0.0f;
}

/*////////////////////////////////////////////////////////////////////////
//Function:	TimerUpdate
//
//Purpose:	This function finds the change in time between OldSystemTime 
//and the current system time, and adds that time to the GameClock.  
//If the game is paused and this function is called, it will do nothing.
//
/////////////////////////////////////////////////////////////////////////*/
void TimerUpdate()
{
	//LARGE_INTEGER iDifference = {0,0};
	//float fDifference = 0.0f;

	if (bPaused == FALSE)
	{
		//find current time
		QueryPerformanceCounter(&iCurrentSystemTime);

		//subtract old time from current time
		iDifference.QuadPart = iCurrentSystemTime.QuadPart - iOldSystemTime.QuadPart;
		iOldSystemTime = iCurrentSystemTime;

		//divide by ticks per second
		fDifference = (float)iDifference.QuadPart / (float)iTicksPerSecond.QuadPart;

		//add to gameclock
		fGameClock += fDifference;
	}
}

/*////////////////////////////////////////////////////////////////////////
//Function:	TimerPause
//
//Purpose:	This function sets the GameTimer to paused. 
//
/////////////////////////////////////////////////////////////////////////*/
void TimerPause()
{
	bPaused = TRUE;
}

/*////////////////////////////////////////////////////////////////////////
//Function:	TimerUnpause
//
//Purpose:	This function upauses the GameTimer.  It sets the OldSystemTime 
//to the current system time.
//
/////////////////////////////////////////////////////////////////////////*/
void TimerUnpause()
{
	bPaused = FALSE;
	QueryPerformanceCounter(&iOldSystemTime);
}

/*////////////////////////////////////////////////////////////////////////
//Function:	GetGameTimer
//
//Return:	the current game time
//
//Purpose:	This function returns the current time of the Game Clock. in seconds.
//
/////////////////////////////////////////////////////////////////////////*/
float GetGameTimer()
{
	return fGameClock;
}