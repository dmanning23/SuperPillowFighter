#ifndef _EVENTHANDLER_H_
#define _EVENTHANDLER_H_
//////////////////////////////////////////////////////////////////////////////////////////
//File Name:	EventHandler.h
// 
//Author:		Allan Bittan
//
//Date Created:	8/12/04
//
//Purpose:		This is the Event Handler Class.  It will be in charge of collision responses
//				It'll also add points to players, and sub life from player
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "..\Entities\Pillow.h"
#include "..\Entities\Player.h"
#include "..\Entities\Powerup.h"
#include "..\Entities\LevelObject.h"

enum
{
	ItemPickUp_EventSound,
	ItemSplat_EventSound,
	ItemSplat3_EventSound,
	ItemSplatBell_EventSound,
	HardPillowHit_EventSound,
	MedPillowHit_EventSound,
    SoftPillow1_EventSound,
	SoftPillow2_EventSound,
	SoftPillow3_EventSound,
	SoftPillow4_EventSound,
	Num_EventSounds
};

class CEventHandler
{
	private:

		//singleont functionality of object manager
		static CEventHandler *m_pInstance;

		CEventHandler() {}
		CEventHandler(const CEventHandler &CEventHandlerRef) {}
		CEventHandler &operator=(const CEventHandler &CEventHandlerRef) {}
		~CEventHandler(void) {}

		//array of ints to holds sound ids
		//these sounds are the game defaults
		int EventSounds[Num_EventSounds];

	public:

		/*////////////////////////////////////////////////////////////////////////
		//Function:	GetInstance
		//
		//Return:	static pointer to the only instance of the event handler
		//
		//Purpose:	provide an interface for other modules to interact w/ event handler
		//
		/////////////////////////////////////////////////////////////////////////*/
		static CEventHandler *GetInstance();

		/*////////////////////////////////////////////////////////////////////////
		//Function:	DeleteInstance
		//
		//Purpose:	clean up memory used for the event handler
		//
		/////////////////////////////////////////////////////////////////////////*/
		static void DeleteInstance();

		/*////////////////////////////////////////////////////////////////////////
		//Function:	Init
		//
		//Purpose:	Prepare the event handler for a game
		//
		/////////////////////////////////////////////////////////////////////////*/
		void Init();

		/*////////////////////////////////////////////////////////////////////////
		//Function:	PillowHitPlayer
		//
		//Input:
		//	pPlayer:	the player that got hit
		//	pPillow:	the pillow that hit the player
		//
		//Purpose:	Used to determine the reaction to a pillow - player collision
		//
		/////////////////////////////////////////////////////////////////////////*/
		void PillowHitPlayer(CPillow *pPillow, CPlayer *pPlayer);

		/*////////////////////////////////////////////////////////////////////////
		//Function:	PowerupHitPlayer
		//
		//Input:
		//	pPlayer:	the player that got hit
		//	pPowerup:	the powerup that is hitting the player
		//
		//Purpose:	Used to determine the outcome of a player and powerup collision
		//
		/////////////////////////////////////////////////////////////////////////*/
		bool PowerupHitPlayer(CPowerup *pPowerup, CPlayer *pPlayer);

		/*////////////////////////////////////////////////////////////////////////
		//Function:	PlayerHitBoundary
		//
		//Input:	
		//	pPlayer:		the player doing the hitting
		//	iBoundaryID:	the boundary the player hit, as enumerated in LevelManager.h
		//
		//Purpose:	Determine the outcome of a player - boundary collision
		//
		/////////////////////////////////////////////////////////////////////////*/
		void PlayerHitBoundary(CPlayer *pPlayer, int iBoundaryID);

		/*////////////////////////////////////////////////////////////////////////
		//Function:	PlayerHitLevelObject
		//
		//Input:
		//	pPlayer:		The player who is hitting the level object
		//	pLevelObject:	the level object being hit
		//
		//Purpose:	Determine the outcome of a player - levelobject collision
		//			This function is only called the first time the player hits
		//			a level object, and NOT while a player is being ground clamped.
		//
		/////////////////////////////////////////////////////////////////////////*/
		void PlayerHitLevelObject(CPlayer *pPlayer, CLevelObject *pLevelObject);

		/*////////////////////////////////////////////////////////////////////////
		//Function:	PowerupHitLevelObject
		//
		//Input:
		//	pPowerup:		the powerup doing the hitting
		//	pLevelObject:	the level object being hit
		//
		//Purpose:	Determine the outcome of a powerup and level object collsion
		//			This function is only to be called the first time a powerup collides
		//			with a level object, and NOT while a powerup is being ground clamped.
		//
		/////////////////////////////////////////////////////////////////////////*/
		void PowerupHitLevelObject(CPowerup *pPowerup, CLevelObject *pLevelObject);

	private:

		/*////////////////////////////////////////////////////////////////////////
		//Function:	PlayerHit
		//
		//Input:
		//	pHittingObject:	the base object that is hitting the player
		//	pPlayer:		the player who is being hit
		//
		//Purpose:	This function is called by the event handler after is has
		//			been determined that the player has been hit by a damaging
		//			object.  This function determines the new velocity of the player.
		//
		/////////////////////////////////////////////////////////////////////////*/
		void PlayerHit(CBaseObject *pHittingObject, CPlayer *pPlayer);
		
		/*////////////////////////////////////////////////////////////////////////
		//Function:	GetRandomChance
		//
		//Return:	
		//	bool: whether or not the random chance was successful
		//
		//Purpose:	Used to determine if a sound should be played or not
		//
		/////////////////////////////////////////////////////////////////////////*/
		bool RandomChance();

		/*////////////////////////////////////////////////////////////////////////
		//Function:	PlayerLand
		//
		//Input:	
		//	pPlayer:		the player that is landing
		//	pLevelObject:	the level object they are landing on
		//
		//Purpose:	Called by the event handler when the player lands
		//			on a level object
		//
		/////////////////////////////////////////////////////////////////////////*/
		void PlayerLand(CPlayer *pPlayer, CLevelObject *pLevelObject);
};

#endif
