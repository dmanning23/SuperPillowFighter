//////////////////////////////////////////////////////////////////////////////////////////
//File Name:	EventHandler.h
// 
//Author:		Dan Manning
//
//Date Created:	8/12/04 (my fucking birthday, asshole!)
//
//Purpose:		This is the Event Handler Class.  It will be in charge of collision responses
//				It'll also add points to players, and sub life from player
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "eventhandler.h"
#include "..\MemManager\MemManager.h"
#include "..\Sound\Sound.h"
#include "..\ParticleEngine\ParticleEngine.h"
#include "..\ParticleEngine\RandomNumbers.h"
#include "..\ObjectManager\ObjectManager.h"
#include "..\GameManager\GameManager.h"
#include "..\LevelManager\LevelManager.h"
#include "CollisionDetector.h"
#include "..\ParticleEngine\RandomNumbers.h"

CEventHandler *CEventHandler::m_pInstance = 0;

//This is the chance between 0 and ten that a sound will be played
//If the random number is <= this number, the sound will be played
#define RANDOM_SOUND_CHANCE 3

//If the player velocity is above this after they get hit, add a particle effect
#define SMOKE_VEL 49.0f

//Min and max powerup velocity on x after thrown at a level object
#define MIN_PVEL -2.0f
#define MAX_PVEL 2.0f
#define PYVEL 10.0f

/*////////////////////////////////////////////////////////////////////////
//Function:	GetInstance
//
//Return:	static pointer to the only instance of the event handler
//
//Purpose:	provide an interface for other modules to interact w/ event handler
//
/////////////////////////////////////////////////////////////////////////*/
CEventHandler *CEventHandler::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = (CEventHandler*)MemMalloc(sizeof(CEventHandler));
		m_pInstance->CEventHandler::CEventHandler();
	}
	return m_pInstance;
}

/*////////////////////////////////////////////////////////////////////////
//Function:	DeleteInstance
//
//Purpose:	clean up memory used for the event handler
//
/////////////////////////////////////////////////////////////////////////*/
void CEventHandler::DeleteInstance()
{
	m_pInstance->CEventHandler::~CEventHandler();
	MemFree(m_pInstance);
	m_pInstance = 0;
}

/*////////////////////////////////////////////////////////////////////////
//Function:	Init
//
//Purpose:	Prepare the event handler for a game
//
/////////////////////////////////////////////////////////////////////////*/
void CEventHandler::Init()
{
	//load sounds
	EventSounds[HardPillowHit_EventSound] = CSound::GetInstance()->LoadSound("ResourceFiles\\Sounds\\Haid Pillow Hit.wav");
	EventSounds[ItemPickUp_EventSound] = CSound::GetInstance()->LoadSound("ResourceFiles\\Sounds\\Item Pick-up.wav");
	EventSounds[ItemSplat_EventSound] = CSound::GetInstance()->LoadSound("ResourceFiles\\Sounds\\Item Splat.wav");
	EventSounds[ItemSplat3_EventSound] = CSound::GetInstance()->LoadSound("ResourceFiles\\Sounds\\Item Splat 3.wav");
	EventSounds[ItemSplatBell_EventSound] = CSound::GetInstance()->LoadSound("ResourceFiles\\Sounds\\Item Splat Bell.wav");
	EventSounds[MedPillowHit_EventSound] = CSound::GetInstance()->LoadSound("ResourceFiles\\Sounds\\Med Pillow Hit.wav");
	EventSounds[SoftPillow1_EventSound] = CSound::GetInstance()->LoadSound("ResourceFiles\\Sounds\\Soft Pillow 1.wav");
	EventSounds[SoftPillow2_EventSound] = CSound::GetInstance()->LoadSound("ResourceFiles\\Sounds\\Soft Pillow 2.wav");
	EventSounds[SoftPillow3_EventSound] = CSound::GetInstance()->LoadSound("ResourceFiles\\Sounds\\Soft Pillow 3.wav");
	EventSounds[SoftPillow4_EventSound] = CSound::GetInstance()->LoadSound("ResourceFiles\\Sounds\\Soft Pillow 4.wav");
}

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
void CEventHandler::PillowHitPlayer(CPillow *pPillow, CPlayer *pPlayer)
{
	//check that the pillow doesn't belong to the player
	if (pPlayer->m_pPillow == pPillow)
	{
		//if yes, return
		return;
	}

	//TODO
	//make sure pillow positions are o on the z coord

	//check if the pillow is active
	if (pPillow->m_bActive)
	{
		//send hit message to player
		pPlayer->SendStateMessage(Hit_action);

		//check if the player's state changed to "falling"
		if (pPlayer->GetState() == Falling_state)
		{			
			//set the player's "last player to hit" to the pillow owner
			pPlayer->m_pLastPlayerToHit = pPillow->m_pOwner;

			//add damage to the player
			pPlayer->m_iDamage += pPillow->m_pOwner->m_iAttackPower;

			//play a random "pillow hit" sound
			int iSoundIndex = RandomInt(HardPillowHit_EventSound, SoftPillow4_EventSound);
			CSound::GetInstance()->PlaySoundEffect(EventSounds[iSoundIndex]);

			//add a feather particle emitter to the pillow
			CParticleEngine::GetInstance()->CreateEmitter(Feathers_ParticleEffect, 0, 0, pPillow);

			//add a "playerHit" particle emitter to the player
			CParticleEngine::GetInstance()->CreateEmitter(PlayerHit_ParticleEffect, 0, 0, pPlayer);	

			//random chance: play the pillow owner's "hit" sound
			if (RandomChance())
			{
				CSound::GetInstance()->PlaySoundEffect(pPillow->m_pOwner->m_iUniqueSounds[HIT_SOUND]);
			}

			//Turn off the player's pillow
			pPillow->m_bActive = false;

			//call the playerhit function, passing in both objects
			PlayerHit(pPillow, pPlayer);
		}
	}
}

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
bool CEventHandler::PowerupHitPlayer(CPowerup *pPowerup, CPlayer *pPlayer)
{
	//find powerup state
	switch (pPowerup->GetState())
	{
		case Emitted_state:
			{
				//check if player is in the "attack transition" state
				if (pPlayer->GetState() == AttackTransition_state	|| 
					pPlayer->GetState() == ForwardAttack_state		||	
					pPlayer->GetState() == SpecialAttack_state		||
					pPlayer->GetState() == UpAttack_state			||
					pPlayer->GetState() == AirSpecialAttack_state	||
					pPlayer->GetState() == AirDownAttack_state		||
					pPlayer->GetState() == AirUpAttack_state		||
					pPlayer->GetState() == AirForwardAttack_state	)
				{
					//if yes, tell the player to pick up the powerup
					if (pPlayer->PickUpPowerup(pPowerup))
					{
						//play the pickup sound
						CSound::GetInstance()->PlaySoundEffect(EventSounds[ItemPickUp_EventSound]);
					}

					return true;
				}
			}
			break;

		case Held_state:
			{
				//the player will never interact with a held powerup
				return false;
			}
			break;

		case Thrown_state:
			{
				//check if this player threw the powerup
				if (pPowerup->m_pPlayerOwner == pPlayer)
				{
					//if yes, return
					return false;
				}
				
				//else call the playerhit function, passing in both objects
				// This is now called later, but it is still called.
				//PlayerHit(pPowerup, pPlayer);

				//send the player a hit message
				pPlayer->SendStateMessage(Hit_action);

				//add damage to the player
				pPlayer->m_iDamage += pPowerup->m_iEffect;

				// Set the last person to hit this character to the owner of the pillow.
				// Added: 9-20-04 10:17 PM - STRAND
				pPlayer->m_pLastPlayerToHit = pPowerup->m_pPlayerOwner;

				//play a random "powerup hit" sound
				int iSoundIndex = RandomInt(ItemSplat_EventSound, ItemSplatBell_EventSound);
				CSound::GetInstance()->PlaySoundEffect(EventSounds[iSoundIndex]);

				//add an "itemhit" particle effect at position of powerup
				Vec4DT PowerupPos;
				getPosVec4DT(&PowerupPos, pPowerup->m_PandO);
				CParticleEngine::GetInstance()->CreateEmitter(ItemHit_ParticleEffect, &PowerupPos);

				//add a "playerhit" particle effect to the player
				CParticleEngine::GetInstance()->CreateEmitter(PlayerHit_ParticleEffect, 0, 0, pPlayer);

				//random chance: play the powerup thrower's "taunt" sound
				if (RandomChance())
				{
					CSound::GetInstance()->PlaySoundEffect(pPowerup->m_pPlayerOwner->m_iUniqueSounds[TAUNT_SOUND]);
				}

				PlayerHit(pPowerup, pPlayer);

				//remove the powerup from the game
				if (CObjectManager::GetInstance()->KillPowerup(pPowerup))
				{
					pPowerup = 0;
				}

				return true;
			}
			break;
			
		default:
			{
				return false;
			}
			break;
	}
	return false;
}

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
void CEventHandler::PlayerHitBoundary(CPlayer *pPlayer, int iBoundaryID)
{
	//find out which mode the game is in
	switch (CGameManager::GetInstance()->m_pGameData->iGameType)
	{
		case TIMEATTACK:
		{
			//add 1 to the "last player to hit"'s score
			if (pPlayer->m_pLastPlayerToHit)
			{
				pPlayer->m_pLastPlayerToHit->m_iScore++;
			}
			else if(pPlayer->m_iScore)
			{
				pPlayer->m_iScore--;
			}
		}
		break;

		case KNOCKOUT:
		{
			//add 1 to the "last player to hit"'s score
			if (pPlayer->m_pLastPlayerToHit)
			{
				pPlayer->m_pLastPlayerToHit->m_iScore++;
			}
			else if(pPlayer->m_iScore)
			{
				pPlayer->m_iScore--;
			}
		}
		break;

		//case SURVIVAL:
		//{
		//	//subtract 1 from the player's score
		//	pPlayer->m_iScore--;
		//}
		//break;
	}

	NEW_VEC4D(VelVec);
	Vec4DT PlayerPos;
	getPosVec4DT(&PlayerPos, pPlayer->m_PandO);

	//find out which boundary the player hit
	switch (iBoundaryID)
	{
		case LEFT_BOUND:
		{
			//make a "player death" emitter at the player's position with
			//the velocity traveling in the positive x direction
			VelVec.tfX = 35.0f;
			CParticleEngine::GetInstance()->CreateEmitter(PlayerDeath_ParticleEffect, &PlayerPos, &VelVec, 0, pPlayer->m_pPlayerData->PaletteColor);
		}
		break;

		case RIGHT_BOUND:
		{
            //make a "player death" emitter at the player's position with
			//the velocity traveling in the negative x direction
			VelVec.tfX = -35.0f;
			CParticleEngine::GetInstance()->CreateEmitter(PlayerDeath_ParticleEffect, &PlayerPos, &VelVec, 0, pPlayer->m_pPlayerData->PaletteColor);
		}
		break;

		//TOP_BOUND
		case TOP_BOUND:
		{
            //make a "player death" emitter at the player's position with
			//the velocity traveling in the negative y direction
			VelVec.tfY = -30.0f;
			CParticleEngine::GetInstance()->CreateEmitter(PlayerDeath_ParticleEffect, &PlayerPos, &VelVec, 0, pPlayer->m_pPlayerData->PaletteColor);
		}
		break;

		case BOTTOM_BOUND:
		{
			//make a "player death" emitter at the player's position with
			//the velocity traveling in the positive y direction
			VelVec.tfY = 35.0f;
			CParticleEngine::GetInstance()->CreateEmitter(PlayerDeath_ParticleEffect, &PlayerPos, &VelVec, 0, pPlayer->m_pPlayerData->PaletteColor);
		}
		break;

		//TODO
		//shoot particle effect towards the world origin... only if we have time for polish
	}
			
	//set the player's damage to 0
	pPlayer->m_iDamage = 0;

	//respawn the player
	CObjectManager::GetInstance()->RespawnPlayer(pPlayer);

	//add a "playerrespawn" particle emitter to the player's position
	getPosVec4DT(&PlayerPos, pPlayer->m_PandO);
	switch (pPlayer->m_pPlayerData->iCharacter)
	{
		case Josie_Player:
		{
			CParticleEngine::GetInstance()->CreateEmitter(JosieSpawn_ParticleEffect, &PlayerPos, 0, 0, pPlayer->m_pPlayerData->PaletteColor);
			//CParticleEngine::GetInstance()->CreateEmitter(PlayerSpawn_ParticleEffect, &PlayerPos);
		}
		break;

		case CaptainSally_Player:
		{
			CParticleEngine::GetInstance()->CreateEmitter(CaptainSallySpawn_ParticleEffect, &PlayerPos, 0, 0, pPlayer->m_pPlayerData->PaletteColor);
			//CParticleEngine::GetInstance()->CreateEmitter(PlayerSpawn_ParticleEffect, &PlayerPos);
		}
		break;

		case MissElizabeth_Player:
		{
			CParticleEngine::GetInstance()->CreateEmitter(MissElizabethSpawn_ParticleEffect, &PlayerPos, 0, 0, pPlayer->m_pPlayerData->PaletteColor);
			//CParticleEngine::GetInstance()->CreateEmitter(PlayerSpawn_ParticleEffect, &PlayerPos);
		}
		break;

		case Sumi_Player:
		{
			CParticleEngine::GetInstance()->CreateEmitter(SumiSpawn_ParticleEffect, &PlayerPos, 0, 0, pPlayer->m_pPlayerData->PaletteColor);
			//CParticleEngine::GetInstance()->CreateEmitter(PlayerSpawn_ParticleEffect, &PlayerPos);
		}
		break;
	}

	//play the players "gothit" sound
	CSound::GetInstance()->PlaySoundEffect(pPlayer->m_iUniqueSounds[GOTHIT_SOUND]);
}

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
void CEventHandler::PlayerHitLevelObject(CPlayer *pPlayer, CLevelObject *pLevelObject)
{
	//check if player is in the air
	switch (pPlayer->GetState())	
	{
		//check "air" states
		case Jumping_state:
		{
			PlayerLand(pPlayer, pLevelObject);
		}
		break;

		case DoubleJumping_state:
		{
			PlayerLand(pPlayer, pLevelObject);
		}
		break;

		case AirSpecialAttack_state:
		{
			PlayerLand(pPlayer, pLevelObject);
		}
		break;

		case AirUpAttack_state:
		{
			PlayerLand(pPlayer, pLevelObject);
		}
		break;

		case AirForwardAttack_state:
		{
			PlayerLand(pPlayer, pLevelObject);
		}
		break;

		//check transition states
		case Throwing_state:
		{
			switch (pPlayer->m_iPreviousState)
			{
				//check "air" states
				case Jumping_state:
				{
					PlayerLand(pPlayer, pLevelObject);
				}
				break;

				case DoubleJumping_state:
				{
					PlayerLand(pPlayer, pLevelObject);
				}
				break;

				case AirSpecialAttack_state:
				{
					PlayerLand(pPlayer, pLevelObject);
				}
				break;

				case AirUpAttack_state:
				{
					PlayerLand(pPlayer, pLevelObject);
				}
				break;

				case AirForwardAttack_state:
				{
					PlayerLand(pPlayer, pLevelObject);
				}
				break;

				default:
				break;
			}
		}
		break;

		case AttackTransition_state:
		{
			switch (pPlayer->m_iPreviousState)
			{
				//check "air" states
				case Jumping_state:
				{
					PlayerLand(pPlayer, pLevelObject);
				}
				break;

				case DoubleJumping_state:
				{
					PlayerLand(pPlayer, pLevelObject);
				}
				break;

				case AirSpecialAttack_state:
				{
					PlayerLand(pPlayer, pLevelObject);
				}
				break;

				case AirUpAttack_state:
				{
					PlayerLand(pPlayer, pLevelObject);
				}
				break;

				case AirForwardAttack_state:
				{
					PlayerLand(pPlayer, pLevelObject);
				}
				break;

				default:
				break;
			}
		}
		break;

		//check "falling" state
		case Falling_state:
		{
			//swap the player's x and y velocity
			//this will make the player go in the opposite direction
			Vec4DT temp = pPlayer->m_Velocity;
			pPlayer->m_Velocity.tfY += temp.tfX;

			pPlayer->m_Velocity.tfX = 0.0f;

			if (pPlayer->m_Velocity.tfY <= 0.0f)
			{
				pPlayer->m_Velocity.tfY *= -1.0f;
			}

			//set terminal velocity
			if (pPlayer->m_Velocity.tfY > 19.6f)
			{
				pPlayer->m_Velocity.tfY = 19.6f;
			}

			if (pPlayer->m_Velocity.tfY < 8.0f)
			{
				pPlayer->m_Velocity.tfY = 8.0f;
			}
		}
		break;

		default:
		break;
	}

	pPlayer->SendStateMessage(GroundCollision_action);
}

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
void CEventHandler::PowerupHitLevelObject(CPowerup *pPowerup, CLevelObject *pLevelObject)
{
	//get the powerup's state
	switch (pPowerup->GetState())
	{
		case Emitted_state:
		{
			return;
		}
		break;

		case Held_state:
		{
			//Level objects will never interact with held powerups
			return;
		}
		break;

		case Thrown_state:
		{
			//set velocity to shoot up in the y
			pPowerup->m_Velocity.tfY = PYVEL;
			pPowerup->m_Velocity.tfX = RandomFloat(MIN_PVEL, MAX_PVEL);
			pPowerup->m_Velocity.tfZ = 0.0f;

			//update the powerup
			pPowerup->SendStateMessage(Collision_action);
			pPowerup->m_Timer.Start(pPowerup->m_fTimeToLive);
		}
		break;
	
		default:
		{
            return;
		}
		break;
	}
}

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
void CEventHandler::PlayerHit(CBaseObject *pHittingObject, CPlayer *pPlayer)
{
	//determine new velocity for player

	//vector from pillow position to player's cente
	Vec4DT PlayerPos;	//the vector to hold the new player velocity
	Vec4DT ObjectPos;
	getPosVec4DT(&PlayerPos, pPlayer->m_PandO);
	getPosVec4DT(&ObjectPos, pHittingObject->m_PandO);
	subVec4DT(&PlayerPos, PlayerPos, ObjectPos);
	
	//normailze
	normalizeVec4DT(&PlayerPos, PlayerPos);

	//make sure is in the positive y
	if (PlayerPos.tfY < 0.0f)
	{
		PlayerPos.tfY *= -1;
	}

	//throw away z coordinate
	PlayerPos.tfZ = 0.0f;

	//player's damage
	float fDamage = ((float)pPlayer->m_iDamage / 3.0f);
	//if players damage is over 100, double the vector
	if (fDamage >= 100.0f)
	{
		fDamage *= 2.0f;
	}
	sMultVec4DT(&PlayerPos, fDamage, PlayerPos);

	////player's mass
	//float fMass = (1.0f - ((float)pPlayer->m_iMass / 100.0f));
	//PlayerPos.tfX += fMass;
	//PlayerPos.tfY += fMass;

	//check the player's velocity
	if (pPlayer->m_Velocity.tfY <= 5.0f)
	{
		pPlayer->m_Velocity.tfY = 5.0f;
	}

	//copy the new velocity to the player
	memcpy(&pPlayer->m_Velocity, &PlayerPos, sizeof(Vec4DT));
}

/*////////////////////////////////////////////////////////////////////////
//Function:	GetRandomChance
//
//Return:	
//	bool: whether or not the random chance was successful
//
//Purpose:	Used to determine if a sound should be played or not
//
/////////////////////////////////////////////////////////////////////////*/
bool CEventHandler::RandomChance()
{
	return (RandomInt(0, 10) <= RANDOM_SOUND_CHANCE);
}

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
void CEventHandler::PlayerLand(CPlayer *pPlayer, CLevelObject *pLevelObject)
{
	//place a "playerland" particle effect at the players position
	Vec4DT playerPos;
	getPosVec4DT(&playerPos, pPlayer->m_PandO);

	CParticleEngine::GetInstance()->CreateEmitter(PlayerLand_ParticleEffect, &playerPos);

	//play the level objects "playerland" sound
	CSound::GetInstance()->PlaySoundEffect(pLevelObject->m_iPlayerLandSoundIndex);

	// Since they have collided with the ground, set the pointer to the last
	//  person that hit them to 0.
	// Added: 9-20-04 10:17 PM - STRAND
	pPlayer->m_pLastPlayerToHit = 0;
}