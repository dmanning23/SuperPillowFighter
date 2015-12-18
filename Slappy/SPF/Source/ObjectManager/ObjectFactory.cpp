/*////////////////////////////////////////////////////////////////////////
//File:	ObjectFactory.cpp
//
//Author:	Dan Manning
//
//Creation Date:	August 17, 2004
//
//Purpose:	Function definitions for Object factory
//
/////////////////////////////////////////////////////////////////////////*/
#include "objectfactory.h"
#include "..\MemManager\MemManager.h"
#include "..\ContentLoader\GeometryImp.h"
#include "..\ContentLoader\animImp.h"
#include "..\Sound\Sound.h"
#include "..\GameManager\GameManager.h"

CObjectFactory *CObjectFactory::m_pInstance = 0;

/*////////////////////////////////////////////////////////////////////////
//Function:	default constructor
//
//Purpose:	initailizes the object factory and logger (debug mode)
//
/////////////////////////////////////////////////////////////////////////*/
CObjectFactory::CObjectFactory(void)
{
#ifdef _DEBUG
	this->ObjLogger.Init("ObjectFactoryLogFile.txt");
	ObjLogger << "Constructor called\n";
#endif

	//set list sizes to zero
	m_iNumPlayers = 0;
	m_iNumLevelObjects = 0;

	//initialize collsion data
	//load from file
	FILE *pFile;
	pFile = fopen("ResourceFiles\\Collision.data", "rb");

	fread(&Center_Point, sizeof(float), 1, pFile);
	fread(&Center_Radius, sizeof(float), 1, pFile);
	fread(&Entire_Radius, sizeof(float), 1, pFile);
	fread(&Top_Center_Point, sizeof(float), 1, pFile);
	fread(&Top_Radius, sizeof(float), 1, pFile);
	fread(&Bottom_Center_Point, sizeof(float), 1, pFile);
	fread(&Bottom_Radius, sizeof(float), 1, pFile);
	fread(&Item_Radius, sizeof(float), 1, pFile);
	fread(&Pillow_Radius, sizeof(float), 1, pFile);

	fclose(pFile);

	//initialize lists to null
	m_PillowList = 0;
	m_LevelObjectList = 0;
	m_PlayerList = 0;
	m_PowerupTemplates = 0;
}

/*////////////////////////////////////////////////////////////////////////
//Function:	destructor
//
//Purpose:	This will check that all the memory allocated to this class
//is deleted properly.  After this function is called, DeleteInstance will need to 
//be called.
//
/////////////////////////////////////////////////////////////////////////*/
CObjectFactory::~CObjectFactory(void)
{
#ifdef _DEBUG
	//check if shutdown was called
	if (m_PillowList || m_PlayerList || m_PowerupTemplates)
	{
		ObjLogger << "WARNING: Shutdown not called before destructor!\n";
	}
	ObjLogger << "Destructor called\n";
	ObjLogger.Shutdown();
#endif
}

/*////////////////////////////////////////////////////////////////////////
//Function:	Create
//
//Input:	
//iType:	the type to create, as enumerated in BaseObject.h
//
//Return:	CBaseObject:	pointer to the base object created
//
//Purpose:	Provide functionality for a way to create game objects.
//
//NOTE:	This function WILL NOT load geometry OR collision data for any objects.
//It WILL call the constructor of the level object.
//
/////////////////////////////////////////////////////////////////////////*/
CBaseObject *CObjectFactory::Create(int iType)
{
	//allocate memory and create the correct type
	CBaseObject *pTemp = 0;

	switch (iType)
	{
		case Player_obj:
			{
				pTemp = (CBaseObject*)MemMalloc(sizeof(CPlayer));
				memset(pTemp, 0, sizeof(CPlayer));
				
				//add object to correct linked list
				BaseNode *pNewNode = (BaseNode*)MemMalloc(sizeof(BaseNode));
				pNewNode->myObject = pTemp;

				//iterate through linked list to find the last element
				if (!m_PlayerList)
				{
					pNewNode->pNext = m_PlayerList;
					m_PlayerList = pNewNode;
				}
				else
				{
					BaseNode *pIter = m_PlayerList;
					while (pIter->pNext)
					{
						pIter = pIter->pNext;
					}
					pIter->pNext = pNewNode;
					pNewNode->pNext = 0;
				}

				//load collision data
				pTemp->CollisionData = MemMalloc(sizeof(int) * 7);
				void *pVoid = pTemp->CollisionData;
				memcpy(pVoid, &Center_Point, sizeof(int));
				pVoid = ((char*)pVoid + (sizeof(int)));
				memcpy(pVoid, &Center_Radius, sizeof(int));
				pVoid = ((char*)pVoid + (sizeof(int)));
				memcpy(pVoid, &Entire_Radius, sizeof(int));
				pVoid = ((char*)pVoid + (sizeof(int)));
				memcpy(pVoid, &Top_Center_Point, sizeof(int));
				pVoid = ((char*)pVoid + (sizeof(int)));
				memcpy(pVoid, &Top_Radius, sizeof(int));
				pVoid = ((char*)pVoid + (sizeof(int)));
				memcpy(pVoid, &Bottom_Center_Point, sizeof(int));
				pVoid = ((char*)pVoid + (sizeof(int)));
				memcpy(pVoid, &Bottom_Radius, sizeof(int));

				//increment num players
				m_iNumPlayers++;

				break;
			}
		case Level_obj:
			{
				pTemp = (CBaseObject*)MemMalloc(sizeof(CLevelObject));
				memset(pTemp, 0, sizeof(CLevelObject));

				//add object to correct linked list
				BaseNode *pNewNode = (BaseNode*)MemMalloc(sizeof(BaseNode));
				pNewNode->myObject = pTemp;
				pNewNode->pNext = m_LevelObjectList;
				m_LevelObjectList = pNewNode;

				//increment num level objects
				m_iNumLevelObjects++;

				//call constrcutor
				//((CLevelObject*)pTemp)->CLevelObject::CLevelObject();

				break;
			}
		case Powerup_obj:
			{
				//This should only be called by the object factory
				pTemp = (CBaseObject*)MemMalloc(sizeof(CPowerup));
				memset(pTemp, 0, sizeof(CPowerup));
				//load collision data
				pTemp->CollisionData = MemMalloc(sizeof(int));
				memcpy(pTemp->CollisionData, &Item_Radius, sizeof(int));
				break;
			}
		case Pillow_obj:
			{
				pTemp = (CBaseObject*)MemMalloc(sizeof(CPillow));
				memset(pTemp, 0, sizeof(CPillow));

				//add object to correct linked list
				BaseNode *pNewNode = (BaseNode*)MemMalloc(sizeof(BaseNode));
				pNewNode->myObject = pTemp;
				pNewNode->pNext = m_PillowList;
				m_PillowList = pNewNode;

				//load collision data
				pTemp->CollisionData = MemMalloc(sizeof(int));
				memcpy(pTemp->CollisionData, &Pillow_Radius, sizeof(int));

				break;
			}
		default:
			{
#ifdef _DEBUG
				ObjLogger << "ERROR: Invalid type passed to Create\n";
#endif
				break;
			}
	}

	pTemp->m_iType = iType;
	return pTemp;
}

/*////////////////////////////////////////////////////////////////////////
//Function:	CreatePlayer
//
//Input:	
//PlayerStuff:	the PlayerDataT for the player to create
//
//Purpose:	This function creates a player object based on the player 
//data passed in.  This function will call the content loader to load 
//the geometry, the animation engine to load animation data, and will 
//open the correct player.txt file and load Bob Dole's stats and collision 
//data.  This function will also load and initialize each player's pillow. 
//
/////////////////////////////////////////////////////////////////////////*/
void CObjectFactory::CreatePlayer(PlayerDataT *pPlayerData)
{
	//error check
	if (pPlayerData == 0)
	{
		return;
	}

	//create the player
	CPlayer *pPlayer = (CPlayer*)Create(Player_obj);
	pPlayer->CPlayer::CPlayer(pPlayerData);

	//load player data required by other modules
	LoadGeometry(pPlayer, pPlayer->m_iModelIndex);
	loadAnim(pPlayer, pPlayer->m_iAnimationIndex);

	//SOUND
	//load player's sounds
	pPlayer->m_iUniqueSounds[PICKUP_SOUND] = CSound::GetInstance()->LoadSound(pPlayer->m_strPickupFileName);
	pPlayer->m_iUniqueSounds[VICTORY_SOUND] = CSound::GetInstance()->LoadSound(pPlayer->m_strVictoryFileName);
	pPlayer->m_iUniqueSounds[TAUNT_SOUND] = CSound::GetInstance()->LoadSound(pPlayer->m_strTauntFileName);
	pPlayer->m_iUniqueSounds[HIT_SOUND] = CSound::GetInstance()->LoadSound(pPlayer->m_strHitFileName);
	pPlayer->m_iUniqueSounds[GOTHIT_SOUND] = CSound::GetInstance()->LoadSound(pPlayer->m_strGotHitFileName);

	//create the player's pillow
	CPillow *pPillow = (CPillow*)Create(Pillow_obj);
	pPillow->CPillow::CPillow();
	pPillow->m_pOwner = pPlayer;
	pPlayer->m_pPillow = pPillow;

	//GEOMETRY
	//load pillow geometry
	LoadGeometry(pPillow, pPlayer->m_iPillowGeometry);

	//set up player for different game modes
	switch (CGameManager::GetInstance()->m_pGameData->iGameType)
	{
		case TIMEATTACK:
		{
			pPlayer->m_iScore = 0;
		}
		break;

		case KNOCKOUT:
		{
			pPlayer->m_iScore = 0;
		}
		break;
		
		//case SURVIVAL:
		//{
		//	//Knockout length is in GameManager.h
		//	pPlayer->m_iScore = KNOCKOUT_LENGTH;
		//}
		//break;
	}

	//associate the new pillow with the player
	pPlayer->m_pPillow = pPillow;
}

/*////////////////////////////////////////////////////////////////////////
//Function:	CreateLevelObject
//
//Input:	
//	iType:	the type of level object to create, as enumerated in levelobject.h
//
//Return:
//	CLevelObject*:	a pointer to the created level object
//
//Purpose:	This function will create and set up a level object of the 
//			defined type.
//
/////////////////////////////////////////////////////////////////////////*/
CLevelObject *CObjectFactory::CreateLevelObject(int iType)
{
	//create the level object
	CLevelObject *pTemp = (CLevelObject*)Create(Level_obj);
	pTemp->CLevelObject::CLevelObject(iType);

	//load the geometry of this object
	LoadGeometry(pTemp, pTemp->m_iModelIndex);

	//load the sounds of this object
	if (pTemp->m_iNumLines > 0)
	{
		pTemp->m_iPlayerLandSoundIndex = CSound::GetInstance()->LoadSound(pTemp->m_strPlayerLandFileName);
	}
	if (pTemp->m_bHasEmitter)
	{
		pTemp->m_iEmitPowerupSoundIndex = CSound::GetInstance()->LoadSound(pTemp->m_strEmitPowerupFileName);
	}

	return pTemp;
}

/*////////////////////////////////////////////////////////////////////////
//Function:	LoadItems
//
//Purpose:	This function will load all the items into the templates array.
//This function will also load all the collision data and geometry for the 
//item.
//
/////////////////////////////////////////////////////////////////////////*/
void CObjectFactory::LoadItems()
{
	//allocate memory for powerups
	this->m_PowerupTemplates = (CPowerup**)MemMalloc(sizeof(CPowerup*) * Num_PowerupTypes);
	memset(m_PowerupTemplates, 0, (sizeof(CPowerup*) * Num_PowerupTypes));

	//load all powerups
	for (int i = 0; i < Num_PowerupTypes; i++)
	{
		m_PowerupTemplates[i] = (CPowerup*)Create(Powerup_obj);
		m_PowerupTemplates[i]->CPowerup::CPowerup(i);

		//load geometry data
		LoadGeometry(m_PowerupTemplates[i], m_PowerupTemplates[i]->m_iModelIndex);
	}
}

/*////////////////////////////////////////////////////////////////////////
//Function:	Shutdown
//
//Purpose:	Clean up all memory allocated for list nodes
//
/////////////////////////////////////////////////////////////////////////*/
void CObjectFactory::Shutdown()
{
#ifdef _DEBUG
	ObjLogger << "Number of players generated: " << m_iNumPlayers << '\n';
	ObjLogger << "Number of level objects generated: " << m_iNumLevelObjects << '\n';
#endif

	//clean up nodes for player list
	BaseNode *pIter;
	pIter = m_PlayerList;
	while (pIter)
	{
		BaseNode *pTemp = pIter;
		pIter = pIter->pNext;
		m_PlayerList = pIter;
		MemFree(pTemp);
	}

	//clean up pillow list
	pIter = m_PillowList;
	while (pIter)
	{
		BaseNode *pTemp = pIter;
		pIter = pIter->pNext;
		m_PillowList = pIter;
		MemFree(pTemp);
	}

	//clean up level object list
	pIter = m_LevelObjectList;
	while (pIter)
	{
		BaseNode *pTemp = pIter;
		pIter = pIter->pNext;
		m_LevelObjectList = pIter;
		MemFree(pTemp);
	}

	//set list sizes to zero
	m_iNumPlayers = 0;
	m_iNumLevelObjects = 0;

	//initialize lists to null
	m_PillowList = 0;
	m_LevelObjectList = 0;
	m_PlayerList = 0;
	m_PowerupTemplates = 0;
}

/*////////////////////////////////////////////////////////////////////////
//Function:	GetInstance
//
//Return:	static pointer to the only instance of the object factory
//
//Purpose:	Provide other modules with a way to access the functionality 
//of the object factory
//
/////////////////////////////////////////////////////////////////////////*/
CObjectFactory *CObjectFactory::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = (CObjectFactory*)MemMalloc(sizeof(CObjectFactory));
		m_pInstance->CObjectFactory::CObjectFactory();
	}
	return m_pInstance;
}

/*////////////////////////////////////////////////////////////////////////
//Function:	DeleteInstance
//
//Purpose:	clean up all memory used for the object factory singleton
//
/////////////////////////////////////////////////////////////////////////*/
void CObjectFactory::DeleteInstance()
{
	m_pInstance->Shutdown();
	m_pInstance->CObjectFactory::~CObjectFactory();
	MemFree(m_pInstance);
	m_pInstance = 0;
}