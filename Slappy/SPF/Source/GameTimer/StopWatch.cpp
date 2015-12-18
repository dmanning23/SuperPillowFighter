#include "stopwatch.h"
#include "GameTimer.h"

/*////////////////////////////////////////////////////////////////////////
//Function:	Start
//
//Input:
//	fSeconds:	the amount of time to run this stopwatch, in seconds
//
//Purpose:	start the stopwatch
//
/////////////////////////////////////////////////////////////////////////*/
void CStopWatch::Start(float fSeconds)
{
	m_fSeconds = fSeconds;
	m_fStartTime = GetGameTimer();
	m_bIsActive = true;
}

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
float CStopWatch::GetRemainingTime()
{
	return (m_fSeconds - (GetGameTimer() - m_fStartTime));
}