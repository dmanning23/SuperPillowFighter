//////////////////////////////////////////////////////////////////////////////////////////
// File Name:      DonkeyExt.cpp
//
// Author:         Kevin Cecelski
//
// Date Created:   Friday, August 27, 2004
//
// Purpose:        This file defines all the extensions that the donkey supports.
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "DonkeyExt.h"
#include "../../Logger/Logger.h"
#include "string.h"

// Which extensions are supported?
unsigned int uExtSupport;	

// Begin with null pointers.
// Vertex buffer functions.
void (APIENTRY * BindBufferARB)(unsigned int target, unsigned int buffer) = 0;
void (APIENTRY * DeleteBuffersARB)(int n, const GLuint *buffers) = 0;
void (APIENTRY * GenBuffersARB)(int n, GLuint *buffers) = 0;
void (APIENTRY * BufferDataARB)(unsigned int target, int size, const void *data,
                       unsigned int usage) = 0;

// Wgl functions.
const char *(APIENTRY * wglGetExtensionsStringARB)(HDC hdc) = 0;

// Pixel buffer extension.
HPBUFFERARB (APIENTRY * wglCreatePbufferARB)(HDC hDC, int iPixelFormat, int iWidth,
				int iHeight, const int *piAttribList) = 0;
HDC (APIENTRY * wglGetPbufferDCARB)(HPBUFFERARB hPbuffer) = 0;
int (APIENTRY * wglReleasePbufferDCARB)(HPBUFFERARB hPbuffer,HDC hDC) = 0;
BOOL (APIENTRY * wglDestroyPbufferARB)(HPBUFFERARB hPbuffer) = 0;
BOOL (APIENTRY * wglQueryPbufferARB)(HPBUFFERARB hPbuffer, int iAttribute, int *piValue) = 0;

// Functions for the pixel format extension.
BOOL (APIENTRY * wglGetPixelFormatAttribivARB)(HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes,
								  const int *piAttributes, int *piValues) = 0;
BOOL (APIENTRY * wglGetPixelFormatAttribfvARB)(HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes,
                                  const int *piAttributes, FLOAT *pfValues) = 0;
BOOL (APIENTRY * wglChoosePixelFormatARB)(HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats,
                             int *piFormats, UINT *nNumFormats) = 0;

// Functions for the render to texture extension.
BOOL (APIENTRY * wglBindTexImageARB)(HPBUFFERARB hPbuffer, int iBuffer) = 0;
BOOL (APIENTRY * wglReleaseTexImageARB)(HPBUFFERARB hPbuffer, int iBuffer) = 0;
BOOL (APIENTRY * wglSetPbufferAttribARB)(HPBUFFERARB hPbuffer, const int *piAttribList) = 0;

// Number of extensions supported.
#define NUM_EXTS 4

// List of donkey supported extension names.
static const char *extNames[NUM_EXTS] = {
	"GL_ARB_vertex_buffer_object",		// For vertex buffers.
	"WGL_ARB_pbuffer",					// For rendering to texture.
	"WGL_ARB_pixel_format",				// For rendering to texture.
	"WGL_ARB_render_texture",			// For rendering to texture.
};

static CLogger outLog;			// For logging.	

//////////////////////////////////////////////////////////////////////////////////////////
// Function:	getPBufFunctions()
//
// Purpose:		To get the GL extensions for pixel buffers.
//
// Last Modified:  Monday, September 13, 2004
//
// Input:		None.
//
// Output:		None.
//
// Returns:		true if all the extension functions were loaded, false otherwise.
//
//////////////////////////////////////////////////////////////////////////////////////////
static bool getPBufFunctions()
{
	// For creating a pixel buffer.
	wglCreatePbufferARB = (HPBUFFERARB (APIENTRY *)(HDC hDC, int iPixelFormat, int iWidth,
				int iHeight, const int *piAttribList))
		wglGetProcAddress("wglCreatePbufferARB");
	if(!wglCreatePbufferARB)
		outLog << "\twglCreatePbufferARB not loaded...\n";
	else
		outLog << "\twglCreatePbufferARB loaded!\n";

	// For getting the DC of a pixel buffer.
	wglGetPbufferDCARB = (HDC (APIENTRY *)(HPBUFFERARB hPbuffer))
		wglGetProcAddress("wglGetPbufferDCARB");
	if(!wglGetPbufferDCARB)
		outLog << "\twglGetPbufferDCARB not loaded...\n";
	else
		outLog << "\twglGetPbufferDCARB loaded!\n";

	// For releasing the DC the pixel buffer is using.
	wglReleasePbufferDCARB = (int (APIENTRY *)(HPBUFFERARB hPbuffer,HDC hDC))
		wglGetProcAddress("wglReleasePbufferDCARB");
	if(!wglReleasePbufferDCARB)
		outLog << "\twglReleasePbufferDCARB not loaded...\n";
	else
		outLog << "\twglReleasePbufferDCARB loaded!\n";

	// For cleanup of the pixel buffer.
	wglDestroyPbufferARB = (BOOL (APIENTRY *)(HPBUFFERARB hPbuffer))
		wglGetProcAddress("wglDestroyPbufferARB");
	if(!wglDestroyPbufferARB)
		outLog << "\twglDestroyPbufferARB not loaded...\n";
	else
		outLog << "\twglDestroyPbufferARB loaded!\n";

	// For discovering information about the pixel buffer.
	wglQueryPbufferARB = (BOOL (APIENTRY *)(HPBUFFERARB hPbuffer, int iAttribute, int *piValue))
		wglGetProcAddress("wglQueryPbufferARB");
	if(!wglDestroyPbufferARB)
		outLog << "\twglQueryPbufferARB not loaded...\n";
	else
		outLog << "\twglQueryPbufferARB loaded!\n";

	// Were we able to load all the functions we need?
	if(wglCreatePbufferARB && wglGetPbufferDCARB && wglReleasePbufferDCARB && wglDestroyPbufferARB && wglQueryPbufferARB)
		return true;
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Function:	getPixfmtFunctions()
//
// Purpose:		To get the GL extensions for pixel formats.
//
// Last Modified:  Monday, September 13, 2004
//
// Input:		None.
//
// Output:		None.
//
// Returns:		true if all the extension functions were loaded, false otherwise.
//
//////////////////////////////////////////////////////////////////////////////////////////
static bool getPixfmtFunctions()
{
	// For getting a pixel format attribute.
	wglGetPixelFormatAttribivARB = (BOOL (APIENTRY *)(HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes,
								  const int *piAttributes, int *piValues))
		wglGetProcAddress("wglGetPixelFormatAttribivARB");
	if(!wglGetPixelFormatAttribivARB)
		outLog << "\twglGetPixelFormatAttribivARB not loaded...\n";
	else
		outLog << "\twglGetPixelFormatAttribivARB loaded!\n";

	// For getting the a format attribute.
	wglGetPixelFormatAttribfvARB = (BOOL (APIENTRY *)(HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes,
                                  const int *piAttributes, FLOAT *pfValues))
		wglGetProcAddress("wglGetPixelFormatAttribfvARB");
	if(!wglGetPixelFormatAttribfvARB)
		outLog << "\twglGetPixelFormatAttribfvARB not loaded...\n";
	else
		outLog << "\twglGetPixelFormatAttribfvARB loaded!\n";

	// For choosing a pixel format.
	wglChoosePixelFormatARB = (BOOL (APIENTRY *)(HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats,
                             int *piFormats, UINT *nNumFormats))
		wglGetProcAddress("wglChoosePixelFormatARB");
	if(!wglChoosePixelFormatARB)
		outLog << "\twglChoosePixelFormatARB not loaded...\n";
	else
		outLog << "\twglChoosePixelFormatARB loaded!\n";

	// Were we able to load all the functions we need?
	if(wglGetPixelFormatAttribivARB && wglGetPixelFormatAttribfvARB && wglChoosePixelFormatARB)
		return true;
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Function:	getRndTexFunctions()
//
// Purpose:		To get the GL extensions for rendering to texture.
//
// Last Modified:  Monday, September 13, 2004
//
// Input:		None.
//
// Output:		None.
//
// Returns:		true if all the extension functions were loaded, false otherwise.
//
//////////////////////////////////////////////////////////////////////////////////////////
static bool getRndTexFunctions()
{
	// For binding to the rendered texture.
	wglBindTexImageARB  = (BOOL (APIENTRY *)(HPBUFFERARB hPbuffer, int iBuffer))
		wglGetProcAddress("wglBindTexImageARB");
	if(!wglBindTexImageARB )
		outLog << "\twglBindTexImageARB  not loaded...\n";
	else
		outLog << "\twglBindTexImageARB  loaded!\n";

	// For releasing the render texture.
	wglReleaseTexImageARB  = (BOOL (APIENTRY *)(HPBUFFERARB hPbuffer, int iBuffer))
		wglGetProcAddress("wglReleaseTexImageARB");
	if(!wglReleaseTexImageARB )
		outLog << "\twglReleaseTexImageARB  not loaded...\n";
	else
		outLog << "\twglReleaseTexImageARB  loaded!\n";

	// For render to texture attribute.
	wglSetPbufferAttribARB  = (BOOL (APIENTRY *)(HPBUFFERARB hPbuffer, const int *piAttribList))
		wglGetProcAddress("wglSetPbufferAttribARB");
	if(!wglSetPbufferAttribARB )
		outLog << "\twglSetPbufferAttribARB  not loaded...\n";
	else
		outLog << "\twglSetPbufferAttribARB  loaded!\n";

	// Were we able to load all the functions we need?
	if(wglBindTexImageARB && wglReleaseTexImageARB && wglSetPbufferAttribARB)
		return true;
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Function:	getVertexBufferFunctions()
//
// Purpose:		To get the GL extensions for vertex buffers.
//
// Last Modified:  Monday, September 13, 2004
//
// Input:		None.
//
// Output:		None.
//
// Returns:		true if all the extension functions were loaded, false otherwise.
//
//////////////////////////////////////////////////////////////////////////////////////////
static bool getVertexBufferFunctions()
{
	// For binding a vertex buffer.
	BindBufferARB = (void (APIENTRY *)(unsigned int target, unsigned int buffer))
		wglGetProcAddress("glBindBufferARB");
	if(!BindBufferARB)
		outLog << "\tBindBufferARB not loaded...\n";
	else
		outLog << "\tBindBufferARB loaded!\n";

	// For deleting vertex buffers.
	DeleteBuffersARB = (void (APIENTRY *)(int n, const GLuint *buffers))
		wglGetProcAddress("glDeleteBuffersARB");
	if(!DeleteBuffersARB)
		outLog << "\tDeleteBuffersARB not loaded...\n";
	else
		outLog << "\tDeleteBuffersARB loaded!\n";

	// Gor creating vertex buffers.
	GenBuffersARB = (void (APIENTRY *)(int n, GLuint *buffers))
		wglGetProcAddress("glGenBuffersARB");
	if(!GenBuffersARB)
		outLog << "\tGenBuffersARB not loaded...\n";
	else
		outLog << "\tGenBuffersARB loaded!\n";

	// For setting the data of a vertex buffer.
	BufferDataARB = (void (APIENTRY *)(unsigned int target, int size, const void *data, unsigned int usage))
		wglGetProcAddress("glBufferDataARB");
	if(!BufferDataARB)
		outLog << "\tBufferDataARB not loaded...\n";
	else
		outLog << "\tBufferDataARB loaded!\n";

	// Were we able to load all the functions we need?
	if(BindBufferARB && DeleteBuffersARB && GenBuffersARB && BufferDataARB)
		return true;
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Function:	checkForSupport()       
//
// Purpose:		This checks if the given extension is supported on this hardware.
//
// Last Modified:  Saturday, August 28, 2004
//
// Input:		strGLExt		- The name of the extension to check.
//
// Output:		None.
//
// Returns:		true if this harware supports the given extension, false otherwise. 
//
//////////////////////////////////////////////////////////////////////////////////////////
static bool checkForSupport(const char *strGLExt)
{
	int ii;
	for(ii = 0; ii < NUM_EXTS; ii++)
		if(!strcmp(strGLExt, extNames[ii]))
			return true;	// Extension found.

	// No support.
	return false;
}


//////////////////////////////////////////////////////////////////////////////////////////
// Function:	getExtFunctions()       
//
// Purpose:		Gets the extension functions for the given OpenGL extension.
//
// Last Modified:  Saturday, August 28, 2004
//
// Input:		strGLExt		- The name of the extension to check.
//
// Output:		None.
//
// Returns:		None.
//
//////////////////////////////////////////////////////////////////////////////////////////
static void getExtFunctions(const char *strGLExt)
{
	// Find the extension 
	// Is it the vertex buffer extension?
	if(!strcmp(strGLExt, "GL_ARB_vertex_buffer_object"))
	{
		outLog << "GL_ARB_vertex_buffer_object is supported!\n";
		outLog << "the functions:\n";
		if(getVertexBufferFunctions())
			uExtSupport |= VERTEX_BUF;
	}

	// Is it the pixel buffer extension?
	if(!strcmp(strGLExt, "WGL_ARB_pbuffer"))
	{
		outLog << "WGL_ARB_pbuffer is supported!\n";
		outLog << "the functions:\n";
		if(getPBufFunctions())
			uExtSupport |= PIXEL_BUF;
	}

	// Is it the pixel formatting extension?
	if(!strcmp(strGLExt, "WGL_ARB_pixel_format"))
	{
		outLog << "WGL_ARB_pixel_format is supported!\n";
		outLog << "the functions:\n";
		if(getPixfmtFunctions())
			uExtSupport |= PIX_FMT;
	}

	// Is it the render to texture extension?
	if(!strcmp(strGLExt, "WGL_ARB_render_texture"))
	{
		outLog << "WGL_ARB_render_texture is supported!\n";
		outLog << "the functions:\n";
        if(getRndTexFunctions())
			uExtSupport |= RND_TEX;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
// Function:       loadExtensions()
//
// Purpose:        Get the extensions the donkey needs.
//
// Last Modified:  Friday, August 27, 2004
//
// Input:          None.
//
// Output:         None.
//
// Returns:        Return false if 1 or more extensions were not loaded.
//
//////////////////////////////////////////////////////////////////////////////////////////
void loadExtensions(HDC hDc)
{
	char *strName;			
	char *strExtensions;	// Hold all the extension names.
	char *strWglExts;		// Stores the supported wgl extensions.

	// All are false right now.
	memset(&uExtSupport, 0, sizeof(unsigned int));

	// The list of extensions supported by this machine.
	strExtensions = (char *)glGetString(GL_EXTENSIONS);

	outLog.Init("DonkeyExtensions.txt");
	outLog << "This machine supports the following OpenGL extensions:\n";

	// Get the wgl extension string function so that we may find the wgl extensions
	// supported on this machine.
	if(strExtensions)
	{
		// First, get a pointer to the wgl extension string function.
		wglGetExtensionsStringARB =  (const char *(APIENTRY *)(HDC hdc))
			wglGetProcAddress("wglGetExtensionsStringARB");

		// Get the list of wgl extensions.
		strWglExts = (char *)wglGetExtensionsStringARB(hDc);
		strName = strtok(strWglExts, " ");

		// Go through each wgl extension supported and find out if 
		// the donkey uses it.
		while(true)
		{
			// Break at the end of the list.
			if(!strName)
				break;

			// Find if we support the current extension.
			if(checkForSupport(strName))
				getExtFunctions(strName);

			// Next wgl extension.
			strName = strtok(0, " ");
		}
	}

	// Go through each extension supported by this hardware.
	strName = strtok(strExtensions, " ");
	while(true)
	{
		if(!strName)
			break;			// No more extensions.

		// Find if we support this extension.
		if(checkForSupport(strName))
			getExtFunctions(strName);

		// Next extension.
		strName = strtok(0, " ");
	}
}