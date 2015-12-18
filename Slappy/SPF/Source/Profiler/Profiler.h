#ifndef _PROFILER_H_
#define _PROFILER_H_
/*////////////////////////////////////////////////////////////////////////
//File:	Profiler.h
//	
//Author:	Dan Manning
//
//Creation Date:	8-26-4
//
//Purpose:	function definitions for prfiler module
//
/////////////////////////////////////////////////////////////////////////*/

/*
NOTE***NOTE***NOTE***NOTE***NOTE
Turn off this flag to remove the profiler module from the game
*/
#define _RUNPROFILER 0

//defines to remove profiler functions from release mode
#if _RUNPROFILER
#define InitializeProfiler() _InitializeProfiler()
#else
#define InitializeProfiler()  
#endif

#if _RUNPROFILER
#define StartProfiler(i) _StartProfiler(i)
#else
#define StartProfiler(i)  
#endif

#if _RUNPROFILER
#define StopProfiler() _StopProfiler()
#else
#define StopProfiler()  
#endif

#if _RUNPROFILER
#define ProfilerDump() _ProfilerDump()
#else
#define ProfilerDump()  
#endif

#if _RUNPROFILER
#define StartPreviousProfiler() _StartPreviousProfiler()
#else
#define StartPreviousProfiler()  
#endif

#ifdef _RUNPROFILER
//Enumeration of profilers
/*
NOTE:
If you want to add a profiler to this module, add the enumeration for it here, 
and then add the output string to the "ProfilerTags" array so it gets outputted to the
file right.

**NOTE**NOTE**NOTE***NOTE!!!!
You must let everyone know when you add a profiler so there are no conflicts with CVS
*/
enum 
{

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

	Num_Profilers
};
#endif

/*////////////////////////////////////////////////////////////////////////
//Function:	InitializeProfiler
//
//Purpose:	Resets all profiler data to 0
//
/////////////////////////////////////////////////////////////////////////*/
void _InitializeProfiler();

/*////////////////////////////////////////////////////////////////////////
//Function:	_UpdateProfiler
//
//Purpose:	This function is called at the beginning of each frame to set
//	the internal clock of the profiler
//
/////////////////////////////////////////////////////////////////////////*/
void _UpdateProfiler();

/*////////////////////////////////////////////////////////////////////////
//Function:	_StartProfiler
//
//Input:	
//	iProfilerID:	the id of the profiler to start, as enumerated in profiler.h
//
//Purpose:	Start profiling a function
//
/////////////////////////////////////////////////////////////////////////*/
void _StartProfiler(int iProfilerID);

/*////////////////////////////////////////////////////////////////////////
//Function:	StopProfiler
//
//Purpose:	Stop the profiler module at the end of a frame
//
/////////////////////////////////////////////////////////////////////////*/
void _StopProfiler();

/*////////////////////////////////////////////////////////////////////////
//Function:	ProfilerDump
//
//Purpose:	Output the contents of the profiler to profiler.txt
//
/////////////////////////////////////////////////////////////////////////*/
void _ProfilerDump();

/*////////////////////////////////////////////////////////////////////////
//Function:	StartPreviousProfiler
//
//Purpose:	This function starts the previous profiler.  If you call StartProfiler
//			outside of the GameManager, you NEED to call this function when you 
//			want to stop profiling your function!!!!!!!!
//
/////////////////////////////////////////////////////////////////////////*/
void _StartPreviousProfiler();

#endif