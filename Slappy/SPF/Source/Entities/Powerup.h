#ifndef _CPOWERUP_H_
#define _CPOWERUP_H_
/*////////////////////////////////////////////////////////////////////////
//File:	Powerup.h
//
//Author:	Dan Manning
//
//Creation Date:	August 12, 2004 (it's my birthday, bitch!)
//
//Purpose:	Class and function definitions for the CPowerup class
//
/////////////////////////////////////////////////////////////////////////*/

#include "baseobject.h"
#include "..\\StateMachine\\State_Manager.h"
#include "..\GameTimer\StopWatch.h"

class CPlayer;

//enumeration of all possible powerup types
enum PowerupTypes
{
	Pear_Powerup,
	Grapes_Powerup,
	IceCreamCone_Powerup,
	Pineapple_Powerup,
	BlueberryPie_Powerup,
	WholeTurkey_Powerup,
	Num_PowerupTypes
};

//Enumeration of possible powerup states
enum PowerupStates
{
	Emitted_state,
	Held_state,
	Thrown_state,
	DestroyMe_state,
	Num_PowerupStates
};

//enumeration of possible actions to send to a powerup
enum PowerupActions
{
	ItemTimer_action,
	PickedUp_action,
	ThrowItem_action,
	Collision_action,	//used when a thrown powerup collides with something
	Num_PowerupActions
};

class CPowerup : public CBaseObject
{
	private:

		//The state machine for the powerup
		CStateMachine m_StateMachine;

	public:

		//This variable holds the velocity of the player.  This is used by the physics engine
		//when the player gets hit.
		Vec4DT m_Velocity;

		//model index for this powerup
		int m_iModelIndex;

		/*////////////////////////////////////////////////////////////////////////
		//Function:	constructor
		//
		//Input:
		//itype:	the type of powerup to create as enumerated in Poweruptypes
		//
		//Purpose:	intialize a powerup with correct values
		//
		/////////////////////////////////////////////////////////////////////////*/
		CPowerup(int iType);

		virtual ~CPowerup(void);
		
		/*////////////////////////////////////////////////////////////////////////
		//Function:	Copy constructor
		//
		//Input:	reference to another CPowerup to be copied
		//
		//Purpose:	This allows one of each type of powerup to be loaded at initialization
		//	and copies of those made during runtime as we need them
		//
		/////////////////////////////////////////////////////////////////////////*/
		CPowerup(const CPowerup &PowerupRef);		

		//The timer for this powerup
		//Used for emitted state
		CStopWatch m_Timer;

		//how long this powerup lasts in the emitted_state
		//before it is removed.
		float m_fTimeToLive;

		//add doubly linked list functionality to the powerup
		CPowerup *m_pPrev;
		CPowerup *m_pNext;

		//ways to tell if this is a health item or not
		bool m_bHealthItem;

		//amount of damage or health recovery this item will do
		int m_iEffect;

		//type of powerup this is, useful for debugging
		int m_iPowerupType;

		//used for rotating the object until it hits the ground.
		bool m_bHitGround;


		//pointer to the player who is the owner of this powerup
		//The owner is the person who picked it up and/or threw it
		CPlayer *m_pPlayerOwner;

		/*////////////////////////////////////////////////////////////////////////
		//Function:	GetState
		//
		//Return:
		//int: the current state of the item, as enumerated in PowerupStates
		//
		//Purpose:	Provides a way for other modules to get at the current state of
		//			the powerup
		//
		/////////////////////////////////////////////////////////////////////////*/
		int GetState() { return m_StateMachine.GetCurrentState(); }

		/*////////////////////////////////////////////////////////////////////////
		//Function:	SendStateMessage
		//
		//Input:	
		//iAction: the message to send to the powerup
		//
		//Purpose:	Provide a way for other modules to send messages to this powerup
		//
		/////////////////////////////////////////////////////////////////////////*/
		void SendStateMessage(int iAction) { m_StateMachine.SendStateMessage(iAction); }
		
		/*////////////////////////////////////////////////////////////////////////
		//Function:	Update
		//
		//Purpose:	This updates a powerup, checking such things as whether the powerup
		//needs to be removed or what
		//
		/////////////////////////////////////////////////////////////////////////*/
		void Update();
};
#endif
