//////////////////////////////////////////////////////////////////////////////////////////
//File Name:		spfMath.h
// 
//Author:			Kevin Cecelski
//
//Date Created:		8/6/2004
//
//Purpose:			This file provides basic math functions along 
//					with 4D vector functions and 4x4 matrix functions.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef _SPFMATH
#define _SPFMATH

#include <math.h>		// For math!
#include <stdio.h>		// For output
#include "crtdbg.h"		// For assertions.

// All defines are rounded to seven digit precision.
#define SPF_PI				(3.1415927f)		// PI
#define SPF_HALF_PI			(1.5707964f)		// PI divided by 2
#define SPF_TWO_PI			(2.0f * SPF_PI)		// PI multiplied by 2
#define SPF_SQRT2			(1.4142136f)		// The sqrt of 2
#define SPF_E				(2.7182818f)		// the illustrious e(base of the natural log)
#define SPF_DEG_TO_RAD		(SPF_PI/180.0f)		// Converts degrees to radians.
#define SPF_RAD_TO_DEG		(180.0f/SPF_PI)		// Converts radians to degrees.

#define SPF_SQRT(_fin)		(sqrtf(_fin))		// Square root
#define SPF_INVSQRT(_fin)	(1.0f/sqrtf(_fin))	// Inverse of the square root
#define SPF_SIN(_fin)		(sinf(_fin))		// Sine
#define SPF_ASIN(_fin)		(asinf(_fin))		// Inverse sine
#define SPF_COS(_fin)		(cosf(_fin))		// Cosine
#define SPF_ACOS(_fin)		(acosf(_fin))		// Inverse cosine
#define SPF_TAN(_fin)		(tanf(_fin))		// Tangent
#define SPF_ATAN(_fin)		(atanf(_fin))		// Inverse tangent

// ****************** 4 dimensional vectors ******************

// A 4-d Vector definition.
struct Vec4DT
{
	float tfX;	// X component
	float tfY;	// Y component
	float tfZ;	// Z component
	//float tfW;	// W component
};

// Maybe initialize all vectors to this zero vector?
const Vec4DT zeroVector = { 0.0f, 0.0f, 0.0f }; //, 1.0f };

// A homogeneous unit vector that lies on the x-axis.
const Vec4DT xVector = { 1.0f, 0.0f, 0.0f }; //, 1.0f };

// A homogeneous unit vector that lies on the y-axis.
const Vec4DT yVector = { 0.0f, 1.0f, 0.0f }; //, 1.0f };
 
// A homogeneous unit vector that lies on the z-axis.
const Vec4DT zVector = { 0.0f, 0.0f, 1.0f }; //, 1.0f };

// Use this to make a new INITIALIZED 4D vector.
#define NEW_VEC4D(_vecName)		Vec4DT _vecName = zeroVector;

//////////////////////////////////////////
// Function: spfMathLerp()
//
// Purpose:	This is used to perform simple LINEAR interpolation
//			between to values given a parameter.
//
// Last Modified: 8/6/2004
//
// Input: fstart	the start value
//		  fend		the end value
//		  ft		the parameter
//
// Output: None. 
//
// Returns: The result of the linear interpolation between the values
//			we are given.
//
//
// NOTE:	Linear interpolation requires: 0 <= ft <= 1
//
//////////////////////////////////////////
#ifdef _DEBUG 
_inline float spfMathLerp(const float fstart, const float fend, const float ft)
{
	return ((1.0f - ft)*fstart) + (ft*fend);
}
#else 
	#define addVec4DT(_result, _fisrt, _second)\
		(_result)->tfX = (_fisrt).tfX + (_second).tfX;\
		(_result)->tfY = (_fisrt).tfY + (_second).tfY;\
		(_result)->tfZ = (_fisrt).tfZ + (_second).tfZ;
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name:	printVec4DT()
//
//Purpose:		This function print out the 3D coordinates of a 4D vector to a output stream
//
//Last Modified:	8/6/2004
// 
//Input:			*fOutf	- The output stream where the data will be written.
//					vec		- The vector to print.
//
//Output:		None.
//
//Return:		None.
//
//Notes:		To print to stadard output, pass in stdout.
////////////////////////////////////////////////////////////////////////////////////////////////////////
void printVec4DT(FILE *fOutf, Vec4DT &vec);
	
////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name:	addVec4DT()
//
//Purpose:		This function adds two 4D vectors and returns the result.
//
//Last Modified:	8/6/2004
// 
//Input:			*resultVec	- Where the result of the addition will be stored.
//					firstVec	- The first operand.
//					secondVec	- The second operand.
//
//Output:		*resultVec		- The result of the vector addition.
//
//Return:		None.
////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG 
inline void addVec4DT(Vec4DT *resultVec, const Vec4DT &firstVec, const Vec4DT &secondVec) 
{
	// Do not give me a null pointer!
	_ASSERT(resultVec != 0);

	// Do vector addition.
	resultVec->tfX = firstVec.tfX + secondVec.tfX;
	resultVec->tfY = firstVec.tfY + secondVec.tfY;
	resultVec->tfZ = firstVec.tfZ + secondVec.tfZ;
}
#else 
	#define addVec4DT(_result, _fisrt, _second)\
		(_result)->tfX = (_fisrt).tfX + (_second).tfX;\
		(_result)->tfY = (_fisrt).tfY + (_second).tfY;\
		(_result)->tfZ = (_fisrt).tfZ + (_second).tfZ;
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name:	subVec4DT()
//
//Purpose:		This function subtracts two 4D vectors and returns the result.
//
//Last Modified:	8/6/2004
// 
//Input:			*resultVec	- Where the result of the subtraction will be stored.
//					firstVec	- The first operand.
//					secondVec	- The second operand.
//
//Output:		*resultVec		- The result of the vector subtraction.
//
//Return:		None.
////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG 
inline void subVec4DT(Vec4DT *resultVec, const Vec4DT &firstVec, const Vec4DT &secondVec)
{
	// Do not give me a null pointer!
	_ASSERT(resultVec != 0);

	// Do vector subtraction.
	resultVec->tfX = firstVec.tfX - secondVec.tfX;
	resultVec->tfY = firstVec.tfY - secondVec.tfY;
	resultVec->tfZ = firstVec.tfZ - secondVec.tfZ;
}
#else 
	#define subVec4DT(_result, _fisrt, _second)\
		(_result)->tfX = (_fisrt).tfX - (_second).tfX;\
		(_result)->tfY = (_fisrt).tfY - (_second).tfY;\
		(_result)->tfZ = (_fisrt).tfZ - (_second).tfZ;
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name:	sMultVec4DT()
//
//Purpose:		This function multiplies a vector by a scalar.
//
//Last Modified:	8/6/2004
// 
//Input:			*resultVec	- Where the result of the subtraction will be stored.
//					fScalar		- The scalar value.
//					secondVec	- The vector used in the operation.
//
//Output:		*resultVec		- The result of the multiplication.
//
//Return:		None.
////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG 
inline void sMultVec4DT(Vec4DT *resultVec, const float fScalar, /*const*/ Vec4DT &vec)
{
	// Do not give me a null pointer!
	_ASSERT(resultVec != 0);
	
	// Do scalar by vector multiplication.
   	resultVec->tfX = fScalar * vec.tfX;
	resultVec->tfY = fScalar * vec.tfY;
	resultVec->tfZ = fScalar * vec.tfZ; 
}
#else 
	#define sMultVec4DT(_result, _scal, _vec)\
		(_result)->tfX = _scal * (_vec).tfX;\
		(_result)->tfY = _scal * (_vec).tfY;\
		(_result)->tfZ = _scal * (_vec).tfZ;
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name:	DotVec4DT()
//
//Purpose:		This function subtracts two 4D vectors and returns the result.
//
//Last Modified:	8/6/2004
// 
//Input:			firstVec	- The first operand.
//					secondVec	- The second operand.
//
//Output:		None.
//
//Return:		The result of the dot product.
////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG 
inline float DotVec4DT(const Vec4DT &firstVec, const Vec4DT &secondVec)
{
	return (firstVec.tfX * secondVec.tfX) + (firstVec.tfY * secondVec.tfY) 
		+ (firstVec.tfZ * secondVec.tfZ);
}
#else 
	#define DotVec4DT(_first, _second)\
		( (_first).tfX * (_second).tfX + (_first).tfY * (_second).tfY +\
			(_first).tfZ * (_second).tfZ )
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name:	crossVec4DT()
//
//Purpose:		This function finds the cross product of two vectors.
//
//Last Modified:	8/6/2004
// 
//Input:			*resultVec	- Where the result of the cross product will be stored.
//					firstVec	- The first operand.
//					secondVec	- The second operand.
//
//Output:		*resultVec		- The result of the cross product.
//
//Return:		None.
////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG 
inline void crossVec4DT(Vec4DT *resultVec, const Vec4DT &firstVec, const Vec4DT &secondVec)
{
	// Do not give me a null pointer!
	_ASSERT(resultVec != 0);

	resultVec->tfX = firstVec.tfY * secondVec.tfZ - firstVec.tfZ * secondVec.tfY;
	resultVec->tfY = firstVec.tfZ * secondVec.tfX - firstVec.tfX * secondVec.tfZ;
	resultVec->tfZ = firstVec.tfX * secondVec.tfY - firstVec.tfY * secondVec.tfX;
}
#else 
	#define crossVec4DT(_result, _first, _second)\
		(_result)->tfX = (_first).tfY * (_second).tfZ - (_first).tfZ * (_second).tfY;\
		(_result)->tfY = (_first).tfZ * (_second).tfX - (_first).tfX * (_second).tfZ;\
		(_result)->tfZ = (_first).tfX * (_second).tfY - (_first).tfY * (_second).tfX;
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name:	LengthVec4DT()
//
//Purpose:		This function finds the length of a vector.
//
//Last Modified:	8/6/2004
// 
//Input:			vec	- The vector...
//
//Output:		None.
//
//Return:		The length of the vector given.
////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG 
inline float LengthVec4DT(const Vec4DT &vec)
{
	return SPF_SQRT((vec.tfX * vec.tfX) + (vec.tfY * vec.tfY) + (vec.tfZ * vec.tfZ));
}
#else 
	#define LengthVec4DT(_vec)\
		 (SPF_SQRT(((_vec).tfX * (_vec).tfX) + ((_vec).tfY * (_vec).tfY) + ((_vec).tfZ * (_vec).tfZ)))
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name:	LengthSqrdVec4DT()
//
//Purpose:		This function finds the length squared of a vector.
//
//Last Modified:	8/6/2004
// 
//Input:		vec	- The vector...
//
//Output:		None.
//
//Return:		The length of the vector squared.
////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG 
inline float LengthSqrdVec4DT(const Vec4DT &vec)
{
	return (vec.tfX * vec.tfX) + (vec.tfY * vec.tfY) + (vec.tfZ * vec.tfZ);
}
#else 
	#define LengthSqrdVec4DT(_vec)\
		 ( ((_vec).tfX * (_vec).tfX) + ((_vec).tfY * (_vec).tfY) + ((_vec).tfZ * (_vec).tfZ) )
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name:	normalizeVec4DT()
//
//Purpose:		This function normalizes a vector.
//
//Last Modified:	8/6/2004
// 
//Input:			*resultVec	- Where the result of the cross product will be stored.
//					vec			- The vector to normalize.
//
//Output:		*resultVec		- The result of the normalize.
//
//Return:		None.
////////////////////////////////////////////////////////////////////////////////////////////////////////
inline void normalizeVec4DT(Vec4DT *resultVec, const Vec4DT &vec)
{
	// Do not give me a null pointer!
	_ASSERT(resultVec != 0);

	// Find the reciprocal of the length of this vector. 
	float fInvLength = SPF_INVSQRT((vec.tfX * vec.tfX) + (vec.tfY * vec.tfY) + (vec.tfZ * vec.tfZ));

	// Normalize the vector.
	resultVec->tfX = vec.tfX * fInvLength;
	resultVec->tfY = vec.tfY * fInvLength;
	resultVec->tfZ = vec.tfZ * fInvLength;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name:	distVec4DT()
//
//Purpose:		This function finds the distance between two vectors.
//
//Last Modified:	8/6/2004
// 
//Input:			firstVec	- The first operand.
//					secondVec	- The second operand.
//
//Output:		None.
//
//Return:		The distance between the two vectors.
////////////////////////////////////////////////////////////////////////////////////////////////////////
inline float distVec4DT(const Vec4DT &firstVec, const Vec4DT &secondVec)
{
    Vec4DT	distVec;
	subVec4DT(&distVec, firstVec, secondVec);
	
    return LengthVec4DT(distVec);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name:	distSqrdVec4DT()
//
//Purpose:		This function finds the distance between two vectors squared!
//
//Last Modified:	8/6/2004
// 
//Input:			firstVec	- The first operand.
//					secondVec	- The second operand.
//
//Output:		None.
//
//Return:		The distance between the two vectors squared.
////////////////////////////////////////////////////////////////////////////////////////////////////////
inline float distSqrdVec4DT(const Vec4DT &firstVec, const Vec4DT &secondVec)
{
    Vec4DT	distVec;
	subVec4DT(&distVec, firstVec, secondVec);
	
    return LengthSqrdVec4DT(distVec);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name:	LERPVec4DT()
//
//Purpose:		This interpolates between two vectors.
//
//Last Modified:	8/6/2004
// 
//Input:			*resultVec	- Where the result of the cross product will be stored.
//					startVec	- The start vector.
//					endVec	- The end vector.
//
//Output:		*resultVec		- The result interpolation.
//
//Return:		None.
//
//NOTE:			Length is not preserved with linear interpolation!  
//				Linear interpolation requires: 0 <= ft <= 1
////////////////////////////////////////////////////////////////////////////////////////////////////////
inline void LERPVec4DT(Vec4DT *resultVec, const Vec4DT &startVec, const Vec4DT &endVec, const float ft)
{
	// Do not give me a null pointer!
	_ASSERT(resultVec != 0);

	resultVec->tfX = ((1.0f - ft)*startVec.tfX) + (ft*endVec.tfX);
	resultVec->tfY = ((1.0f - ft)*startVec.tfY) + (ft*endVec.tfY);
	resultVec->tfZ = ((1.0f - ft)*startVec.tfZ) + (ft*endVec.tfZ);
}

// ****************** 4x4 Matrices ******************
// Used to access rows of a 4x4 matrix.
struct MatRowVecT
{
	float tfX;	// X component
	float tfY;	// Y component
	float tfZ;	// Z component
	float tfW;	// W component
};

// A 4x4 matrix.
struct Matrix4x4T {
	union {
		MatRowVecT	rowVecs[4];			// Row vectors.
		float		fElements[16];		// 16 Elements in matrix!
	};
};

// An useful identity matrix.
static const Matrix4x4T identityMatrix = { 1, 0, 0, 0,
									0, 1, 0, 0,
									0, 0, 1, 0,
									0, 0, 0, 1 };

// Use this to make a new INITIALIZED 4x4 matrix.
#define NEW_MAT4X4(_matName)		Matrix4x4T _matName = identityMatrix;

////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name:	getRightVec4DT()
//
//Purpose:		This gets the right vector from the matrix.
//
//Last Modified:	8/6/2004
// 
//Input:		mat			- The matrix extract the vector from.
//				*RightVec	- Storage for the right vector.
//
//Output:		*RightVec	- Where the right vector will be stored.
//
//Return:		None.
////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG 
inline void getRightVec4DT(Vec4DT *RightVec, const Matrix4x4T &mat) 
{
	// Do not give me a null pointer!
	_ASSERT(RightVec != 0);

	RightVec->tfX = mat.rowVecs[0].tfX; 
	RightVec->tfY = mat.rowVecs[0].tfY; 
	RightVec->tfZ = mat.rowVecs[0].tfZ; 
}
#else 
	#define getRightVec4DT(_rvec, _mat)\
		((_rvec)->tfX) = (_mat).rowVecs[0].tfX;\
		((_rvec)->tfY) = (_mat).rowVecs[0].tfY;\
		((_rvec)->tfZ) = (_mat).rowVecs[0].tfZ;
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name:	getUpVec4DT()
//
//Purpose:		This gets the up vector from the matrix.
//
//Last Modified:	8/6/2004
// 
//Input:		mat			- The matrix extract the vector from.
//				*UpVec	- Storage for the up vector.
//
//Output:		*UpVec	- Where the up vector will be stored.
//
//Return:		None.
////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG 
inline void getUpVec4DT(Vec4DT *UpVec, const Matrix4x4T &mat) 
{
	// Do not give me a null pointer!
	_ASSERT(UpVec != 0);

	UpVec->tfX = mat.rowVecs[1].tfX;
	UpVec->tfY = mat.rowVecs[1].tfY;
	UpVec->tfZ = mat.rowVecs[1].tfZ;
	//UpVec->tfW = 1.0f;
}
#else 
	#define getUpVec4DT(_uvec, _mat)\
		((_uvec)->tfX) = (_mat).rowVecs[1].tfX;\
		((_uvec)->tfY) = (_mat).rowVecs[1].tfY;\
		((_uvec)->tfZ) = (_mat).rowVecs[1].tfZ; 
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name:	getLookAtVec4DT()
//
//Purpose:		This gets the look at vector from the matrix.
//
//Last Modified:	8/6/2004
// 
//Input:		mat			- The matrix extract the vector from.
//				*LookAtVec	- Storage for the look at vector.
//
//Output:		*LookAtVec	- Where the look at vector will be stored.
//
//Return:		None.
////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
inline void getLookAtVec4DT(Vec4DT *LookAtVec, const Matrix4x4T &mat) 
{
	// Do not give me a null pointer!
	_ASSERT(LookAtVec != 0);

	LookAtVec->tfX = mat.rowVecs[2].tfX;
	LookAtVec->tfY = mat.rowVecs[2].tfY;
	LookAtVec->tfZ = mat.rowVecs[2].tfZ;
}
#else 
	#define getLookAtVec4DT(_lavec, _mat)\
		((_lavec)->tfX) = (_mat).rowVecs[2].tfX;\
		((_lavec)->tfY) = (_mat).rowVecs[2].tfY;\
		((_lavec)->tfZ) = (_mat).rowVecs[2].tfZ;
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name:	getLookAtVec4DT()
//
//Purpose:		This gets the position vector from the matrix.
//
//Last Modified:	8/6/2004
// 
//Input:		mat			- The matrix extract the vector from.
//				*PosVec	- Storage for the position vector.
//
//Output:		*PosVec	- Where the position vector will be stored.
//
//Return:		None.
////////////////////////////////////////////////////////////////////////////////////////////////////////
#if _DEBUG
inline void getPosVec4DT(Vec4DT *PosVec, const Matrix4x4T &mat) 
{
	// Do not give me a null pointer!
	_ASSERT(PosVec != 0);

	PosVec->tfX = mat.rowVecs[3].tfX;
	PosVec->tfY = mat.rowVecs[3].tfY;
	PosVec->tfZ = mat.rowVecs[3].tfZ;
}
#else 
	#define getPosVec4DT(_pvec, _mat)\
		((_pvec)->tfX) = (_mat).rowVecs[3].tfX;\
		((_pvec)->tfY) = (_mat).rowVecs[3].tfY;\
		((_pvec)->tfZ) = (_mat).rowVecs[3].tfZ;
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name:	getRightVec4DT()
//
//Purpose:		This gets the right vector from the matrix.
//
//Last Modified:	8/6/2004
// 
//Input:		mat			- The matrix extract the vector from.
//				*RightVec	- Storage for the right vector.
//
//Output:		*RightVec	- Where the right vector will be stored.
//
//Return:		None.
////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
inline void setRightVec4DT(Vec4DT &RightVec, Matrix4x4T &mat) 
{
	mat.rowVecs[0].tfX = RightVec.tfX;
	mat.rowVecs[0].tfY = RightVec.tfY;
	mat.rowVecs[0].tfZ = RightVec.tfZ;
}
#else 
	#define setRightVec4DT(_rvec, _mat)\
		(_mat).rowVecs[0].tfX = (_rvec).tfX;\
		(_mat).rowVecs[0].tfY = (_rvec).tfY;\
		(_mat).rowVecs[0].tfZ = (_rvec).tfZ
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////
////Function Name:	setUpVec4DT()
////
////Purpose:		This sets the up vector from the matrix.
////
////Last Modified:	8/6/2004
//// 
////Input:		mat			- The matrix extract the vector from.
////				*UpVec	- Storage for the up vector.
////
////Output:		*UpVec	- Where the up vector will be stored.
////
////Return:		None.
//////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
inline void setUpVec4DT(Vec4DT &UpVec, Matrix4x4T &mat) 
{
	mat.rowVecs[1].tfX = UpVec.tfX;
	mat.rowVecs[1].tfY = UpVec.tfY;
	mat.rowVecs[1].tfZ = UpVec.tfZ;
}
#else 
	#define setUpVec4DT(_uvec, _mat)\
		(_mat).rowVecs[1].tfX = (_uvec).tfX;\
		(_mat).rowVecs[1].tfY = (_uvec).tfY;\
		(_mat).rowVecs[1].tfZ = (_uvec).tfZ

#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////
////Function Name:	setLookAtVec4DT()
////
////Purpose:		This gets the look at vector from the matrix.
////
////Last Modified:	8/6/2004
//// 
////Input:		mat			- The matrix extract the vector from.
////				*LookAtVec	- Storage for the look at vector.
////
////Output:		*LookAtVec	- Where the look at vector will be stored.
////
////Return:		None.
//////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
inline void setLookAtVec4DT(Vec4DT &LookAtVec, Matrix4x4T &mat) 
{
	mat.rowVecs[2].tfX = LookAtVec.tfX;
	mat.rowVecs[2].tfY = LookAtVec.tfY;
	mat.rowVecs[2].tfZ = LookAtVec.tfZ;
}
#else 
	#define setLookAtVec4DT(_lavec, _mat)\
		(_mat).rowVecs[2].tfX = (_lavec).tfX;\
		(_mat).rowVecs[2].tfY = (_lavec).tfY;\
		(_mat).rowVecs[2].tfZ = (_lavec).tfZ
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////
////Function Name:	setLookAtVec4DT()
////
////Purpose:		This gets the position vector from the matrix.
////
////Last Modified:	8/6/2004
//// 
////Input:		mat			- The matrix extract the vector from.
////				*PosVec	- Storage for the position vector.
////
////Output:		*PosVec	- Where the position vector will be stored.
////
////Return:		None.
//////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
inline void setPosVec4DT(Vec4DT &PosVec, Matrix4x4T &mat) 
{
	mat.rowVecs[3].tfX = PosVec.tfX;
	mat.rowVecs[3].tfY = PosVec.tfY;
	mat.rowVecs[3].tfZ = PosVec.tfZ;
}
#else 
	#define setPosVec4DT(_pvec, _mat)\
		(_mat).rowVecs[3].tfX = (_pvec).tfX;\
		(_mat).rowVecs[3].tfY = (_pvec).tfY;\
		(_mat).rowVecs[3].tfZ = (_pvec).tfZ
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name:	printMat4x4()
//
//Purpose:		This prints a matrix.
//
//Last Modified:	8/6/2004
// 
//Input:		fOutf		- The output stream where the data will be written.
//				mat			- The matrix to print.
//
//Output:		None.
//
//Return:		None.
////////////////////////////////////////////////////////////////////////////////////////////////////////
void printMat4x4T(FILE *fOutf, Matrix4x4T mat);

////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name:	addMat4x4T()
//
//Purpose:		This adds two matrices.
//
//Last Modified:	8/6/2004
// 
//Input:		*resultVec			- Stores the addition result.
//				firstMat			- The first operand.
//				secondMat			- The second operand.
//
//Output:		*resultVec			- Recieves the result of the matrix addition.
//
//Return:		None.
////////////////////////////////////////////////////////////////////////////////////////////////////////
void addMat4x4T(Matrix4x4T *resultVec, const Matrix4x4T &firstMat, const Matrix4x4T &secondMat);

////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name:	subMat4x4T()
//
//Purpose:		This subtracts two matrices.
//
//Last Modified:	8/6/2004
// 
//Input:		*resultVec			- Stores the subtraction result.
//				firstMat			- The first operand.
//				secondMat			- The second operand.
//
//Output:		*resultVec			- Recieves the result of the matrix subtraction.
//
//Return:		None.
////////////////////////////////////////////////////////////////////////////////////////////////////////
void subMat4x4T(Matrix4x4T *resultVec, const Matrix4x4T &firstMat, const Matrix4x4T &secondMat);

////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name:	sMultMat4x4T()
//
//Purpose:		Multiplies a matrix by a scalar.
//
//Last Modified:	8/6/2004
// 
//Input:		*resultVec			- Stores the subtraction result.
//				fScalar				- The scalar used in the multiplication.
//				mat					- The matrix used in the multiplication.
//
//Output:		*resultVec			- Stores the result of the multiplication.
//
//Return:		None.
////////////////////////////////////////////////////////////////////////////////////////////////////////
void sMultMat4x4T(Matrix4x4T *resultVec, const float fScalar, const Matrix4x4T &mat);

////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name:	MMultMat4x4T()
//
//Purpose:		Multiplies a matrix by a matrix.
//
//Last Modified:	8/6/2004
// 
//Input:		*resultVec			- Stores the subtraction result.
//				firstMat			- The first matrix used in the multiplication.
//				secondMat			- The second matrix used in the multiplication.
//
//Output:		*resultVec			- Stores the result of the multiplication.
//
//Return:		None.
//////////////////////////////////////////////////////////////////////////////////////////////////////
void MMultMat4x4T(Matrix4x4T *resultVec, const Matrix4x4T &firstMat, const Matrix4x4T &secondMat);

////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name:	MatVecMult()
//
//Purpose:		This post-multiplies a vector by a matrix.
//
//Last Modified:	8/6/2004
// 
//Input:		*resultVec			- Stores the multiplication result.
//				mat					- The first matrix used in the multiplication.
//				vec					- The vector used in the multiplication.
//
//Output:		*resultVec			- Stores the result of the multiplication.
//
//Return:		None.
////////////////////////////////////////////////////////////////////////////////////////////////////////
inline void MatVecMult(Vec4DT *resultVec, const Matrix4x4T &mat, const Vec4DT &vec)
{
	// Do not give me a null pointer!
	_ASSERT(resultVec != 0);

	// Get the x component.
	resultVec->tfX = mat.rowVecs[0].tfX * vec.tfX + mat.rowVecs[1].tfX * vec.tfY
		+ mat.rowVecs[2].tfX * vec.tfZ + mat.rowVecs[3].tfX;

	// Get the y component.
	resultVec->tfY =  mat.rowVecs[0].tfY * vec.tfX + mat.rowVecs[1].tfY * vec.tfY 
		+ mat.rowVecs[2].tfY * vec.tfZ + mat.rowVecs[3].tfY;

	// Get the z component.
	resultVec->tfZ =  mat.rowVecs[0].tfZ * vec.tfX + mat.rowVecs[1].tfZ * vec.tfY 
		+ mat.rowVecs[2].tfZ * vec.tfZ + mat.rowVecs[3].tfZ;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name:	transposeMat4x4T()
//
//Purpose:		Finds the transpose of a matrix.
//
//Last Modified:	8/6/2004
// 
//Input:		mat			- The matrix to transpose.
//				*resultMat	- Where the transposed matrix will be stored.
//
//Output:		*resultMat	- The transpose of the matrix we are given.
//
//Return:		None.
////////////////////////////////////////////////////////////////////////////////////////////////////////
void transposeMat4x4T(Matrix4x4T *resultMat, const Matrix4x4T &mat);

////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name:	rotXMat4x4T()
//
//Purpose:		This creates a matrix for rotation around the x-axis.
//
//Last Modified:	8/6/2004
// 
//Input:		*resultMat			- Storage for the rotation matrix.
//				fAng				- The angle of rotation(in radians).
//
//Output:		*resultMat			- A matrix for rotation around the x-axis based on the angle given. 
//
//Return:		None.
////////////////////////////////////////////////////////////////////////////////////////////////////////
void rotXMat4x4T(Matrix4x4T *resultMat, const float fAng);

////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name:	rotYMat4x4T()
//
//Purpose:		This creates a matrix for rotation around the y-axis.
//
//Last Modified:	8/6/2004
// 
//Input:		*resultMat			- Storage for the rotation matrix.
//				fAng				- The angle of rotation(in radians).
//
//Output:		*resultMat			- A matrix for rotation around the y-axis based on the angle given. 
//
//Return:		None.
////////////////////////////////////////////////////////////////////////////////////////////////////////
void rotYMat4x4T(Matrix4x4T *resultMat, const float fAng);

////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name:	rotZMat4x4T()
//
//Purpose:		This creates a matrix for rotation around the z-axis.
//
//Last Modified:	8/6/2004
// 
//Input:		*resultMat			- Storage for the rotation matrix.
//				fAng				- The angle of rotation(in radians).
//
//Output:		*resultMat			- A matrix for rotation around the z-axis based on the angle given. 
//
//Return:		None.
////////////////////////////////////////////////////////////////////////////////////////////////////////
void rotZMat4x4T(Matrix4x4T *resultMat, const float fAng);

////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name:	translateMat4x4T()
//
//Purpose:		Adds to the position vector the amount specfied.
//
//Last Modified:	8/14/2004
// 
//Input:		*resultMat			- Storage for the rotation matrix.
//				fx					- the amount to translate in the x direction.
//				fy					- the amount to translate in the y direction.
//				fx					- the amount to translate in the z direction.
//
//Output:		*resultMat			- The affected matrix.
//
//Return:		None.
////////////////////////////////////////////////////////////////////////////////////////////////////////
inline void translateMat4x4T(Matrix4x4T *resultMat, float fx, float fy, float fz)
{
	resultMat->rowVecs[3].tfX += fx;
	resultMat->rowVecs[3].tfY += fy;
	resultMat->rowVecs[3].tfZ += fz;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name:	XtranslateMat4x4T()
//
//Purpose:		Adds to the position vector the amount specfied in the x direction only.
//
//Last Modified:	8/14/2004
// 
//Input:		*resultMat			- Storage for the rotation matrix.
//				fx					- the amount to translate in the x direction.
//
//Output:		*resultMat			- The affected matrix.
//
//Return:		None.
////////////////////////////////////////////////////////////////////////////////////////////////////////
inline void XtranslateMat4x4T(Matrix4x4T *resultMat, float fx)
{
	resultMat->rowVecs[3].tfX += fx;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name:	YtranslateMat4x4T()
//
//Purpose:		Adds to the position vector the amount specfied in the y direction only.
//
//Last Modified:	8/14/2004
// 
//Input:		*resultMat			- Storage for the rotation matrix.
//				fy					- the amount to translate in the y direction.
//
//Output:		*resultMat			- The affected matrix.
//
//Return:		None.
////////////////////////////////////////////////////////////////////////////////////////////////////////
inline void YtranslateMat4x4T(Matrix4x4T *resultMat, float fy)
{
	resultMat->rowVecs[3].tfY += fy;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name:	ZtranslateMat4x4T()
//
//Purpose:		Adds to the position vector the amount specfied in the z direction only.
//
//Last Modified:	8/14/2004
// 
//Input:		*resultMat			- Storage for the rotation matrix.
//				fz					- the amount to translate in the z direction.
//
//Output:		*resultMat			- The affected matrix.
//
//Return:		None.
////////////////////////////////////////////////////////////////////////////////////////////////////////
inline void ZtranslateMat4x4T(Matrix4x4T *resultMat, float fz)
{
	resultMat->rowVecs[3].tfZ += fz;
}

#endif _SPFMATH