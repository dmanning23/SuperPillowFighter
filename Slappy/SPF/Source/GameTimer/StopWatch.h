#ifndef _STOPWATCH_H_
#define _STOPWATCH_H_
/*////////////////////////////////////////////////////////////////////////
//File:	Stopwatch.h
//
//Author:	Dan Manning
//
//Creation Date:	8-23-4
//
//Purpose:	This class provides an easy way for modules to utilize the
//			GameTimer as a stopwatch for timing events
//
/////////////////////////////////////////////////////////////////////////*/

class CStopWatch
{
	//The amount of time to run this stopwatch for
	float m_fSeconds;

	//The time this stopwatch was started at
	float m_fStartTime;

	//used to kill a stopwatch
	bool m_bIsActive;
	
public:

	CStopWatch(void) : m_fSeconds(0.0f), m_fStartTime(0.0f), m_bIsActive(false) {}
	virtual ~CStopWatch(void) {}

	/*////////////////////////////////////////////////////////////////////////
	//Function:	Start
	//
	//Input:
	//	fSeconds:	the amount of time to run this stopwatch, in seconds
	//
	//Purpose:	start the stopwatch
	//
	/////////////////////////////////////////////////////////////////////////*/
	void Start(float fSeconds);

	/*////////////////////////////////////////////////////////////////////////
	//Function:	GetRemainingTime
	//
	//Return:	the amount of time left in this stopwatch
	//
	//Purpose:	Check how much time is remaining in this stopwatch
	//			This number will be negative once the number of elapsed seconds
	//			is greater than the amount of time on the stopwatch.
	//
	/////////////////////////////////////////////////////////////////////////*/
	float GetRemainingTime();

	/*////////////////////////////////////////////////////////////////////////
	//Function:	KillTimer
	//
	//Purpose:	This function sets a timer to the inactive state.
	//
	/////////////////////////////////////////////////////////////////////////*/
	void KillTimer() { m_bIsActive = false; }

	/*////////////////////////////////////////////////////////////////////////
	//Function:	GetActive
	//
	//Return:	true:	the timer is active
	//			false:	the timer is inactive
	//
	//Purpose:	provide a way to access the active bool
	//
	/////////////////////////////////////////////////////////////////////////*/
	bool GetActive() { return m_bIsActive; }
};

#endif