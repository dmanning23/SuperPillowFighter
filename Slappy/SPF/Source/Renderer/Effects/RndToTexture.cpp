#include "rndtotexture.h"
#include "../DisplayDonkey.h"

//////////////////////////////////////////////////////////////////////////////////////////
// Function:	createRT()
//
// Purpose:		This reserves a partition of texture memory for the rendering-to-texture purposes.
//
// Last Modified:  Monday, September 13, 2004
//
// Input:		hWnd	- The window opengl is using for rendering.
//				iWidth	- The width of the render-to-texture space.
//				iHeight	- The height of the render-to-texture space.
//
// Output:		None.
//
// Returns:		true if the texture partition was successfully created.
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CRndToTexture::createRT(HWND hWnd, int iWidth, int iHeight)
{
	// Set up the texture environment.
	glGenTextures(1, &m_rndTex);
	glBindTexture(GL_TEXTURE_2D, m_rndTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Create the pixel buffer.
	m_pixBuf.create(hWnd, 32, 16, 8, iWidth, iHeight, CRenderTarget::PIXEL_BUF_DONK);

	// Setup the GL environment for the pixel buffer.
	m_pixBuf.becomeCurrent();

	// Setup this environment to match the donkey's environment.
	CDisplayDonkey::GetInstance()->initGLStates();
	CDisplayDonkey::GetInstance()->initLight();
	CDisplayDonkey::GetInstance()->setLight();

	// Map the OpenGL coordinates.
	glViewport(0, 0, iWidth, iHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set up the view frustrum.
	gluPerspective(FOV, ((float)iWidth)/iHeight, NEAR_CLIP_PLANE, FAR_CLIP_PLANE);

	// Go back to model/view matrix mode.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();  

	// Clear the pixel buffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	wglShareLists(CDisplayDonkey::GetInstance()->m_rndTarg.m_hRC, m_pixBuf.m_hRC);

	// Back to the screen rendering target.
	CDisplayDonkey::GetInstance()->m_rndTarg.becomeCurrent();

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Function:	startTexRender()
//	
// Purpose:		Starts the rendering-to-texture process.
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
void CRndToTexture::startTexRender()
{
	// Make the pixel buffer the render target.
	m_pixBuf.becomeCurrent();

	// Clear the pixel buffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Function:	endTexRender()
//
// Purpose:		Ends the rendering-to-texture process.
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
void CRndToTexture::endTexRender()
{
	// Back to the screen rendering target.
	CDisplayDonkey::GetInstance()->m_rndTarg.becomeCurrent();
}

//////////////////////////////////////////////////////////////////////////////////////////
// Function:	useRenderTex()
//
// Purpose:		Bind the render-to-texture space for use as a regular texture.
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
void CRndToTexture::useRenderTex()
{
	// Bind the rendered texture to a texture object.
	glBindTexture(GL_TEXTURE_2D, m_rndTex);
	wglBindTexImageARB(m_pixBuf.m_hPixBuf, WGL_FRONT_LEFT_ARB);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Function:	releaseRenderTex()
//
// Purpose:		Release the render-to-texture space so that it may be used again for rendering.
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
void CRndToTexture::releaseRenderTex()
{
	// Release the pixel texture.
	wglReleaseTexImageARB(m_pixBuf.m_hPixBuf, WGL_FRONT_LEFT_ARB);
}



