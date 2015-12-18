//////////////////////////////////////////////////////////////////////////////////////////
// File Name:		RenderTarget.cpp
//
// Author:			Kevin Cecelski
//
// Date Created:	Monday, September 13, 2004
//
// Purpose:			This defines the rendertarget class.
//
//////////////////////////////////////////////////////////////////////////////////////////

#include "rendertarget.h"
//////////////////////////////////////////////////////////////////////////////////////////
// Function:	CRenderTarget()
//
// Purpose:		This is the default constructor.
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
CRenderTarget::CRenderTarget(void)
{
	m_iType = -1;
	m_hRC = NULL;
    m_hDC = NULL;
	m_hPixBuf = NULL;
	m_iWidth = 0;
	m_iHeight = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Function:	create()
//
// Purpose:		This creates the render target specified.
//
// Last Modified:  Monday, September 13, 2004
//
// Input:		hWnd		- the window that we will use.
//				iColDepth	- the color depth for this target.
//				iDepthPrec	- the depth precision for this target.
//				iAlphaBits	- the number of bits dedicated to the alpha channel.
//				iWidth		- the width of the render target.
//				iHeight		- the height of the render target.
//				iType		- the type of render target(two type: screen and pixel buffers)
//
// Output:		None.
//
//////////////////////////////////////////////////////////////////////////////////////////
bool CRenderTarget::create(HWND hWnd, int iColDepth, int iDepthPrec, int iAlphaBits,
	int iWidth, int iHeight, int iType)
{
	m_iType = iType;

	m_iWidth = iWidth;
	m_iHeight = iHeight;

    // Screen buffer.
	if(iType == SCREEN_BUF_DONK)
	{
		// Get the device context.
		m_hDC = GetDC(hWnd);

		// Pick a pixel format.
		int iPixFormat;
		PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,	// Draw to a window using a double buffer.
			PFD_TYPE_RGBA,	// Red green blue and alpha color channels.
			iColDepth, // color depth
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			iDepthPrec, // depth buffer
			0, 0, 0, 0, 0, 0, 0,
		};        

		// Try to set the pixel format to the one we specified
		// in the structure.
		iPixFormat = ChoosePixelFormat(m_hDC, &pfd);
		SetPixelFormat(m_hDC, iPixFormat, &pfd);

		// Create the OpenGL Rendering Context.
		m_hRC = wglCreateContext(m_hDC);
	}

	// Pixel Buffer.
	if(iType == PIXEL_BUF_DONK)
	{
		const int intAttributes[] = {
			WGL_DRAW_TO_PBUFFER_ARB, true,	// This is a pixel buffer
			WGL_COLOR_BITS_ARB, iColDepth,	// color
			WGL_DEPTH_BITS_ARB, iDepthPrec,	// depth buffer
			WGL_ALPHA_BITS_ARB, iAlphaBits,	// alpha channel
			WGL_BIND_TO_TEXTURE_RGBA_ARB, true,
			0 };							// End with a 0

		const float floatAttributes[] = { 0 };  // NOP

		// Get the device context.
		HDC hDCtemp = GetDC(hWnd);

		// Get a pixel format that is close to the one we want.
		int iChoice;
		unsigned int uFormats;
		wglChoosePixelFormatARB(hDCtemp, intAttributes, floatAttributes, 1,
			&iChoice, &uFormats);

		// The buffer attributes.
		const int ibufAttr[] = {
			WGL_PBUFFER_LARGEST_ARB, 1,		// Get the largest buffer possible.
			WGL_TEXTURE_FORMAT_ARB, WGL_TEXTURE_RGBA_ARB,	// The texture format RGBA
			WGL_TEXTURE_TARGET_ARB, WGL_TEXTURE_2D_ARB,		// Treat it as a 2D texture
			0,
		};

		// Create the pixel buffer.
		m_hPixBuf = wglCreatePbufferARB(hDCtemp, iChoice, iWidth, iHeight,
			ibufAttr);

		// Create a device context for the pixel buffer.
		m_hDC = wglGetPbufferDCARB(m_hPixBuf);

        // Get the rendering context.
		m_hRC = wglCreateContext(m_hDC);
	}

	return true;
}
	
//////////////////////////////////////////////////////////////////////////////////////////
// Function:	destroy()	
//
// Purpose:		This destroys the render target.
//
// Last Modified:  Monday, September 13, 2004
//
// Input:		hWnd	- The window that we used for this render target.
//
// Output:		None.
//
// Returns:		None.
//
//////////////////////////////////////////////////////////////////////////////////////////
void CRenderTarget::destroy(HWND hWnd)
{
    // Screen buffer.
	if(m_iType == SCREEN_BUF_DONK)
	{
		// Release the opengl rendering context.
		wglMakeCurrent(m_hDC, NULL);	

		// Delete the rendering context.
		wglDeleteContext(m_hRC);		

		// Release the device context.
		ReleaseDC(hWnd, m_hDC);	
	}

	// Pixel Buffer.
	if(m_iType == PIXEL_BUF_DONK)
	{
		// Destroy the rendering context.
		if(m_hRC)
			wglDeleteContext(m_hRC);
	    
		// Destroy the device context.
		if(m_hDC)
			wglReleasePbufferDCARB(m_hPixBuf, m_hDC);

		// Destroy the pbuffer.
		wglDestroyPbufferARB(m_hPixBuf);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
// Function:	becomeCurrent()
//
// Purpose:		This makes all OpenGL rendering occur on this target.
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
void CRenderTarget::becomeCurrent()
{
	wglMakeCurrent(m_hDC, m_hRC);
}