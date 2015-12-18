//////////////////////////////////////////////////////////////////////////////////////////
// File Name:		RndToTexture.h
//
// Author:			Kevin Cecelski
//
// Date Created:	Monday, September 13, 2004
//
// Purpose:			This declares the render-to-texture class.
//
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef _RNDTOTEX_H_
#define _RNDTOTEX_H_
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "../RenderTarget/RenderTarget.h"

class CRndToTexture
{
	// The pixel buffer that we will use for rendering.
	CRenderTarget m_pixBuf;

public:
	// The id of the texture object that contains the 
	// rendered image.
	GLuint m_rndTex;

	//////////////////////////////////////////////////////////////////////////////////////////
	// Function:	CRndToTexture()
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
	CRndToTexture(void) {}

	//////////////////////////////////////////////////////////////////////////////////////////
	// Function:	~CRndToTexture()
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
	~CRndToTexture(void) {}

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
	// Returns:		true if the partition was successfully created.
	//
	//////////////////////////////////////////////////////////////////////////////////////////
	bool createRT(HWND hWnd, int iWidth, int iHeight);

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
	void startTexRender();

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
	void endTexRender();

	//////////////////////////////////////////////////////////////////////////////////////////
	// Function:	useRenderTex()
	//
	// Purpose:		Bind the render-to-texture space for use a regular texture.
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
	void useRenderTex();

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
	void releaseRenderTex();
};

#endif