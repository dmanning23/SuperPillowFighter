#ifndef _MEMMANAGER_H_
#define _MEMMANAGER_H_

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

//Macro to hide line and file in debug mode
#ifdef _DEBUG
#define MemMalloc(_numBytes) (_MemMgrDebugMalloc(_numBytes, __LINE__, __FILE__))
//#define MemMalloc(_numBytes)    malloc(_numBytes)
#else
#define MemMalloc(_numBytes) (_MemMgrMalloc(_numBytes))
#endif

//Macro to hide calls to memcheck in release mode
#ifdef _DEBUG
#define MemCheck() (_MemMgrCheck())
#else
#define MemCheck()  
#endif

/*////////////////////////////////////////////////////////////////////////
//Function:	MemInit
//
//Input:
//uBytes:
//Number of bytes used for the heap
//
//Return:
//bool:
//True upon success, false otherwise
//
//Purpose:
//This function allocates the physical memory for the memory manager.
//It checks if the memory initialization was successful.
//
/////////////////////////////////////////////////////////////////////////*/
bool MemInit(unsigned int uBytes);

/*////////////////////////////////////////////////////////////////////////
//Function:	MemShutDown
//
//Purpose:
//Shuts down the module and releases all memory.
/////////////////////////////////////////////////////////////////////////*/
void MemShutDown();

/*////////////////////////////////////////////////////////////////////////
//Function:	_MemMalloc
//
//Input:	
//uBytes:
//Number of bytes to allocate
//
//Return:
//void *:	pointer to the memory allcated
//
//Purpose:
//Allocates memory from the heap if it is available.
//This function is only called in release mode.
//This module will return null if there is not enough memory or the
//heap has not been initialized.  
//
/////////////////////////////////////////////////////////////////////////*/
void *_MemMgrMalloc(unsigned int uBytes);

#ifdef _DEBUG
/*////////////////////////////////////////////////////////////////////////
//Function:	_MemDebugMalloc
//
//Input:	
//uBytes:
//Number of bytes to allocate
//
//Return:
//void *:	pointer to the memory allcated
//
//Purpose:
//Allocates memory from the heap if it is available.
//This function is only called in debug mode.
//This module will return null if there is not enough memory or the
//heap has not been initialized.  
//
/////////////////////////////////////////////////////////////////////////*/
void *_MemMgrDebugMalloc(unsigned int uBytes, unsigned int uLineNum, char *strFileName);
#endif

/*////////////////////////////////////////////////////////////////////////
//Function:	MemFree
//
//Input:
//pMem:
//Memory to be freed
//
//Purpose:
//Frees a block of memory in the heap for use again.
//
/////////////////////////////////////////////////////////////////////////*/
void MemFree(void *pMem);

/*////////////////////////////////////////////////////////////////////////
//Function:	MemFlush
//
//Purpose:
//Flushes all allocations from the heap and makes all of the heap
//memory available for use again.
/////////////////////////////////////////////////////////////////////////*/
void MemFlush();

/*////////////////////////////////////////////////////////////////////////
//Function:	MemMgrCheck
//
//Purpose:
//This function goes through the linked list and checks if any buffer overflows
//have occurred.  This function is only used in debug mode, and is preporcessed
//out in release.
//
/////////////////////////////////////////////////////////////////////////*/
void _MemMgrCheck();

#endif
