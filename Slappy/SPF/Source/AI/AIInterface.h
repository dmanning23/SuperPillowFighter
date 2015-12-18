////////////////////////////////////////////////////////
//
// Name: "AIInterface.h"
//
// Author: Daron Keeler (DK)
//
// Creation Date: August 24, 2004
//
// Purpose: The AI manager that controls all ingame AI bots.
//
////////////////////////////////////////////////////////

#ifndef _AIINTERFACE_H_
#define _AIINTERFACE_H_

#include "..\\ObjectManager\\ObjectManager.h"
#include "..\\Logger\\Logger.h"
#include "..\\MathLibrary\\spfMath.h"


#define PLAYERREACH 1.25f
#define GRACEPERIOD 0.55f
#define AI_UPVELOCITY 0.1f
#define RECOVERY_GRACE 1.0f

// Enum of all possible goals.
enum { AI_FINDGOAL = 0, AI_ATTACKPLAYER, AI_GETITEM, AI_RECOVER, AI_NUMOFGOALS };
// Bots left and right
enum { AI_LEFT = 0, AI_RIGHT, AI_UP, AI_DOWN, AI_NUMDIRS};


struct AIBotDataT
{
    // The BOT's index into CObjectManager's player array
    int iBotIndex;

    // Is this one an active AI bot?
    bool bActive;
    // Current character goal
    int iGoal;
    // index into CObjectManager's player array, this is the target player.
    int iPlayerTarget;
    // Bot's current powerup target (if applicable)
    CPowerup * pPowerup;
    // This bot's timer.
    CStopWatch StopWatch;
	// Flag set for the ai bots to keep track of directional things
	bool Directions[4];
	// Left or Right level point?  True is left.
	bool bLeft;
	// For recovery use only, do they need to jump?
	bool bJump;

};

class AIInterface
{
private:

    static AIInterface * pInstance;

    CLogger m_LogMe;

    AIBotDataT BotData[4];

    int m_iNumBots;



	

    ////////////////////////////////////////////////////////////////////
    //
    // Function: All constructors
    //
    // Last Modified: August 24, 2004
    //
    // Input: none
    //
    // Output: none
    //
    // Return: none
    //
    // Purpose: All overridden constructors..
    ////////////////////////////////////////////////////////////////////
    AIInterface() {}
    AIInterface(const AIInterface &c) {}
    AIInterface & operator = (const AIInterface &c) {}

    
    ////////////////////////////////////////////////////////////////////
    //
    // Function: "FindNearbyItems"
    //
    // Last Modified: August 27, 2004
    //
    // Input: iCurBot   the current bot being worked on
    //
    // Output: none
    //
    // Return: bool     true if there is
    //
    // Purpose: Decide whether or not there is an item nearby to go for.
    ////////////////////////////////////////////////////////////////////
    CPowerup* FindNearbyItems(int iCurBot);

    ////////////////////////////////////////////////////////////////////
    //
    // Function: "FindNearestEnemy"
    //
    // Last Modified: August 27, 2004
    //
    // Input: iCurBot   the current bot being worked on
    //
    // Output: none
    //
    // Return: int      the index into CObject's m_PlayerArray of the player.    
    //
    // Purpose: Decide whether or not there is an item nearby to go for.
    ////////////////////////////////////////////////////////////////////
    int FindNearestEnemy(int iCurBot);

    //////////////////////////////////////////////////////////////////// \
    //
    // Function: "CheckPlayerNearby"
    //
    // Last Modified: August 27, 2004
    //
    // Input: iCurBot    the current bot being checked.
    //
    // Output: none
    //
    // Return: bool     true if they are close enough to attack, false means move closer.
    //
    // Purpose: Is the target player close enough to attack?
    ////////////////////////////////////////////////////////////////////
    bool CheckPlayerNearby(int iCurBot);

    ////////////////////////////////////////////////////////////////////   
    //
    // Function: "EvaluateGoal"
    //
    // Last Modified: August 27, 2004
    //
    // Input: iCurBot   the current bot being worked on
    //
    // Output: none
    //
    // Return: bool     false means it failed to find a goal.
    //
    // Purpose: Checks previous goal every 2 seconds or when their previous goal is completed.
    ////////////////////////////////////////////////////////////////////
    bool EvaluateGoal(int iCurBot);

    //////////////////////////////////////////////////////////////////// 
    //
    // Function: "NeedRecovery"
    //
    // Last Modified: August 27, 2004
    //
    // Input: none
    //
    // Output: none
    //
    // Return: bool     true if the bot should jump towards their target, false if not.
    //
    // Purpose: Checks to see if an edge is near the bot.
    ////////////////////////////////////////////////////////////////////
    bool NeedRecovery(int iCurBot);

	////////////////////////////////////////////////////////////////////
    //
    // Function: "DirectionCheck"
    //
    // Last Modified: August 27, 2004
    //
    // Input: int iCurBot the current bot's index
	//		  Vec4DT * ObjectToCheck  this is the vector of the target object, player or item.
    //
    // Output: none
    //
    // Return: bool     true if the bot should jump towards their target, false if not.
    //
    // Purpose: Checks to see if an edge is near the bot.
    ////////////////////////////////////////////////////////////////////
    void DirectionCheck(int iCurBot, Vec4DT * ObjectToCheck);




public:
    ////////////////////////////////////////////////////////////////////
    //
    // Function: "Destructor"
    //
    // Last Modified: August 24, 2004
    //
    // Input: none
    //
    // Output: none
    //
    // Return: none
    //
    // Purpose: Destructor.
    ////////////////////////////////////////////////////////////////////
    ~AIInterface() {}

    ////////////////////////////////////////////////////////////////////
    //
    // Function: "InitAI"
    //
    // Last Modified: August 24, 2004
    //
    // Input: none
    //
    // Output: none
    //
    // Return: none
    //
    // Purpose: Initializes all AI.
    ////////////////////////////////////////////////////////////////////
    void InitAI();

    ////////////////////////////////////////////////////////////////////
    //
    // Function: "GetInstance"
    //
    // Last Modified: August 24, 2004
    //
    // Input: none
    //
    // Output: none
    //
    // Return: none
    //
    // Purpose: Gets AI instance.
    ////////////////////////////////////////////////////////////////////
    static AIInterface* GetInstance();

    ////////////////////////////////////////////////////////////////////
    //
    // Function: "DeleteInstance"
    //
    // Last Modified: August 24, 2004
    //
    // Input: none
    //
    // Output: none
    //
    // Return: none
    //
    // Purpose: Deletes AI instance.
    ////////////////////////////////////////////////////////////////////
    static void DeleteInstance();

    ////////////////////////////////////////////////////////////////////
    //
    // Function: "UpdateAI"
    //
    // Last Modified: August 24, 2004
    //
    // Input: none
    //
    // Output: none
    //
    // Return: none
    //
    // Purpose: Updates AI each frame, handling goals, etc.
    ////////////////////////////////////////////////////////////////////
    void UpdateAI();

    ////////////////////////////////////////////////////////////////////
    //
    // Function: "ShutdownAI"
    //
    // Last Modified: August 24, 2004
    //
    // Input: none
    //
    // Output: none
    //
    // Return: none
    //
    // Purpose: Shuts AI module down properly.
    ////////////////////////////////////////////////////////////////////
    void ShutDownAI();

    ///////////////////////////////////////////////////////////////////
    //
    // Function: "GetBotGoal"
    //
    // Last Modified: August 30, 2004
    //
    // Input: int iPlayerIndex     the PLAYER ARRAY index.
    //
    // Output: none
    //
    // Return: int  The current goal, enumerated in AIInterface.h
    //
    // Purpose: retrieves current bot goal.
    ////////////////////////////////////////////////////////////////////
    int GetBotGoal(int iPlayerIndex)
    {
        // search through
        for(int i = 0; i < CObjectManager::GetInstance()->m_iPlayerArraySize; i++)
        {
            // we found it
            if(BotData[i].iBotIndex == iPlayerIndex && BotData[i].bActive)
                return BotData[i].iGoal;

        }
            // Failed to find it.
            return -1;
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
	void powerUpDied(CPowerup *pDeadPup);

};
#endif
