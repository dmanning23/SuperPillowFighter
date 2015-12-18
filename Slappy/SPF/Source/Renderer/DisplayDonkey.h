//////////////////////////////////////////////////////////////////////////////////////////
//File Name:		displayDonkey.h
// 
//Author:			Kevin Cecelski
//
//Date Created:		8/16/2004
//
//Purpose:			This defines the display donkey interface.
//
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef _DISPLAY_DONKEY_H_
#define _DISPLAY_DONKEY_H_

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "../MemManager/memmanager.h"
#include "../logger/logger.h"
#include "../Mathlibrary/spfMath.h"
#include "geometryTypes.h"
#include "../bitmapmanager/bitmapmanager.h"
#include "../ObjectManager/ObjectManager.h"
#include "camera/camera.h"
#include "../Profiler/profiler.h"
#include "../ParticleEngine/ParticleEngine.h"
#include "../HUd/hud.h"
#include "DonkeyExt/DonkeyExt.h"
#include "RenderTarget/RenderTarget.h"
#include "Effects/DonkEffects.h"
#include "Effects/rndtotexture.h"

#define CIRC_RES		15
#define PARTICLE_SIZE	pCurEmit->tParticles[ii].fSize
#define HALF_PART_SIZE	PARTICLE_SIZE/2.0f
#define FOV				45.0f
#define FAR_CLIP_PLANE	500.0f
#define NEAR_CLIP_PLANE	0.1f
#define CAMERA_ANGLE	SPF_PI/12.0f

class CDisplayDonkey
{
private:
	// The display modes available.
	enum {
		PERSPECTIVE_DONK = 0x01,	// Perspective mode.
		ORTHOGRAPHIC_DONK = 0x02,	// Orthographic mode.
	};

	// Enumerations for using the display lists.
	enum { CIRCLE_DISP = 0, NUM_DISPS };

	CRndToTexture m_pop;

	// The only instance of the donkey.
	static CDisplayDonkey *m_pInstance;

	// Are we initialized?
	bool m_bInitialized;

	// A sorted list of geometry to draw.
	GeometryT **m_pDisplayList;

	// Tracks which display mode we are in.
	unsigned char m_ucDisplayMode;

	// Width and height of the window.
	int m_iHeight, m_iWidth;

	// A handle to the window that the donkey is using for rendering.
	HWND m_hWnd;

	// Is the buffer locked for rendering?
	bool m_bBufferLocked;

	// The log file for this manager.
	CLogger m_outLog;   

	// The camera used to view the world.
	Ccamera	m_theCamera;

	// Display lists for faster mode switches and rendering.
	GLuint m_dispLists[NUM_DISPS];

	// This is used for neat-o effects!
    CDonkEffects	m_effects;

	// ** Overide functions that singletons should not expose. **
		// Copy constructor.
		CDisplayDonkey(const CDisplayDonkey &cbm) {}	
		// Assignment operator.
		CDisplayDonkey &operator=(const CDisplayDonkey &cbm) {}
		// Default constructor.
		CDisplayDonkey(void);
		// **********************************************************

	//////////////////////////////////////////////////////////////////////////////////////////
	// Function:       drawCircle()
	//
	// Purpose:        This function draws a circle.
	//
	// Last Modified:  Friday, August 27, 2004
	//
	// Input:          iRes		- The resolution of the circle(Number of points on the 
	//								circle to use to connect the lines forming the circle).
	//
	// Output:         None.
	//
	// Returns:        None.
	//
	//////////////////////////////////////////////////////////////////////////////////////////
	void drawCircle(int iRes);

	//////////////////////////////////////////////////////////////////////////////////////////
	// Function:       drawDebug()
	//
	// Purpose:        Draws debug information for an object.
	//
	// Last Modified:  Friday, August 27, 2004
	//
	// Input:          pBo		- The object that will have its debug information drawn.
	//
	// Output:         None.
	//
	// Returns:        None.
	//
	//////////////////////////////////////////////////////////////////////////////////////////
	void drawDebug(CBaseObject *pBO);

	//////////////////////////////////////////////////////////////////////////////////////////
	// Function:       drawParticles()
	//
	// Purpose:        draws all the particles in the particle system.
	//
	// Last Modified:  Friday, August 27, 2004
	//
	// Input:          None.
	//
	// Output:         None.
	//
	// Returns:        None.
	//
	//////////////////////////////////////////////////////////////////////////////////////////
	void drawParticles();

	//////////////////////////////////////////////////////////////////////////////////////////
	// Function:	drawGameObjects()
	//
	// Purpose:		This draws all the objects in play.(excluding effects)
	//
	// Last Modified:  Monday, September 13, 2004
	//
	// Input:		None.
	//
	// Output:		None.
	//
	// Returns:		None.
	//
	//////////////////////////////////////////////////////////////////////////////////////////
	void drawGameObjects();

public:

	// A rendering target for drawing to the screen.
	CRenderTarget m_rndTarg;

	//////////////////////////////////////////////////////////////////////////////////////////
	// Function:	~CDisplayDonkey()  
	//
	// Purpose:		Default Destructor
	//
	// Last Modified:  Friday, August 27, 2004
	//
	// Input:		None.
	//
	// Output:		None.
	//
	// Returns:		None.
	//
	//////////////////////////////////////////////////////////////////////////////////////////
	~CDisplayDonkey(void);	

	//////////////////////////////////////////
	// Function: GetInstance()
	//
	// Purpose:	 Returns a pointer to the only instance of this class.
	//
	// Last Modified: 8/16/2004
	//
	// Input: None.
	//
	// Output: None. 
	//
	// Returns: A pointer to the only instance of this class.
	//
	//////////////////////////////////////////
	static CDisplayDonkey *GetInstance()
	{
		if(!m_pInstance)
		{
			m_pInstance = (CDisplayDonkey *)MemMalloc(sizeof(CDisplayDonkey));	// Make a new instance.
			m_pInstance->CDisplayDonkey::CDisplayDonkey();
		}

		// Give 'em that pointer.
        return m_pInstance;
	}

	//////////////////////////////////////////
	// Function: DeleteInstance()
	//
	// Purpose:	 Deletes the only instance of this class.
	//
	// Last Modified: 8/16/2004
	//
	// Input: None.
	//
	// Output: None. 
	//
	// Returns: None.
	//
	//////////////////////////////////////////
	static void DeleteInstance()
	{
		if(m_pInstance)
		{
			// Get rid of this instance.
			m_pInstance->CDisplayDonkey::~CDisplayDonkey();
			MemFree(m_pInstance);
			m_pInstance = 0;
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	// Function:	initialize()      
	//
	// Purpose:		This initializea the donkey.
	//
	// Last Modified:  Tuesday, August 24, 2004
	//
	// Input:		hWnd		- the window used during rendering.
	//				iWidth		- the width of the given window.
	//				iHeight		- the height of the given window.
	//
	// Output:		None.
	//
	// Returns:		true if successful, false otherwise.
	//
	//////////////////////////////////////////////////////////////////////////////////////////
	bool initialize(HWND hWnd, int iWidth, int iHeight);

	//////////////////////////////////////////
	// Function: shutdown()
	//
	// Purpose:	Shuts down the display donkey.
	//
	// Last Modified: 8/17/2004
	//
	// Input: None.
	//
	// Output: None. 
	//
	// Returns: None.
	//
	////////////////////////////////////////
	void shutdown();

	//////////////////////////////////////////////////////////////////////////////////////////
	// Function:       setOrthographic()
	//
	// Purpose:        This switches the rendering mode.
	//
	// Last Modified:  Tuesday, August 24, 2004
	//
	// Input:          bOrtho		- What mode should we use?
	//								  true - orthographic mode.
	//								  false - perspective mode.
	//
	// Output:         None.
	//
	// Returns:        None.
	//
	//////////////////////////////////////////////////////////////////////////////////////////
	void setOrthographic(bool bOrtho);

	//////////////////////////////////////////////////////////////////////////////////////////
	// Function:       changeResolution()
	//
	// Purpose:        This adjusts the donkey to work in a different resolution.
	//
	// Last Modified:  Tuesday, August 24, 2004
	//
	// Input:          iNewWidth	- the new width of the screen.
	//				   iNewHeight	- the new height of the screen.
	//
	// Output:         None.
	//
	// Returns:        None.
	//
	//////////////////////////////////////////////////////////////////////////////////////////
	void changeResolution(int iNewWidth, int iNewHeight);

	//////////////////////////////////////////////////////////////////////////////////////////
	// Function:       drawWorld()
	//
	// Purpose:        This draws the 3d objects in the world.
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
	void drawWorld();

	//////////////////////////////////////////////////////////////////////////////////////////
	// Function:       Render()
	//
	// Purpose:        This swaps the buffers, displays the image drawn and
	//					clears the backbuffer.
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
	void Render();

	//////////////////////////////////////////////////////////////////////////////////////////
	// Function:       getCameraPos() 
	//
	// Purpose:        This gives back the position of the camera.
	//
	// Last Modified:  Friday, August 27, 2004
	//
	// Input:          None.
	//
	// Output:         None.
	//
	// Returns:        The position of the vector.
	//
	//////////////////////////////////////////////////////////////////////////////////////////
	Vec4DT getCameraPos() 
	{	
		Vec4DT retVec;

		// Get the components of the camera's position vector.
		retVec.tfX = m_theCamera.m_CameraMatrix.rowVecs[3].tfX;
		retVec.tfY = m_theCamera.m_CameraMatrix.rowVecs[3].tfY;
		retVec.tfZ = m_theCamera.m_CameraMatrix.rowVecs[3].tfZ;

		return retVec;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	// Function:	initGLStates()
	//
	// Purpose:		This initializes the GL state machine to match our rendering needs.
	//
	// Last Modified:  Monday, September 13, 2004
	//
	// Input:		None.
	//
	// Output:		None.
	//
	// Returns:		None.
	//
	//////////////////////////////////////////////////////////////////////////////////////////
	void initGLStates();

	//////////////////////////////////////////////////////////////////////////////////////////
	// Function:	initLight()
	//
	// Purpose:		This initializes the lighting environment.
	//
	// Last Modified:  Monday, September 13, 2004
	//
	// Input:		None.
	//
	// Output:		None.
	//	
	// Returns:		None.
	//
	//////////////////////////////////////////////////////////////////////////////////////////
	void initLight();

	//////////////////////////////////////////////////////////////////////////////////////////
	// Function:	setLight()
	//
	// Purpose:		This sets up the light for our environment.
	//
	// Last Modified:  Monday, September 13, 2004
	//
	// Input:		None.
	//
	// Output:		None.
	//
	// Returns:		None.
	//
	//////////////////////////////////////////////////////////////////////////////////////////
	void setLight();

	//////////////////////////////////////////////////////////////////////////////////////////
	// Function:	makeFrustrum()
	//
	// Purpose:		This creates and sets the viewing frustrum used when rendering 
	//				in perspective mode.
	//
	// Last Modified:  Monday, September 13, 2004
	//
	// Input:		None.
	//
	// Output:		None
	//
	// Returns:		None.
	//
	//////////////////////////////////////////////////////////////////////////////////////////
	void makeFrustrum()
	{
		gluPerspective(FOV, ((float)m_iWidth)/m_iHeight, NEAR_CLIP_PLANE, FAR_CLIP_PLANE);
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	// Function:	newGame()
	//
	// Purpose:		This resets any of the data in the donkey for a new game.
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
	void newGame();

	void DrawEnding(int iWinnerIndex);
};

#endif