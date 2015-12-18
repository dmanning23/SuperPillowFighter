//////////////////////////////////////////////////////////////////////////////////////////
//File Name:		bitmapmanager.cpp
// 
//Author:			Kevin Cecelski
//
//Date Created:		8/7/2004
//
//Purpose:			This implements the bitmap manager interface.
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "bitmapmanager.h"

// Initialize the only pointer to this class.
CBitmapManager *CBitmapManager::m_pInstance = 0;

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
int CBitmapManager::findUnusedNode()
{
	// Ready... 
	int i = -1;

	// Try to find an empty slot.
	while(++i < MAX_BITMAPS)
		if(!m_strBmpIds[i].tbUsed)
			return i;

	// None found.
    return -1;
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
void CBitmapManager::Initialize()
{
	// Clear the management data.
	memset(m_strBmpIds, 0, sizeof(bitmapNodeT)*MAX_BITMAPS);

	// Log the creation of this manager.
	m_outLog.Init("BitmapManager.txt");

	m_outLog << "Initializing the bitmap manager.\n";

	// No bitmaps yet...
	m_iNumBitmaps = 0;
}

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
void CBitmapManager::Shutdown()
{
	m_outLog << "Shutting down the bitmap manager.\n";
	m_outLog << "Textures left in the bitmap manager: " << 	m_iNumBitmaps << "\n";
	cleanup();
	m_outLog.Shutdown();
}

//////////////////////////////////////////
// Function: LoadImage()
//
// Purpose:	Loads a TGA image.
//
// Last Modified: 8/7/2004
//
// Input: strFilename		- the name of the file where the TGA image
//							  is stored.
//
// Output: None.
//
// Returns: The id of this bitmap.
//
//////////////////////////////////////////
int CBitmapManager::LoadImage(char *strFilename, bool stdTexEnv)
{
	m_outLog << "\tAttempting to load TGA image: " << strFilename << "\n";

	// Make sure the image is not already loaded.
	for(int ii = 0; ii < m_iNumBitmaps; ii++)
	{
		if(!strcmp(strFilename, m_strBmpIds[ii].tstrFName))
		{
			m_outLog << "\t\tImage " << strFilename << " already loaded.\n";
            // Only increase the reference count.
			m_strBmpIds[ii].tiRefCount++;
			return ii;
		}
	}

	// Try to load the file.
	unsigned char *pData = 0;
	int iWidth, iHeight;
	if(!loadTGAImage(strFilename, &iWidth, &iHeight, &pData))
	{
		m_outLog << "\tCould not load the image.\n";
		return -1;
	}

    m_outLog << "\t" << strFilename << " was successfully loaded.\n";

	// Try to find an empty slot.
	int iEmpty = findUnusedNode();
	if(iEmpty == -1)
	{
        m_outLog << "\t No more room for another texture. Not loading\n";		
		MemFree(pData);	// Release the temporary image data.
		return -1;
	}

	// Start tracking this bitmap.
	m_iNumBitmaps++;
	m_strBmpIds[iEmpty].tbUsed = true; 

	// Create a texture object for this texture.
	glGenTextures(1, &m_textureHandles[iEmpty]);

	m_strBmpIds[iEmpty].tstrFName = (char *)MemMalloc((int)strlen(strFilename)+1);
	strcpy(m_strBmpIds[iEmpty].tstrFName, strFilename);

	// Check to see if the dimensions of the image loaded are powers of two...
	if( (iWidth&-iWidth) != iWidth || (iHeight&-iHeight) != iHeight )
	{
		// Find the closest power of 2 for the width.
		int isclWidth = getClosestPowOf2(iWidth);

		// Find the closest power of 2 for the height.
		int isclHeight = getClosestPowOf2(iHeight);

		unsigned int uSclImageSize = isclWidth * isclHeight * 4;

		// Get some storage.
		unsigned char *pSclData = (unsigned char *)MemMalloc(uSclImageSize);
   
		// Rescale the image to dimensions that are powers of 2.
		int iResult = gluScaleImage(GL_RGBA, iWidth, iHeight, GL_UNSIGNED_BYTE, pData,
			isclWidth, isclHeight, GL_UNSIGNED_BYTE, pSclData);

		// Bind the 
		if(iResult == 0)
		{
			// Bind the texture to the texture object.
			glBindTexture(GL_TEXTURE_2D, m_textureHandles[iEmpty]);
			glTexImage2D(GL_TEXTURE_2D, 0, 4, isclWidth, isclHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pSclData);
		}
		else
		{
			// Cleanup.
			m_iNumBitmaps--;
			m_strBmpIds[iEmpty].tbUsed = false; 
			glDeleteTextures(1, &m_textureHandles[iEmpty]);


			MemFree(m_strBmpIds[iEmpty].tstrFName);
			m_strBmpIds[iEmpty].tstrFName = 0;


			m_outLog << "\t" << gluErrorString(iResult) << "\n";
			MemFree(pData);	// Release the temporary image data.
			MemFree(pSclData);	// Release more temporary image data.
			m_outLog << "\tError loading the texture..\n";
			return -1;
		}

		MemFree(pSclData);
	}
	else
	{
		// Bind the texture to the texture object.
		glBindTexture(GL_TEXTURE_2D, m_textureHandles[iEmpty]);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, iWidth, iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pData);
	}

	if(stdTexEnv)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	// Increase the reference count.
	m_strBmpIds[iEmpty].tiRefCount++;
	
	MemFree(pData);	// Release the temporary image data.
	return iEmpty;
}

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
void CBitmapManager::UnloadImage(int iId)
{
	// Make sure the id is valid.
	if(iId < 0 || iId >= MAX_BITMAPS || !m_strBmpIds[iId].tbUsed)
		return;

	m_outLog << "\tUnloading texture: " << m_strBmpIds[iId].tstrFName << "\n";

	// Free the memory for the file name.
	MemFree(m_strBmpIds[iId].tstrFName);
	m_strBmpIds[iId].tstrFName = 0;


	// Decrease the reference count.
	m_strBmpIds[iId].tiRefCount--;

	// If the reference count goes to zero, release the texture.
	if(m_strBmpIds[iId].tiRefCount <= 0)
	{
		// One less bitmap..
		m_iNumBitmaps--;

		// Free the texture object.
		glDeleteTextures(1, &m_textureHandles[iId]);

		// This slot is now available for use.
		m_strBmpIds[iId].tbUsed = false;
	
		// Reset the reference count.
		m_strBmpIds[iId].tiRefCount = 0;
	}
}

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
void CBitmapManager::cleanup()
{
    m_iNumBitmaps = 0;

	for(int i = 0; i < MAX_BITMAPS; i++)
	{
		if(!m_strBmpIds[i].tbUsed)
			continue;

		m_outLog << "\tUnloading texture: " << m_strBmpIds[i].tstrFName << "\n";

		// Free the memory for the file name.
		MemFree(m_strBmpIds[i].tstrFName);
		m_strBmpIds[i].tstrFName = 0;

		// Free the texture object.
		glDeleteTextures(1, &m_textureHandles[i]);

		// This slot is now available for use.
		m_strBmpIds[i].tbUsed = false;
	}
}

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
bool CBitmapManager::loadTGAImage(char *strFileName, int *piWidth, int *piHeight, unsigned char **pcData)
{
	FILE *pFile;				// Used to read the file.
	unsigned int uImageSize; 	// Stores the size of the image.
	unsigned char cHeader[18];	// Stores the header.
	short xorigin, yorigin;		// Stores where the x and y origin are located.
	short w, h;					// Temporary storage of the image width and height.

	// Open the file.
	pFile = fopen(strFileName, "rb");

	// Were we able to open the file?
    if(!pFile)
	{
		m_outLog << "\t\tCould not open the file: " << strFileName << "\n";
		return false;	// No, give back nothing!
	}

	// Read in the header of the file.
	fread(&(cHeader[0]), sizeof(unsigned char), 18, pFile); 

	// Make sure that the file is an uncompressed rgb TGA file.
	if(cHeader[2] != 2)
	{
		m_outLog << "\t\tThe file is not in rgb format!\n";
		return false;	// No, get out.
	}

	// Get origin's x-coordinate.
	memcpy(&xorigin, &(cHeader[8]), sizeof(short));

	// Get origin's x-coordinate.
	memcpy(&yorigin, &(cHeader[10]), sizeof(short));

	// Get the width of the image.
	memcpy(&w, &(cHeader[12]), sizeof(short));

	// Get the height of the image.
	memcpy(&h, &(cHeader[14]), sizeof(short));

	// Make sure that there is 32 bits per pixel.
	if(cHeader[16] != 32)
	{
		m_outLog << "\t\tThis image is not using 32-bits per pixel!\n";
		return false;	// Not 32-bit per pixel, get out.
	}

	*piWidth = w;
	*piHeight = h;

	// Get the size of the image in bytes.
	uImageSize = (*piWidth) * (*piHeight) * 4;

	// Get some space for the image.
	*pcData = (unsigned char *)MemMalloc(uImageSize);
	
	// The width of the image in bits.
	int ibWid = 4*w;

	// Read in the image from bottom to top.(tga is upside-down)
	int iRetVal;		// Status returned from a file read.
	int iIndex;			// The index where data read in should be placed in memory.
	for(unsigned int i = 1; i <= uImageSize; i++)
	{
		// Reverse the row order, but keep the column order proper.
		iIndex = (( (uImageSize - i)/ibWid + 1) * ibWid) - ((uImageSize - i)%ibWid) - 1;

		// Check which color value is being read.
		if((iIndex%4)==0)
			iRetVal = (int)fread((unsigned char *)&((*pcData)[iIndex + 2]),
				(int)sizeof(unsigned char), 1, pFile); // Place the blue value in the correct spot.
		else if(iIndex%4 == 2)
			iRetVal = (int)fread((unsigned char *)&((*pcData)[iIndex - 2]),
				(int)sizeof(unsigned char), 1, pFile); // Place the red value in the correct spot.
		else
			iRetVal = (int)fread((unsigned char *)&((*pcData)[iIndex]),
				(int)sizeof(unsigned char), 1, pFile); // Do not affect placement of alpha or green values.

		// Check the file read status.
		if(iRetVal != 1)
		{
			// The header data is wrong, get out.
			m_outLog << "\t\tThe header for this image is corrupted.\n";
			MemFree(pcData);
			return false;
		}
	}

	// Close the image file.
	fclose(pFile);

	return true;
}

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
bool CBitmapManager::BindTexture(int iId)
{
	static int iLastBind = -5;
	// Make sure the id is valid.
	if(iId < 0 || iId >= (int)MAX_BITMAPS || iId >= m_iNumBitmaps)
		return false;

	if(iId == iLastBind)
		return true;

	// Bind the texture.
	glBindTexture(GL_TEXTURE_2D, m_textureHandles[iId]);
	iLastBind = iId;

	return true;
}