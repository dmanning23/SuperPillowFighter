#include "VertHT.h"

// Structure for storing data in the hash table.
struct vertNodeT
{
	int iVertIndex;		// Index into the condensed vertex array.
	vertNodeT *pNext;	// Pointer to the next node in the list.
};

// The vertex hash table.
static vertNodeT *vertHashTable[HASH_TABLE_SIZE];

//////////////////////////////////////////////////////////////////////////////////////////
// Function:	hashFunc()
//
// Purpose:		Given a vertex, this will return which bucket it shall be placed in.
//
// Last Modified:  Tuesday, September 07, 2004
//
// Input:		inVert		the vertex we are inquiring about.
//
// Output:		None.
//
// Returns:		The bucket that this vertex shall be placed in.
//
//////////////////////////////////////////////////////////////////////////////////////////
static inline int hashFunc(VertexT &inVert)
{
	return abs(((int)((inVert.tPosition.tfX + inVert.tPosition.tfY + inVert.tPosition.tfZ +
		inVert.tfTex[0] + inVert.tfTex[1])*HASH_TABLE_SIZE))%HASH_TABLE_SIZE);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Function:	cmpVerts()
//
// Purpose:		Compare two vertices.
//
// Last Modified:  Tuesday, September 07, 2004
//
// Input:		vertL		the first vertex in the comparision.
//				vertR		the second vertex in the comparision.
//
// Output:		None.
//
// Returns:		true if they are equivalent according to our stipulations, false otherwise.
//
//////////////////////////////////////////////////////////////////////////////////////////
static inline bool cmpVerts(VertexT &vertL, VertexT &vertR)
{
	return ( (vertL.tPosition.tfX == vertR.tPosition.tfX) && (vertL.tPosition.tfY == vertR.tPosition.tfY) 
		&& (vertL.tPosition.tfZ == vertR.tPosition.tfZ) && (vertL.tfTex[0] == vertR.tfTex[0])
		&& (vertL.tfTex[1] == vertR.tfTex[1]) );
}

//////////////////////////////////////////////////////////////////////////////////////////
// Function:	createVertHT()
//
// Purpose:		This creates both the hash table and the condensed version of the 
//				vertex list.
//
// Last Modified:  Tuesday, September 07, 2004
//
// Input:		iNvts		the number of vertices in the uncondensed vertex list.
//				pData		the uncondensed vertex list.
//				pCndList	the condensed list of vertices.				 
//
// Output:		pCndList	the resultant condensed vertex list.         
//
// Returns:		the number of vertices in the condensed list.	
//
// NOTE:    
// There is the assumption that memory has already been allocated for both lists.
// pData and pCndList should also be the same size, that is, iNvts elements.
//
//////////////////////////////////////////////////////////////////////////////////////////
int createVertHT(int iNvts, VertexT *pData, VertexT *pCndList)
{
	int iNodesCount, ii, iBucket;
	vertNodeT *pHead, *pPrev;
	bool bFound;

	iNodesCount = 0;

	// Clear the hash table.
	memset(vertHashTable, 0, sizeof(vertNodeT *) * HASH_TABLE_SIZE);
	memset(pCndList, 0, sizeof(VertexT) * iNvts);

    // Go through each vertex in the list, insert it into the table and build
	// the condensed list.
	for(ii = 0; ii < iNvts; ii++)
	{
		// Get the bucket where this vertex should go.
		iBucket = hashFunc(pData[ii]);

		// Find out if this vertex is already in the bucket.
		bFound = false;
		pPrev = vertHashTable[iBucket];
		pHead = pPrev;
		while(pHead)
		{
			// Make sure this not in the list and that the hash table is not empty.
			if(cmpVerts(pCndList[pHead->iVertIndex], pData[ii]) && iNodesCount > 0)
			{
				bFound = true;
				break;
			}

			pPrev = pHead;
			pHead = pHead->pNext;
		}

		if(bFound)
			continue; // It is in the list!  Go to the next vertex.
        
		// This is a new vertex, add it to the hash table and the condensed list.
		if(vertHashTable[iBucket] == 0)
		{
			// We are at the head of the list.
			pPrev = (vertNodeT *)MemMalloc(sizeof(vertNodeT));
			vertHashTable[iBucket] = pPrev;
		}
		else
		{
			pPrev->pNext = (vertNodeT *)MemMalloc(sizeof(vertNodeT));
			pPrev = pPrev->pNext;
		}

		memset(pPrev, 0, sizeof(vertNodeT));

		// Set the index in the condensed array for this vertex.
		pPrev->iVertIndex = iNodesCount;
			
		// Add it to the condensed list.
		pCndList[iNodesCount++] = pData[ii];					
	}

	// Give back the size of the condensed list.
	return iNodesCount;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Function:	getCndVertIndex()
//
// Purpose:		Get the index in the condensed list for a given vertex from the hash table.
//
// Last Modified:  Tuesday, September 07, 2004
//
// Input:		vert		the vertex we are inquiring about.
//				pCndList	the condensed list.
//
// Output:		None.
//
// Returns:		the index in the condensed list for the vertex we are given.
//
//////////////////////////////////////////////////////////////////////////////////////////
int getCndVertIndex(VertexT &vert, VertexT *pCndList)
{
	vertNodeT *pHead;
	int iBucket;

	// Get the bucket where this vertex should reside.
	iBucket = hashFunc(vert);

	// Start at the beginning of the bucket.
	pHead = vertHashTable[iBucket];

	// Find this vertex in this bucket.
	while(pHead)
	{
		// Is this the vertex we are looking for?
		if( cmpVerts(pCndList[pHead->iVertIndex], vert) )
			return pHead->iVertIndex;	// It is, give back the index for this vertex in the condensed list.

		pHead = pHead->pNext;		// Check the next vertex in this bucket.
	}

	return -1;	// We should never be here!
}

//////////////////////////////////////////////////////////////////////////////////////////
// Function:	cleanupVertHT()
//
// Purpose:		Cleanup the vertex hashtable.
//
// Last Modified:  Tuesday, September 07, 2004
//
// Input:		None.
//
// Output:		None.
//
// Returns:		None.
//
//////////////////////////////////////////////////////////////////////////////////////////
void cleanupVertHT()
{
	int ii;
	vertNodeT *pHead;
    
	// Go through each bucket.
	for(ii = 0; ii < HASH_TABLE_SIZE; ii++)
	{
		// Cleanup each bucket.
		pHead = vertHashTable[ii];
		while(pHead)
		{
			vertHashTable[ii] = vertHashTable[ii]->pNext;
			MemFree(pHead);
			pHead = vertHashTable[ii];
		}
	}
}