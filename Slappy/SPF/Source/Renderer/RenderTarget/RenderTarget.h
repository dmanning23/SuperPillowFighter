//////////////////////////////////////////////////////////////////////////////////////////
// File Name:		RenderTarget.h
//
// Author:			Kevin Cecelski
//
// Date Created:	Monday, September 13, 2004
//
// Purpose:			This declares the render target class.
//
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef _RENDER_TARG_H_
#define _RENDER_TARG_H_ 
#include <windows.h>
#include "gl/gl.h"
#include "gl/glu.h"
#include "../DonkeyExt/DonkeyExt.h"

class CRenderTarget
{
public:
	// Two types of rendering targets.
	enum TARGET_TYPES_DONK{ SCREEN_BUF_DONK = 0, PIXEL_BUF_DONK};

	// What type of rendering target?
	int m_iType;

	// Handle to the rendering context.
	HGLRC m_hRC;

	// Handle to the device context.
	HDC	m_hDC;

	// *** The following members only apply to pixel buffer
	// *** targets.
	// Handle to the pixel buffer.
	HPBUFFERARB m_hPixBuf;

	// Width and height of the pixel buffer.
	int m_iWidth, m_iHeight;        

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
	CRenderTarget(void);

	//////////////////////////////////////////////////////////////////////////////////////////
	// Function:	~CRenderTarget()
	//
	// Purpose:		This is the destructor.
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
	~CRenderTarget(void) {}

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
	//
	//////////////////////////////////////////////////////////////////////////////////////////
	bool create(HWND hWnd, int iColDepth, int iDepthPrec, int iAlphaBits = 0,
		int iWidth = 0, int iHeight = 0, int iType = SCREEN_BUF_DONK);
	
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
	void destroy(HWND hWnd);

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
	void becomeCurrent();
};

#endif