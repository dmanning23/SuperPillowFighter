#pragma once
//////////////////////////////////////////////////////////////////////////////////////////
//File Name:	CollisionDetector.h
// 
//Author:		Allan Bittan
//
//Date Created:	8/5/04
//
//Purpose:		This is going to be the module used for collision detection.  It'll
//				call the Event Handler to do the collision responses.
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "..\\entities\\player.h"
#include "..\\objectmanager\\objectmanager.h"
#include "..\\MemManager\\MemManager.h"
#include "..\\entities\\levelobject.h"

// Where Information is stored inside the collision data pointer... access it array style
#define CENTERPOINT			0
#define CENTERRADIUS		1
#define ENTIRERADIUS		2
#define TOPCENTERPOINT		3 
#define TOPRADIUS			4
#define BOTTOMCENTERPOINT	5
#define BOTTOMRADIUS		6

// Player's Collision Data Gets
#define GET_ENTIRE_RADIUS(Player) (*( (float*) Player->CollisionData+ENTIRERADIUS))
#define GET_CENTER_RADIUS(Player) (*( (float*) Player->CollisionData+CENTERRADIUS))
#define GET_BOTTOM_RADIUS(Player) (*( (float*) Player->CollisionData+BOTTOMRADIUS))
#define GET_TOP_RADIUS(Player) (*( (float*) Player->CollisionData+TOPRADIUS))
#define GET_CENTER_POINT(Player) (*( (float*) Player->CollisionData+CENTERPOINT))
#define GET_TOP_POINT(Player) (*( (float*) Player->CollisionData+TOPCENTERPOINT))
#define GET_BOTTOM_POINT(Player) (*( (float*) Player->CollisionData+BOTTOMCENTERPOINT))

// Powerup Collision Data Get
#define GET_POWERUP_RADIUS(PowerUp) (*( (float*) PowerUp->CollisionData))

// Pillow Collision Data Get
#define GET_PILLOW_RADIUS(Pillow) (*( (float*) Pillow->CollisionData))


#define MoveMemPointerForward(NumberOfBytes, CD) (CD) = (void*)((char*)(CD) + (NumberOfBytes));
//#define MoveMemPointerBackward(NumberOfBytes) PlayerInput->CollisionData = (void*)((char*)CollisionData - NumberOfBytes);

class CCollisionDetector
{

// singleton shit
private:
	static CCollisionDetector *pInstance;
	
	CCollisionDetector();
	CCollisionDetector(const CCollisionDetector&);
	CCollisionDetector &operator = (const CCollisionDetector&);
	virtual ~CCollisionDetector();




public:
	
	// the collision data structure of the player
	struct PlayerCollisionData
	{
		float fCenterPoint;
		float fCenterRadius;
		float fEntireRadius;
		float fTopCenterPoint;
		float fTopRadius;
		float fBottomCenterPoint;
		float fBottomRadius;
	};

	// the collision data for world lines
	struct WorldCollisionData
	{
		Vec4DT StartLine;
		Vec4DT EndLine;
	};

	// the collision data for powerups
	struct PowerUpCollisionData
	{
		float fRadius;
	};

 
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
	static CCollisionDetector *GetInstance(void);

	//////////////////////////////////////////////////////////////////////////////////////////
	//Function Name:	EntireCircleToEntireCircle()
	//
	//Purpose:			This function will be used for all collision dectection between objects
	//					in the game, ie... Player to Player, Player to PowerUp, and PowerUp to PowerUp
	//					Player to pillow.
	//
	//Last Modified:	8/5/04
	// 
	//Input:			CBaseObject, CBaseObject
	//
	//Output:			none
	//
	//Return:			true if collision. false if no collision
	//
	//Notes:			This function will receive two baseobjects, because the objects being 
	//					passed in stores the type, the function can cast it to the proper type
	//					in order to preform tasks that are associated with it.
	//////////////////////////////////////////////////////////////////////////////////////////
	bool EntireCircleToEntireCircle(CBaseObject* object1, CBaseObject* object2);

	//Function Name:	TopCircleToTopCircle()
	//
	//Purpose:			This function will test the top bounding circle of the player with
	//					any other object which is bounded by a circle.
 	//
	//Last Modified:	8/5/04
	// 
	//Input:			CBaseObject, CBaseObject
	//
	//Output:			none
	//
	//Return:			true if collision. false if no collision
	//
	//Notes:			if two players are passed in, the two top circles are tested.
	//					
	//////////////////////////////////////////////////////////////////////////////////////////
	bool TopCircleToTopCircle(CBaseObject* object1, CBaseObject* object2);
	
	//Function Name:	CenterCircleToCenterCircle()
	//
	//Purpose:			This function will test the center bounding circle of the player with
	//					any other object which is bounded by a circle.
 	//
	//Last Modified:	8/5/04
	// 
	//Input:			CBaseObject, CBaseObject
	//
	//Output:			none
	//
	//Return:			true if collision. false if no collision
	//
	//Notes:			if two players are passed in, the two center circles are tested.
	//					
	//////////////////////////////////////////////////////////////////////////////////////////
	bool CenterCircleToCenterCircle(CBaseObject* object1, CBaseObject* object2);

	//Function Name:	BottomCircleToBottomCircle()
	//
	//Purpose:			This function will test the bottom bounding circle of the player with
	//					any other object which is bounded by a circle.
 	//
	//Last Modified:	8/5/04
	// 
	//Input:			CBaseObject, CBaseObject
	//
	//Output:			none
	//
	//Return:			true if collision. false if no collision
	//
	//Notes:			if two players are passed in, the two bottom circles are tested.
	//					
	//////////////////////////////////////////////////////////////////////////////////////////
	bool BottomCircleToBottomCircle(CBaseObject* object1, CBaseObject* object2);

	//Function Name:	CircleToCircleFromEntireToBottomTest()
	//
	//Purpose:			This function is used to test the players bounding hierarchy circle with
	//					all sorts of shit
 	//
	//Last Modified:	8/5/04
	// 
	//Input:			CBaseObject, CBaseObject
	//
	//Output:			none
	//
	//Return:			true if collision. false if no collision
	//
	//Notes:			this calls the 4 circle tests and returns true if any collision happened.
	//					
	//////////////////////////////////////////////////////////////////////////////////////////
	bool CircleToCircleFromEntireToBottomTest(CBaseObject* object1, CBaseObject* object2);





	//////////////////////////////////////////////////////////////////////////////////////////
	//Function Name:	LineCircleTest()
	//
	//Purpose:			This function will be used for all collision dectection between objects
	//					in the game...Player to Ground, PowerUp to Ground...
	//
	//Last Modified:	8/5/04
	// 
	//Input:			CBaseObject, WorldCollisionData
	//
	//Output:			none
	//
	//Return:			True if collision, False if NOT
	//
	//Notes:			This function will not only test for line collision but also 
	//					Does ground clamping for verticle/near-verticle lines.
	////////////////////////////////////////////////////////////////////////////////////////// 
	bool LineCircleTest(CBaseObject *Object, WorldCollisionData TestLine);
	

	//////////////////////////////////////////////////////////////////////////////////////////
	//Function Name:	LoadPlayerCollisionData()
	//
	//Purpose:			This Function will load the Collision Data for a player Object.
	//
	//Last Modified:	8/23/04
	// 
	//Input:			CPlayer 
	//
	//Output:			BaseObject, BaseObject
	//
	//Return:			None
	//
	//Notes:			This function will be used by the AI system.
 	//////////////////////////////////////////////////////////////////////////////////////////
	void LoadPlayerCollisionData(PlayerCollisionData* CollisionOutput, CPlayer* Player);
 

	//////////////////////////////////////////////////////////////////////////////////////////
	//Function Name:	GroudClamps2()
	//
	//Purpose:			This function will be used for the player ground clamping.
	//
	//Last Modified:	8/23/04
	// 
	//Input:			CPlayer, TestLine, oldPOS, pDanObject 
	//
	//Output:			player
	//
	//Return:			None
	//
	//Notes:			this function is used for ground clamps. 
 	//////////////////////////////////////////////////////////////////////////////////////////
	void GroundClamps2(CPlayer *player, WorldCollisionData TestLine, WorldCollisionData oldPOS, 
					   CLevelObject *pDanObject);

	//////////////////////////////////////////////////////////////////////////////////////////
	//Function Name:	GroudClamps2()
	//
	//Purpose:			This function will be used for powerup ground clamping.
	//
	//Last Modified:	8/23/04
	// 
	//Input:			CPowerup, TestLine 
	//
	//Output:			none
	//
	//Return:			None
	//
	//Notes:			this function is used for ground clamps. 
 	//////////////////////////////////////////////////////////////////////////////////////////
	void GroundClamps2(CPowerup *powerup, WorldCollisionData TestLine);



	//////////////////////////////////////////////////////////////////////////////////////////
	//Function Name:	Elastic()
	//
	//Purpose:			This is the elastic collision routine for 2 players
	//
	//Last Modified:	8/23/04
	// 
	//Input:			CPlayer CPlayer
	//
	//Output:			CPlayer, CPlayer
	//
	//Return:			None
	//
	//Notes:			yeeeehaw
 	//////////////////////////////////////////////////////////////////////////////////////////
	void Elastic(CPlayer *Player1, CPlayer *Player2);


};

