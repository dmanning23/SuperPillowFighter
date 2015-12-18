/*////////////////////////////////////////////////////////////////////////
//File:	Powerup.cpp
//
//Author:	Dan Manning
//
//Creation Date:	august 17, 2004
//
//Purpose:	Function defintions for the CPowerup class
//
/////////////////////////////////////////////////////////////////////////*/
#include <stdio.h>
#include "powerup.h"

CPowerup::CPowerup(int iType) : m_StateMachine(Num_PowerupStates, Num_PowerupActions, Emitted_state)
{
	//set up state machine
	m_StateMachine.SetEntry(Emitted_state, ItemTimer_action, DestroyMe_state);
	m_StateMachine.SetEntry(Emitted_state, PickedUp_action, Held_state);
	m_StateMachine.SetEntry(Held_state, ThrowItem_action, Thrown_state);
	m_StateMachine.SetEntry(Thrown_state, Collision_action, Emitted_state);

	//load type specific values from correct file
	FILE *pFile = 0;
	switch (iType)
	{
		case Pear_Powerup:
			{
				pFile = fopen("ResourceFiles\\Pear.data", "rb");
				break;
			}
		case Grapes_Powerup:
			{
				pFile = fopen("ResourceFiles\\Grapes.data", "rb");
				break;
			}
		case IceCreamCone_Powerup:
			{
				pFile = fopen("ResourceFiles\\IceCreamCone.data", "rb");
				break;
			}
		case Pineapple_Powerup:
			{
				pFile = fopen("ResourceFiles\\Pineapple.data", "rb");
				break;
			}
		case BlueberryPie_Powerup:
			{
				pFile = fopen("ResourceFiles\\BlueberryPie.data", "rb");
				break;
			}
		case WholeTurkey_Powerup:
			{
				pFile = fopen("ResourceFiles\\WholeTurkey.data", "rb");
				break;
			}
	}

	if (!pFile)
	{
		//load zeros
		m_bHealthItem = 0;
		m_iEffect = 0;
		m_fTimeToLive = 0;
		m_iModelIndex = 0;
	}
	else
	{
		fread(&m_bHealthItem, sizeof(bool), 1, pFile);
		fread(&m_iEffect, sizeof(int), 1, pFile);
		fread(&m_fTimeToLive, sizeof(float), 1, pFile);
		fread(&m_iModelIndex, sizeof(int), 1, pFile);
	}

	fclose(pFile);

	// used for rotation
	m_bHitGround = false;
}

/*////////////////////////////////////////////////////////////////////////
//Function:	Copy constructor
//
//Input:	reference to another CPowerup to be copied
//
//Purpose:	This allows one of each type of powerup to be loaded at initialization
//	and copies of those made during runtime as we need them
//
/////////////////////////////////////////////////////////////////////////*/
CPowerup::CPowerup(const CPowerup &PowerupRef) : m_StateMachine(PowerupRef.m_StateMachine)
{
	//copy all parameters into this one
	m_bHealthItem = PowerupRef.m_bHealthItem;
	m_iEffect = PowerupRef.m_iEffect;
	m_fTimeToLive = PowerupRef.m_fTimeToLive;
	m_iType = PowerupRef.m_iType;

	m_bHitGround = PowerupRef.m_bHitGround;
	//start the timer
	m_Timer.Start(m_fTimeToLive);
}

CPowerup::~CPowerup(void)
{
}

/*////////////////////////////////////////////////////////////////////////
//Function:	Update
//
//Purpose:	This updates a powerup, checking such things as whether the powerup
//needs to be removed or what
//
/////////////////////////////////////////////////////////////////////////*/
void CPowerup::Update()
{
	//check if state is emitted and timer is up
	if (m_StateMachine.GetCurrentState() == Emitted_state)
	{
		//check if the timer is done
		if (m_Timer.GetRemainingTime() <= 0.0f)
		{
			m_StateMachine.SendStateMessage(ItemTimer_action);
		}
	}
}