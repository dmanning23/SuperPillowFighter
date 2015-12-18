//////////////////////////////////////////////////////////////////////////////////////////
// File Name:      camera.h
//
// Author:         Kevin Cecelski
//
// Date Created:   Tuesday, August 24, 2004
//
// Purpose:        This declares the camera class.
//
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef _CAMERA_H_
#define _CAMERA_H_
#include "../../MathLibrary/spfmath.h"
#include "../../ObjectManager/ObjectManager.h"
#include "../../Logger/Logger.h"
#include "../../GameTimer/GameTimer.h"
#include "../../GameTimer/StopWatch.h"
#include "../../LevelManager/LevelManager.h"

#define FAR_RIGHT			100.0f
#define FAR_LEFT			-100.0f
#define FAR_TOP				100.0f
#define FAR_BOTTOM			-100.0f
#define INF_DIST			1000000.0f
#define MAX_CHANGE			4.5f
#define MAX_CHANGE_ADJUST	0.25f
#define CAMERA_BUFFER		4.0f
#define DISTANCE_FROM_WINNER	10.0f

class Ccamera
{
private:
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
	void computeCameraPos(Vec4DT &outVec);

public:
	// The matrix for the camera.
	Matrix4x4T	m_CameraMatrix;

	// Used to log this class.
	CLogger m_outLog;

	// Reciprocal of the tangent of half of the view frustrum's angle.
	float m_fRcpTanFrust;

	// Adjustment for the angle of the view frustrum.
	float m_fViewFrustAdjust;

	// Ratio of the width and height of the viewport.
	float m_fWHRatio;

    // The rate at which we will adjust the camera.
	float m_fSampRate;

	// The start position of the camera in our current LERP.
	Vec4DT m_startPos;

	// The end position of the camera in our current LERP.
	Vec4DT m_endPos;

	// A stop watch to use during interpolation.
	CStopWatch m_swLERP;

	// Camera angle.
	float m_CameraAngle;

	// Camera limitations.
	float m_MaxCameraBotHeight;
	float m_CameraZoomCap, m_CameraZoomOutCap;

	// Field of view for the camera.
	float m_fFVO;

	// Are we zooming in on the winner?
	bool m_bWinnerMode;

	//////////////////////////////////////////////////////////////////////////////////////////
	// Function:       Ccamera
	//
	// Purpose:        This is the default constructor.
	//
	// Last Modified:  Tuesday, August 24, 2004
	//
	// Input:          None.
	//
	// Output:         None.
	//
	// Returns:        None.
	//
	//////////////////////////////////////////////////////////////////////////////////////////
	Ccamera(void);
	
	//////////////////////////////////////////////////////////////////////////////////////////
	// Function:       ~Ccamera
	//
	// Purpose:        This is the destructor.
	//
	// Last Modified:  Tuesday, August 24, 2004
	//
	// Input:          None.
	//
	// Output:         None.
	//
	// Returns:        None.
	//
	//////////////////////////////////////////////////////////////////////////////////////////
	~Ccamera(void);

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
	void setCameraAttributes(int iVwptWidth, int iVwptHeight, float fFov, float camerAng);

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
	void updateCamera();

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
	void resetCamera();

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
	void makeViewMatrix(Vec4DT &lookAtVec, Vec4DT &posOfCamera);

	void applyCamera();

	void goToWinnerMode(int iWinnerIndex);

};

#endif