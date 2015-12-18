//////////////////////////////////////////////////////////////////////////////////////////
//File Name:		levelManager.h
// 
//Author:			Kevin Cecelski
//
//Date Created:		8/20/2004
//
//Purpose:			This defines the level manager interface.
//
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef _LEVELMANAGE_H_
#define _LEVELMANAGE_H_
#include "../Logger/Logger.h"
#include "../MemManager/MemManager.h"
#include "../Mathlibrary/spfmath.h"
#include "../Renderer/geometrytypes.h"
#include "../Objectmanager/objectfactory.h"
#include "../ContentLoader/GeometryImp.h"



// Enumeration for each level.
enum { PIRATEBOAT_LEVEL = 0, FERRIS_WHEEL_LEVEL, NUM_LEVELS }; 

// Associate each enumeration with a file name.
static const char *strLevelName[NUM_LEVELS] = {
	"pirateBoat.spl",		// The pirate level.
	"ferrisWheel.spl",		// The ferris wheel level.
};

// Path to where the level files exist.
static const char *strLevelPath = "ResourceFiles//Levels//";

// Enumeration for the indices into the boundary array.
enum { LEFT_BOUND = 0, RIGHT_BOUND, TOP_BOUND, BOTTOM_BOUND };

// Used to store information about the background images.
struct BackGroundTexT
{
	unsigned int	tuLayer;	// The layer where this image resides.
	int				tiBitmapID;	// The ID of the image this background uses.
	float			tfScrollH;	// The speed that the background scrolls vertically.
	float			tfScrollW;	// The speed that the background scrolls horizontally.
};

class CLevelManager
{
private:
	// Level file header information.
	struct LevelHeaderT {
		unsigned int	tuNumLevObjs;	// Number of level objects.
		unsigned int	tuNumSpPoints;	// Number of spawn points.
		unsigned int	tuNumLights;	// Number of lights.
		unsigned int	tuNumBgs;		// Number of background images.
	};

	// Used to store information about the background images.
	struct FileBckGndTexT
	{
		unsigned int	tuLayer;	// The layer where this image resides.
		char			tstrBitmapID[256];	// The ID of the image this background uses.
		float			tfScrollH;	// The speed that the background scrolls vertically.
		float			tfScrollW;	// The speed that the background scrolls horizontally.
	};

	// The only instance of this manager.
	static CLevelManager *m_pInstance;

	CLogger m_outLog;    

	// Is the level currently loaded?
	bool	m_bLevelLoaded;

	// ** Overide functions that singletons should not expose. **
		// Copy constructor.
		CLevelManager(const CLevelManager &cbm) {}		
		// Assignment operator.
		CLevelManager &operator=(const CLevelManager &cbm) {}
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
		CLevelManager(void);
	// **********************************************************

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
	bool getBlock1(FILE *pFile);

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
	bool getBlock2(FILE *pFile, unsigned int iNumLevelObjs);

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
	bool getBlock3(FILE *pFile);

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
	bool getBlock5(FILE *pFile);

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
	bool getBlock6(FILE *pFile);

public:	
	// The number of spawn points.
	unsigned int	m_uNumSpawnPoints;

	// The locations of the spawn points.
    Vec4DT			*m_pSpawnPoints;	

	// The number of lights.
	unsigned int	m_uNumLights;

	// The locations of the lights.
    Vec4DT			*m_pLights;

	// The boundaries of the level.
	float			m_fBoundaries[4];

	// The number of backgrounds.
	unsigned int	m_NumBackTexs;

	// The information about each background layer.
	BackGroundTexT	*m_pBackTexs;

	//This is how often powerup will be emitted in this level
	float m_fPowerupTime;

	//Character arrays to hold filenames of sound effects
	char **m_strSoundFileNames;

	//number of background sound effects
	int m_iNumSoundEffects;

	//indexes of loaded sound effects
	int *m_iSoundIndexes;

	//char array to hold music file
	char *m_strMusicFileName;

	//the water texture
	int iWaterTexture;

	// Camera Information.
	// The angle of the camera(in radians).
	float m_fcameraAngle;
	float m_MaxCameraBotHeight;
	float m_CameraZoomCap, m_CameraZoomOutCap;

	// Level of the water.
	float m_fWaterLevel;

	//////////////////////////////////////////
	// Function: CLevelManager()
	//
	// Purpose:	 Returns a pointer to the only instance of this class.
	//
	// Last Modified: 8/20/2004
	//
	// Input: None.
	//
	// Output: None. 
	//
	// Returns: A pointer to the only instance of this class.
	//
	//////////////////////////////////////////
	static CLevelManager *GetInstance()
	{
		if(!m_pInstance)
		{
			m_pInstance = (CLevelManager *)MemMalloc(sizeof(CLevelManager));	// Make a new instance.
			memset(m_pInstance, 0, sizeof(CLevelManager));
			m_pInstance->CLevelManager::CLevelManager();
		}

		// Give 'em that pointer.
        return m_pInstance;
	}

	//////////////////////////////////////////
	// Function: DeleteInstance()
	//
	// Purpose:	 Deletes the only instance of this class.
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
	static void DeleteInstance()
	{
		if(m_pInstance)
		{
			// Get rid of this instance.
			m_pInstance->CLevelManager::~CLevelManager();
			MemFree(m_pInstance);
			m_pInstance = 0;
		}
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
	bool LoadLevel(int iLevelID);

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
	void UnLoadLevel();

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
	~CLevelManager(void);

	/*////////////////////////////////////////////////////////////////////////
	//Function:	StartAmbientSounds
	//
	//Purpose:	This function will start playing all the ambient sounds for
	//			the level that has already been loaded
	//
	/////////////////////////////////////////////////////////////////////////*/
	void StartAmbientSounds();
};

#endif