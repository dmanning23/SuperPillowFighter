//////////////////////////////////////////////////////////////////////////////////////////
//File Name:		BitmapManager.h
// 
//Author:			Kevin Cecelski
//
//Date Created:		8/8/2004
//
//Purpose:			This defines the CBitmapManager class.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef _BITMAP_H_
#define _BITMAP_H_
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "..\logger\logger.h"
#include "..\MemManager\MemManager.h"
#include <stdio.h>

// Maximum number bitmaps that this manager will handle at one
#define MAX_BITMAPS		256

class CBitmapManager
{
private:
	// The only instance of this manager.
	static CBitmapManager *m_pInstance;

	// This structure is used to manage the bitmaps.  
	// It will tell us the exact texture object ID and whether that
	// texture object is being used.
	struct			bitmapNodeT
	{
		bool		tbUsed;		// Is this node storing a bitmap ID?
		char		*tstrFName;	// Used in debug mode to store the name of the image.
		int			tiRefCount;	// The number of references to this bitmap.
	};

	// The amount of bitmaps this module is managing.
	int	m_iNumBitmaps;

	// The texture ids that we get back from openGL for the texture objects.
	GLuint m_textureHandles[MAX_BITMAPS];

	// The array of bitmap nodes that is used to track the textures.
    bitmapNodeT		m_strBmpIds[MAX_BITMAPS];

	// The log file for this manager.
	CLogger m_outLog;    

	// ** Overide functions that singletons should not expose. **
		// Copy constructor.
		CBitmapManager(const CBitmapManager &cbm) {}		
		// Assignment operator.
		CBitmapManager &operator=(const CBitmapManager &cbm) {}
		// Default constructor.
		CBitmapManager(void) {}
	// **********************************************************

	//////////////////////////////////////////
	// Function: findUnusedNode()
	//
	// Purpose:	 Finds a node in the node list that is not being used. 
	//
	// Last Modified: 8/7/2004
	//
	// Input: None.
	//
	// Output: None. 
	//
	// Returns: An array index is returned if an open slot is found, -1 otherwise.
	//
	//////////////////////////////////////////
	int findUnusedNode();

	//////////////////////////////////////////
	// Function: loadTGAImage()
	//
	// Purpose:	 Loads the data from a TGA file.
	//
	// Last Modified: 8/10/2004
	//
	// Input: *strFileName		- the name of the file to load.
	//		  *piWidth			- storage for the width of the image.
	//		  *piHeight			- storage for the height of the image.
	//		  **pcData			- storage for the memory address of the data.
	//
	// Output: *piWidth				- contains the width of the image loaded.
	//		   *piHeight			- contains the height of the image loaded.
	//		   **pcData				- contains a pointer to the image data.
	//
	// Returns: true if successful, false otherwise.
	//
	//////////////////////////////////////////
	bool loadTGAImage(char *strFileName, int *piWidth, int *piHeight, unsigned char **pcData);

	//////////////////////////////////////////
	// Function: ~getClosestPowOf2()
	//
	// Purpose:	 Find the power of 2 closest to the number we are given.
	//				(always rounding down. eg: given 255 will return 128)
	//
	// Last Modified: 8/10/2004
	//
	// Input: iNum				- The number for consideration.
	//
	// Output: None. 
	//
	// Returns: The closest power of 2 to the number we are given.
	//
	//////////////////////////////////////////
	inline int getClosestPowOf2(int iNum)
	{
		int num = 1;	// start at a power of 0.
        while((num*2) < iNum)
			num *= 2;

		return num;	// give it back.
	}

public:
	//////////////////////////////////////////
	// Function: ~CBitmapManager()
	//
	// Purpose:	 Default destructor.
	//
	// Last Modified: 8/7/2004
	//
	// Input: None.
	//
	// Output: None. 
	//
	// Returns: None.
	//
	//////////////////////////////////////////
	~CBitmapManager(void) {}

	//////////////////////////////////////////
	// Function: GetInstance()
	//
	// Purpose:	 Returns a pointer to the only instance of this class.
	//
	// Last Modified: 8/7/2004
	//
	// Input: None.
	//
	// Output: None. 
	//
	// Returns: A pointer to the only instance of this class.
	//
	//////////////////////////////////////////
	static CBitmapManager *GetInstance()
	{
		if(!m_pInstance)
		{
			m_pInstance = (CBitmapManager *)MemMalloc(sizeof(CBitmapManager));	// Make a new instance.
			m_pInstance->CBitmapManager::CBitmapManager();
		}

		// Give 'em that pointer.
        return m_pInstance;
	}

	//////////////////////////////////////////
	// Function: DeleteInstance()
	//
	// Purpose:	 Deletes the only instance of this class.
	//
	// Last Modified: 8/7/2004
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
			m_pInstance->CBitmapManager::~CBitmapManager();
			MemFree(m_pInstance);
			m_pInstance = 0;
		}
	}

	//////////////////////////////////////////
	// Function: initialize()
	//
	// Purpose:	Initializes the bitmap manager.
	//
	// Last Modified: 8/7/2004
	//
	// Input: None.
	//
	// Output: None. 
	//
	// Returns: None.
	//
	//////////////////////////////////////////
	void Initialize();

	//////////////////////////////////////////
	// Function: shutdown()
	//
	// Purpose:	Shutdown the bitmap manager.
	//
	// Last Modified: 8/7/2004
	//
	// Input: None.
	//
	// Output: None. 
	//
	// Returns: None.
	//
	//////////////////////////////////////////
	void Shutdown();

	//////////////////////////////////////////
	// Function: LoadImage()
	//
	// Purpose:	Loads a TGA image.
	//
	// Last Modified: 8/7/2004
	//
	// Input: strFilename		- the name of the file where the TGA image
	//							  is stored.
	//		  stdTexEnv			- true if we want the nitmap manager to set
	//							  up the texture environment for this texture
	//							  false if it is to be user defined.
	// Output: None.
	//
	// Returns: The id of this bitmap.
	//
	//////////////////////////////////////////
	int LoadImage(char *strFilename, bool stdTexEnv = false);

	//////////////////////////////////////////
	// Function: BindTexture()
	//
	// Purpose:	Bind a texture for use with opengl calls.
	//
	// Last Modified: 8/7/2004
	//
	// Input: iId		- The ID of the bitmap that we will bind.
	//
	// Output: None. 
	//
	// Returns: true if the id was valid, false otherwise
	//
	//////////////////////////////////////////
	bool BindTexture(int iId);

	//////////////////////////////////////////
	// Function: UnloadImage()
	//
	// Purpose:	Unload a TGA image.
	//
	// Last Modified: 8/7/2004
	//
	// Input: The id of the bitmap to unload.
	//
	// Output: None. 
	//
	// Returns: None.
	//
	//////////////////////////////////////////
	void UnloadImage(int iId);

	//////////////////////////////////////////
	// Function: cleanup()
	//
	// Purpose:	Unload all images.
	//
	// Last Modified: 8/7/2004
	//
	// Input: None.
	//
	// Output: None. 
	//
	// Returns: None.
	//
	//////////////////////////////////////////
	void cleanup();

	//////////////////////////////////////////
	// Function: getNumTexLoaded()
	//
	// Purpose:	Find the amount textures that have been loaded.
	//
	// Last Modified: 8/10/2004
	//
	// Input: None.
	//
	// Output: None. 
	//
	// Returns: The number of textures in memory.
	//
	//////////////////////////////////////////
	inline int getNumTexLoaded()
	{ return m_iNumBitmaps; }
};

#endif