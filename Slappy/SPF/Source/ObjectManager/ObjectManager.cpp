/*////////////////////////////////////////////////////////////////////////
//File:	objectmanager.cpp
//
//Author:	Dan Manning
//
//Creation Date:	8-15-4
//
//Purpose:	CObjectManager function definitions
//
/////////////////////////////////////////////////////////////////////////*/
#include "objectmanager.h"
#include "ObjectFactory.h"
#include "..\MemManager\MemManager.h"
#include "..\LevelManager\LevelManager.h"
#include "..\profiler\profiler.h"
#include "..\ParticleEngine\RandomNumbers.h"
#include "..\ParticleEngine\ParticleEngine.h"
#include "..\Sound\Sound.h"
#include "..\ParticleEngine\RandomNumbers.h"
#include "..\AI\AIInterface.h"

CObjectManager *CObjectManager::m_pInstance = 0;

//default constructor
CObjectManager::CObjectManager(void)
{
	//set all parameters to zero
	m_ActivePowerupList = 0;
	m_Graveyard = 0;
	m_iLevelObjectArraySize = 0;
	m_PillowArray = 0;
	m_PlayerArray = 0;
	m_PowerupTemplates = 0;
	m_iNumVerts = 0;
	m_iNumEmitters = 0;
	m_fMaxPowerupTime = 0.0f;
	m_fMinPowerupTime = 0.0f;
}

/*////////////////////////////////////////////////////////////////////////
//Function:	GetInstance
//
//Return:
//pointer to the only instance of the object manager
//
//Purpose:	Provide other modules with a way to access the functionality
//			of the object manager
//
/////////////////////////////////////////////////////////////////////////*/
CObjectManager *CObjectManager::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = (CObjectManager*)MemMalloc(sizeof(CObjectManager));
		memset(m_pInstance, 0, sizeof(CObjectManager));
		m_pInstance->CObjectManager::CObjectManager();
	}
	return m_pInstance;
}

/*////////////////////////////////////////////////////////////////////////
//Function:	DeleteInstance
//
//Purpose:	This function deletes the singleton memory allocated for the 
//object manager
//
/////////////////////////////////////////////////////////////////////////*/
void CObjectManager::DeleteInstance()
{
	//m_pInstance->Shutdown();
	m_pInstance->CObjectManager::~CObjectManager();
	MemFree(m_pInstance);
	m_pInstance = 0;
}

/*////////////////////////////////////////////////////////////////////////
//Function:	Shutdown
//
//Purpose:	Shuts down the object manager and factory
//
/////////////////////////////////////////////////////////////////////////*/
void CObjectManager::Shutdown()
{
	//delete all memory allocated to game objects

	//delete players
	for (int i = 0; i < m_iPlayerArraySize; i++)
	{
		if (m_PlayerArray[i])
		{
			MemFree(m_PlayerArray[i]->m_pGeometryData->tpVertices);
			MemFree(m_PlayerArray[i]->m_pGeometryData->tpIndices);
			MemFree(m_PlayerArray[i]->m_pGeometryData);
			MemFree(m_PlayerArray[i]->CollisionData);
			unloadAnim(m_PlayerArray[i]);
			m_PlayerArray[i]->CPlayer::~CPlayer();
			MemFree(m_PlayerArray[i]);
		}
		if (m_Graveyard[i])
		{
			MemFree(m_Graveyard[i]->m_pGeometryData->tpVertices);
			MemFree(m_Graveyard[i]->m_pGeometryData->tpIndices);
			MemFree(m_Graveyard[i]->m_pGeometryData);
			MemFree(m_Graveyard[i]->CollisionData);
			unloadAnim(m_Graveyard[i]);
			m_Graveyard[i]->CPlayer::~CPlayer();
			MemFree(m_Graveyard[i]);
		}

		////delete pillows
		////DEBUGGING
		////no pillow geometry
		MemFree(m_PillowArray[i]->m_pGeometryData->tpVertices);
		MemFree(m_PillowArray[i]->m_pGeometryData->tpIndices);
		MemFree(m_PillowArray[i]->m_pGeometryData);
		MemFree(m_PillowArray[i]->CollisionData);
		m_PillowArray[i]->CPillow::~CPillow();
		MemFree(m_PillowArray[i]);
	}

	//delete level objects
	for (int i = 0; i < m_iLevelObjectArraySize; i++)
	{
		MemFree(m_LevelObjectArray[i]->CollisionData);
		MemFree(m_LevelObjectArray[i]->m_pGeometryData->tpVertices);
		MemFree(m_LevelObjectArray[i]->m_pGeometryData->tpIndices);
		MemFree(m_LevelObjectArray[i]->m_pGeometryData);
		m_LevelObjectArray[i]->CLevelObject::~CLevelObject();
		MemFree(m_LevelObjectArray[i]);
	}

	//delete memory allocated for arrays
	MemFree(m_PlayerArray);
	MemFree(m_Graveyard);
	MemFree(m_PillowArray);
	MemFree(m_LevelObjectArray);

	//delete active powerups
	CPowerup *pIter = m_ActivePowerupList;
	while (pIter)
	{
		CPowerup *pTemp = pIter->m_pNext;
		MemFree(pIter->CollisionData);
		pIter->CPowerup::~CPowerup();
		MemFree(pIter);
		pIter = pTemp;
	}

	//delete powerup templates
	if (m_PowerupTemplates)
	{
		for (int i = 0; i < Num_PowerupTypes; i++)
		{
			MemFree(m_PowerupTemplates[i]->m_pGeometryData->tpVertices);
			MemFree(m_PowerupTemplates[i]->m_pGeometryData->tpIndices);
			MemFree(m_PowerupTemplates[i]->m_pGeometryData);
			MemFree(m_PowerupTemplates[i]->CollisionData);
			m_PowerupTemplates[i]->CPowerup::~CPowerup();
			MemFree(m_PowerupTemplates[i]);
		}
		MemFree(m_PowerupTemplates);
	}

	//delete memory allocated to hold emitters
	MemFree(m_EmitterArray);

	m_ActivePowerupList = 0;
	m_Graveyard = 0;
	m_iLevelObjectArraySize = 0;
	m_iPlayerArraySize = 0;
	m_LevelObjectArray = 0;
	m_PillowArray = 0;
	m_PlayerArray = 0;
	m_PowerupTemplates = 0;
	m_iNumVerts = 0;
	m_iNumEmitters = 0;

	memset(this, 0, sizeof(CObjectManager));
}

/*////////////////////////////////////////////////////////////////////////
//Function:	Init
//
//Input:	
//
//Purpose:	This function starts up the Object Manager by storing the 
//level data passed in, and getting the lists of objects from the Object Factory.  
//This function also places the players at the spawn points.
//
/////////////////////////////////////////////////////////////////////////*/
void CObjectManager::Init()
{
	//make sure object manager is clean
	Shutdown();

	//get number of players from object factory
	m_iPlayerArraySize = CObjectFactory::GetInstance()->m_iNumPlayers;

	//get powerup times from level manager
	m_fMinPowerupTime = (CLevelManager::GetInstance()->m_fPowerupTime - 2.0f);
	m_fMaxPowerupTime = (CLevelManager::GetInstance()->m_fPowerupTime + 2.0f);
	if (m_fMinPowerupTime < 0.0f)
	{
		m_fMinPowerupTime = 0.0f;
	}

	//allocate space for players
	//allocate graveyard
	//allocate space for pillows
	m_PlayerArray = (CPlayer**)MemMalloc(sizeof(CPlayer*) * m_iPlayerArraySize);
	memset(m_PlayerArray, 0, (sizeof(CPlayer*) * m_iPlayerArraySize));
	m_Graveyard = (CPlayer**)MemMalloc(sizeof(CPlayer*) * m_iPlayerArraySize);
	memset(m_Graveyard, 0, (sizeof(CPlayer*) * m_iPlayerArraySize));
	m_PillowArray = (CPillow**)MemMalloc(sizeof(CPillow*) * m_iPlayerArraySize);
	memset(m_PillowArray, 0, (sizeof(CPillow*) * m_iPlayerArraySize));

	//copy players into player array
	int iIndex = 0;
	for (BaseNode *pIter = CObjectFactory::GetInstance()->m_PlayerList; pIter; pIter = pIter->pNext)
	{
		m_PlayerArray[iIndex] = (CPlayer*)pIter->myObject;
		//add to number of verts
		m_iNumVerts += (m_PlayerArray[iIndex]->m_pGeometryData->tiNumIndices / 3);
		iIndex++;
	}

	//copy pillows into pillow array
	iIndex = 0;
	for (BaseNode *pIter = CObjectFactory::GetInstance()->m_PillowList; pIter; pIter = pIter->pNext)
	{
		m_PillowArray[iIndex] = (CPillow*)pIter->myObject;
		//PILLOW GEOMETRY
		m_iNumVerts += (m_PillowArray[iIndex]->m_pGeometryData->tiNumIndices / 3);
		iIndex++;
	}

	//get number of level objects from factory
	m_iLevelObjectArraySize = CObjectFactory::GetInstance()->m_iNumLevelObjects;

	//allocate space for level objects
	m_LevelObjectArray = (CLevelObject**)MemMalloc(sizeof(CLevelObject*) * m_iLevelObjectArraySize);

	//copy level objects from factory
	iIndex = 0;
	for (BaseNode *pIter = CObjectFactory::GetInstance()->m_LevelObjectList; pIter; pIter = pIter->pNext)
	{
		m_LevelObjectArray[iIndex] = (CLevelObject*)pIter->myObject;
		m_iNumVerts += (m_LevelObjectArray[iIndex]->m_pGeometryData->tiNumIndices / 3);

		//check if this level object is attached to anything
		if (m_LevelObjectArray[iIndex]->m_pOwner)
		{
			//place this object at the attachment point of the owner, 
			//as indexed by the iattached index number
			m_LevelObjectArray[iIndex]->m_PandO = identityMatrix;
			Vec4DT TempVec;
			//get a pointer to owner
			CLevelObject *pOwner = m_LevelObjectArray[iIndex]->m_pOwner;
			MatVecMult(&TempVec, pOwner->m_PandO, pOwner->m_pOptionalAttachmentPoints[m_LevelObjectArray[iIndex]->m_iAttachmentPointIndex]);
			//add offset
			setPosVec4DT(TempVec, m_LevelObjectArray[iIndex]->m_PandO);
		}

		//check if this object has an emitter attached to it
		if (m_LevelObjectArray[iIndex]->m_bHasEmitter)
		{
			m_iNumEmitters++;
		}

		iIndex++;
	}
	
	//set up emitters array
	//All level objects with emitters will be put into this array
	m_EmitterArray = (CLevelObject**)MemMalloc(sizeof(CLevelObject*) * m_iNumEmitters);

	//Go through level objects and get pointers to all objects that contain emitters
	int iEmitterIndex = 0;
	for (int i = 0; i < m_iLevelObjectArraySize; i++)
	{
		if (m_LevelObjectArray[i]->m_bHasEmitter)
		{
			m_EmitterArray[iEmitterIndex] = m_LevelObjectArray[i];
			iEmitterIndex++;
		}
	}

	//copy powerup templates from object factory
	m_PowerupTemplates = CObjectFactory::GetInstance()->m_PowerupTemplates;

	//place players at spawn points
	for (int i = 0; i < m_iPlayerArraySize; i++)
	{
		if (m_PlayerArray[i])
		{
			m_PlayerArray[i]->m_PandO = identityMatrix;
			setPosVec4DT(CLevelManager::GetInstance()->m_pSpawnPoints[i], m_PlayerArray[i]->m_PandO);
		}
	}

	//set up powerup timer
	SetPowerupTimer();
}

/*////////////////////////////////////////////////////////////////////////
//Function:	KillPlayer
//
//Input:	pDeadPlayer:	player to remove
//
//Purpose:	This function is used to remove a player from the game and 
//place them in the Graveyard array.  This function will only be needed 
//in Survival mode.	
//
/////////////////////////////////////////////////////////////////////////*/
void CObjectManager::KillPlayer(CPlayer *pDeadPlayer)
{
	//iterate player array and find player
	for (int i = 0; i < m_iPlayerArraySize; i++)
	{
		if (m_PlayerArray[i] == pDeadPlayer)
		{
			//move player into the graveyard
			m_Graveyard[i] = pDeadPlayer;

			//set this pointer to null
			m_PlayerArray[i] = 0;
		}
	}
}

/*////////////////////////////////////////////////////////////////////////
//Function:	KillPowerup
//
//Input:	pPowerup:	powerup to remove from gameplay
//
//Purpose:	This function is used to remove a dead powerup from 
//the list of active powerups.
//
/////////////////////////////////////////////////////////////////////////*/
bool CObjectManager::KillPowerup(CPowerup *pDeadPowerup)
{
	//remove pDeadPowerup from list
	if (pDeadPowerup)
	{
		if (pDeadPowerup->m_pPrev)
		{
			pDeadPowerup->m_pPrev->m_pNext = pDeadPowerup->m_pNext;
		}
		if (pDeadPowerup->m_pNext)
		{
			pDeadPowerup->m_pNext->m_pPrev = pDeadPowerup->m_pPrev;
		}
		if (pDeadPowerup == m_ActivePowerupList)
		{
			m_ActivePowerupList = pDeadPowerup->m_pNext;
		}

		//decrement poly count from total
		m_iNumVerts -= (pDeadPowerup->m_pGeometryData->tiNumIndices / 3);

		//free memory allocated to powerup
		pDeadPowerup->CPowerup::~CPowerup();
		AIInterface::GetInstance()->powerUpDied(pDeadPowerup);
		MemFree(pDeadPowerup);
		pDeadPowerup = 0;
		return true;
	}
	return false;
}

/*////////////////////////////////////////////////////////////////////////
//Function:	SpawnPlayer
//
//Input:	pPlayer:	player to respawn
//
//Purpose:	This function will place the specified player at a random
//spawn point.
//
/////////////////////////////////////////////////////////////////////////*/
void CObjectManager::RespawnPlayer(CPlayer *pPlayer)
{
	//tell the player it's dead
	pPlayer->SendStateMessage(Death_action);

	//find a random spawn point and place the player there
	int iSpawnIndex = (rand() % CLevelManager::GetInstance()->m_uNumSpawnPoints);
	pPlayer->m_PandO = identityMatrix;
	setPosVec4DT(CLevelManager::GetInstance()->m_pSpawnPoints[iSpawnIndex], pPlayer->m_PandO);
	//set player velocity to 0
	memset(&pPlayer->m_Velocity, 0, sizeof(Vec4DT));

	//kill the players powerup
	if (pPlayer->m_pPowerup)
	{
		KillPowerup(pPlayer->m_pPowerup);
	}
	pPlayer->m_pPowerup = 0;

	//set the last player to hit to 0
	pPlayer->m_pLastPlayerToHit = 0;
}

/*////////////////////////////////////////////////////////////////////////
//Function:	AddPowerup
//
//Purpose:	Add a random powerup at a random emitted position
//
/////////////////////////////////////////////////////////////////////////*/
void CObjectManager::AddPowerup()
{
	//allocate memory for a new powerup
	CPowerup *pTemp = (CPowerup*)MemMalloc(sizeof(CPowerup));
	memset(pTemp, 0, sizeof(CPowerup));

	//get an index to a powerup type
	int iType = RandomInt(0, (Num_PowerupTypes - 1));

	//copy a random powerup from the template array into the new memory
	memcpy(pTemp, m_PowerupTemplates[iType], sizeof(CPowerup));
	//pTemp->CPowerup::CPowerup(iType);
	pTemp->m_iPowerupType = iType;

	//add new powerup to linked list
	if (m_ActivePowerupList)
	{
		m_ActivePowerupList->m_pPrev = pTemp;
	}
	pTemp->m_pNext = m_ActivePowerupList;
	m_ActivePowerupList = pTemp;

	//add powerup geometry to total
	m_iNumVerts += (pTemp->m_pGeometryData->tiNumIndices / 3);

	//place powerup at an emitter point
	int iSpawnIndex = (rand() % m_iNumEmitters);

	//place this powerup at the emitter point of the level object at the spawn index
	pTemp->m_PandO = identityMatrix;
	Vec4DT TempVec;
	MatVecMult(&TempVec, m_EmitterArray[iSpawnIndex]->m_PandO, m_EmitterArray[iSpawnIndex]->m_EmitterPos);

	//add the emitter's particle effect at that position
	if (m_EmitterArray[iSpawnIndex]->m_iParticleIndex >= 0)
	{
		CParticleEngine::GetInstance()->CreateEmitter(m_EmitterArray[iSpawnIndex]->m_iParticleIndex, &TempVec);
	}

	//fix the z coord and set panda
	//TempVec.tfZ = 0.0f;
	setPosVec4DT(TempVec, pTemp->m_PandO);

	//set powerup velocity to random upward velocity
	pTemp->m_Velocity.tfY = RandomFloat(10.0f, 18.0f);
	pTemp->m_Velocity.tfX = RandomFloat(-9.0f, 12.0f);

	//start powerup time to live timer
	pTemp->m_Timer.Start(pTemp->m_fTimeToLive);

	//play the emitters emit sound
	CSound::GetInstance()->PlaySoundEffect(m_EmitterArray[iSpawnIndex]->m_iEmitPowerupSoundIndex, zeroVector);

	//reset ObjectManager::powerup timer with random number
	SetPowerupTimer();
}

/*////////////////////////////////////////////////////////////////////////
//Function:	Update
//
//Purpose:	This function updates the active players and powerups
//
/////////////////////////////////////////////////////////////////////////*/
void CObjectManager::Update()
{
	StartProfiler(UpdatePlayers_profiler);

	//update active players
	for (int i = 0; i < m_iPlayerArraySize; i++)
	{
		if (m_PlayerArray[i])
		{
			m_PlayerArray[i]->Update();
		}
	}

	StartPreviousProfiler();

	//update active powerups
	CPowerup *pIter = m_ActivePowerupList;
	while (pIter)
	{
		if (pIter->GetState() == DestroyMe_state)
		{
			//remove powerup from game
			CPowerup *pTemp = pIter->m_pNext;
			KillPowerup(pIter);
			pIter = pTemp;
			continue;
		}
		else
		{
			pIter->Update();
		}

		pIter = pIter->m_pNext;
	}

	//update levelobjects
	for (int i = 0; i < this->m_iLevelObjectArraySize; i++)
	{
		if (m_LevelObjectArray[i]->m_pOwner)
		{
			//place this object at the attachment point of the owner, 
			//as indexed by the iattached index number
			Vec4DT TempVec;
			//get a pointer to owner
			CLevelObject *pOwner = m_LevelObjectArray[i]->m_pOwner;
			MatVecMult(&TempVec, pOwner->m_PandO, pOwner->m_pOptionalAttachmentPoints[m_LevelObjectArray[i]->m_iAttachmentPointIndex]);
			//add offset
			setPosVec4DT(TempVec, m_LevelObjectArray[i]->m_PandO);
		}
	}

	//check if it is time to create a new powerup
	if (m_PowerupTimer.GetRemainingTime() <= 0.0f)
	{
		AddPowerup();
	}
}

/*////////////////////////////////////////////////////////////////////////
//Function:	SetPowerupTimer
//
//Purpose:	Sets the powerup timer to a random time between minpoweruptime
//			and maxpoweruptime
//
/////////////////////////////////////////////////////////////////////////*/
void CObjectManager::SetPowerupTimer()
{
	//get random number
	m_PowerupTimer.Start(RandomFloat(m_fMinPowerupTime, m_fMaxPowerupTime));
}