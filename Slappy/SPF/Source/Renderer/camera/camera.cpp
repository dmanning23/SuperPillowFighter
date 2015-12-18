//////////////////////////////////////////////////////////////////////////////////////////
// File Name:      camera.cpp
//
// Author:         Kevin Cecelski
//
// Date Created:   Tuesday, August 24, 2004
//
// Purpose:        This defines the camera class.
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "camera.h"

//////////////////////////////////////////////////////////////////////////////////////////
// Function:       
//
// Purpose:        
//
// Last Modified:  Tuesday, August 24, 2004
//
// Input:          
//
// Output:         
//
// Returns:        
//
//////////////////////////////////////////////////////////////////////////////////////////
Ccamera::Ccamera(void)
{
	m_outLog.Init("camera.txt");
	m_outLog << "Constructing the camera class.\n";
	m_CameraMatrix = identityMatrix;
	m_fWHRatio = 0.0f;
	m_fRcpTanFrust = 0.0f;
	m_bWinnerMode = false;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Function:       
//
// Purpose:        
//
// Last Modified:  Tuesday, August 24, 2004
//
// Input:          
//
// Output:         
//
// Returns:        
//
//////////////////////////////////////////////////////////////////////////////////////////
Ccamera::~Ccamera(void)
{
	m_outLog << "Destructing the camera class.\n";
	m_outLog.Shutdown();
}

//////////////////////////////////////////////////////////////////////////////////////////
// Function:       
//
// Purpose:        
//
// Last Modified:  Tuesday, August 24, 2004
//
// Input:          
//
// Output:         
//
// Returns:        
//
//////////////////////////////////////////////////////////////////////////////////////////
void Ccamera::updateCamera()
{
	Vec4DT temp;

	if(m_bWinnerMode)
		return;

	LERPVec4DT(&temp, m_startPos, m_endPos, (1.0f-(m_swLERP.GetRemainingTime()/m_fSampRate)));

	if(m_swLERP.GetRemainingTime() < 0.0f)
	{
		// Time is up, pick a new point to move the camera to.
		m_startPos = m_endPos;
        computeCameraPos(m_endPos);
		
		// Cap the change in posisition of the camera.
		float fSize = distVec4DT(m_startPos, m_endPos);
		if(fSize > MAX_CHANGE)
		{
			// Find a value between the desired location and the 
			// last location of the camera.
			fSize *= MAX_CHANGE_ADJUST;

			// Find the vector between the start and end positions.
			Vec4DT temp;
			temp.tfX = m_endPos.tfX - m_startPos.tfX;
			temp.tfY = m_endPos.tfY - m_startPos.tfY;
			temp.tfZ = m_endPos.tfZ - m_startPos.tfZ;

			// Find an acceptable position.  
			normalizeVec4DT(&temp, temp);
            sMultVec4DT(&temp, fSize, temp);
			addVec4DT(&m_endPos, temp, m_startPos);
		}

		// Re-Start the interpolation time.
		m_swLERP.Start(m_fSampRate);
	}

	// Set the position of the camera.
	m_CameraMatrix.rowVecs[3].tfX = temp.tfX;
	m_CameraMatrix.rowVecs[3].tfY = temp.tfY;
	m_CameraMatrix.rowVecs[3].tfZ = temp.tfZ;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Function:       
//
// Purpose:        
//
// Last Modified:  Wednesday, August 25, 2004
//
// Input:          
//
// Output:         
//
// Returns:        
//
//////////////////////////////////////////////////////////////////////////////////////////
void Ccamera::setCameraAttributes(int iVwptWidth, int iVwptHeight, float fFov, float camerAng)
{
	m_fWHRatio	= (float)iVwptHeight/iVwptWidth;
	m_fRcpTanFrust = 1.0f/SPF_TAN(fFov/2.0f);
	m_fSampRate = 0.15f;
	m_startPos.tfX = m_endPos.tfX  = 0.0f;
	m_startPos.tfY = m_endPos.tfX  = 0.0f;
	m_startPos.tfZ = m_endPos.tfX  = -60.0f;
	m_CameraAngle = camerAng;
	m_fViewFrustAdjust = SPF_SIN(camerAng);//1.0f/SPF_TAN(camerAng/2.0f);
	
	// Find the direction of the camera's vector.
	NEW_VEC4D(lookDir);

	// Get the direction of the look.
	lookDir.tfY = SPF_SIN(m_CameraAngle);
	lookDir.tfZ = -SPF_COS(m_CameraAngle);

	// Make the matrix.
	makeViewMatrix(lookDir, lookDir);

	// Get the limits on the camera movement.
	m_CameraAngle = CLevelManager::GetInstance()->m_fcameraAngle;
	m_MaxCameraBotHeight = CLevelManager::GetInstance()->m_MaxCameraBotHeight;
	m_CameraZoomCap = CLevelManager::GetInstance()->m_CameraZoomCap;
	m_CameraZoomOutCap = CLevelManager::GetInstance()->m_CameraZoomOutCap;

	m_fFVO = fFov;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Function:       
//
// Purpose:        
//
// Last Modified:  Wednesday, August 25, 2004
//
// Input:          
//
// Output:         
//
// Returns:        
//
//////////////////////////////////////////////////////////////////////////////////////////
void Ccamera::computeCameraPos(Vec4DT &outVec)
{
	CBaseObject *p;
	int ii;
	float fySize, fxSize;
    float fyPoint, fxPoint;
	float fDist, fTempDist;

	if(m_bWinnerMode)
		return;

	// Initial values should be positions that the players will never occupy.
	float fLeft = FAR_RIGHT, fRight = FAR_LEFT, fTop = FAR_BOTTOM, fBot = FAR_TOP;

	// Find the extents of the view.
	for(ii = 0; ii < CObjectManager::GetInstance()->m_iPlayerArraySize ; ii++)
	{
		// Get the player.
		p = CObjectManager::GetInstance()->m_PlayerArray[ii];
		
		// Is this a player?
		if(!p)
			continue;

		fDist = (((float *)p->CollisionData))[2];
		fTempDist = (((float *)p->CollisionData))[0];

		// Check against the current farthest left component.
		if(p->m_PandO.rowVecs[3].tfX-fDist< fLeft)
			fLeft = p->m_PandO.rowVecs[3].tfX-fDist;

		// Check against the current farthest right component.
		if(p->m_PandO.rowVecs[3].tfX+fDist > fRight)
			fRight = p->m_PandO.rowVecs[3].tfX+fDist;

		// Check against the current farthest top component.
		if(p->m_PandO.rowVecs[3].tfY+fDist+fTempDist > fTop)
			fTop = p->m_PandO.rowVecs[3].tfY+fDist+fTempDist;

		// Check against the current farthest bottom component.
		if(p->m_PandO.rowVecs[3].tfY < (fBot))
			fBot = p->m_PandO.rowVecs[3].tfY;
	}

	// Do not go outside the boundaries of the level.
	CLevelManager *pLevel = CLevelManager::GetInstance();
	if(fLeft < pLevel->m_fBoundaries[LEFT_BOUND])
		fLeft = pLevel->m_fBoundaries[LEFT_BOUND];
	if(fRight > pLevel->m_fBoundaries[RIGHT_BOUND])
		fRight = pLevel->m_fBoundaries[RIGHT_BOUND];
	if(fTop > pLevel->m_fBoundaries[TOP_BOUND])
		fTop = pLevel->m_fBoundaries[TOP_BOUND];
	if(fBot < pLevel->m_fBoundaries[BOTTOM_BOUND])
		fBot = pLevel->m_fBoundaries[BOTTOM_BOUND];

	// Do not let the camera go too high!
	if(fBot > m_MaxCameraBotHeight)
		fBot = m_MaxCameraBotHeight;

	// Find the point on the xy plane where the camera shall be placed.
	fySize = fabsf((fTop-fBot)/2.0f);
	fxSize = fabsf((fRight-fLeft)/2.0f);
    fyPoint = fBot + fySize;
	fxPoint = fLeft + fxSize;
	fDist = INF_DIST;
    
	// Find the z position required to view the entire height.
	fTempDist = ((fySize*m_fRcpTanFrust*m_fViewFrustAdjust)+ fySize)*m_fRcpTanFrust;
	fDist = (fxSize+(fySize*m_fRcpTanFrust*m_fViewFrustAdjust))*m_fRcpTanFrust*m_fWHRatio;

	// Set the vector.
	outVec.tfX = -fxPoint;
	outVec.tfY = -fyPoint;
	outVec.tfZ = ((fTempDist>fDist)?(-fTempDist):(-fDist));

	// Find the length a side of the view frustrum.
	float fLen = (-outVec.tfZ)/SPF_COS(m_fFVO/2.0f);
	float fX = (SPF_SIN(m_fFVO/2.0f) * fLen);

	// Make sure the camera never shows the level beyond its limits.
	if(-outVec.tfX + fX > pLevel->m_fBoundaries[RIGHT_BOUND]-CAMERA_BUFFER)
		outVec.tfX  = -(pLevel->m_fBoundaries[RIGHT_BOUND]-CAMERA_BUFFER-fX);

	if(-outVec.tfX - fX < pLevel->m_fBoundaries[LEFT_BOUND]+CAMERA_BUFFER)
		outVec.tfX  = -(pLevel->m_fBoundaries[LEFT_BOUND]+CAMERA_BUFFER+fX);

	fX = (SPF_SIN(m_fFVO/2.0f) * fLen);

	// Make sure the camera never shows the level beyond its limits.
	if(-outVec.tfY + fX > pLevel->m_fBoundaries[TOP_BOUND]-CAMERA_BUFFER)
		outVec.tfY  = -(pLevel->m_fBoundaries[TOP_BOUND]-CAMERA_BUFFER-fX);

	if(-outVec.tfY - fX < pLevel->m_fBoundaries[BOTTOM_BOUND]+CAMERA_BUFFER)
		outVec.tfY  = -(pLevel->m_fBoundaries[BOTTOM_BOUND]+CAMERA_BUFFER+fX);

	//// Cap how close the camera will be to the player.
	if(outVec.tfZ > m_CameraZoomCap)
		outVec.tfZ = m_CameraZoomCap;

	// And how far it will zoom out.
	if(outVec.tfZ < m_CameraZoomOutCap)
		outVec.tfZ = m_CameraZoomOutCap;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Function:	resetCamera()
//	
// Purpose:		resets the camera.
//
// Last Modified:  Saturday, September 18, 2004
//
// Input:		None.
//
// Output:		None.
//
// Returns:		None.
//
//////////////////////////////////////////////////////////////////////////////////////////
void Ccamera::resetCamera()
{
	// No longer in winner mode.
	m_bWinnerMode = false;

	// Compute the camera based on the player position.
	computeCameraPos(m_endPos);
	m_startPos = m_endPos;

	// Find the direction of the camera's vector.
	NEW_VEC4D(lookDir);

	// Get the direction of the look.
	lookDir.tfY = SPF_SIN(m_CameraAngle);
	lookDir.tfZ = -SPF_COS(m_CameraAngle);

	makeViewMatrix(lookDir, m_endPos);

	// Reset the camera timer.
	m_swLERP.Start(m_fSampRate);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Function:	makeViewMatrix()
//	
// Purpose:		make a view matrix from the given look at vector.
//
// Last Modified:  Saturday, September 18, 2004
//
// Input:		lookAtVec		- The look at vector used to make the camera matrix.
//				posOfCamera		- The position of the vector.
//
// Output:		None.
//
// Returns:		None.
//
//////////////////////////////////////////////////////////////////////////////////////////
void Ccamera::makeViewMatrix(Vec4DT &lookAtVec, Vec4DT &posOfCamera)
{
    Vec4DT rightVec, upVec;

	// Set the look at vector for the camera.
	normalizeVec4DT(&lookAtVec, lookAtVec);
	setLookAtVec4DT(lookAtVec, m_CameraMatrix);

	// Find the right vector by crossing the look at vector with world's y up vector.
	crossVec4DT(&rightVec, lookAtVec, yVector);
	normalizeVec4DT(&rightVec, rightVec);		// Unit length vectors only.
	setRightVec4DT(rightVec, m_CameraMatrix);

	// Find the up vector by crossing the right vector  with the look at vector.
	crossVec4DT(&upVec, rightVec, lookAtVec);
	normalizeVec4DT(&upVec, upVec);				// Unit length vectors only.
    setUpVec4DT(upVec, m_CameraMatrix);

	// Set the position of the camera.
	setPosVec4DT(posOfCamera, m_CameraMatrix);

	// Adjust for opengl's crazy coordinate system.
	m_CameraMatrix.fElements[8] = -m_CameraMatrix.fElements[8];
	m_CameraMatrix.fElements[9] = -m_CameraMatrix.fElements[9];
	m_CameraMatrix.fElements[10] = -m_CameraMatrix.fElements[10];
}

void Ccamera::applyCamera()
{
	glMultMatrixf(m_CameraMatrix.fElements);
}

void Ccamera::goToWinnerMode(int iWinnerIndex)
{
	m_bWinnerMode = true;
	getPosVec4DT(&m_endPos,	CObjectManager::GetInstance()->m_PlayerArray[iWinnerIndex]->m_PandO);
	m_endPos.tfZ += DISTANCE_FROM_WINNER;
	m_endPos.tfY += ((float *)(CObjectManager::GetInstance()->m_PlayerArray[iWinnerIndex]->CollisionData))[2];
    
	// Re-Start the interpolation time.
	m_swLERP.Start(m_fSampRate);

	// Set the position of the camera.
	m_CameraMatrix.rowVecs[3].tfX = -m_endPos.tfX;
	m_CameraMatrix.rowVecs[3].tfY = -m_endPos.tfY;
	m_CameraMatrix.rowVecs[3].tfZ = -m_endPos.tfZ;
}