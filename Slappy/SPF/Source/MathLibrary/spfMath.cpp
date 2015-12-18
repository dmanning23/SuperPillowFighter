#include "spfmath.h"

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
void printVec4DT(FILE *fOutf, Vec4DT &vec)
{
	fprintf(fOutf, "x:%.2f y:%.2f z:%.2f\n", vec.tfX, vec.tfY, vec.tfZ);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name:	printMat4x4()
//
//Purpose:		This prints a matrix.
//
//Last Modified:	8/6/2004
// 
//Input:		fOutf		- The output stream where the data will be written.
//				mat			- The matrix to print.
//Output:		None.
//
//Return:		None.
////////////////////////////////////////////////////////////////////////////////////////////////////////
void printMat4x4T(FILE *fOutf, Matrix4x4T mat)
{
	// For every row....
	for(int i = 0; i < 4; i++)
	{	
		// For every column...
		for(int j = 0; j < 4; j++)
			fprintf(fOutf, "%.2f ", mat.fElements[i*4+j]);
		fprintf(fOutf, "\n");  // Next row.
	}

	fprintf(fOutf, "\n");
}

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
void addMat4x4T(Matrix4x4T *resultVec, const Matrix4x4T &firstMat, const Matrix4x4T &secondMat)
{
	// Do not give me a null pointer!
	_ASSERT(resultVec != 0);

	// Column 0.
	resultVec->rowVecs[0].tfX =  firstMat.rowVecs[0].tfX + secondMat.rowVecs[0].tfX;
	resultVec->rowVecs[1].tfX =  firstMat.rowVecs[1].tfX + secondMat.rowVecs[1].tfX;
	resultVec->rowVecs[2].tfX =  firstMat.rowVecs[2].tfX + secondMat.rowVecs[2].tfX;
	resultVec->rowVecs[3].tfX =  firstMat.rowVecs[3].tfX + secondMat.rowVecs[3].tfX;

	// Column 1.
	resultVec->rowVecs[0].tfY =  firstMat.rowVecs[0].tfY + secondMat.rowVecs[0].tfY;
	resultVec->rowVecs[1].tfY =  firstMat.rowVecs[1].tfY + secondMat.rowVecs[1].tfY;
	resultVec->rowVecs[2].tfY =  firstMat.rowVecs[2].tfY + secondMat.rowVecs[2].tfY;
	resultVec->rowVecs[3].tfY =  firstMat.rowVecs[3].tfY + secondMat.rowVecs[3].tfY;

	// Column 2.
	resultVec->rowVecs[0].tfZ =  firstMat.rowVecs[0].tfZ + secondMat.rowVecs[0].tfZ;
	resultVec->rowVecs[1].tfZ =  firstMat.rowVecs[1].tfZ + secondMat.rowVecs[1].tfZ;
	resultVec->rowVecs[2].tfZ =  firstMat.rowVecs[2].tfZ + secondMat.rowVecs[2].tfZ;
	resultVec->rowVecs[3].tfZ =  firstMat.rowVecs[3].tfZ + secondMat.rowVecs[3].tfZ;

	// Column 3.
	resultVec->rowVecs[0].tfW =  firstMat.rowVecs[0].tfW + secondMat.rowVecs[0].tfW;
	resultVec->rowVecs[1].tfW =  firstMat.rowVecs[1].tfW + secondMat.rowVecs[1].tfW;
	resultVec->rowVecs[2].tfW =  firstMat.rowVecs[2].tfW + secondMat.rowVecs[2].tfW;
	resultVec->rowVecs[3].tfW =  firstMat.rowVecs[3].tfW + secondMat.rowVecs[3].tfW;
}

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
void subMat4x4T(Matrix4x4T *resultVec, const Matrix4x4T &firstMat, const Matrix4x4T &secondMat)
{
	// Do not give me a null pointer!
	_ASSERT(resultVec != 0);

	// Column 0.
	resultVec->rowVecs[0].tfX =  firstMat.rowVecs[0].tfX - secondMat.rowVecs[0].tfX;
	resultVec->rowVecs[1].tfX =  firstMat.rowVecs[1].tfX - secondMat.rowVecs[1].tfX;
	resultVec->rowVecs[2].tfX =  firstMat.rowVecs[2].tfX - secondMat.rowVecs[2].tfX;
	resultVec->rowVecs[3].tfX =  firstMat.rowVecs[3].tfX - secondMat.rowVecs[3].tfX;

	// Column 1.
	resultVec->rowVecs[0].tfY =  firstMat.rowVecs[0].tfY - secondMat.rowVecs[0].tfY;
	resultVec->rowVecs[1].tfY =  firstMat.rowVecs[1].tfY - secondMat.rowVecs[1].tfY;
	resultVec->rowVecs[2].tfY =  firstMat.rowVecs[2].tfY - secondMat.rowVecs[2].tfY;
	resultVec->rowVecs[3].tfY =  firstMat.rowVecs[3].tfY - secondMat.rowVecs[3].tfY;

	// Column 2.
	resultVec->rowVecs[0].tfZ =  firstMat.rowVecs[0].tfZ - secondMat.rowVecs[0].tfZ;
	resultVec->rowVecs[1].tfZ =  firstMat.rowVecs[1].tfZ - secondMat.rowVecs[1].tfZ;
	resultVec->rowVecs[2].tfZ =  firstMat.rowVecs[2].tfZ - secondMat.rowVecs[2].tfZ;
	resultVec->rowVecs[3].tfZ =  firstMat.rowVecs[3].tfZ - secondMat.rowVecs[3].tfZ;

	// Column 3.
	resultVec->rowVecs[0].tfW =  firstMat.rowVecs[0].tfW - secondMat.rowVecs[0].tfW;
	resultVec->rowVecs[1].tfW =  firstMat.rowVecs[1].tfW - secondMat.rowVecs[1].tfW;
	resultVec->rowVecs[2].tfW =  firstMat.rowVecs[2].tfW - secondMat.rowVecs[2].tfW;
	resultVec->rowVecs[3].tfW =  firstMat.rowVecs[3].tfW - secondMat.rowVecs[3].tfW;
}

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
void sMultMat4x4T(Matrix4x4T *resultVec, const float fScalar, const Matrix4x4T &mat)
{
	// Do not give me a null pointer!
	_ASSERT(resultVec != 0);

	// Column 0.
	resultVec->rowVecs[0].tfX = fScalar * mat.rowVecs[0].tfX;
	resultVec->rowVecs[1].tfX = fScalar * mat.rowVecs[1].tfX;
	resultVec->rowVecs[2].tfX = fScalar * mat.rowVecs[2].tfX;
	resultVec->rowVecs[3].tfX = fScalar * mat.rowVecs[3].tfX;

	// Column 1.
	resultVec->rowVecs[0].tfY = fScalar * mat.rowVecs[0].tfY;
	resultVec->rowVecs[1].tfY = fScalar * mat.rowVecs[1].tfY;
	resultVec->rowVecs[2].tfY = fScalar * mat.rowVecs[2].tfY;
	resultVec->rowVecs[3].tfY = fScalar * mat.rowVecs[3].tfY;

	// Column 2.
	resultVec->rowVecs[0].tfZ = fScalar * mat.rowVecs[0].tfZ;
	resultVec->rowVecs[1].tfZ = fScalar * mat.rowVecs[1].tfZ;
	resultVec->rowVecs[2].tfZ = fScalar * mat.rowVecs[2].tfZ;
	resultVec->rowVecs[3].tfZ = fScalar * mat.rowVecs[3].tfZ;

	// Column 3.
	resultVec->rowVecs[0].tfW = fScalar * mat.rowVecs[0].tfW;
	resultVec->rowVecs[1].tfW = fScalar * mat.rowVecs[1].tfW;
	resultVec->rowVecs[2].tfW = fScalar * mat.rowVecs[2].tfW;
	resultVec->rowVecs[3].tfW = fScalar * mat.rowVecs[3].tfW;
}

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
////////////////////////////////////////////////////////////////////////////////////////////////////////
void MMultMat4x4T(Matrix4x4T *resultVec, const Matrix4x4T &firstMat, const Matrix4x4T &secondMat)
{
	// Do not give me a null pointer!
	_ASSERT(resultVec != 0);

	// Row one.
	resultVec->rowVecs[0].tfX = firstMat.rowVecs[0].tfX * secondMat.rowVecs[0].tfX + firstMat.rowVecs[0].tfY * secondMat.rowVecs[1].tfX +
		firstMat.rowVecs[0].tfZ * secondMat.rowVecs[2].tfX + firstMat.rowVecs[0].tfW * secondMat.rowVecs[3].tfX;
	resultVec->rowVecs[0].tfY = firstMat.rowVecs[0].tfX * secondMat.rowVecs[0].tfY + firstMat.rowVecs[0].tfY * secondMat.rowVecs[1].tfY +
		firstMat.rowVecs[0].tfZ * secondMat.rowVecs[2].tfY + firstMat.rowVecs[0].tfW * secondMat.rowVecs[3].tfY;
	resultVec->rowVecs[0].tfZ = firstMat.rowVecs[0].tfX * secondMat.rowVecs[0].tfZ + firstMat.rowVecs[0].tfY * secondMat.rowVecs[1].tfZ +
		firstMat.rowVecs[0].tfZ * secondMat.rowVecs[2].tfZ + firstMat.rowVecs[0].tfW * secondMat.rowVecs[3].tfZ;
	resultVec->rowVecs[0].tfW = firstMat.rowVecs[0].tfX * secondMat.rowVecs[0].tfW + firstMat.rowVecs[0].tfY * secondMat.rowVecs[1].tfW +
		firstMat.rowVecs[0].tfZ * secondMat.rowVecs[2].tfW + firstMat.rowVecs[0].tfW * secondMat.rowVecs[3].tfW;

	// Row two.
	resultVec->rowVecs[1].tfX = firstMat.rowVecs[1].tfX * secondMat.rowVecs[0].tfX + firstMat.rowVecs[1].tfY * secondMat.rowVecs[1].tfX +
		firstMat.rowVecs[1].tfZ * secondMat.rowVecs[2].tfX + firstMat.rowVecs[1].tfW * secondMat.rowVecs[3].tfX;
	resultVec->rowVecs[1].tfY = firstMat.rowVecs[1].tfX * secondMat.rowVecs[0].tfY + firstMat.rowVecs[1].tfY * secondMat.rowVecs[1].tfY +
		firstMat.rowVecs[1].tfZ * secondMat.rowVecs[2].tfY + firstMat.rowVecs[1].tfW * secondMat.rowVecs[3].tfY;
	resultVec->rowVecs[1].tfZ = firstMat.rowVecs[1].tfX * secondMat.rowVecs[0].tfZ + firstMat.rowVecs[1].tfY * secondMat.rowVecs[1].tfZ +
		firstMat.rowVecs[1].tfZ * secondMat.rowVecs[2].tfZ + firstMat.rowVecs[1].tfW * secondMat.rowVecs[3].tfZ;
	resultVec->rowVecs[1].tfW = firstMat.rowVecs[1].tfX * secondMat.rowVecs[0].tfW + firstMat.rowVecs[1].tfY * secondMat.rowVecs[1].tfW +
		firstMat.rowVecs[1].tfZ * secondMat.rowVecs[2].tfW + firstMat.rowVecs[1].tfW * secondMat.rowVecs[3].tfW;

	// Row three.
	resultVec->rowVecs[2].tfX = firstMat.rowVecs[2].tfX * secondMat.rowVecs[0].tfX + firstMat.rowVecs[2].tfY * secondMat.rowVecs[1].tfX +
		firstMat.rowVecs[2].tfZ * secondMat.rowVecs[2].tfX + firstMat.rowVecs[2].tfW * secondMat.rowVecs[3].tfX;
	resultVec->rowVecs[2].tfY = firstMat.rowVecs[2].tfX * secondMat.rowVecs[0].tfY + firstMat.rowVecs[2].tfY * secondMat.rowVecs[1].tfY +
		firstMat.rowVecs[2].tfZ * secondMat.rowVecs[2].tfY + firstMat.rowVecs[2].tfW * secondMat.rowVecs[3].tfY;
	resultVec->rowVecs[2].tfZ = firstMat.rowVecs[2].tfX * secondMat.rowVecs[0].tfZ + firstMat.rowVecs[2].tfY * secondMat.rowVecs[1].tfZ +
		firstMat.rowVecs[2].tfZ * secondMat.rowVecs[2].tfZ + firstMat.rowVecs[2].tfW * secondMat.rowVecs[3].tfZ;
	resultVec->rowVecs[2].tfW = firstMat.rowVecs[2].tfX * secondMat.rowVecs[0].tfW + firstMat.rowVecs[2].tfY * secondMat.rowVecs[1].tfW +
		firstMat.rowVecs[2].tfZ * secondMat.rowVecs[2].tfW + firstMat.rowVecs[2].tfW * secondMat.rowVecs[3].tfW;

	// Row four.
	resultVec->rowVecs[3].tfX = firstMat.rowVecs[3].tfX * secondMat.rowVecs[0].tfX + firstMat.rowVecs[3].tfY * secondMat.rowVecs[1].tfX +
		firstMat.rowVecs[3].tfZ * secondMat.rowVecs[2].tfX + firstMat.rowVecs[3].tfW * secondMat.rowVecs[3].tfX;
	resultVec->rowVecs[3].tfY = firstMat.rowVecs[3].tfX * secondMat.rowVecs[0].tfY + firstMat.rowVecs[3].tfY * secondMat.rowVecs[1].tfY +
		firstMat.rowVecs[3].tfZ * secondMat.rowVecs[2].tfY + firstMat.rowVecs[3].tfW * secondMat.rowVecs[3].tfY;
	resultVec->rowVecs[3].tfZ = firstMat.rowVecs[3].tfX * secondMat.rowVecs[0].tfZ + firstMat.rowVecs[3].tfY * secondMat.rowVecs[1].tfZ +
		firstMat.rowVecs[3].tfZ * secondMat.rowVecs[2].tfZ + firstMat.rowVecs[3].tfW * secondMat.rowVecs[3].tfZ;
	resultVec->rowVecs[3].tfW = firstMat.rowVecs[3].tfX * secondMat.rowVecs[0].tfW + firstMat.rowVecs[3].tfY * secondMat.rowVecs[1].tfW +
		firstMat.rowVecs[3].tfZ * secondMat.rowVecs[2].tfW + firstMat.rowVecs[3].tfW * secondMat.rowVecs[3].tfW;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name:	transposeMat4x4T()
//
//Purpose:		This prints a matrix.
//
//Last Modified:	8/6/2004
// 
//Input:		mat			- The matrix to print.
//
//Output:		None.
//
//Return:		None.
////////////////////////////////////////////////////////////////////////////////////////////////////////
void transposeMat4x4T(Matrix4x4T *resultMat, const Matrix4x4T &mat)
{
    // Row zero becomes column zero.
	resultMat->rowVecs[0].tfX = mat.rowVecs[0].tfX;
	resultMat->rowVecs[1].tfX = mat.rowVecs[0].tfY;
	resultMat->rowVecs[2].tfX = mat.rowVecs[0].tfZ;
	resultMat->rowVecs[3].tfX = mat.rowVecs[0].tfW;

	// Row one becomes column one.
	resultMat->rowVecs[0].tfY = mat.rowVecs[1].tfX;
	resultMat->rowVecs[1].tfY = mat.rowVecs[1].tfY;
	resultMat->rowVecs[2].tfY = mat.rowVecs[1].tfZ;
	resultMat->rowVecs[3].tfY = mat.rowVecs[1].tfW;

    // Row two becomes column two.
	resultMat->rowVecs[0].tfZ = mat.rowVecs[2].tfX;
	resultMat->rowVecs[1].tfZ = mat.rowVecs[2].tfY;
	resultMat->rowVecs[2].tfZ = mat.rowVecs[2].tfZ;
	resultMat->rowVecs[3].tfZ = mat.rowVecs[2].tfW;

    // Row three becomes column three.
	resultMat->rowVecs[0].tfW = mat.rowVecs[3].tfX;
	resultMat->rowVecs[1].tfW = mat.rowVecs[3].tfY;
	resultMat->rowVecs[2].tfW = mat.rowVecs[3].tfZ;
	resultMat->rowVecs[3].tfW = mat.rowVecs[3].tfW;
}

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
void rotXMat4x4T(Matrix4x4T *resultMat, const float fAng)
{
	// Row 0.
	resultMat->rowVecs[0].tfX = 1.0f;
	resultMat->rowVecs[0].tfY = 0.0f;
	resultMat->rowVecs[0].tfZ = 0.0f;
	resultMat->rowVecs[0].tfW = 0.0f;

	// Row 1.
	resultMat->rowVecs[1].tfX = 0.0f;
	resultMat->rowVecs[1].tfY = SPF_COS(fAng);
	resultMat->rowVecs[1].tfZ = -SPF_SIN(fAng);
	resultMat->rowVecs[1].tfW = 0.0f;

	// Row 2.
	resultMat->rowVecs[2].tfX = 0.0f;
	resultMat->rowVecs[2].tfY = -resultMat->rowVecs[1].tfZ;//SPF_SIN(fAng);
	resultMat->rowVecs[2].tfZ = resultMat->rowVecs[1].tfY; //SPF_COS(fAng);
	resultMat->rowVecs[2].tfW = 0.0f;

	// Row 3.
	resultMat->rowVecs[3].tfX = 0.0f;
	resultMat->rowVecs[3].tfY = 0.0f;
	resultMat->rowVecs[3].tfZ = 0.0f;
	resultMat->rowVecs[3].tfW = 1.0f;
}

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
void rotYMat4x4T(Matrix4x4T *resultMat, const float fAng)
{
	// Row 0.
	resultMat->rowVecs[0].tfX = SPF_COS(fAng);
	resultMat->rowVecs[0].tfY = 0.0f;
	resultMat->rowVecs[0].tfZ = SPF_SIN(fAng);
	resultMat->rowVecs[0].tfW = 0.0f;

	// Row 1.
	resultMat->rowVecs[1].tfX = 0.0f;
	resultMat->rowVecs[1].tfY = 1.0f;
	resultMat->rowVecs[1].tfZ = 0.0f;
	resultMat->rowVecs[1].tfW = 0.0f;

	// Row 2.
	resultMat->rowVecs[2].tfX = -resultMat->rowVecs[0].tfZ ; //-SPF_SIN(fAng);
	resultMat->rowVecs[2].tfY = 0.0f;
	resultMat->rowVecs[2].tfZ = resultMat->rowVecs[0].tfX; //SPF_COS(fAng);
	resultMat->rowVecs[2].tfW = 0.0f;

	// Row 3.
	resultMat->rowVecs[3].tfX = 0.0f;
	resultMat->rowVecs[3].tfY = 0.0f;
	resultMat->rowVecs[3].tfZ = 0.0f;
	resultMat->rowVecs[3].tfW = 1.0f;
}

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
void rotZMat4x4T(Matrix4x4T *resultMat, const float fAng)
{
	// Row 0.
	resultMat->rowVecs[0].tfX = SPF_COS(fAng);
	resultMat->rowVecs[0].tfY = -SPF_SIN(fAng);
	resultMat->rowVecs[0].tfZ = 0.0f;
	resultMat->rowVecs[0].tfW = 0.0f;

	// Row 1.
	resultMat->rowVecs[1].tfX = -resultMat->rowVecs[0].tfY; //SPF_SIN(fAng);
	resultMat->rowVecs[1].tfY = resultMat->rowVecs[0].tfX; //SPF_COS(fAng);
	resultMat->rowVecs[1].tfZ = 0.0f;
	resultMat->rowVecs[1].tfW = 0.0f;

	// Row 2.
	resultMat->rowVecs[2].tfX = 0.0f;
	resultMat->rowVecs[2].tfY = 0.0f;
	resultMat->rowVecs[2].tfZ = 1.0f;
	resultMat->rowVecs[2].tfW = 0.0f;

	// Row 3.
	resultMat->rowVecs[3].tfX = 0.0f;
	resultMat->rowVecs[3].tfY = 0.0f;
	resultMat->rowVecs[3].tfZ = 0.0f;
	resultMat->rowVecs[3].tfW = 1.0f;
}
