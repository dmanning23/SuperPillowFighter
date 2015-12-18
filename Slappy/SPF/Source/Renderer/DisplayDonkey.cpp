//////////////////////////////////////////////////////////////////////////////////////////
//File Name:		displayDonkey.cpp
// 
//Author:			Kevin Cecelski
//
//Date Created:		8/16/2004
//
//Purpose:			This implements the display donkey interface.
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "displaydonkey.h"
#include "../GameManager/gamemanager.h"
#include "../Physics/CollisionDetector.h"
#include "VertBufCtrl/VertBufCtrl.h"

// Initialize the only pointer to this class. 
CDisplayDonkey *CDisplayDonkey::m_pInstance = 0;

//////////////////////////////////////////////////////////////////////////////////////////
// Function:       CDisplayDonkey()
//
// Purpose:        Default constructor.
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
CDisplayDonkey::CDisplayDonkey(void)
{
	// Log the construction of the display donkey.
	m_outLog.Init("DisplayDonkey.txt");
	m_outLog << "Constructing the display donkey... \n";

	m_bInitialized = false;		// Not yet initialized.
    m_pDisplayList = 0;			// Nothing in the display list.
	m_ucDisplayMode = ORTHOGRAPHIC_DONK;	// Start in orthographic mode.
	m_iHeight = m_iWidth = 0;		// No window defined yet...
	m_hWnd = 0;						// No window to use.
	m_bBufferLocked = false;		// Buffer is not locked.
}

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
CDisplayDonkey::~CDisplayDonkey(void)
{
	m_outLog << "Destroying the display donkey... \n";
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
bool CDisplayDonkey::initialize(HWND hWnd, int iWidth, int iHeight)
{
	m_outLog << "Initializing the display donkey... \n";
	m_iWidth = iWidth;
	m_iHeight = iHeight;

	// Make the screen a rendering target.
	m_rndTarg.create(hWnd, 32, 16);
	m_rndTarg.becomeCurrent();

	// Set GL states.
	initGLStates();

	// Set up light states.
	initLight();

	// Position and setup the light.
	setLight();

	// Set-up the camera.
	m_theCamera.setCameraAttributes(iWidth, iHeight, FOV*SPF_DEG_TO_RAD, CLevelManager::GetInstance()->m_fcameraAngle);

	// Make display lists for drawing debug objects.
	// Circle display list.
	m_dispLists[CIRCLE_DISP] = glGenLists(1);
	glNewList(m_dispLists[CIRCLE_DISP], GL_COMPILE);
		// Draw the circle.
		drawCircle(CIRC_RES);
	glEndList();

	// Load extensions.
	//loadExtensions(m_rndTarg.m_hDC);

	// Start in orthographic mode.
	setOrthographic(true);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//m_pop.createRT(hWnd, 128, 128);

	// Initialize the vertex buffer controller.
	//CVertBufCtrl::GetInstance()->Initialize();

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Function:       shutdown()
//
// Purpose:        Shuts down the display donkey.
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
void CDisplayDonkey::shutdown()
{
	// Release the vertex buffers.
	//CVertBufCtrl::GetInstance()->Shutdown();
	//CVertBufCtrl::GetInstance()->DeleteInstance();

	m_outLog << "Shutting down the display donkey... \n";
	
	// Destroy the rendering target.
	m_rndTarg.destroy(m_hWnd);
}

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
void CDisplayDonkey::Render()
{
	// Swap the buffers.
	SwapBuffers(m_rndTarg.m_hDC);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// The buffer is now unlocked.
	m_bBufferLocked = false;
}

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
void CDisplayDonkey::changeResolution(int iNewWidth, int iNewHeight)
{
	// If the buffer is locked, no resolution change is allowed.
	if(m_bBufferLocked)
		return;

	// We have a new width and height!
	m_iWidth = iNewWidth;
	m_iHeight = iNewHeight;

	// Map the OpenGL coordinates.
	glViewport(0, 0, iNewWidth, iNewHeight);

	// Change matrix mode to alter the 
	// type of projection.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if(m_ucDisplayMode == ORTHOGRAPHIC_DONK)
		// Set the orthographic coordinate system to
		// be the same as texture space.
		gluOrtho2D(0.0f, 1.0f, 1.0f, 0.0f);
	else
		// Create a standard view frustrum.
		makeFrustrum();

	// Go back to model/view matrix mode.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();   

	// Reconfigure the camera.
	m_theCamera.setCameraAttributes(iNewWidth, iNewHeight, FOV*SPF_DEG_TO_RAD, CLevelManager::GetInstance()->m_fcameraAngle);
}

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
void CDisplayDonkey::drawWorld()
{
	// For grabbing the world objects.
	CObjectManager *pObjects = CObjectManager::GetInstance();

	// If the buffer is locked, do not draw anything.
	if(m_bBufferLocked)
		return;

	/*if(!pObjects)
		pObjects = CObjectManager::GetInstance();*/

	// If the donkey is in orthographic, do not draw anything.
	if(m_ucDisplayMode == ORTHOGRAPHIC_DONK)
		return;

	//// Draw the background for this level.
	//m_effects.drawBackground();


	// Apply transforms to the world now.
	glPushMatrix();

		// Update and apply the camera's transformation.
		StartProfiler(CCamera_profiler);
		m_theCamera.updateCamera();
		StartPreviousProfiler();
		//glMultMatrixf(m_theCamera.m_CameraMatrix.fElements);
		m_theCamera.applyCamera();

		// Draw the background for this level.
		m_effects.drawBackground();

		// Draw the entire scene.
		drawGameObjects();
		m_effects.drawWater();		
		drawParticles();

		// Draw the world's boundaries if requested.
		if(CGameManager::GetInstance()->GameDebugInfo.bCollisionData)
			drawDebug(0);

	glPopMatrix();

	// The buffer is now locked.
	m_bBufferLocked = true;
}

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
void CDisplayDonkey::setOrthographic(bool bOrtho)
{
	// Change matrix mode to alter the 
	// type of projection.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Map the OpenGL coordinates.
	glViewport(0, 0, m_iWidth, m_iHeight);

	// Which mode do they want?
	if(bOrtho)
	{
		// Orthographic mode.
		m_ucDisplayMode = ORTHOGRAPHIC_DONK;

		// No lighting or depth testing.
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);

		// Set the orthographic coordinate system to
		// be the same as texture space.
		gluOrtho2D(0.0f, 1.0f, 1.0f, 0.0f);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		
		// Make all textures white.
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else
	{
		// Perspective mode.
		m_ucDisplayMode = PERSPECTIVE_DONK;

		// We need lighting and depth testing.
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);	

		// Create a standard view frustrum.
		makeFrustrum();
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	}

	// Go back to model/view matrix mode.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();   
}

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
void CDisplayDonkey::drawCircle(int iRes)
{
	// How are we going to walk around the circle?
	float fStep = SPF_TWO_PI/iRes;

	// Opengl, Begin!
	glBegin(GL_LINE_LOOP);

	// Walk around the circle once!
	for(float i = 0; i < SPF_TWO_PI; i+=fStep)
        glVertex3f(SPF_COS(i), SPF_SIN(i), 0.0f);        

	glEnd();	// Enough already.
}

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
void CDisplayDonkey::drawDebug(CBaseObject *pBO)
{
	// Set up the correct environment for drawing debug information.
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);   
	glLineWidth(1.5f);

	// Draw board's boundaries.
	if(!pBO)
	{
		CLevelManager *pLevel = CLevelManager::GetInstance();
		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
		glBegin(GL_LINES);

			// Left.
			glVertex3f( pLevel->m_fBoundaries[LEFT_BOUND], 
				pLevel->m_fBoundaries[TOP_BOUND], 0.0f);
			glVertex3f( pLevel->m_fBoundaries[LEFT_BOUND], 
				pLevel->m_fBoundaries[BOTTOM_BOUND], 0.0f);

			// Right.
			glVertex3f( pLevel->m_fBoundaries[RIGHT_BOUND],
				pLevel->m_fBoundaries[BOTTOM_BOUND], 0.0f);
			glVertex3f( pLevel->m_fBoundaries[RIGHT_BOUND], 
				pLevel->m_fBoundaries[TOP_BOUND], 0.0f);

			// Top.
			glVertex3f( pLevel->m_fBoundaries[RIGHT_BOUND], 
				pLevel->m_fBoundaries[TOP_BOUND], 0.0f);
			glVertex3f( pLevel->m_fBoundaries[LEFT_BOUND],
				pLevel->m_fBoundaries[TOP_BOUND], 0.0f);
					
			// Bottom.
			glVertex3f( pLevel->m_fBoundaries[LEFT_BOUND],
				pLevel->m_fBoundaries[BOTTOM_BOUND], 0.0f);
			glVertex3f( pLevel->m_fBoundaries[RIGHT_BOUND],
				pLevel->m_fBoundaries[BOTTOM_BOUND], 0.0f);
	
		glEnd();

		// Back to the regular attributes.
    	glPopAttrib();
		return;
	}

	// Draw the circle.
	// Find out what type of object we are debugging.
	switch(pBO->m_iType)
	{
		case Player_obj:
		{
			glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
			float *pHeadData = ((float *)pBO->CollisionData);

			// Draw the player's collision circles.
			glPushMatrix();
				// Draw the entire circle.
				glTranslatef(pBO->m_PandO.fElements[12], pBO->m_PandO.fElements[13]+pHeadData[0], 0.0f); 
				glScalef(pHeadData[2], pHeadData[2], pHeadData[2]);
				glCallList(m_dispLists[CIRCLE_DISP]);
			glPopMatrix();
			glPushMatrix();
				// Draw the center circle.
				glTranslatef(pBO->m_PandO.fElements[12], pBO->m_PandO.fElements[13]+pHeadData[0], 0.0f); 
				glScalef(pHeadData[1], pHeadData[1], pHeadData[1]);
				glCallList(m_dispLists[CIRCLE_DISP]);
			glPopMatrix();
			glPushMatrix();
				// Draw the top circle.
				glTranslatef(pBO->m_PandO.fElements[12], pBO->m_PandO.fElements[13]+pHeadData[3], 0.0f); 
   				glScalef(pHeadData[4], pHeadData[4], pHeadData[4]);
				glCallList(m_dispLists[CIRCLE_DISP]);
			glPopMatrix();
			glPushMatrix();
				// Draw the bottom circle.
				glTranslatef(pBO->m_PandO.fElements[12], pBO->m_PandO.fElements[13]+pHeadData[5], 0.0f); 
   				glScalef(pHeadData[6], pHeadData[6], pHeadData[6]);
				glCallList(m_dispLists[CIRCLE_DISP]);
			glPopMatrix();

			//// Get the player's animation data.
			//AnimationDataT *pAnim = ((CPlayer *)pBO)->m_pAnimationData;
			//glPushMatrix();	
			//	// Draw the player's bone structure.
			//	glMultMatrixf(pBO->m_PandO.fElements);
			//	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
			//	for(int i = 0; i < pAnim->tpAnimations[pAnim->tiPrevState].tiJointCount; i++)
			//	{
			//		// Get the the position of the parent joint.
			//		Vec4DT vecFinalP;
			//		MatVecMult(&vecFinalP, pAnim->tpAnimations[pAnim->tiPrevState].tpKeyInfo[pAnim->tiPrevFrame].tpJointInfo[i].tConcMat, zeroVector);

			//		// Go through each child.
			//		for(int j = 0; j < pAnim->tpAnimations[pAnim->tiPrevState].tpKeyInfo[pAnim->tiPrevFrame].tpJointInfo[i].tiNumChild; j++)
			//		{
			//			// Get the position of the child joint.
			//			Vec4DT vecFinalC;
			//			MatVecMult(&vecFinalC, pAnim->tpAnimations[pAnim->tiPrevState].
			//								tpKeyInfo[pAnim->tiPrevFrame].
			//								tpJointInfo[
			//									pAnim->tpAnimations[pAnim->tiPrevState].
			//									tpKeyInfo[pAnim->tiPrevFrame].
			//									tpJointInfo[i].
			//									tpiChildren[j]-1
			//								].tConcMat, zeroVector);

			//			// Draw a line between the two joints.
			//			glBegin(GL_LINES);
			//			glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
			//				glVertex3f(vecFinalP.tfX, vecFinalP.tfY, vecFinalP.tfZ);
			//				glVertex3f(vecFinalC.tfX, vecFinalC.tfY, vecFinalC.tfZ);
			//			glEnd();

			//			// Draw a circle to represent the parent joint.
			//			glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
			//			glPushMatrix();
			//			glTranslatef(vecFinalP.tfX, vecFinalP.tfY, vecFinalP.tfZ); 
			//			glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
			//			glScalef(0.05f, 0.05f, 0.05f);
			//			glCallList(m_dispLists[CIRCLE_DISP]);
			//			glPopMatrix();

			//			// Draw a circle to represent the child joint.
			//			glPushMatrix();
			//			glTranslatef(vecFinalC.tfX, vecFinalC.tfY, vecFinalC.tfZ); 
			//			glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
			//			glScalef(0.05f, 0.05f, 0.05f);
			//			glCallList(m_dispLists[CIRCLE_DISP]);
			//			glPopMatrix();
			//		}
			//	}

			//glPopMatrix();
		}
		break;
		case Level_obj:
		{
			// Draw the level object's collision lines.
			glColor4f(0.0f, 1.0f, 0.0f, 1.0f);

			Vec4DT *pStartVec = (Vec4DT *)(((unsigned char *)(pBO->CollisionData))+4);

			// Draw every collision line attached to the level object.
			glBegin(GL_LINES);
				for(unsigned int ii = 0; ii < (*((unsigned int *)(pBO->CollisionData)))*2; ii+=2)
				{
					glVertex3f( pStartVec[ii].tfX, pStartVec[ii].tfY, 0.0f);
					glVertex3f( pStartVec[ii+1].tfX, pStartVec[ii+1].tfY, 0.0f);
				}
			glEnd();
		}
		break;
		case Powerup_obj:
		{
			float fRadius = *(((float *)pBO->CollisionData));

			// Draw the powerup's collision circle.
			glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
            glPushMatrix();
				glTranslatef(pBO->m_PandO.fElements[12], pBO->m_PandO.fElements[13], 0.0f); 
				glScalef(fRadius, fRadius, fRadius);
				glCallList(m_dispLists[CIRCLE_DISP]);
			glPopMatrix();
		}
		break;
		case Pillow_obj:
		{
			// Get the pillow's radius.
			float *pRad = (float *)pBO->CollisionData;

			// Is the pillow active?
            if( ((CPillow *)pBO)->m_bActive )
				glColor4f(1.0f, 0.0f, 0.0f, 1.0f);// Red for an active pillow.
			else
				glColor4f(0.0f, 1.0f, 0.0f, 1.0f);// Green for an inactive pillow.

			// Draw the pillow's collision circle.
            glPushMatrix();
				glTranslatef(pBO->m_PandO.fElements[12], pBO->m_PandO.fElements[13], 0.0f); 
				glScalef(*pRad, *pRad, *pRad);
				glCallList(m_dispLists[CIRCLE_DISP]);
			glPopMatrix();
		}
		break;
		default:
		break;
	}
	
	// Back to the regular attributes.
	glPopAttrib();
}

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
void CDisplayDonkey::drawParticles()
{
	// Temporary storage for particle position.
	static float x;
	static float y;
	static float z;
	static int ii;
	static float *pfColor;

	CEmitter *pCurEmit = CParticleEngine::GetInstance()->m_ActiveEmitterList;
	if(!pCurEmit)
		return;

	// No depth testing or lighting for particle rendering.
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	while(pCurEmit)
	{
		// Get the color.
		pfColor = pCurEmit->fColor;

		CBitmapManager::GetInstance()->BindTexture(pCurEmit->m_EmitterStuff->iBitmapID);

		// Use triangle strips for rendering.
		glBegin(GL_TRIANGLE_STRIP);

		// Create the geometry for all particles.
		for(ii = 0; ii < pCurEmit->iNumParticles; ii++)
		{
			// Set the position, color and alpha channel for this particle.
			glColor4f(pfColor[0], pfColor[1], pfColor[2], pCurEmit->tParticles[ii].fAlpha);
			x = pCurEmit->tParticles[ii].tPosition.tfX;
			y = pCurEmit->tParticles[ii].tPosition.tfY;
			z = pCurEmit->tParticles[ii].tPosition.tfZ;

			// Top-Left corner.
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-HALF_PART_SIZE+x, HALF_PART_SIZE+y,z);

			if(ii != 0)
				// used to make a degenerate triangle.
				glVertex3f(-HALF_PART_SIZE+x, HALF_PART_SIZE+y, z);

			// Bottom-left corner.
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-HALF_PART_SIZE+x, -HALF_PART_SIZE+y, z);

			// Top-right corner.
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(HALF_PART_SIZE+x, HALF_PART_SIZE+y, z);

			// Bottom-right corner.
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(HALF_PART_SIZE+x, -HALF_PART_SIZE+y, z);

			// used to make a degenerate triangle.
			glVertex3f(HALF_PART_SIZE+x, -HALF_PART_SIZE+y, z);		
		}
		glEnd();

		pCurEmit = pCurEmit->m_pNext;
	}

	glEnable(GL_DEPTH_TEST);	// Back to depth-testing.
	glEnable(GL_LIGHTING);		// Back to lighting.
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
void CDisplayDonkey::initGLStates()
{
	// Set the clear color and enable depth testing.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Enable depth testing and texturing.
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	// Alpha blending.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

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
void CDisplayDonkey::initLight()
{
	// Lighting as well.
	glEnable(GL_LIGHTING);
	// Enable one light.
	glEnable(GL_LIGHT0);

    // Apply colors to all vertices after a glColor call.
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Back-face culling.
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

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
void CDisplayDonkey::setLight()
{
	// Set the direction of the light.
	static const float lightPos[4] = 
	{ 0.1f, 1.0f, 1.0f, 0.0f };

	// A little ambient light.
	static const float ambVal[4] = 
	{ 0.2f, 0.2f, 0.2f, 1.0f };

	// Set the strength of the sun's light.
	static const float diffuseVal[4] = 
	{ 0.6f, 0.6f, 0.6f, 1.0f };

	// Set these values.
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambVal);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseVal);
}

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
void CDisplayDonkey::drawGameObjects()
{
	// For grabbing the world objects.
	CBaseObject *p;
	CPowerup *pHead;	// For stepping through the powerups.
	int ii;				// Loop iterator.
	CObjectManager *pObjects = CObjectManager::GetInstance();

	//if(!pObjects)
	//	pObjects = CObjectManager::GetInstance();

	// Enable the vertex array types we need.
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	// Get each object to draw...
	ii = pObjects->m_iLevelObjectArraySize;
	while(ii--)
	{
		p = pObjects->m_LevelObjectArray[ii];

		// ..and draw them.
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		CBitmapManager::GetInstance()->BindTexture(p->m_pGeometryData->tuTexID);
		glInterleavedArrays(GL_T2F_N3F_V3F, sizeof(VertexT),  p->m_pGeometryData->tpVertices);

		glPushMatrix();
			glMultMatrixf(p->m_PandO.fElements);
			glDrawElements(GL_TRIANGLES, p->m_pGeometryData->tiNumIndices, 
				GL_UNSIGNED_INT, p->m_pGeometryData->tpIndices);
			// Shall we draw the debug information?
			if(CGameManager::GetInstance()->GameDebugInfo.bCollisionData)
				drawDebug(p);			
		glPopMatrix();

	}

	pHead = pObjects->m_ActivePowerupList;
	while(pHead)
	{
		if(!pHead->m_pGeometryData)
			continue;

			// Draw the powerup now!
		glPushMatrix();
			glMultMatrixf(pHead->m_PandO.fElements);

			// Bind the vertex buffer.
			//CVertBufCtrl::GetInstance()->BindBuffer(pHead->m_pGeometryData->tiVertID);
			CBitmapManager::GetInstance()->BindTexture(pHead->m_pGeometryData->tuTexID);
			glInterleavedArrays(GL_T2F_N3F_V3F, sizeof(VertexT), pHead->m_pGeometryData->tpVertices);
			glDrawElements(GL_TRIANGLES, pHead->m_pGeometryData->tiNumIndices, 
				GL_UNSIGNED_INT, pHead->m_pGeometryData->tpIndices);			

		glPopMatrix();

		// Shall we draw the debug information?
		if(CGameManager::GetInstance()->GameDebugInfo.bCollisionData)
			drawDebug(pHead);	

		pHead = pHead->m_pNext;		// Get the next power-up.
	}

	// Get each object to draw...
	ii = pObjects->m_iPlayerArraySize;
	while(ii--)
	{
		p = pObjects->m_PlayerArray[ii];

		// Check if this player is still in play.
		if(!p)
			continue;	// Not in play!, go to the next character.
		
		glInterleavedArrays(GL_T2F_N3F_V3F, sizeof(VertexT), p->m_pGeometryData->tpVertices);
		CBitmapManager::GetInstance()->BindTexture(p->m_pGeometryData->tuTexID);

		// Draw the player now!
		glColor3fv(((CPlayer *)p)->m_pPlayerData->PaletteColor);
		glPushMatrix();
			glMultMatrixf(p->m_PandO.fElements);
			glDrawElements(GL_TRIANGLES, p->m_pGeometryData->tiNumIndices, 
				GL_UNSIGNED_INT, p->m_pGeometryData->tpIndices);
		glPopMatrix();

		// Should we draw their collision information?
		if(CGameManager::GetInstance()->GameDebugInfo.bCollisionData)
			drawDebug(p);

		// Get this player's pillow.
		p = ((CPlayer *)p)->m_pPillow;
		if(!p)
			continue;

		// Setup the pillow geometry source.
		glInterleavedArrays(GL_T2F_N3F_V3F, sizeof(VertexT),  p->m_pGeometryData->tpVertices);
		CBitmapManager::GetInstance()->BindTexture(p->m_pGeometryData->tuTexID);

		// Draw the pillow now!
		glPushMatrix();
			glMultMatrixf(p->m_PandO.fElements);
			glDrawElements(GL_TRIANGLES, p->m_pGeometryData->tiNumIndices, 
				GL_UNSIGNED_INT, p->m_pGeometryData->tpIndices);
		glPopMatrix();

		// Should we draw pillow collision information?
		if(CGameManager::GetInstance()->GameDebugInfo.bCollisionData)
			drawDebug(p);
	}

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	// No more vertex buffers.
	//CVertBufCtrl::GetInstance()->ReleaseBufferUsage();
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
void CDisplayDonkey::newGame()
{
	// Reset the camera.
	m_effects.startEffects();
	m_theCamera.setCameraAttributes(m_iWidth, m_iHeight, FOV*SPF_DEG_TO_RAD, CLevelManager::GetInstance()->m_fcameraAngle);
    m_theCamera.resetCamera();
}

void CDisplayDonkey::DrawEnding(int iWinnerIndex)
{
	m_theCamera.goToWinnerMode(iWinnerIndex);
}
