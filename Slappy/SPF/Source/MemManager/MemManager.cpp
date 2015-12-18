#include "memmanager.h"
#include <memory.h>
#include <stdlib.h>
/*////////////////////////////////////////////////////////////////////////
//File:	MemManager.cpp
//
//Author:	Dan Manning
//
//Creation Date:	7-12-4
//
//Purpose:	This function contains the function definitions of the memmanager module
//
/////////////////////////////////////////////////////////////////////////*/

#ifdef _DEBUG
//logger will only be used in debug mode
#include "..\\Logger\\Logger.h"
#endif

////////////////////////////////////////////////////////////////////////
// Private Datatypes
////////////////////////////////////////////////////////////////////////

//NOTE: In debug mode, checks for overflow will be added to allocated nodes
typedef struct MemNode_t
{
	//pointer to memory
	void *pMem;

	//holds size of memory block
	unsigned int uByteSize;

	//add linked list functionality to memnode
	struct MemNode_t *pNext;
	struct MemNode_t *pPrev;

#ifdef _DEBUG
	//add debugging information
	char *strFileName;
	unsigned int uLineNum;
#endif

} MemNodeT;

////////////////////////////////////////////////////////////////////////
// Private Members
////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
static CLogger MemLogger;

//This variable will be used to store the memory usage peak
static int iCurrentBytesUsed = 0;
static int iMaxBytesUsed = 0;
#endif

//bool to tell if init has already been called
static bool bMemHeapInitialized = false;

//size of heap
static unsigned int iHeapSize;

//pointer to our heap
static void *pHeap;

//head of the linked list of free nodes
static MemNodeT *pFreeNodes;

//head of the linked list of allocated nodes
static MemNodeT *pAllocatedNodes;

////////////////////////////////////////////////////////////////////////
// Private Prototypes
////////////////////////////////////////////////////////////////////////

/*////////////////////////////////////////////////////////////////////////
//Function:	MemMerge
//
//Input:	
//pNode:
//The node to check if it can be merged with contiguous free memory
//
//Purpose:
//This function attempts to merge contiguous free memory.
//It should be called everytime memory is freed.
//
/////////////////////////////////////////////////////////////////////////*/
static void MemMerge(MemNodeT *pNode);

/*////////////////////////////////////////////////////////////////////////
//Function:	MemShow
//
//Purpose:	This function show several stats about memory usage by the
//			memory manager
//
/////////////////////////////////////////////////////////////////////////*/
static void MemShow();

////////////////////////////////////////////////////////////////////////
// Public Functions
////////////////////////////////////////////////////////////////////////

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
bool MemInit(unsigned int uBytes)
{
#ifdef _DEBUG
	MemLogger.Init("LogMemManager.txt");
	MemLogger << "Memory Manager initialized with " << uBytes << " bytes\n";
#endif

	if (!bMemHeapInitialized)
	{
		//initialize heap
		//allocate enough memory and point array at memory
		pHeap = malloc(uBytes);
		iHeapSize = uBytes;
		//set initialized flag
		bMemHeapInitialized = true;

		//intialize node
		//HACK: used malloc
		pFreeNodes = (MemNodeT*)malloc(sizeof(MemNodeT));
		pFreeNodes->pMem = pHeap;
		//check if malloc succeeded
		if (!pFreeNodes->pMem)
		{
			return false;
		}
		pFreeNodes->uByteSize = uBytes;
		pFreeNodes->pNext = 0;
		pFreeNodes->pPrev = 0;
#ifdef _DEBUG
		//intialize debug specific members
		//Do not put any filenames in free nodes
		pFreeNodes->strFileName = 0;
		pFreeNodes->uLineNum = 0;
#endif
	}
#ifdef _DEBUG
	else
	{
		//if in debug mode, print an error message
		MemLogger << "Warning: Attempted to initialize memory heap more than once\n";
	}
#endif

	return true;
}

/*////////////////////////////////////////////////////////////////////////
//Function:	MemShutDown
//
//Purpose:
//Shuts down the module and releases all memory.
/////////////////////////////////////////////////////////////////////////*/
void MemShutDown(void)
{
	//Free physical memory
	if (bMemHeapInitialized)
	{
		free(pHeap);
		bMemHeapInitialized = false;
		iHeapSize = 0;
	}
#ifdef _DEBUG
	else
	{
		MemLogger << "Warning: Attempted to shutdown memory heap before initialization\n";
	}
#endif

	//Show who leaked memory
	MemShow();

	//Free memory allocated to nodes
	while (pAllocatedNodes)
	{
		//This includes freeing strFileName in debug mode
#ifdef _DEBUG
		free(pAllocatedNodes->strFileName);
#endif
		//use free to delete memory allocated for node
		MemNodeT *pTemp = pAllocatedNodes->pNext;
		free(pAllocatedNodes);
		pAllocatedNodes = pTemp;
	}

	//Free all free nodes
	while (pFreeNodes)
	{
		//use free to delete memory allocated for node
		MemNodeT *pTemp = pFreeNodes->pNext;
		free(pFreeNodes);
		pFreeNodes = pTemp;
	}
#ifdef _DEBUG
	//show stats about memory usage
	MemLogger << "Memory Manager shutdown...\n";
	MemLogger.Shutdown();
#endif
}

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
void *_MemMgrMalloc(unsigned int uBytes)
{
	//pointer to allocated memory
	void *pMem = 0;

	////check that the heap was initialized
	//if (!bMemHeapInitialized)
	//{
	//	MemLogger << "Warning: " << strFileName << " at line " << uLineNum
	//		<< " tried to allocate memory without initializing MemManager module\n";
	//}

	//find the first node with enough free memory
	for (MemNodeT *pIndex = pFreeNodes; pIndex; pIndex = pIndex->pNext)
	{
		//check if there is enough memory in this node
		//add four for overflow check
		if (pIndex->uByteSize >= (uBytes + 4))
		{
			////check if this is the peak usage
			//iCurrentBytesUsed += (uBytes + 16);
			//if (iCurrentBytesUsed > iMaxBytesUsed)
			//{
			//	iMaxBytesUsed = iCurrentBytesUsed;
			//}

			//create allocated node
			MemNodeT *pTempNode;
			//use malloc to allocate memory for node
			pTempNode = (MemNodeT*)malloc(sizeof(MemNodeT));
			memset(pTempNode, 0, sizeof(MemNodeT));
			pTempNode->uByteSize = (uBytes + 4);
			//set up debugging info of node
			//pTempNode->uLineNum = uLineNum;
			//pTempNode->strFileName = (char*)malloc(strlen(strFileName) + 1);
			//strcpy(pTempNode->strFileName, strFileName);
			//add to linked list of allocated nodes
			pTempNode->pNext = pAllocatedNodes;
			if (pAllocatedNodes)
			{
				pAllocatedNodes->pPrev = pTempNode;
			}
			pAllocatedNodes = pTempNode;

			//point new node memory to memory of free node
			pTempNode->pMem = pIndex->pMem;
			pMem = pTempNode->pMem;
			//add overflow check
			//void *Overflow = malloc(sizeof(unsigned int));
			//*(unsigned int*)(Overflow) = 0xc0c0c0;
			//memcpy(((char*)pMem + uBytes), Overflow, sizeof(unsigned int));
			//free(Overflow);

			//update free node
			//subtract ubytes from bytesize of freenode
			//add four for overflow check
			pIndex->uByteSize -= (uBytes + 4);
			//if bytesize == 0, remove freenode
			if (pIndex->uByteSize == 0)
			{
				if (pIndex->pPrev)
				{
					pIndex->pPrev->pNext = pIndex->pNext;
				}
				if (pIndex->pNext)
				{
					pIndex->pNext->pPrev = pIndex->pPrev;
                    if (pFreeNodes == pIndex)
                    {
                        pFreeNodes = pIndex->pNext;
                    }
				}
				
				free(pIndex);
			}
			else
			{
				//else move pmem of freenode to remaining free memory
				pIndex->pMem = (void*)((char*)pIndex->pMem + (uBytes + 4));
			}
			break;
		}
	}

	//if (pMem == 0) 
	//{
	//	MemLogger << "Warning: " << strFileName << " at line " << uLineNum
	//		<< " was unable to find free memory\n";
	//}

	//if it was unable to find free memory or the module has not been 
	//intialized, this point will still point to null
	return pMem;
}

#ifdef _DEBUG
/*////////////////////////////////////////////////////////////////////////
//Function:	_MemDebugMalloc
//
//Input:	
//uBytes:		Number of bytes to allocate
//uLineNum:		line number where this function was called
//strFileName:	file where this function was called
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
void *_MemMgrDebugMalloc(unsigned int uBytes, unsigned int uLineNum, char *strFileName)
{
	//pointer to allocated memory
	void *pMem = 0;

	//check that the heap was initialized
	if (!bMemHeapInitialized)
	{
		MemLogger << "Warning: " << strFileName << " at line " << uLineNum
			<< " tried to allocate memory without initializing MemManager module\n";
	}

	//find the first node with enough free memory
	for (MemNodeT *pIndex = pFreeNodes; pIndex; pIndex = pIndex->pNext)
	{
		//check if there is enough memory in this node
		//add four for overflow check
		if (pIndex->uByteSize >= (uBytes + 4))
		{
			//check if this is the peak usage
			iCurrentBytesUsed += (uBytes + 16);
			if (iCurrentBytesUsed > iMaxBytesUsed)
			{
				iMaxBytesUsed = iCurrentBytesUsed;
			}

			//create allocated node
			MemNodeT *pTempNode;
			//use malloc to allocate memory for node
			pTempNode = (MemNodeT*)malloc(sizeof(MemNodeT));
			memset(pTempNode, 0, sizeof(MemNodeT));
			pTempNode->uByteSize = (uBytes + 4);
			//set up debugging info of node
			pTempNode->uLineNum = uLineNum;
			pTempNode->strFileName = (char*)malloc(strlen(strFileName) + 1);
			strcpy(pTempNode->strFileName, strFileName);
			//add to linked list of allocated nodes
			pTempNode->pNext = pAllocatedNodes;
			if (pAllocatedNodes)
			{
				pAllocatedNodes->pPrev = pTempNode;
			}
			pAllocatedNodes = pTempNode;

			//point new node memory to memory of free node
			pTempNode->pMem = pIndex->pMem;
			pMem = pTempNode->pMem;
			//add overflow check
			void *Overflow = malloc(sizeof(unsigned int));
			*(unsigned int*)(Overflow) = 0xc0c0c0;
			memcpy(((char*)pMem + uBytes), Overflow, sizeof(unsigned int));
			free(Overflow);

			//update free node
			//subtract ubytes from bytesize of freenode
			//add four for overflow check
			pIndex->uByteSize -= (uBytes + 4);
			//if bytesize == 0, remove freenode
			if (pIndex->uByteSize == 0)
			{
				if (pIndex->pPrev)
				{
					pIndex->pPrev->pNext = pIndex->pNext;
				}
				if (pIndex->pNext)
				{
					pIndex->pNext->pPrev = pIndex->pPrev;
                    if (pFreeNodes == pIndex)
                    {
                        pFreeNodes = pIndex->pNext;
                    }
				}
				
				free(pIndex);
			}
			else
			{
				//else move pmem of freenode to remaining free memory
				pIndex->pMem = (void*)((char*)pIndex->pMem + (uBytes + 4));
			}
			break;
		}
	}

	if (pMem == 0) 
	{
		MemLogger << "Warning: " << strFileName << " at line " << uLineNum
			<< " was unable to find free memory\n";
	}

	//if it was unable to find free memory or the module has not been 
	//intialized, this point will still point to null
	return pMem;
}
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
void MemFree(void *pMem)
{
	//every ten times free is called, call merge on pMem
	//use this variable to count
	//static int iFreeCounter = 0;

	//find pMem
	for (MemNodeT *pIndex = pAllocatedNodes; pIndex; pIndex = pIndex->pNext)
	{
		if (pMem == pIndex->pMem)
		{
#ifdef _DEBUG
			//The size of this node in bytes is: ubytes + size of node (16) - size of overflow check (4)
			iCurrentBytesUsed -= (pIndex->uByteSize + 12);
			//free the filename
			free(pIndex->strFileName);
			pIndex->strFileName = 0;
#endif
			//remove node from allocated list
			if (pIndex->pPrev)
			{
				pIndex->pPrev->pNext = pIndex->pNext;
			}
			if (pIndex->pNext)
			{
				pIndex->pNext->pPrev = pIndex->pPrev;
			}
			if (pAllocatedNodes == pIndex)
			{
				pAllocatedNodes = pIndex->pNext;
			}

			//add node to free list using insertion sort
			MemNodeT* pFreeIter = pFreeNodes;
			while (pFreeIter)
			{
				if (pIndex->pMem >= pFreeIter->pMem)
				{
					pFreeIter = pFreeIter->pNext;
				}
				else
				{
					pIndex->pNext = pFreeIter;
					pIndex->pPrev = pFreeIter->pPrev;
					pFreeIter->pPrev = pIndex;
					if (pIndex->pPrev)
					{
						pIndex->pPrev->pNext = pIndex;
					}
					//check if this should be the head node
                    if (pFreeNodes == pFreeIter)
					{
						pFreeNodes = pIndex;
					}
					break;
				}
			}

			//point pmem at 0 so users don't try to mess with allocated memory
			pMem = 0;

			//if this function has been called ten times, called merge mem
//			if ((iFreeCounter % 10) == 0)
//			{
				MemMerge(pIndex);
//			}
			break;
		}
	}

	//iFreeCounter++;
}

/*////////////////////////////////////////////////////////////////////////
//Function:	MemFlush
//
//Purpose:
//Flushes all allocations from the heap and makes all of the heap
//memory available for use again.
/////////////////////////////////////////////////////////////////////////*/
void MemFlush()
{
	//used to iterate lists
	MemNodeT *pIter = pAllocatedNodes;

	//clean allocated list
	while (pIter)
	{
#ifdef _DEBUG
		//The size of this node in bytes is: ubytes + size of node (16) 
		iCurrentBytesUsed -= (pIter->uByteSize + 16);
		free(pIter->strFileName);
		pIter->strFileName = 0;
#endif
		//free node
		MemNodeT *pTemp = pIter;
		pIter = pIter->pNext;
		//HACK: used free
		free(pTemp);
	}
	pAllocatedNodes = 0;

	//clean free list
	pIter = pFreeNodes->pNext;
	while (pIter)
	{
		//free node
		MemNodeT *pTemp = pIter;
		pIter = pIter->pNext;
		//HACK: used free
		free(pTemp);
	}

	//set up first node of free list
	pFreeNodes->uByteSize = iHeapSize;
	pFreeNodes->pMem = pHeap;
	pFreeNodes->pNext = 0;
	pFreeNodes->pPrev = 0;
}

#ifdef _DEBUG
/*////////////////////////////////////////////////////////////////////////
//Function:	MemCheck
//
//Purpose:
//This function goes through the linked list and checks if any buffer overflows
//have occurred.
//
/////////////////////////////////////////////////////////////////////////*/
void _MemMgrCheck()
{
	//check allocated nodes for buffer overflow nodes
	for (MemNodeT *pIter = pAllocatedNodes; pIter; pIter = pIter->pNext)
	{
		//get pointer to last four bytes of memory in node
		void *Overflow = malloc(sizeof(unsigned int));
		memcpy((void*)Overflow, ((char*)pIter->pMem + (pIter->uByteSize - 4)), sizeof(unsigned int));

		//check if a buffer overflow occured
		if (*(unsigned int*)Overflow != 0xc0c0c0)
		{
			MemLogger << "BUFFER OVERFLOW: " << pIter->strFileName << " at line "
				<< pIter->uLineNum << '\n';
		}

		free(Overflow);
	}
}
#endif

/*////////////////////////////////////////////////////////////////////////
//Function:	MemMerge
//
//Input:	
//pNode:
//The node to check if it can be merged with contiguous free memory
//
//Purpose:
//This function attempts to merge contiguous free memory.
//It checks the nodes on both sides of the node.
//It should be called everytime memory is freed.
//
/////////////////////////////////////////////////////////////////////////*/
void MemMerge(MemNodeT *pNode)
{
	//check if previous element is free memory
	if (pNode->pPrev)
	{
		if (((char*)(pNode->pPrev->pMem) + (pNode->pPrev->uByteSize)) == pNode->pMem)
		{
			//set previous to point to whole clump
			pNode->pPrev->uByteSize += pNode->uByteSize;
			pNode->pPrev->pNext = pNode->pNext;
			if (pNode->pNext)
			{
				pNode->pNext->pPrev = pNode->pPrev;
			}

			//remove this node
#ifdef _DEBUG
			free(pNode->strFileName);
#endif
			MemNodeT *pTemp = pNode->pPrev;
			//HACK: used free
			free(pNode);
			pNode = pTemp;
		}
	}

	//check if there is a next
	if (pNode->pNext)
	{
		if (((char*)(pNode->pMem) + (pNode->uByteSize)) == pNode->pNext->pMem)
		{
			//get pointer to next node, used for deletion
			MemNodeT * pNextTemp = pNode->pNext;

			//set node to point to whole clump
			pNode->uByteSize += pNode->pNext->uByteSize;
			pNode->pNext = pNode->pNext->pNext;
			if (pNode->pNext)
			{
				pNode->pNext->pPrev = pNode;
			}

			//remove this node
#ifdef _DEBUG
			free(pNextTemp->strFileName);
#endif
			//HACK: used free
			free(pNextTemp);
		}
	}
}

/*////////////////////////////////////////////////////////////////////////
//Function:	MemShow
//
//Purpose:	This function show several stats about memory usage by the
//			memory manager
//
/////////////////////////////////////////////////////////////////////////*/
void MemShow()
{
#ifdef _DEBUG
	//Check if any memory was leaked
	MemNodeT *pIter = pAllocatedNodes;
	while (pIter)
	{
		MemLogger << "MEMORY LEAK: " << pIter->strFileName << " at line "
			<< pIter->uLineNum << ": " << (pIter->uByteSize - 4) << " bytes\n";
		pIter = pIter->pNext;
	}

	//show max amount of memory used
	MemLogger << "Peak memory usage was " << iMaxBytesUsed << " bytes\n";
#endif
}