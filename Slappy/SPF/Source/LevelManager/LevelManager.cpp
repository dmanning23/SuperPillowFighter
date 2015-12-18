//////////////////////////////////////////////////////////////////////////////////////////
//File Name:		levelmanager.cpp
// 
//Author:			Kevin Cecelski
//
//Date Created:		8/20/2004
//
//Purpose:			This implements the level manager interface.
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "levelmanager.h"
#include "..\Sound\Sound.h"
#include <memory.h>

// Assists in pulling out collision line information from a hunk of data.
#define getColPtr(_data, _colnum)  ( ((char *)(_data)) + sizeof(unsigned int) + sizeof(Vec4DT) * (_colnum))

#define MAX_STRING_LENGTH 64

// Initialize the only pointer to this class.
CLevelManager *CLevelManager::m_pInstance = 0;

//////////////////////////////////////////
// Function: CLevelManager()
//
// Purpose: Default constructor.
//
// Last Modified: 8/21/2004
//
// Input: None.
//
// Output: None.
//
// Returns: true if the data was loaded, false otherwise.
//
//////////////////////////////////////////
CLevelManager::CLevelManager(void)
{
	m_bLevelLoaded = false;
	m_outLog.Init("LevelManager.txt");
	m_outLog << "Constructing the level manager.\n";

	// Initial Values.
	m_uNumSpawnPoints = 0;
	m_uNumLights = 0;
	m_NumBackTexs = 0;
	memset(m_fBoundaries, 0, sizeof(float));
	m_bLevelLoaded = false;

	// Make everything point to null.
	m_pSpawnPoints = 0;
	m_pLights = 0;
	m_pBackTexs = 0;
}

//////////////////////////////////////////
// Function: ~CLevelManager()
//
// Purpose: Default destructor.
//
// Last Modified: 8/21/2004
//
// Input: None.
//
// Output: None.
//
// Returns: true if the data was loaded, false otherwise.
//
//////////////////////////////////////////
CLevelManager::~CLevelManager(void)
{
	m_outLog << "Destroying the level manager.\n";
	m_outLog.Shutdown();
}

//////////////////////////////////////////
// Function: LoadLevel()
//
// Purpose:	 Loads a level from file.
//
// Last Modified: 8/20/2004
//
// Input: iLevelID		- the ID of the level to load.
//
// Output: None. 
//
// Returns: true if the level was loaded, false otherwise.
//
//////////////////////////////////////////
bool CLevelManager::LoadLevel(int iLevelID)
{
	if(m_bLevelLoaded)
		return false;	// A level is already loaded, do nothing.

	// Get the full path for the level.
    int iLevelPathLen = (int)strlen(strLevelPath) + (int)strlen(strLevelName[iLevelID]);
    char *plevelFilePathName = (char *)MemMalloc(sizeof(char) * (iLevelPathLen + 1));

	// Copy the full path for the model file into the appropriate 
	// array.
	strcpy(plevelFilePathName, strLevelPath);
	strcat(plevelFilePathName, strLevelName[iLevelID]);

	m_outLog << "\tAttempting to load model: \"" << plevelFilePathName << "\"\n";

	// Open the level file.
	FILE *pFile;
	pFile = fopen(plevelFilePathName, "rb");

	// Block one, level boundaries.
	if(!pFile)
	{
		// There was an error opening the file... go away.
		m_outLog << "\tThere was an error loading model: \"" << plevelFilePathName << "\"\n";
		MemFree(plevelFilePathName);
		return false;
	}

	// Read in the header.
	LevelHeaderT HeaderData;		// Storage for the header information.
	fread(&HeaderData, sizeof(LevelHeaderT), 1, pFile);
    	
	// Set member variables.
	m_outLog << "\tHeader data: \n";
	m_outLog << "\t\tNumber of level objects: " << HeaderData.tuNumLevObjs << "\n";
	int iNumLevelObjs = HeaderData.tuNumLevObjs;

	m_outLog << "\t\tNumber of spawn points: " << HeaderData.tuNumSpPoints << "\n";
	m_uNumSpawnPoints = HeaderData.tuNumSpPoints;

	m_outLog << "\t\tNumber of lights: " << HeaderData.tuNumLights << "\n";
	m_uNumLights = HeaderData.tuNumLights;

	m_outLog << "\t\tNumber of bg images: " << HeaderData.tuNumBgs << "\n";
	m_NumBackTexs = HeaderData.tuNumBgs;

	// Block one, level boundaries.
	if(!getBlock1(pFile))
		return false;

	// Block two, level objects.
	if(!getBlock2(pFile, iNumLevelObjs))
        return false;

	// Block three, spawn points.
	if(!getBlock3(pFile))
		return false;

	// Block five, lights.
	if(!getBlock5(pFile))
		return false;

	// Block six, background information.
	if(!getBlock6(pFile))
		return false;

	//Read in powerup time
	fread(&m_fPowerupTime, 1, sizeof(float), pFile);
	m_outLog << "\t\tPowerup time delta: " << m_fPowerupTime << '\n';

	// level successfully loaded.
	m_bLevelLoaded = true;

	//get number of sound effects
	fread(&m_iNumSoundEffects, sizeof(int), 1, pFile);
	m_outLog << "Number of sound effects for this level: " << m_iNumSoundEffects << '\n';

	//load sound effects for this level
	m_strSoundFileNames = (char**)MemMalloc(sizeof(char**) * m_iNumSoundEffects);
	m_iSoundIndexes = (int*)MemMalloc(sizeof(int) * m_iNumSoundEffects);
	for (int i = 0; i < m_iNumSoundEffects; i++)
	{
		m_strSoundFileNames[i] = (char*)MemMalloc(MAX_STRING_LENGTH);
		memset(m_strSoundFileNames[i], 0, MAX_STRING_LENGTH);
		fread(m_strSoundFileNames[i], sizeof(char), MAX_STRING_LENGTH, pFile);
		m_outLog << '\t' << m_strSoundFileNames[i] << '\n';
		m_iSoundIndexes[i] = CSound::GetInstance()->LoadSound(m_strSoundFileNames[i]);
	}

	//load music file name
	m_strMusicFileName = (char*)MemMalloc(MAX_STRING_LENGTH);
	memset(m_strMusicFileName, 0, MAX_STRING_LENGTH);
	fread(m_strMusicFileName, sizeof(char), MAX_STRING_LENGTH, pFile);
	m_outLog << "Music file: " << m_strMusicFileName << '\n';

	//load the water texture
	iWaterTexture = CBitmapManager::GetInstance()->LoadImage("ResourceFiles//textures//water.tga", true);

	// Load camera limits.
	fread(&m_fcameraAngle, sizeof(float), 1, pFile);
	fread(&m_MaxCameraBotHeight, sizeof(float), 1, pFile);
	fread(&m_CameraZoomCap, sizeof(float), 1, pFile);
	fread(&m_CameraZoomOutCap, sizeof(float), 1, pFile);

	// Get the water level.
	fread(&m_fWaterLevel, sizeof(float), 1, pFile);

	fclose(pFile);
	MemFree(plevelFilePathName);
	return true;
}

//////////////////////////////////////////
// Function: UnLoadLevel()
//
// Purpose:	 Unloads a level.
//
// Last Modified: 8/20/2004
//
// Input: None.
//
// Output: None. 
//
// Returns: None.
//
//////////////////////////////////////////
void CLevelManager::UnLoadLevel()
{
	if(!m_bLevelLoaded)
		return;		// No level loaded, there is nothing to do.

    // Free up the data.
	MemFree(m_pSpawnPoints);
	MemFree(m_pLights);
	MemFree(m_pBackTexs);
	for (int i = 0; i < m_iNumSoundEffects; i++)
	{
		MemFree(m_strSoundFileNames[i]);
	}
	MemFree(m_strSoundFileNames);
	MemFree(m_iSoundIndexes);
	MemFree(m_strMusicFileName);

	// Reset Values.
	m_strSoundFileNames = 0;
	m_strMusicFileName = 0;
	m_iSoundIndexes = 0;
	m_iNumSoundEffects = 0;
	m_uNumSpawnPoints = 0;
	m_uNumLights = 0;
	m_NumBackTexs = 0;
	memset(m_fBoundaries, 0, sizeof(float));
	m_bLevelLoaded = false;

	// Make everything point to null.
	m_pSpawnPoints = 0;
	m_pLights = 0;
	m_pBackTexs = 0;
}

//////////////////////////////////////////
// Function: getBlock1()
//
// Purpose:	Loads the the first block of data: the level boundaries.
//
// Last Modified: 8/21/2004
//
// Input: pFile			- pointer to the file we are reading.
//
// Output: None.
//
// Returns: true if the data was loaded, false otherwise.
//
//////////////////////////////////////////
bool CLevelManager::getBlock1(FILE *pFile)
{
	if(fread(m_fBoundaries, sizeof(float), 4, pFile) != 4)
	{
		// Something went wrong reading the boundary information.
		m_outLog << "\tError reading the boundary data.\n";
		return false;
	}

	// Output the log data.
	m_outLog << "\t\tBoundaries: \n";
	m_outLog << "\t\tLeft: " << m_fBoundaries[0] << "\n";
	m_outLog << "\t\tRight: " << m_fBoundaries[1] << "\n";
	m_outLog << "\t\tTop: " << m_fBoundaries[2] << "\n";
	m_outLog << "\t\tBottom: " << m_fBoundaries[3] << "\n";

	return true; // Alright...
}

//////////////////////////////////////////
// Function: getBlock2()
//
// Purpose:	Loads the the second block of data: the level objects.
//
// Last Modified: 8/21/2004
//
// Input: pFile			- pointer to the file we are reading.
//
// Output: None.
//
// Returns: true if the data was loaded, false otherwise.
//
//////////////////////////////////////////
bool CLevelManager::getBlock2(FILE *pFile, unsigned int iNumLevelObjs)
{
	//int iModelID;	// Store the model ID.
	//unsigned int uNumColLines;		// Store the number of collision lines.
	void *pTempCol = 0;				// Where we will temporarily store collision data.

	// Get all the level objects.
	for(unsigned int ii = 0; ii < iNumLevelObjs; ii++)
	{
		// Read in a level object.

		//Get and create the type of level oject to create
		int iTemp = 0;
		fread(&iTemp, sizeof(int), 1, pFile);
		CLevelObject *pTempLevObj = CObjectFactory::GetInstance()->CreateLevelObject(iTemp);

		// Get the orientation matirx.
		Matrix4x4T mTempOP;
		fread(&mTempOP, sizeof(Matrix4x4T), 1, pFile);
		pTempLevObj->m_PandO = mTempOP;

		// Output the matrix.
		m_outLog << "\tOrientation and position matrix for this level object: \n\t";
		for(unsigned int ik = 0; ik < 16; ik++)
		{
            if(!(ik%4) && ik != 0)
				m_outLog << "\n\t";
			m_outLog << mTempOP.fElements[(ik/4)*4+ik%4] << "\t";            
		}
		m_outLog << "\n\n";

	}
	return true;
}

//////////////////////////////////////////
// Function: getBlock3()
//
// Purpose:	Loads the the third block of data, the spawn points.
//
// Last Modified: 8/21/2004
//
// Input: pFile			- pointer to the file we are reading.
//
// Output: None.
//
// Returns: true if the data was loaded, false otherwise.
//
//////////////////////////////////////////
bool CLevelManager::getBlock3(FILE *pFile)
{
	// Make room for the spawn points.
	m_pSpawnPoints = (Vec4DT *)MemMalloc(sizeof(Vec4DT) * m_uNumSpawnPoints);
	if(fread(m_pSpawnPoints, sizeof(Vec4DT), m_uNumSpawnPoints, pFile) != 
		m_uNumSpawnPoints)
	{
		m_outLog << "\tThere was an error reading in the spawn points.\n";
		MemFree(m_pSpawnPoints);
		return false;
	}

	m_outLog << "\tSpawn points for this level: \n";
	// OutPut the spawn points.
	for(unsigned int ii = 0; ii < m_uNumSpawnPoints; ii++)
	{
		m_outLog << "\t\tSpawn point " << ii << " - (" << m_pSpawnPoints[ii].tfX << ", " 
			<< m_pSpawnPoints[ii].tfY << ", " << m_pSpawnPoints[ii].tfZ << ")\n";
	}

	return true;		// Nice
}

//////////////////////////////////////////
// Function: getBlock5()
//
// Purpose:	Loads the the fifth block of data, the light sources.
//
// Last Modified: 8/21/2004
//
// Input: pFile			- pointer to the file we are reading.
//
// Output: None.
//
// Returns: true if the data was loaded, false otherwise.
//
//////////////////////////////////////////
bool CLevelManager::getBlock5(FILE *pFile)
{
	// Make room for the emitter points.
	m_pLights = (Vec4DT *)MemMalloc(sizeof(Vec4DT) * m_uNumLights);
	if(fread(m_pLights, sizeof(Vec4DT), m_uNumLights, pFile) != 
		m_uNumLights)
	{
		m_outLog << "\tThere was an error reading in the light points.\n";
		MemFree(m_pLights);
		return false;
	}

	m_outLog << "\tLights for this level: \n";
	// OutPut the light points.
	for(unsigned int ii = 0; ii < m_uNumLights; ii++)
	{
		m_outLog << "\t\tLight " << ii << " - (" << m_pLights[ii].tfX << ", " 
			<< m_pLights[ii].tfY << ", " << m_pLights[ii].tfZ << ")\n";
	}

	return true;	
}

//////////////////////////////////////////
// Function: getBlock6()
//
// Purpose:	Loads the the sixth block of data, the background data.
//
// Last Modified: 8/21/2004
//
// Input: pFile			- pointer to the file we are reading.
//
// Output: None.
//
// Returns: true if the data was loaded, false otherwise.
//
//////////////////////////////////////////
bool CLevelManager::getBlock6(FILE *pFile)
{
	// Make room for the texture information.
	FileBckGndTexT *pTempTex =
		(FileBckGndTexT *)MemMalloc(sizeof(FileBckGndTexT) * m_NumBackTexs);
	if(fread(pTempTex, sizeof(FileBckGndTexT), m_NumBackTexs, pFile) != 
		m_NumBackTexs)
	{
		m_outLog << "\tThere was an error reading in the background data.\n";
		MemFree(pTempTex);
		return false;
	}

	// Make room for this object's internal background data.
	m_pBackTexs = (BackGroundTexT *)MemMalloc(sizeof(BackGroundTexT) * m_NumBackTexs);

	// Store the data about each background layer.
    for(unsigned int ij = 0; ij < m_NumBackTexs; ij++)
	{
		m_pBackTexs[ij].tfScrollH = pTempTex[ij].tfScrollH;
		m_pBackTexs[ij].tfScrollW = pTempTex[ij].tfScrollW;
		m_pBackTexs[ij].tiBitmapID = 
			CBitmapManager::GetInstance()->LoadImage(pTempTex[ij].tstrBitmapID, true);
		m_pBackTexs[ij].tuLayer = pTempTex[ij].tuLayer;
	}

	m_outLog << "\tTextures for this level: \n";
	// OutPut the spawn points.
	for(unsigned int ii = 0; ii < m_NumBackTexs; ii++)
	{
		m_outLog << "\t\tBackground " << ii << "\n";
		m_outLog << "\t\tLayer: " << m_pBackTexs[ii].tuLayer << "\n";
		m_outLog << "\t\tBitmap ID: " << m_pBackTexs[ii].tiBitmapID << "\n";
		m_outLog << "\t\tVertical Scrolling: " << m_pBackTexs[ii].tfScrollH << "\n";
		m_outLog << "\t\tHorizontal Scrolling: " << m_pBackTexs[ii].tfScrollW << "\n";
	}

	// Cleanup the temp data.
	MemFree(pTempTex);

	return true;	// Success.
}

/*////////////////////////////////////////////////////////////////////////
//Function:	StartAmbientSounds
//
//Purpose:	This function will start playing all the ambient sounds for
//			the level that has already been loaded
//
/////////////////////////////////////////////////////////////////////////*/
void CLevelManager::StartAmbientSounds()
{
	//play all sounds
	for (int i = 0; i < m_iNumSoundEffects; i++)
	{
		CSound::GetInstance()->PlaySoundEffect(m_iSoundIndexes[i], zeroVector, true);
	}

	//play a song
	CSound::GetInstance()->LoadMusic(m_strMusicFileName);
}