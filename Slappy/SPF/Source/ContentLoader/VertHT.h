#ifndef _VERT_HT_H_
#define _VERT_HT_H_
#include <stdio.h>
#include "../Renderer/GeometryTypes.h"
#include "../MathLibrary/spfMath.h"
#include "../Memmanager/Memmanager.h"

#define HASH_TABLE_SIZE	100		// Number of buckets in the hash table.

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
int createVertHT(int iNvts, VertexT *pData, VertexT *pCndList);

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
int getCndVertIndex(VertexT &vert, VertexT *pCndList);

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
void cleanupVertHT();

#endif
