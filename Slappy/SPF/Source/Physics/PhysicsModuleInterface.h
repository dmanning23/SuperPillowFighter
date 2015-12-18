#pragma once
//////////////////////////////////////////////////////////////////////////////////////////
//File Name:	PhysicsModuleInterface.h
// 
//Author:		Allan Bittan
//
//Date Created:	8/5/04
//
//Purpose:		This is the Public Interface class for the Physics Module.  
//				This module will be in charge of calling the Collision Detection Module
//				The process for physics will be:
//					1) Test Collision
//						a) The Collision test will call the Event Handler to do 
//						   Collision Responses
//					2) Apply Movement to Players, Powerups, and World Objects.
//			
//				** Note Use UpdatePhysics()  **
//
//////////////////////////////////////////////////////////////////////////////////////////

//#include "collisionDetector.h"
//#include CEventHandler
#include "..\\MathLibrary\\spfMath.h"
#include "..\\entities\\player.h"
#include "..\\entities\\levelobject.h"
#include "..\\entities\\baseobject.h"
#include "..\\objectmanager\\objectmanager.h"
#include "..\\GameTimer\\GameTimer.h"
#include "..\\MemManager\\MemManager.h"
#include "..\\LevelManager\\levelmanager.h"
#include "CollisionDetector.h"

 

#define GRAVITY 9.8f

class CPhysicsModule
{



// singleton shits
private:
	static CPhysicsModule *pInstance;
	CPhysicsModule();
	CPhysicsModule(const CPhysicsModule&);
	CPhysicsModule &operator = (const CPhysicsModule&);

	virtual ~CPhysicsModule();

// public members
public:

	float m_fElapseTime;
	float m_fCurrentTime;
	float m_fOldTime;
	int m_iNumLines;

	Matrix4x4T *m_oldPando;
	CCollisionDetector::WorldCollisionData OldTestLine;


	// shits pointer for the shits collisiondetector
	CCollisionDetector* CollisionDetector;

public:
	
	///////////////////////////////////////////////////////////////////////////////////
	//
	// Function:		DeleteInst
	//
	// Last Modified:	09/23/04
	//
	// Input:			none
	//
	// Output:			none
	//
	// Return:			none
	//
	// Purpose:			Delete the instance of this class
	//
	///////////////////////////////////////////////////////////////////////////////////
	static void DeleteInst(void);


	///////////////////////////////////////////////////////////////////////////////////
	//
	// Function:		GetInst
	//
	// Last Modified:	03/01/04
	//
	// Input:			none
	//
	// Output:			none
	//
	// Return:			current instance
	//
	// Purpose:			get the current instance, if it exists.
	//
	///////////////////////////////////////////////////////////////////////////////////
	static CPhysicsModule *GetInstance(void);

	
	//////////////////////////////////////////////////////////////////////////////////////////
	//Function Name:	UpdatePhysics()
	//
	//Purpose:			This is the Public Method that will be called from the main game loop.
	//					It'll be in charge of calling the collision testing, and updating
	//					the position of the player and objects - (world and powerups)
	//
	//Last Modified:	8/5/04
	// 
	//Input:			PowerUpList, WorldGeometryList, PlayerList
	//
	//Output:			PowerUpList, WorldGeometryList, PlayerList
	//
	//Return:			None
	//
	//Notes:			Call this Method in the Main Game loop
	//////////////////////////////////////////////////////////////////////////////////////////
	void UpdatePhysics();

	//////////////////////////////////////////////////////////////////////////////////////////
	//Function Name:	ApplyForceToPlayer()
	//
	//Purpose:			This function will apply movement to each player.  Depending on what 
	//					action I receive from the Player's State Machine.
	//					 
	//Last Modified:	8/5/04
	// 
	//Input:			PlayerList
	//
	//Output:			PlayerList
	//
	//Return:			None
	//
	//Notes:			Called from this module
	//////////////////////////////////////////////////////////////////////////////////////////
    void ApplyForceToPlayer(CPlayer*);

	//////////////////////////////////////////////////////////////////////////////////////////
	//Function Name:	ApplyForceToPowerUps()
	//
	//Purpose:			This function will apply movement to each Active PowerUp.  
	//					 
	//Last Modified:	8/5/04
	// 
	//Input:			PowerUpList
	//
	//Output:			PowerUpList
	//
	//Return:			None
	//
	//Notes:			Called from this module
	//////////////////////////////////////////////////////////////////////////////////////////
	void ApplyForceToPowerUps();

	//////////////////////////////////////////////////////////////////////////////////////////
	//Function Name:	ApplyForceToWorldObjects()
	//
	//Purpose:			This function will apply movement to each WorldObjets.
	//					 
	//Last Modified:	8/5/04
	// 
	//Input:			WorldObject
	//
	//Output:			WorldObject
	//
	//Return:			None
	//
	//Notes:			Called from this module
	//////////////////////////////////////////////////////////////////////////////////////////
	void ApplyForceToWorldObjects();
	
	//////////////////////////////////////////////////////////////////////////////////////////
	//Function Name:	ApplyRotation()
	//
	//Purpose:			This function will apply a rotation vector to the object's matrix. 
	//					 
	//Last Modified:	8/5/04
	// 
	//Input:			WorldObject
	//
	//Output:			WorldObject
	//
	//Return:			None
	//
	//Notes:			Called from this module
	//////////////////////////////////////////////////////////////////////////////////////////
	void ApplyRotation();

	//////////////////////////////////////////////////////////////////////////////////////////
	//Function Name:	ApplyBobbing()
	//
	//Purpose:			This function will be used to cause world-objects to move up and down
	//					 
	//Last Modified:	8/5/04
	// 
	//Input:			WorldObject
	//
	//Output:			WorldObject
	//
	//Return:			None
	//
	//Notes:			Called from this module
	//////////////////////////////////////////////////////////////////////////////////////////
	void ApplyBobbing();

	//////////////////////////////////////////////////////////////////////////////////////////
	//Function Name:	InitLevelData()
	//
	//Purpose:			This function HAS to be call when a new level is loaded/started
	//					 
	//Last Modified:	8/14/04
	// 
	//Input:			None
	//
	//Output:			m_OldPandO
	//
	//Return:			None
	//
	//Notes:			MUST BE CALLED FIRST AFTER CONSTRUCTOR
	//////////////////////////////////////////////////////////////////////////////////////////
	void InitLevelData();

	//////////////////////////////////////////////////////////////////////////////////////////
	//Function Name:	KillLevelData()
	//
	//Purpose:			This function HAS to be call when current level is done with.
	//					 
	//Last Modified:	8/14/04
	// 
	//Input:			None
	//
	//Output:			m_OldPandO
	//
	//Return:			None
	//
	//Notes:			MUST BE CALLED AFTER LEVEL is unloaded.
	//////////////////////////////////////////////////////////////////////////////////////////
	void KillLevelData();
};  


