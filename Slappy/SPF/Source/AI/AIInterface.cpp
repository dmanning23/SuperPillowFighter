////////////////////////////////////////////////////////
//
// Name: "AIInterface.cpp"
//
// Author: Daron Keeler (DK)
//
// Creation Date: August 24, 2004
//
// Purpose: The AI manager that controls all ingame AI bots.
//
///////////////////////////////////////////////////////

#include "AIInterface.h"
#include "..\\MemManager\\MemManager.h"
#include "..\\GameTimer\\StopWatch.h"
#include "..\\Physics\\CollisionDetector.h"


AIInterface * AIInterface::pInstance = 0;


AIInterface * AIInterface::GetInstance()
{
    // Make one if it hasnt been done.
    if(!pInstance)
    {
        pInstance = (AIInterface *)MemMalloc(sizeof(AIInterface));
        pInstance->AIInterface::AIInterface();
    }

    // return an instance.
    return pInstance;
}

void AIInterface::DeleteInstance()
{
    if(pInstance)
    {
        pInstance->AIInterface::~AIInterface();
        MemFree(pInstance);

        pInstance = NULL;
    }
}

void AIInterface::InitAI()
{
    m_LogMe.Init("AILogger.txt");

    m_iNumBots = 0;
    // Set all bots to empty.
    for(int i = 0; i < 4; i++)
    {
        BotData[i].bActive = false;
        BotData[i].iGoal = AI_FINDGOAL;
        BotData[i].iPlayerTarget = -1;
        BotData[i].pPowerup = NULL;
        BotData[i].iBotIndex = -1;
		BotData[i].bLeft = false;
		BotData[i].bJump = false;
		

		for(int j = 0; j < AI_NUMDIRS; j++)
		{
			BotData[i].Directions[j] = false;
		}

        // Start the stopwatch for two seconds.
        BotData[i].StopWatch.Start(2.0f);
    }



    // Go through and check each of the four possible players to see which are bots
    for(int i = 0; i < CObjectManager::GetInstance()->m_iPlayerArraySize; i++)
    {
        // make sure there is a player here
        if(CObjectManager::GetInstance()->m_PlayerArray[i])
        {
            // make sure its a bot
            if(CObjectManager::GetInstance()->m_PlayerArray[i]->m_pPlayerData->iInputType == 2)
            {
                BotData[m_iNumBots].bActive = true;
                BotData[m_iNumBots].iBotIndex = i;
                m_iNumBots++;
            }
        }
    }

    m_LogMe << "AI Initialized";
}

void AIInterface::UpdateAI()
{
    // The main loop to go through each bot and run their actions.
    for(int i = 0; i < m_iNumBots; i++)
    {
        // Decide whether or not to keep previous goals
        if(EvaluateGoal(i) == false)
        {
            BotData[i].iGoal = AI_FINDGOAL;
        }

		switch(BotData[i].iGoal)
		{
		case AI_FINDGOAL:
			{

			}
			break;

		case AI_RECOVER:
			{
				if(BotData[i].bLeft)
					CObjectManager::GetInstance()->m_PlayerArray[BotData[i].iBotIndex]->m_iMovement = 2;
				else
					CObjectManager::GetInstance()->m_PlayerArray[BotData[i].iBotIndex]->m_iMovement = 1;

				if(BotData[i].bJump)
				{
					CObjectManager::GetInstance()->m_PlayerArray[BotData[i].iBotIndex]->SendStateMessage(Jump_action);
					BotData[i].bJump = false;
				}


			}
			break;

		case AI_ATTACKPLAYER:
			{
				NEW_VEC4D(PlayerLoc);
				
				// Get the position of the f'in target
				getPosVec4DT(&PlayerLoc, CObjectManager::GetInstance()->m_PlayerArray[(BotData[i].iPlayerTarget)]->m_PandO);

				 //PlayerLoc.tfY += GET_CENTER_POINT(CObjectManager::GetInstance()->m_PlayerArray[(BotData[i].iPlayerTarget)]);
				// Go in and set up the four direction flags.
				DirectionCheck(i, &PlayerLoc);


				// true means attack the player!
				if(CheckPlayerNearby(i))
				{
					// If something above em, attack up.
					if(BotData[i].Directions[AI_UP])
					{
						if(BotData[i].Directions[AI_LEFT] || BotData[i].Directions[AI_RIGHT])
						{
							CObjectManager::GetInstance()->m_PlayerArray[BotData[i].iBotIndex]->SendStateMessage(ForwardAttack_action);	
							break;
						}
						CObjectManager::GetInstance()->m_PlayerArray[BotData[i].iBotIndex]->SendStateMessage(UpAttack_action);
						break;
					}
					// They are just left or right, kill em!
					if(BotData[i].Directions[AI_LEFT] || BotData[i].Directions[AI_RIGHT])
					{
						CObjectManager::GetInstance()->m_PlayerArray[BotData[i].iBotIndex]->SendStateMessage(Attack_action);
						break;
					}

					if(BotData[i].Directions[AI_DOWN])
					{
						CObjectManager::GetInstance()->m_PlayerArray[BotData[i].iBotIndex]->SendStateMessage(DownAttack_action);
						break;
					}

				}
				// Otherwise move towards the guy.
				else
				{
					if(BotData[i].Directions[AI_LEFT])
						CObjectManager::GetInstance()->m_PlayerArray[BotData[i].iBotIndex]->m_iMovement = 2;
					else if(BotData[i].Directions[AI_RIGHT])
						CObjectManager::GetInstance()->m_PlayerArray[BotData[i].iBotIndex]->m_iMovement = 1;
					else
						CObjectManager::GetInstance()->m_PlayerArray[BotData[i].iBotIndex]->m_iMovement	= 0;


					// CObjectManager::GetInstance()->m_PlayerArray[BotData[i].iBotIndex]->m_PandO;

					// JUMP!
					if(BotData[i].Directions[AI_UP])
					{
						//NEW_VEC4D(BotY);

						if(CObjectManager::GetInstance()->m_PlayerArray[BotData[i].iBotIndex]->m_Velocity.tfY < AI_UPVELOCITY)
							CObjectManager::GetInstance()->m_PlayerArray[BotData[i].iBotIndex]->SendStateMessage(Jump_action);
						
					}
				}
			}
			break;
		case AI_GETITEM:
			{
				NEW_VEC4D(ItemLoc);
				
				// Get the position of the f'in target
				 getPosVec4DT(&ItemLoc, BotData[i].pPowerup->m_PandO);
				// Go in and set up the four direction flags.
				 DirectionCheck(i, &ItemLoc);

				// If this is true pick up the item, otherwise move towards it.
				if(CCollisionDetector::GetInstance()->CircleToCircleFromEntireToBottomTest(CObjectManager::GetInstance()->m_PlayerArray[(BotData[i].iBotIndex)], 
					BotData[i].pPowerup))
				{
					// Stop em from moving and fuckin up eh?
					CObjectManager::GetInstance()->m_PlayerArray[BotData[i].iBotIndex]->m_iMovement = 0;

					CObjectManager::GetInstance()->m_PlayerArray[BotData[i].iBotIndex]->SendStateMessage(Attack_action);
					BotData[i].iGoal = AI_FINDGOAL;
					break;
				}
				else
				{
					if(BotData[i].Directions[AI_LEFT])
						CObjectManager::GetInstance()->m_PlayerArray[BotData[i].iBotIndex]->m_iMovement = 2;
					else if(BotData[i].Directions[AI_RIGHT])
						CObjectManager::GetInstance()->m_PlayerArray[BotData[i].iBotIndex]->m_iMovement = 1;
					else
						CObjectManager::GetInstance()->m_PlayerArray[BotData[i].iBotIndex]->m_iMovement	= 0;

					// JUMP!
					if(BotData[i].Directions[AI_UP])
					{
						NEW_VEC4D(BotY);

						if(CObjectManager::GetInstance()->m_PlayerArray[BotData[i].iBotIndex]->m_Velocity.tfY < AI_UPVELOCITY)
							CObjectManager::GetInstance()->m_PlayerArray[BotData[i].iBotIndex]->SendStateMessage(Jump_action);
						
					}
				}	  
			}
			break;
				
		};
    }
}

void AIInterface::DirectionCheck(int iCurBot, Vec4DT * ObjectToCheck)
{
	NEW_VEC4D(BotLoc);

	getPosVec4DT(&BotLoc, CObjectManager::GetInstance()->m_PlayerArray[BotData[iCurBot].iBotIndex]->m_PandO);
	//BotLoc.tfY += GET_CENTER_POINT(CObjectManager::GetInstance()->m_PlayerArray[BotData[iCurBot].iBotIndex]);


	for(int i = 0; i < AI_NUMDIRS; i++)
	{
		BotData[iCurBot].Directions[i] = false;
	}
												   
	if(BotLoc.tfY + GRACEPERIOD < ObjectToCheck->tfY - GRACEPERIOD)
		BotData[iCurBot].Directions[AI_UP] = true;

	if(BotLoc.tfY - GRACEPERIOD > ObjectToCheck->tfY + GRACEPERIOD)
		BotData[iCurBot].Directions[AI_DOWN] = true;

	if(BotLoc.tfX - GRACEPERIOD < ObjectToCheck->tfX + GRACEPERIOD)
		BotData[iCurBot].Directions[AI_LEFT] = true;

	if(BotLoc.tfX + GRACEPERIOD > ObjectToCheck->tfX - GRACEPERIOD)
		BotData[iCurBot].Directions[AI_RIGHT] = true;


}

bool AIInterface::EvaluateGoal(int iCurBot)
{
    // Time isn't up yet, if goal is still valid, keep it.
    if(BotData[iCurBot].StopWatch.GetRemainingTime() > 0.0f && BotData[iCurBot].iGoal != AI_FINDGOAL)
    {

		// Emergency recovery!
		if(NeedRecovery(iCurBot))
		{
			BotData[iCurBot].StopWatch.Start(0.5f);
			BotData[iCurBot].iGoal = AI_RECOVER;
		}

        // TODO :do a switch on the possibilities and see if the goal is valid.    
        return true;
         
    }
    // Okay re-evaluate goals.
    else
    {
        BotData[iCurBot].StopWatch.Start(2.0f);

        // find closest enemy
        BotData[iCurBot].iPlayerTarget = FindNearestEnemy(iCurBot);

        // No enemy found on screen?!
        if(BotData[iCurBot].iPlayerTarget == -1)
        {
            BotData[iCurBot].iGoal = AI_FINDGOAL;
            return true;
        }

        if(!(CObjectManager::GetInstance()->m_PlayerArray[BotData[iCurBot].iBotIndex]->m_pPowerup))
        {

            // Is there an item nearby that AI could possibly go for?
            BotData[iCurBot].pPowerup = FindNearbyItems(iCurBot);

            // If there was an item found, decide whether or not to go for it.
            if(BotData[iCurBot].pPowerup)
            {
                // temporarily goes for item 100% of the time.
                BotData[iCurBot].iGoal = AI_GETITEM;
                return true;

                // TODO: add random.
                // random chance to go for the item or not.
            } 
        }

        // They aren't going for the item, so they must be going for an enemy.
        if(BotData[iCurBot].iPlayerTarget >= 0)
        {
            BotData[iCurBot].iGoal = AI_ATTACKPLAYER;
            return true;
        }
		


        return false;
    }

    // Failsafe, shouldnt reach this
    return false;
}

int AIInterface::FindNearestEnemy(int iCurBot)
{
    int iClosestEnemy = -1;
    NEW_VEC4D(PlayerLoc);
    NEW_VEC4D(BotLoc);

    float ClosestDist = 100.0f;
    float NewDist = 100.0f;

	getPosVec4DT(&BotLoc, CObjectManager::GetInstance()->m_PlayerArray[BotData[iCurBot].iBotIndex]->m_PandO);

    // loop through all players excluding the actual bot and save the closes one.
    for(int i = 0; i < CObjectManager::GetInstance()->m_iPlayerArraySize; i++)
    {   
        // If target player is the bot, or it's a dead player, skip it.
        if(i == BotData[iCurBot].iBotIndex || CObjectManager::GetInstance()->m_PlayerArray[i]->GetState() == Dead_state)
            continue;

        getPosVec4DT(&PlayerLoc, CObjectManager::GetInstance()->m_PlayerArray[i]->m_PandO);
        // get distance between these current ones, and then compare them to the last one, closest enemy is saved.
        NewDist = distVec4DT(PlayerLoc, BotLoc);   

        if(NewDist < ClosestDist)
        {
            iClosestEnemy = i;
            ClosestDist = NewDist;
        }
    }
    // Send back the closest player.
    return iClosestEnemy;
}

bool AIInterface::CheckPlayerNearby(int iCurBot)
{
	NEW_VEC4D(TargetLoc);
	NEW_VEC4D(BotLoc);
	float NewDist = 100.0f;

	getPosVec4DT(&BotLoc, CObjectManager::GetInstance()->m_PlayerArray[BotData[iCurBot].iBotIndex]->m_PandO);
	getPosVec4DT(&TargetLoc, CObjectManager::GetInstance()->m_PlayerArray[(BotData[iCurBot].iPlayerTarget)]->m_PandO);

	NewDist = distVec4DT(TargetLoc, BotLoc);

	// if true, the target is nearby.
	if(NewDist < PLAYERREACH)
		return true;

	return false;


}

CPowerup * AIInterface::FindNearbyItems(int iCurBot)
{
    // Iterator
    CPowerup * CurPwrUp;

    CPowerup * pClosestPower = NULL;
    NEW_VEC4D(PowerLoc);
    NEW_VEC4D(BotLoc);

    float ClosestDist = 5.0f;
    float NewDist = 1000.0f;

    // Get the head of the list
    CurPwrUp = CObjectManager::GetInstance()->m_ActivePowerupList;

    // get the bot's position.
	getPosVec4DT(&BotLoc, CObjectManager::GetInstance()->m_PlayerArray[BotData[iCurBot].iBotIndex]->m_PandO);

    // While there are still powerups, compare them to the player's distance.
    while(CurPwrUp)
    {
        if(CurPwrUp->GetState() != Emitted_state)
        {
            CurPwrUp = CurPwrUp->m_pNext;
            continue;
        }

        // get the current powerup's position and calc distance
        getPosVec4DT(&PowerLoc, CurPwrUp->m_PandO);
        NewDist = distVec4DT(PowerLoc, BotLoc);

        if(NewDist <= ClosestDist)
        {
            pClosestPower = CurPwrUp;
            ClosestDist = NewDist;
        }
        CurPwrUp = CurPwrUp->m_pNext;
    }

    // CObjectManager::GetInstance()->m_ActivePowerupList-

    return pClosestPower;
}

// Private
bool AIInterface::NeedRecovery(int iCurBot)
{

	NEW_VEC4D(BotLoc);
	NEW_VEC4D(LevelPos);
	NEW_VEC4D(TempLvl);
	


	float ClosestDist = 1000.0f;
    float NewDist = 1000.0f;

	getPosVec4DT(&BotLoc, CObjectManager::GetInstance()->m_PlayerArray[(BotData[iCurBot].iBotIndex)]->m_PandO);

	for(int i = 0; i < CObjectManager::GetInstance()->m_iLevelObjectArraySize; i++)
	{
		// This isnt a level object we want to test.
		if(CObjectManager::GetInstance()->m_LevelObjectArray[i]->m_iNumLines == 0)
			continue;

		getPosVec4DT(&TempLvl, CObjectManager::GetInstance()->m_LevelObjectArray[i]->m_PandO);
		addVec4DT(&TempLvl, TempLvl, CObjectManager::GetInstance()->m_LevelObjectArray[i]->m_LeftMostPoint); 
		// Get the distance between the bot and the left
        NewDist = distVec4DT(BotLoc, TempLvl);

		// Check if its closer than any others.
		if(NewDist <= ClosestDist)
        {
			// Set them to left
			BotData[iCurBot].bLeft = true;
			// save new dist		
            ClosestDist = NewDist;

			// copy over the data
			LevelPos.tfX = CObjectManager::GetInstance()->m_LevelObjectArray[i]->m_LeftMostPoint.tfX;
			LevelPos.tfY = CObjectManager::GetInstance()->m_LevelObjectArray[i]->m_LeftMostPoint.tfY;

        }
		
		getPosVec4DT(&TempLvl, CObjectManager::GetInstance()->m_LevelObjectArray[i]->m_PandO);
		addVec4DT(&TempLvl, TempLvl, CObjectManager::GetInstance()->m_LevelObjectArray[i]->m_RightMostPoint);
		// Get dist between bot and the right
		NewDist = distVec4DT(BotLoc, TempLvl);

		if(NewDist <= ClosestDist)
		{
			// Set them to right
			BotData[iCurBot].bLeft = false;
			// save new dist
			ClosestDist = NewDist;

			// copy over the data
			LevelPos.tfX = CObjectManager::GetInstance()->m_LevelObjectArray[i]->m_RightMostPoint.tfX;
			LevelPos.tfY = CObjectManager::GetInstance()->m_LevelObjectArray[i]->m_RightMostPoint.tfY;
		}
	}
	
	// If true, then we are dealing with a left object.
	if(BotData[iCurBot].bLeft)
	{
		if(BotLoc.tfX < LevelPos.tfX)
		{
			if(BotLoc.tfY < LevelPos.tfY + RECOVERY_GRACE)
				BotData[iCurBot].bJump = true;
			else
				BotData[iCurBot].bJump = false;

			return true;
		}

	}
	// Woops its a right object
	else
	{
		if(BotLoc.tfX > LevelPos.tfX)
		{	
			if(BotLoc.tfY < LevelPos.tfY + RECOVERY_GRACE)
				BotData[iCurBot].bJump = true;
			else
				BotData[iCurBot].bJump = false;

			return true;
		}
	}


	return false;

}

void AIInterface::ShutDownAI()
{
    m_LogMe << "AI ShutDown";
}

///////////////////////////////////////////////////////////////////
//
// Function: "powerUpDied"
//
// Last Modified: August 30, 2004
//
// Input: powerUpDied		- The address of the powerup that is about to die.
//
// Output: none
//
// Return: none.
//
// Purpose: use this to inform the AI that a powerup has died.  
////////////////////////////////////////////////////////////////////
void AIInterface::powerUpDied(CPowerup *pDeadPup)
{
    // Look through all the bots and make sure their goal is no longer aimed at
	// the power up that is dying.
	for(int i = 0; i < m_iNumBots; i++)
	{
		if(BotData[i].iBotIndex == -1)
			continue;		// This is not a bot.

		if(BotData[i].pPowerup == pDeadPup)
		{
			// This bot is chasing a ghost!  Make it stop.
			BotData[i].pPowerup = 0;
			BotData[i].iGoal = AI_FINDGOAL;
		}
	}
}
