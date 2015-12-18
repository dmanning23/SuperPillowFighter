/*////////////////////////////////////////////////////////////////////////
//File:	RandomNumbers.cpp
//	
//Author:	Dan Manning
//
//Creation Date:	august 28, 2004
//
//Purpose:	function definitions for random number stuffs
//
/////////////////////////////////////////////////////////////////////////*/
#include "RandomNumbers.h"
#include <stdlib.h>

/*////////////////////////////////////////////////////////////////////////
//Function:	RandomFloat
//
//Input:
//	min:	the lower bound
//	max:	the upper bound
//
//Return:	a random number between the min and max
//
//Purpose:	Generate a random float between a range
//
/////////////////////////////////////////////////////////////////////////*/
float RandomFloat(float fMin, float fMax)
{
	return (((fMax - fMin) * (float)rand()/RAND_MAX) + fMin);
}

/*////////////////////////////////////////////////////////////////////////
//Function:	RandomInt
//
//Input:
//	min:	the lower bound
//	max:	the upper bound
//
//Return:	a random number between the min and max
//
//Purpose:	Generate a random integer between a range
//
/////////////////////////////////////////////////////////////////////////*/
int RandomInt(int fMin, int fMax)
{
	return rand() % (fMax - fMin + 1) + fMin;
}