#ifndef _OBJECTMANAGER_H_
#define _OBJECTMANAGER_H_
/*////////////////////////////////////////////////////////////////////////
//File:	ObjectManager.h
//
//Author:	Dan Manning
//
//Creation Date:	August 15, 2004
//
//Purpose:	Class and function definitions for CObjectManager class
//
/////////////////////////////////////////////////////////////////////////*/
#include "..\Entities\Pillow.h"
#include "..\Entities\Player.h"
#include "..\Entities\Powerup.h"
#include "..\Entities\levelobject.h"
#include "..\GameTimer\StopWatch.h"

class CObjectManager
{
	private:

		//singleton functionality of object manager
		static CObjectManager *m_pInstance;

		//default constructor
		CObjectManager(void);

		CObjectManager(const CObjectManager &CObjectManagerRef) {}
		CObjectManager &operator=(const CObjectManager &CObjectManagerRef) {}

		//array of powerup templates
		//Used to create copies of new powerups
		CPowerup **m_PowerupTemplates;

		//Timer for testing if a new powerup should be emitted
		CStopWatch m_PowerupTimer;

		//Time deltas for creating powerups
		float m_fMaxPowerupTime;
		float m_fMinPowerupTime;

		virtual ~CObjectManager(void) {}

	public:

		/*////////////////////////////////////////////////////////////////////////
		//Function:	SetPowerupTimer
		//
		//Purpose:	Sets the powerup timer to a random time between minpoweruptime
		//			and maxpoweruptime
		//
		/////////////////////////////////////////////////////////////////////////*/
		void SetPowerupTimer();

		//number of verts held in the object manager
		unsigned int m_iNumVerts;

		//Arrays of pointers to game objects
		CPillow **m_PillowArray;
		CPlayer **m_PlayerArray;
		CLevelObject **m_LevelObjectArray;

		//the number of players in this game
		//alod used for the number of pillows in the game
		int m_iPlayerArraySize;
		int m_iLevelObjectArraySize;

		//Linked list of active powerups
		CPowerup *m_ActivePowerupList;

		//Array of dead players
		CPlayer **m_Graveyard;

	private:

		//Number of powerup emitters in this game
		int m_iNumEmitters;

		//pointers to level objects that contain emitters
		CLevelObject **m_EmitterArray;

	public:

		/*////////////////////////////////////////////////////////////////////////
		//Function:	GetInstance
		//
		//Return:
		pointer to the only instance of the object manager
		//
		//Purpose:	Provide other modules with a way to access the functionality
		//			of the object manager
		//
		/////////////////////////////////////////////////////////////////////////*/
		static CObjectManager *GetInstance();

		/*////////////////////////////////////////////////////////////////////////
		//Function:	DeleteInstance
		//
		//Purpose:	This function deletes the singleton memory allocated for the 
		//object manager
		//
		/////////////////////////////////////////////////////////////////////////*/
		static void DeleteInstance();

		/*////////////////////////////////////////////////////////////////////////
		//Function:	Shutdown
		//
		//Purpose:	Shuts down the object manager and factory
		//
		/////////////////////////////////////////////////////////////////////////*/
		void Shutdown();

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
		void Init();

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
		void KillPlayer(CPlayer *pDeadPlayer);

		/*////////////////////////////////////////////////////////////////////////
		//Function:	KillPowerup
		//
		//Input:	pPowerup:	powerup to remove from gameplay
		//
		//Purpose:	This function is used to remove a dead powerup from 
		//the list of active powerups.
		//
		/////////////////////////////////////////////////////////////////////////*/
		bool KillPowerup(CPowerup *pDeadPowerup);

		/*////////////////////////////////////////////////////////////////////////
		//Function:	SpawnPlayer
		//
		//Input:	pPlayer:	player to respawn
		//
		//Purpose:	This function will place the specified player at a random
		//spawn point.
		//
		/////////////////////////////////////////////////////////////////////////*/
		void RespawnPlayer(CPlayer *pPlayer);

		/*////////////////////////////////////////////////////////////////////////
		//Function:	Update
		//
		//Purpose:	This function updates the active players and powerups and levelobjects
		//
		/////////////////////////////////////////////////////////////////////////*/
		void Update();

		/*////////////////////////////////////////////////////////////////////////
		//Function:	AddPowerup
		//
		//Purpose:	Add a random powerup at a random emitted position
		//
		/////////////////////////////////////////////////////////////////////////*/
		void AddPowerup();
};

#endif