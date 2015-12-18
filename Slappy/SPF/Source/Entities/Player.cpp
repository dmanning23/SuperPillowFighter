/*////////////////////////////////////////////////////////////////////////
//File:	Player.cpp
//
//Author:	Dan Manning
//
//Creation Date:	8-9-4
//
//Purpose:	function definitoins for player class
//
/////////////////////////////////////////////////////////////////////////*/

#include <stdio.h>

#include "player.h"
#include "..\\ObjectManager\\ObjectManager.h"
#include "..\\ParticleEngine\\ParticleEngine.h"
#include "..\Sound\Sound.h"

#define MAX_STRING_LENGTH	64
#define POWERUP_VEL			20.0f

/*////////////////////////////////////////////////////////////////////////
//Function:	Constructor
//
//Purpose:	This sets up a player and all member variables, including instances
//			of other classes.
//
/////////////////////////////////////////////////////////////////////////*/
CPlayer::CPlayer(PlayerDataT *PlayerData) : m_PlayerStateMachine(Num_Player_States, Num_Player_Actions, Standing_state)
{
	this->m_pPlayerData = PlayerData;

	//set up state machine
	//Entries that are commented out can go to multiple states
	m_PlayerStateMachine.SetEntry(Standing_state, Left_action, Walking_state);
	m_PlayerStateMachine.SetEntry(Standing_state, Right_action, Walking_state);
	m_PlayerStateMachine.SetEntry(Standing_state, Block_action, Blocking_state);
	m_PlayerStateMachine.SetEntry(Standing_state, Hit_action, Falling_state);
	m_PlayerStateMachine.SetEntry(Standing_state, Jump_action, PhysicsUp_state);
	m_PlayerStateMachine.SetEntry(Standing_state, Death_action, Dead_state);
	m_PlayerStateMachine.SetEntry(Standing_state, Attack_action, AttackTransition_state);
	//m_PlayerStateMachine.SetEntry(Standing_state, DownAttack_action, _state);
	m_PlayerStateMachine.SetEntry(Standing_state, UpAttack_action, UpAttack_state);
	m_PlayerStateMachine.SetEntry(Standing_state, ForwardAttack_action, ForwardAttack_state);
	m_PlayerStateMachine.SetEntry(Walking_state, Left_action, Walking_state);
	m_PlayerStateMachine.SetEntry(Walking_state, Right_action, Walking_state);
	m_PlayerStateMachine.SetEntry(Walking_state, Block_action, Blocking_state);
	m_PlayerStateMachine.SetEntry(Walking_state, Hit_action, Falling_state);
	m_PlayerStateMachine.SetEntry(Walking_state, Jump_action, PhysicsUp_state);
	m_PlayerStateMachine.SetEntry(Walking_state, Death_action, Dead_state);
	m_PlayerStateMachine.SetEntry(Walking_state, Attack_action, AttackTransition_state);
	//m_PlayerStateMachine.SetEntry(Walking_state, DownAttack_action, _state);
	m_PlayerStateMachine.SetEntry(Walking_state, UpAttack_action, UpAttack_state);
	m_PlayerStateMachine.SetEntry(Walking_state, ForwardAttack_action, ForwardAttack_state);
	m_PlayerStateMachine.SetEntry(Walking_state, Neutral_action, Standing_state);
	//m_PlayerStateMachine.SetEntry(Jumping_state, Left_action, PhysicsLeft_state);
	//m_PlayerStateMachine.SetEntry(Jumping_state, Right_action, PhysicsRight_state);
	m_PlayerStateMachine.SetEntry(Jumping_state, Hit_action, Falling_state);
	m_PlayerStateMachine.SetEntry(Jumping_state, Jump_action, PhysicsUp_state);
	m_PlayerStateMachine.SetEntry(Jumping_state, Death_action, Dead_state);
	m_PlayerStateMachine.SetEntry(Jumping_state, Attack_action, AttackTransition_state);
	m_PlayerStateMachine.SetEntry(Jumping_state, DownAttack_action, AirDownAttack_state);
	m_PlayerStateMachine.SetEntry(Jumping_state, UpAttack_action, AirUpAttack_state);
	m_PlayerStateMachine.SetEntry(Jumping_state, ForwardAttack_action, AirForwardAttack_state);
	m_PlayerStateMachine.SetEntry(Jumping_state, GroundCollision_action, Standing_state);
	//m_PlayerStateMachine.SetEntry(DoubleJumping_state, Left_action, PhysicsLeft_state);
	//m_PlayerStateMachine.SetEntry(DoubleJumping_state, Right_action, PhysicsRight_state);
	m_PlayerStateMachine.SetEntry(DoubleJumping_state, Hit_action, Falling_state);
	m_PlayerStateMachine.SetEntry(DoubleJumping_state, Death_action, Dead_state);
	m_PlayerStateMachine.SetEntry(DoubleJumping_state, Attack_action, AttackTransition_state);
	m_PlayerStateMachine.SetEntry(DoubleJumping_state, DownAttack_action, AirDownAttack_state);
	m_PlayerStateMachine.SetEntry(DoubleJumping_state, UpAttack_action, AirUpAttack_state);
	m_PlayerStateMachine.SetEntry(DoubleJumping_state, ForwardAttack_action, AirForwardAttack_state);
	m_PlayerStateMachine.SetEntry(DoubleJumping_state, GroundCollision_action, Standing_state);
	m_PlayerStateMachine.SetEntry(Attack_state, Hit_action, Falling_state);
	m_PlayerStateMachine.SetEntry(Attack_state, Death_action, Dead_state);
	m_PlayerStateMachine.SetEntry(Attack_state, Timer_action, Standing_state);
	m_PlayerStateMachine.SetEntry(ForwardAttack_state, Hit_action, Falling_state);
	m_PlayerStateMachine.SetEntry(ForwardAttack_state, Death_action, Dead_state);
	m_PlayerStateMachine.SetEntry(ForwardAttack_state, Timer_action, Standing_state);
	m_PlayerStateMachine.SetEntry(SpecialAttack_state, Hit_action, Falling_state);
	m_PlayerStateMachine.SetEntry(SpecialAttack_state, Death_action, Dead_state);
	m_PlayerStateMachine.SetEntry(SpecialAttack_state, Timer_action, Standing_state);
	m_PlayerStateMachine.SetEntry(UpAttack_state, Hit_action, Falling_state);
	m_PlayerStateMachine.SetEntry(UpAttack_state, Death_action, Dead_state);
	m_PlayerStateMachine.SetEntry(UpAttack_state, Timer_action, Standing_state);
	m_PlayerStateMachine.SetEntry(AirSpecialAttack_state, Hit_action, Falling_state);
	m_PlayerStateMachine.SetEntry(AirSpecialAttack_state, Death_action, Dead_state);
	//m_PlayerStateMachine.SetEntry(AirSpecialAttack_state, Timer_action, _state);
	m_PlayerStateMachine.SetEntry(AirSpecialAttack_state, GroundCollision_action, Standing_state);
	m_PlayerStateMachine.SetEntry(AirDownAttack_state, Hit_action, Falling_state);
	m_PlayerStateMachine.SetEntry(AirDownAttack_state, Death_action, Dead_state);
	//m_PlayerStateMachine.SetEntry(AirDownAttack_state, Timer_action, _state);
	m_PlayerStateMachine.SetEntry(AirDownAttack_state, GroundCollision_action, Standing_state);
	m_PlayerStateMachine.SetEntry(AirUpAttack_state, Hit_action, Falling_state);
	m_PlayerStateMachine.SetEntry(AirUpAttack_state, Death_action, Dead_state);
	//m_PlayerStateMachine.SetEntry(AirUpAttack_state, Timer_action, _state);
	m_PlayerStateMachine.SetEntry(AirUpAttack_state, GroundCollision_action, Standing_state);
	m_PlayerStateMachine.SetEntry(AirForwardAttack_state, Hit_action, Falling_state);
	m_PlayerStateMachine.SetEntry(AirForwardAttack_state, Death_action, Dead_state);
	//m_PlayerStateMachine.SetEntry(AirForwardAttack_state, Timer_action, _state);
	m_PlayerStateMachine.SetEntry(AirForwardAttack_state, GroundCollision_action, Standing_state);
	m_PlayerStateMachine.SetEntry(Blocking_state, Death_action, Dead_state);
	m_PlayerStateMachine.SetEntry(Blocking_state, Neutral_action, Standing_state);
	m_PlayerStateMachine.SetEntry(Falling_state, Hit_action, Falling_state);
	m_PlayerStateMachine.SetEntry(Falling_state, Death_action, Dead_state);
	m_PlayerStateMachine.SetEntry(Falling_state, Timer_action, Jumping_state);
	m_PlayerStateMachine.SetEntry(Dead_state, EndTransitionState_action, Jumping_state);
	m_PlayerStateMachine.SetEntry(PhysicsUp_state, Hit_action, Falling_state);
	m_PlayerStateMachine.SetEntry(PhysicsUp_state, Death_action, Dead_state);
	//m_PlayerStateMachine.SetEntry(PhysicsUp_state, EndTransitionState_action, _state);
	m_PlayerStateMachine.SetEntry(PhysicsLeft_state, Hit_action, Falling_state);
	m_PlayerStateMachine.SetEntry(PhysicsLeft_state, Death_action, Dead_state);
	//m_PlayerStateMachine.SetEntry(PhysicsLeft_state, EndTransitionState_action, _state);
	m_PlayerStateMachine.SetEntry(PhysicsRight_state, Hit_action, Falling_state);
	m_PlayerStateMachine.SetEntry(PhysicsRight_state, Death_action, Dead_state);
	//m_PlayerStateMachine.SetEntry(PhysicsRight_state, EndTransitionState_action, _state);
	m_PlayerStateMachine.SetEntry(Throwing_state, Hit_action, Falling_state);
	m_PlayerStateMachine.SetEntry(Throwing_state, Death_action, Dead_state);
	//m_PlayerStateMachine.SetEntry(Throw_state, Timer_action, _state);
	m_PlayerStateMachine.SetEntry(AttackTransition_state, Hit_action, Falling_state);
	m_PlayerStateMachine.SetEntry(AttackTransition_state, Death_action, Dead_state);
	//m_PlayerStateMachine.SetEntry(AttackTransition_state, EndTransitionState_action, _state);

	//load player stats from players.txt
	LoadPlayer();
}

/*////////////////////////////////////////////////////////////////////////
//Function:	Destructor
//
//Purpose:	This shuts down a player, including instances of other classes.
//		
/////////////////////////////////////////////////////////////////////////*/
CPlayer::~CPlayer(void)
{
}

/*////////////////////////////////////////////////////////////////////////
//Function:	Update
//
//Purpose:	This function updates the player.  It will be called every frame.
//	This function will do such things as update the state machine and check the timer.
//
/////////////////////////////////////////////////////////////////////////*/
void CPlayer::Update()
{
	//if timer is over, send Timer_action and turn off timer
	if (m_PlayerTimer.GetActive() && m_PlayerTimer.GetRemainingTime() <= 0.0f)
	{
		SendStateMessage(Timer_action);
	}

	//if state == dead, remove my powerup from game
	if (m_PlayerStateMachine.GetCurrentState() == Dead_state)
	{
		//remove my powerup from the game
		CObjectManager::GetInstance()->KillPowerup(m_pPowerup);
	}

	////DEBUGGING
	////attach the player's powerup to the player
	//if (m_pPowerup)
	//{
	//	memcpy(&m_pPowerup->m_PandO, &m_PandO, sizeof(Matrix4x4T));
	//}

	////DEBUGGING
	////put the players pillow in front of them
	//memcpy(&m_pPillow->m_PandO, &m_PandO, sizeof(Matrix4x4T));
	//Vec4DT PillowLookAt;
	//Vec4DT PillowPos;
	//getLookAtVec4DT(&PillowLookAt, m_pPillow->m_PandO);
	//getPosVec4DT(&PillowPos, m_pPillow->m_PandO);

	//addVec4DT(&PillowPos, PillowPos, PillowLookAt);
	//PillowPos.tfY += .75f;
	//setPosVec4DT(PillowPos, m_pPillow->m_PandO);
    

	//send EndTransitionState_action to get the player out of the transition state
	//This sets up the player for animating and rendering
	SendStateMessage(EndTransitionState_action);
}

/*////////////////////////////////////////////////////////////////////////
//Function:	SendStateMessage
//
//Input:	
//ActionMessage:
//The action to send to the player
//
//Purpose:	This function provides the functionality for other modules
//to send state actions to the players.
//
/////////////////////////////////////////////////////////////////////////*/
void CPlayer::SendStateMessage(int iActionMessage)
{
	//get current state
	int iCurrentState = m_PlayerStateMachine.GetCurrentState();

	//process new state message
	switch (iActionMessage)
	{
		case DownAttack_action:
			{
				//check for standing and walking states
				//This needs to check if the player has an item (should throw it)
				switch (iCurrentState)
				{
					case Standing_state:
						{
							//check if should throw or do a ground attack
							if (m_pPowerup && m_pPowerup->GetState() == Held_state)
							{
								ThrowPowerup();
							}
							else
							{
								//or do a groud special attack
								m_PlayerStateMachine.ForceState(SpecialAttack_state);

								//set timer
								m_PlayerTimer.Start(m_fAttackSpeed);

								//turn pillow on
								m_pPillow->m_bActive = true;
							}
							break;
						}
					case Walking_state:
						{
							//check if player should go to throwing state
							if (m_pPowerup && m_pPowerup->GetState() == Held_state)
							{
								ThrowPowerup();
							}
							else
							{
								//or do a groud special attack
								m_PlayerStateMachine.ForceState(SpecialAttack_state);

								//set timer
								m_PlayerTimer.Start(m_fAttackSpeed);

								//turn pillow on
								m_pPillow->m_bActive = true;
							}
							break;
						}
					case Jumping_state:
						{
							m_PlayerStateMachine.SendStateMessage(iActionMessage);

							//set timer
							m_PlayerTimer.Start(m_fAttackSpeed);

							//turn pillow on
							m_pPillow->m_bActive = true;

							break;
						}
					case DoubleJumping_state:
						{
							m_PlayerStateMachine.SendStateMessage(iActionMessage);

							//set timer
							m_PlayerTimer.Start(m_fAttackSpeed);

							//turn pillow on
							m_pPillow->m_bActive = true;

							break;
						}
					default:
						{
							//if it isn't any of those states, send it to state machine
							m_PlayerStateMachine.SendStateMessage(iActionMessage);
							break;
						}
				}
				break;
			}
		case UpAttack_action:
			{
				switch (iCurrentState)
				{
					case Standing_state:
						{
							m_PlayerStateMachine.SendStateMessage(iActionMessage);

							//set timer
							m_PlayerTimer.Start(m_fAttackSpeed);

							//turn pillow on
							m_pPillow->m_bActive = true;

							break;
						}
					case Walking_state:
						{
							m_PlayerStateMachine.SendStateMessage(iActionMessage);

							//set timer
							m_PlayerTimer.Start(m_fAttackSpeed);

							//turn pillow on
							m_pPillow->m_bActive = true;

							break;
						}
					case Jumping_state:
						{
							m_PlayerStateMachine.SendStateMessage(iActionMessage);

							//set timer
							m_PlayerTimer.Start(m_fAttackSpeed);

							//turn pillow on
							m_pPillow->m_bActive = true;

							break;
						}
					case DoubleJumping_state:
						{
							m_PlayerStateMachine.SendStateMessage(iActionMessage);

							//set timer
							m_PlayerTimer.Start(m_fAttackSpeed);

							//turn pillow on
							m_pPillow->m_bActive = true;

							break;
						}
					default:
						{
							m_PlayerStateMachine.SendStateMessage(iActionMessage);
							break;
						}
				}
				break;
			}
		case ForwardAttack_action:
			{
				switch (iCurrentState)
				{
					case Standing_state:
							//check if should throw or do a ground attack
							if (m_pPowerup && m_pPowerup->GetState() == Held_state)
							{
								ThrowPowerup();
							}
							else
							{
								//or do a groud special attack
								m_PlayerStateMachine.ForceState(ForwardAttack_state);

								//set timer
								m_PlayerTimer.Start(m_fAttackSpeed);

								//turn pillow on
								m_pPillow->m_bActive = true;
							}
							break;
					case Walking_state:
							//check if should throw or do a ground attack
							if (m_pPowerup && m_pPowerup->GetState() == Held_state)
							{
								ThrowPowerup();
							}
							else
							{
								//or do a groud special attack
								m_PlayerStateMachine.ForceState(ForwardAttack_state);

								//set timer
								m_PlayerTimer.Start(m_fAttackSpeed);

								//turn pillow on
								m_pPillow->m_bActive = true;
							}
							break;
					case Jumping_state:
						{
							m_PlayerStateMachine.SendStateMessage(iActionMessage);

							//set timer
							m_PlayerTimer.Start(m_fAttackSpeed);

							//turn pillow on
							m_pPillow->m_bActive = true;

							break;
						}
					case DoubleJumping_state:
						{
							m_PlayerStateMachine.SendStateMessage(iActionMessage);

							//set timer
							m_PlayerTimer.Start(m_fAttackSpeed);

							//turn pillow on
							m_pPillow->m_bActive = true;

							break;
						}
					default:
						{
							m_PlayerStateMachine.SendStateMessage(iActionMessage);
							break;
						}
				}
				break;
			}

		case Timer_action:
			{
				//turn pillow OFF
				m_pPillow->m_bActive = false;

				//turn timer OFF
				m_PlayerTimer.KillTimer();

				//check for air attack states, throwing state
				switch (iCurrentState)
				{
					case AirSpecialAttack_state:
						{
							//check old state, go to either jumping or double jumping depending
							if (m_iPreviousState == Jumping_state)
							{
								m_PlayerStateMachine.ForceState(Jumping_state);
							}
							else
							{
								m_PlayerStateMachine.ForceState(DoubleJumping_state);
							}
							break;
						}
					case AirUpAttack_state:
						{
							//check old state, go to either jumping or double jumping depending
							if (m_iPreviousState == Jumping_state)
							{
								m_PlayerStateMachine.ForceState(Jumping_state);
							}
							else
							{
								m_PlayerStateMachine.ForceState(DoubleJumping_state);
							}
							break;
						}
					case AirDownAttack_state:
						{
							//check old state, go to either jumping or double jumping depending
							if (m_iPreviousState == Jumping_state)
							{
								m_PlayerStateMachine.ForceState(Jumping_state);
							}
							else
							{
								m_PlayerStateMachine.ForceState(DoubleJumping_state);
							}
							break;
						}
					case AirForwardAttack_state:
						{
							//check old state, go to either jumping or double jumping depending

							if (m_iPreviousState == Jumping_state)
							{
								m_PlayerStateMachine.ForceState(Jumping_state);
							}
							else
							{
								m_PlayerStateMachine.ForceState(DoubleJumping_state);
							}
							break;
						}
					case Throwing_state:
						{
							//check old state, go to either jumping or double jumping depending
							if (m_iPreviousState == Jumping_state)
							{
								m_PlayerStateMachine.ForceState(Jumping_state);
							}
							else if (m_iPreviousState == Standing_state || m_iPreviousState == Walking_state)
							{
								m_PlayerStateMachine.ForceState(Standing_state);
							}
							else
							{
								m_PlayerStateMachine.ForceState(DoubleJumping_state);
							}
							break;
						}
					case Falling_state:
						{
							if (m_iPreviousState == DoubleJumping_state)
							{
								m_PlayerStateMachine.ForceState(DoubleJumping_state);
							}
							else
							{
								m_PlayerStateMachine.ForceState(Jumping_state);
							}
							break;
						}
					default:
						{
							m_PlayerStateMachine.SendStateMessage(iActionMessage);
							break;
						}
				}
				break;
			}
		case EndTransitionState_action:
			{
				//check for physics states, AttackTransitionState
				switch  (iCurrentState)
				{
					case PhysicsLeft_state:
						{
							if (m_iPreviousState == Standing_state || m_iPreviousState == Walking_state)
							{
								m_PlayerStateMachine.ForceState(Walking_state);
							}
							else if (m_iPreviousState == Jumping_state)
							{
								m_PlayerStateMachine.ForceState(Jumping_state);
							}
							else if (m_iPreviousState == DoubleJumping_state)
							{
								m_PlayerStateMachine.ForceState(DoubleJumping_state);
							}
							break;
						}
					case PhysicsRight_state:
						{
							if (m_iPreviousState == Standing_state || m_iPreviousState == Walking_state)
							{
								m_PlayerStateMachine.ForceState(Walking_state);
							}
							else if (m_iPreviousState == Jumping_state)
							{
								m_PlayerStateMachine.ForceState(Jumping_state);
							}
							else if (m_iPreviousState == DoubleJumping_state)
							{
								m_PlayerStateMachine.ForceState(DoubleJumping_state);
							}
							break;
						}
					case PhysicsUp_state:
						{
							if (m_iPreviousState == Standing_state || m_iPreviousState == Walking_state)
							{
								m_PlayerStateMachine.ForceState(Jumping_state);
							}
							else if (m_iPreviousState == Jumping_state)
							{
								m_PlayerStateMachine.ForceState(DoubleJumping_state);
							}
							else if (m_iPreviousState == DoubleJumping_state)
							{
								m_PlayerStateMachine.ForceState(DoubleJumping_state);
							}
							break;
						}
					case AttackTransition_state:
						{
							//check previous state for standing, walking, jumping, and double jumping
							//check if there is an item in the jumping and djumping state
							if (m_iPreviousState == Standing_state || m_iPreviousState == Walking_state)
							{
								m_PlayerStateMachine.ForceState(Attack_state);

								//set timer
								m_PlayerTimer.Start(m_fAttackSpeed);

								//turn pillow on
								m_pPillow->m_bActive = true;
							}
							else if (m_iPreviousState == Jumping_state || m_iPreviousState == DoubleJumping_state)
							{
								//check if player should go to throwing state
								if (m_pPowerup)
								{
									ThrowPowerup();
								}
								else
								{
									m_PlayerStateMachine.ForceState(AirSpecialAttack_state);

									//set timer
									m_PlayerTimer.Start(m_fAttackSpeed);

									//turn pillow on
									m_pPillow->m_bActive = true;
								}
							}
							break;
						}
					default:
						{
							m_PlayerStateMachine.SendStateMessage(iActionMessage);
							break;
						}
				}
				break;
			}
		case Hit_action:
			{
				//turn timer OFF
				m_PlayerTimer.KillTimer();

				//start the player timer for the "falling" state
				m_PlayerTimer.Start(FALLING_TIME);
				
				//turn pillow OFF
				m_pPillow->m_bActive = false;

				m_PlayerStateMachine.SendStateMessage(iActionMessage);
				break;
			}
		case Death_action:
			{
				//turn timer OFF
				m_PlayerTimer.KillTimer();
				
				//turn pillow OFF
				m_pPillow->m_bActive = false;

				m_PlayerStateMachine.ForceState(Jumping_state);
				break;
			}
		case GroundCollision_action:
			{
				//check if player is landing from a jump
				if (iCurrentState == Jumping_state || iCurrentState == DoubleJumping_state ||
					iCurrentState == AirSpecialAttack_state || iCurrentState == AirForwardAttack_state ||
					iCurrentState == AirUpAttack_state || iCurrentState == AirDownAttack_state)
				{
					//turn timer OFF
					m_PlayerTimer.KillTimer();

					//turn pillow off
					m_pPillow->m_bActive = false;
				}

				m_PlayerStateMachine.SendStateMessage(iActionMessage);
				break;
			}
		default:
			{
				m_PlayerStateMachine.SendStateMessage(iActionMessage);
				break;
			}
			
	}

	//check if the transition state is fucking shit up
	if (iActionMessage != EndTransitionState_action 
		&& iCurrentState < Dead_state)
	{
		//set old state
		m_iPreviousState = iCurrentState;
	}
}

/*////////////////////////////////////////////////////////////////////////
//Function:	LoadPlayer
//
//Purpose:	
//This function will load all the player stats from the correct file,
//based on the player id in the m_pPlayerData.
//
/////////////////////////////////////////////////////////////////////////*/
void CPlayer::LoadPlayer()
{
	m_iPreviousState = Jumping_state;

	FILE *pFile = 0;
	
	switch (m_pPlayerData->iCharacter)
	{
		case Josie_Player:
			{
				pFile = fopen("ResourceFiles\\Josie.data", "rb");
				break;
			}
		case CaptainSally_Player:
			{
				pFile = fopen("ResourceFiles\\CaptainSally.data", "rb");
				break;
			}
		case MissElizabeth_Player:
			{
				pFile = fopen("ResourceFiles\\MissElizabeth.data", "rb");
				break;
			}
		case Sumi_Player:
			{
				pFile = fopen("ResourceFiles\\Sumi.data", "rb");
				break;
			}
	}

	if (!pFile)
	{
		m_fAttackSpeed = 0;
		m_fWalkSpeed = 0.0f;
		m_fJumpHeight = 0.0f;
		m_iMass = 0;
		m_iAttackPower = 0;
	}
	else
	{
		//fill player stuff with data from the open file
		fread(&m_fAttackSpeed, sizeof(float), 1, pFile);
		fread(&m_fWalkSpeed, sizeof(float), 1, pFile);
		fread(&m_fJumpHeight, sizeof(float), 1, pFile);
		fread(&m_iMass, sizeof(int), 1, pFile);
		fread(&m_iAttackPower, sizeof(int), 1, pFile);
		fread(&m_iModelIndex, sizeof(int), 1, pFile);
		fread(&m_iAnimationIndex, sizeof(int), 1, pFile);

		//load pillow stuff
		fread(&m_iPillowGeometry, sizeof(int), 1, pFile);

		//read in sound data
		fread(&m_strPickupFileName, MAX_STRING_LENGTH, 1, pFile);
		fread(&m_strVictoryFileName, MAX_STRING_LENGTH, 1, pFile);
		fread(&m_strTauntFileName, MAX_STRING_LENGTH, 1, pFile);
		fread(&m_strHitFileName, MAX_STRING_LENGTH, 1, pFile);
		fread(&m_strGotHitFileName, MAX_STRING_LENGTH, 1, pFile);
	}

	//close file
	fclose(pFile);
}

/*////////////////////////////////////////////////////////////////////////
//Function:	PickUpPowerup
//
//Input:	pPowerup:	the powerup to pick up
//
//Purpose:	provide a streamlined way for other modules to get a player to 
//			hold a powerup
//
/////////////////////////////////////////////////////////////////////////*/
bool CPlayer::PickUpPowerup(CPowerup *pPowerup)
{
	if (!m_pPowerup)
	{
		//check for health powerup
		if (pPowerup->m_bHealthItem)
		{
			//add health to player
			m_iDamage -= pPowerup->m_iEffect;
			if (m_iDamage < 0)
			{
				m_iDamage = 0;
			}

			//add "healthpickup" particle effect to player
			CParticleEngine::GetInstance()->CreateEmitter(HealthPickup_ParticleEffect, 0, 0, this);

			//play "healthup" sound
			CSound::GetInstance()->PlaySoundEffect(m_iUniqueSounds[PICKUP_SOUND]);

			//remove powerup from game
			CObjectManager::GetInstance()->KillPowerup(pPowerup);
			return false;
		}
		else
		{
			//set pointers
			m_pPowerup = pPowerup;
			pPowerup->m_pPlayerOwner = this;

			//send a message to the powerup
			pPowerup->SendStateMessage(PickedUp_action);
			return true;
		}
	}
	else
	{
		return false;
	}
}

/*////////////////////////////////////////////////////////////////////////
//Function:	ThrowPowerup
//
//Purpose:	Provide a streamlined interface for other modules to tell the
//			player to throw his powerup
//
/////////////////////////////////////////////////////////////////////////*/
void CPlayer::ThrowPowerup()
{
	//set the powerup velocity to the players lookat vector
	getLookAtVec4DT(&m_pPowerup->m_Velocity, m_PandO);

	//multiply by POWERUP_SPEED
	if (m_pPowerup->m_Velocity.tfX == 0)
	{
		m_pPowerup->m_Velocity.tfX = -1.0f;
	}
	m_pPowerup->m_Velocity.tfX *= POWERUP_VEL;

	//set the players state
	m_PlayerStateMachine.ForceState(Throwing_state);

	//send powerup the throw message
	m_pPowerup->SendStateMessage(ThrowItem_action);

	//set pointers
	m_pPowerup = 0;

	//set timer
	m_PlayerTimer.Start(m_fAttackSpeed);
}