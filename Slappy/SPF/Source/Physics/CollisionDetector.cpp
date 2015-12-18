#include "collisiondetector.h"
#include "EventHandler.h"

// instance of this class
CCollisionDetector *CCollisionDetector::pInstance = 0;

// default constructor
CCollisionDetector::CCollisionDetector(void)
{
 
}

CCollisionDetector::~CCollisionDetector(void)
{
 }
///////////////////////////////////////////////////////////////////////////////////
//
// Function:		DeleteInst
//
// Last Modified:	08/23/04
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
void CCollisionDetector::DeleteInst()
{
	 // delete the instance
    if(pInstance)
    {
        //delete pInstance;
		pInstance->CCollisionDetector::~CCollisionDetector();
        MemFree(pInstance);

		pInstance = NULL;
    }
}

///////////////////////////////////////////////////////////////////////////////////
//
// Function:		GetInst
//
// Last Modified:	08/23/04
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
CCollisionDetector *CCollisionDetector::GetInstance(void)
{
	if (pInstance == 0)
	{
        pInstance = (CCollisionDetector*)MemMalloc(sizeof(CCollisionDetector));
		pInstance->CCollisionDetector::CCollisionDetector();
    }
	return pInstance;
}


bool CCollisionDetector::LineCircleTest(CBaseObject *ObjectX, WorldCollisionData TestLine)
{
	// gather some information before doing stuff...
	// we need to find out if the line is a verticle/nearverticle line
	NEW_VEC4D(PlayerPosition);
	getPosVec4DT(&PlayerPosition, ObjectX->m_PandO);

	float angle;
	angle = atan2f((TestLine.EndLine.tfY-TestLine.StartLine.tfY), 
			       (TestLine.EndLine.tfX-TestLine.StartLine.tfX));
	angle = SPF_RAD_TO_DEG*angle;
	
	
	float lineLength = 0;
	
	
	// this is where i do stuff if the object is colliding with a verticle line...
	// we're gonna return false because this part of the function will clamp it for us
	if ( angle  <= -57 && angle >= -120)
	{
		// a vector from the circle to the test line.
		NEW_VEC4D(VecToCircle);
		// that line's normal
		NEW_VEC4D(LineNormal);
		// the test line in vector form.
		NEW_VEC4D(LineVec);
		
		subVec4DT(&VecToCircle,  PlayerPosition, TestLine.StartLine);
		subVec4DT(&LineVec, TestLine.EndLine, TestLine.StartLine);

		// lets get that normal.. in 2D it's just -y/x 
		LineNormal.tfX = -LineVec.tfY;
		LineNormal.tfY = LineVec.tfX;

		lineLength =  LengthVec4DT(LineVec);
		normalizeVec4DT(&LineNormal, LineNormal);
		normalizeVec4DT(&LineVec, LineVec);

		// the distance from the circle to the plane (a verticle line)
		float fdis2Plane = DotVec4DT(LineNormal, VecToCircle);
		// the distance on the line where the circle is.
		float fdisOnLine = DotVec4DT(VecToCircle, LineVec);
		
		// check if the object is a player.
		if (ObjectX->m_iType == Player_obj)
		{
			// basically checking to see if she is near the plane,
			// fdis2Plane < 0.0f - half space test
			// fdis2Plane > fdisOnLine - makes this check a line segment check.
			// fdis2Plane > GET-radius - this checks to see if the radius of the player is on the plane
			if ((fdis2Plane < 0.0f) || 
				(fdis2Plane > fdisOnLine) || 
				(fdis2Plane > GET_BOTTOM_RADIUS((CPlayer*)(ObjectX))) ||
				(fdisOnLine < 0.0f) ||
				((fdisOnLine > lineLength +  GET_ENTIRE_RADIUS((CPlayer*)(ObjectX)))) )

			{
				return false;
			} 
		}

		if (ObjectX->m_iType == Powerup_obj)
		{
			if ((fdis2Plane < 0.0f) || 
				(fdis2Plane > fdisOnLine) || 
				(fdis2Plane > GET_POWERUP_RADIUS((CPowerup*)(ObjectX))) ||
				(fdisOnLine < 0.0f) ||
				(fdisOnLine > lineLength) )

			{
  				return false;
			} 
		}

		// Well found out that the object is colliding with the verticle line/near-verticle line
		NEW_VEC4D(newPoint);
		NEW_VEC4D(temp);
		
		// scale out the point on line where the object should be.
		sMultVec4DT(&newPoint, fdisOnLine, LineVec);
		addVec4DT(&newPoint, newPoint, TestLine.StartLine);



		// player object
		if (ObjectX->m_iType == Player_obj)
			sMultVec4DT(&temp, GET_CENTER_RADIUS((CPlayer*)(ObjectX)), LineNormal);
		// powerup object
		if (ObjectX->m_iType == Powerup_obj)
			sMultVec4DT(&temp, GET_POWERUP_RADIUS((CPowerup*)(ObjectX)), LineNormal);

		addVec4DT(&newPoint, newPoint, temp);

		

		// IF THE NEW POINT IS LESS THE THE TESTLINE's endpoint... no fucking way they're collid

		PlayerPosition.tfX = newPoint.tfX;
 
		setPosVec4DT(PlayerPosition, ObjectX->m_PandO);

		//if (newPoint.tfY < TestLine.EndLine.tfY)
		// 	return false;
		
		if (ObjectX->m_iType == Player_obj)
			((CPlayer*) (ObjectX))->m_Velocity.tfX = 0.0f;
		
		if (ObjectX->m_iType == Powerup_obj)
		{
			((CPowerup*) (ObjectX))->m_Velocity.tfX *= -1.50f;
		 	return true;
		}
		

		return false;
	}

	// well it's not a verticle line, so lets test to see if they are colliding with the hort. line
	float Distance = 0;

	float ScaleShit = 0;
	NEW_VEC4D(ShitsShits);
	NEW_VEC4D(lineDirection);
	NEW_VEC4D(VecToCircle);
 
	if (ObjectX->m_iType == Player_obj)
		PlayerPosition.tfY += GET_BOTTOM_POINT(ObjectX);
	// make vector from start of line to the center point of sphere.
	subVec4DT(&VecToCircle, PlayerPosition , TestLine.StartLine );  

	// make vector from start of line to end of line.
	subVec4DT(&lineDirection, TestLine.EndLine, TestLine.StartLine);  

	// Length of the line shits
 	lineLength =  LengthVec4DT(lineDirection);

	// normalize it.
 	normalizeVec4DT(&lineDirection, lineDirection);

	// find the dot product of lineDirection and Vector to Circle
 	ScaleShit =  DotVec4DT(lineDirection, VecToCircle);

   	// not touch so return false. no need to do anything else.
 	if ( (ScaleShit < 0.0f) || (ScaleShit > lineLength) )
 		return false;
 
	sMultVec4DT(&lineDirection, ScaleShit, lineDirection);
 
	addVec4DT(&lineDirection, TestLine.StartLine, lineDirection);
	subVec4DT(&ShitsShits, lineDirection, PlayerPosition);
	
	
	switch (ObjectX->m_iType)
	{
		case Player_obj:
		{
			Distance = (LengthVec4DT(ShitsShits)) - (GET_BOTTOM_RADIUS(ObjectX)) ;
			if (Distance < (GET_BOTTOM_RADIUS(ObjectX)  ))
   				return true;
   			else 
				return false;
		}
		
	 	break;

		case Powerup_obj:
		{
			Distance = (LengthVec4DT(ShitsShits)) - (GET_POWERUP_RADIUS(ObjectX)) ;

			if (Distance < GET_POWERUP_RADIUS(ObjectX))
			{
//					((CPowerup*) (ObjectX))->m_bHitGround = true;
					return true;
			}
			else
			{

				return false;
			}
		}
	 	break;
	}
	
	return false;
}

bool CCollisionDetector::CircleToCircleFromEntireToBottomTest(CBaseObject* object1, CBaseObject* object2)
{
	// start off false
	bool bCollision = false;

	// 1st lets see if the too collide with the entire radius
	bCollision = (EntireCircleToEntireCircle(object1, object2));
	// if this returns false they are not close enough to collide
	if (bCollision == false)
		return false;
    
	// check the inner circle
	// get the fuck out of here and return true if they collided
	bCollision = (CenterCircleToCenterCircle(object1, object2));
	if (bCollision == true)
		return true;
	
	// check the top circle
	// get the fuck out of here and return true if they collided
	bCollision = (TopCircleToTopCircle(object1, object2));
	if (bCollision == true)
		return true;
	
	//check the bottom circle
	// get the fuck out of here and return true if they collided
	bCollision = (BottomCircleToBottomCircle(object1, object2));
	if (bCollision == true)
		return true;
	


	// well if none of these fuckers returned true from the method then no collision has occured.
	return false;
}

bool CCollisionDetector::CenterCircleToCenterCircle(CBaseObject *object1, CBaseObject* object2)
{
	NEW_VEC4D(ObjectPosition1);
	NEW_VEC4D(ObjectPosition2);
	getPosVec4DT(&ObjectPosition1, object1->m_PandO);
	getPosVec4DT(&ObjectPosition2, object2->m_PandO);
	float fsumRadiiSquared = 0;
	float fRadiiObject1 = 0;
	float fRadiiObject2 = 0;
 
	switch(object1->m_iType)
	{
		case Player_obj:
			ObjectPosition1.tfY += GET_CENTER_POINT(object1);
			fRadiiObject1 = GET_CENTER_RADIUS(object1);
			break;
		case Powerup_obj:
			// POWERUP CENTER POINTS is the (x,y) position.
			fRadiiObject1 =  GET_POWERUP_RADIUS(object1);
			break;
		case Pillow_obj:
			//ObjectPosition1.tfY += GET_PILLOW_RADIUS(object1);
			fRadiiObject1 = GET_PILLOW_RADIUS(object1);
			break;
	}

	switch(object2->m_iType)
	{
		case Player_obj:
			ObjectPosition2.tfY += GET_CENTER_POINT(object2);
			fRadiiObject2 = GET_CENTER_RADIUS(object2);
			break;
		case Powerup_obj:
			fRadiiObject2 = GET_POWERUP_RADIUS(object2);
			break;
		case Pillow_obj:
			//ObjectPosition2.tfY += GET_PILLOW_RADIUS(object2);
			fRadiiObject2 = GET_PILLOW_RADIUS(object2);
			break;
	}

	// Calculate Delta-X
	float deltaXsquared = (ObjectPosition2.tfX - ObjectPosition1.tfX);
	// Square it
	deltaXsquared *= deltaXsquared;
	// Calculate Delta-Y
	float deltaYsquared = (ObjectPosition2.tfY - ObjectPosition1.tfY );
	// Square it
	deltaYsquared *= deltaYsquared;
 
	// Calculate the sum Radii and square it Squared.
	fsumRadiiSquared = fRadiiObject1 + fRadiiObject2;
 	fsumRadiiSquared *= fsumRadiiSquared;
 
	// Test if they collide or not.
	if ((deltaXsquared + deltaYsquared) <= fsumRadiiSquared)
		return true;
	else
		return false; 
}

bool CCollisionDetector::BottomCircleToBottomCircle(CBaseObject* object1, CBaseObject* object2)
{
	NEW_VEC4D(ObjectPosition1);
	NEW_VEC4D(ObjectPosition2);
	getPosVec4DT(&ObjectPosition1, object1->m_PandO);
	getPosVec4DT(&ObjectPosition2, object2->m_PandO);
	float fsumRadiiSquared = 0;
	float fRadiiObject1 = 0;
	float fRadiiObject2 = 0;
 
	switch(object1->m_iType)
	{
		case Player_obj:
			ObjectPosition1.tfY += GET_BOTTOM_POINT(object1);
			fRadiiObject1 = GET_BOTTOM_RADIUS(object1);
			break;
		case Powerup_obj:
			// POWERUP CENTER POINTS is the (x,y) position.
			fRadiiObject1 =  GET_POWERUP_RADIUS(object1);
			break;
		case Pillow_obj:
			//ObjectPosition1.tfY += GET_PILLOW_RADIUS(object1);
			fRadiiObject1 = GET_PILLOW_RADIUS(object1);
			break;
	}

	switch(object2->m_iType)
	{
		case Player_obj:
			ObjectPosition2.tfY += GET_BOTTOM_POINT(object2);
			fRadiiObject2 = GET_BOTTOM_RADIUS(object2);
			break;
		case Powerup_obj:
			fRadiiObject2 = GET_POWERUP_RADIUS(object2);
			break;
		case Pillow_obj:
			//ObjectPosition2.tfY += GET_PILLOW_RADIUS(object2);
			fRadiiObject2 = GET_PILLOW_RADIUS(object2);
			break;
	}

	// Calculate Delta-X
	float deltaXsquared = (ObjectPosition2.tfX - ObjectPosition1.tfX);
	// Square it
	deltaXsquared *= deltaXsquared;
	// Calculate Delta-Y
	float deltaYsquared = (ObjectPosition2.tfY - ObjectPosition1.tfY );
	// Square it
	deltaYsquared *= deltaYsquared;
 
	// Calculate the sum Radii and square it Squared.
	fsumRadiiSquared = fRadiiObject1 + fRadiiObject2;
 	fsumRadiiSquared *= fsumRadiiSquared;
 
	// Test if they collide or not.
	if ((deltaXsquared + deltaYsquared) <= fsumRadiiSquared)
		return true;
	else
		return false; 
}

bool CCollisionDetector::TopCircleToTopCircle(CBaseObject* object1, CBaseObject* object2)
{
	NEW_VEC4D(ObjectPosition1);
	NEW_VEC4D(ObjectPosition2);
	getPosVec4DT(&ObjectPosition1, object1->m_PandO);
	getPosVec4DT(&ObjectPosition2, object2->m_PandO);
	float fsumRadiiSquared = 0;
	float fRadiiObject1 = 0;
	float fRadiiObject2 = 0;
 
	switch(object1->m_iType)
	{
		case Player_obj:
			ObjectPosition1.tfY += GET_TOP_POINT(object1);
			fRadiiObject1 = GET_TOP_RADIUS(object1);
			break;
		case Powerup_obj:
			// POWERUP CENTER POINTS is the (x,y) position.
			fRadiiObject1 =  GET_POWERUP_RADIUS(object1);
			break;
		case Pillow_obj:
			//ObjectPosition1.tfY += GET_PILLOW_RADIUS(object1);
			fRadiiObject1 = GET_PILLOW_RADIUS(object1);
			break;
	}

	switch(object2->m_iType)
	{
		case Player_obj:
			ObjectPosition2.tfY += GET_TOP_POINT(object2);
			fRadiiObject2 = GET_TOP_RADIUS(object2);
			break;
		case Powerup_obj:
			fRadiiObject2 = GET_POWERUP_RADIUS(object2);
			break;
		case Pillow_obj:
			//ObjectPosition2.tfY += GET_PILLOW_RADIUS(object2);
			fRadiiObject2 = GET_PILLOW_RADIUS(object2);
			break;
	}

	// Calculate Delta-X
	float deltaXsquared = (ObjectPosition2.tfX - ObjectPosition1.tfX);
	// Square it
	deltaXsquared *= deltaXsquared;
	// Calculate Delta-Y
	float deltaYsquared = (ObjectPosition2.tfY - ObjectPosition1.tfY );
	// Square it
	deltaYsquared *= deltaYsquared;
 
	// Calculate the sum Radii and square it Squared.
	fsumRadiiSquared = fRadiiObject1 + fRadiiObject2;
 	fsumRadiiSquared *= fsumRadiiSquared;
 
	// Test if they collide or not.
	if ((deltaXsquared + deltaYsquared) <= fsumRadiiSquared)
		return true;
	else
		return false; 

}


bool CCollisionDetector::EntireCircleToEntireCircle(CBaseObject* object1, CBaseObject* object2)
{	
	NEW_VEC4D(ObjectPosition1);
	NEW_VEC4D(ObjectPosition2);
	getPosVec4DT(&ObjectPosition1, object1->m_PandO);
	getPosVec4DT(&ObjectPosition2, object2->m_PandO);
	float fsumRadiiSquared = 0;
	float fRadiiObject1 = 0;
	float fRadiiObject2 = 0;
 
	switch(object1->m_iType)
	{
		case Player_obj:
			ObjectPosition1.tfY += GET_CENTER_POINT(object1);
			fRadiiObject1 = GET_ENTIRE_RADIUS(object1);
			break;
		case Powerup_obj:
			// POWERUP CENTER POINTS is the (x,y) position.
			fRadiiObject1 =  GET_POWERUP_RADIUS(object1);
			break;
		case Pillow_obj:
			//ObjectPosition1.tfY += GET_PILLOW_RADIUS(object1);
			fRadiiObject1 = GET_PILLOW_RADIUS(object1);
			break;
	}

	switch(object2->m_iType)
	{
		case Player_obj:
			ObjectPosition2.tfY += GET_CENTER_POINT(object2);
			fRadiiObject2 = GET_ENTIRE_RADIUS(object2);
			break;
		case Powerup_obj:
			fRadiiObject2 = GET_POWERUP_RADIUS(object2);
			break;
		case Pillow_obj:
			//ObjectPosition2.tfY += GET_PILLOW_RADIUS(object2);
			fRadiiObject2 = GET_PILLOW_RADIUS(object2);
			break;
	}

	// Calculate Delta-X
	float fdeltaXsquared = (ObjectPosition2.tfX - ObjectPosition1.tfX);
	// Square it
	fdeltaXsquared *= fdeltaXsquared;
	// Calculate Delta-Y
	float fdeltaYsquared = (ObjectPosition2.tfY - ObjectPosition1.tfY );
	// Square it
	fdeltaYsquared *= fdeltaYsquared;
 
	// Calculate the sum Radii and square it Squared.
	fsumRadiiSquared = fRadiiObject1 + fRadiiObject2;
 	fsumRadiiSquared *= fsumRadiiSquared;
 
	// Test if they collide or not.
	if ((fdeltaXsquared + fdeltaYsquared) <= fsumRadiiSquared)
		return true;
	else
		return false; 
}




void CCollisionDetector::Elastic(CPlayer *Player1, CPlayer *Player2)
{
	//get a vector between the two players
	NEW_VEC4D(PlayerPosition1);
	NEW_VEC4D(PlayerPosition2);
	getPosVec4DT(&PlayerPosition1, Player1->m_PandO);
	getPosVec4DT(&PlayerPosition2, Player2->m_PandO);
	float fVelocityP1 = LengthVec4DT(Player1->m_Velocity);
	float fVelocityP2 = LengthVec4DT(Player2->m_Velocity);
	NEW_VEC4D(P1NewVel);
	NEW_VEC4D(P2NewVel);
	subVec4DT(&P1NewVel, PlayerPosition1, PlayerPosition2);
	subVec4DT(&P2NewVel, PlayerPosition2, PlayerPosition1);

	//The bump is the distance between the two players
	float P1Bump = (LengthVec4DT(P1NewVel) / 2.0f);
	float P2Bump = (LengthVec4DT(P2NewVel) / 2.0f);
	if (P1Bump <= GET_BOTTOM_RADIUS(Player1))
	{
		P1Bump = (GET_BOTTOM_RADIUS(Player1) + .01f);
		P2Bump = (GET_BOTTOM_RADIUS(Player2) + .01f);
	}
	P1Bump -= GET_BOTTOM_RADIUS(Player1);
	P2Bump -= GET_BOTTOM_RADIUS(Player2);

	normalizeVec4DT(&P1NewVel, P1NewVel);
	normalizeVec4DT(&P2NewVel, P2NewVel);

	//move the players apart by bump distance
	NEW_VEC4D(P1MoveVel);
	NEW_VEC4D(P2MoveVel);
	P1MoveVel = P1NewVel;
	P2MoveVel = P2NewVel;
	sMultVec4DT(&P1MoveVel, P1Bump, P1MoveVel);
	sMultVec4DT(&P2MoveVel, P2Bump, P2MoveVel);
	addVec4DT(&PlayerPosition1, PlayerPosition1, P1MoveVel);
	addVec4DT(&PlayerPosition2, PlayerPosition2, P2MoveVel);

	//set the players velocity to the new vel
	setPosVec4DT(PlayerPosition1, Player1->m_PandO);
	setPosVec4DT(PlayerPosition2, Player2->m_PandO);
	sMultVec4DT(&P1NewVel, fVelocityP1, P1NewVel);
	sMultVec4DT(&P2NewVel, fVelocityP2, P2NewVel);
  	Player1->m_Velocity = P1NewVel;
	Player2->m_Velocity = P2NewVel;
}

void CCollisionDetector::LoadPlayerCollisionData(PlayerCollisionData* CollisionOutput, CPlayer* PlayerInput)
{
 	float *temp = (float *)PlayerInput->CollisionData;
 	CollisionOutput->fBottomCenterPoint = temp[BOTTOMCENTERPOINT];
	CollisionOutput->fBottomRadius = temp[BOTTOMRADIUS];
	CollisionOutput->fCenterPoint = temp[CENTERPOINT];
	CollisionOutput->fCenterRadius = temp[CENTERRADIUS];
	CollisionOutput->fEntireRadius = temp[ENTIRERADIUS];
	CollisionOutput->fTopCenterPoint = temp[TOPCENTERPOINT];
	CollisionOutput->fTopRadius = temp[TOPRADIUS];
};



void CCollisionDetector::GroundClamps2(CPlayer *player, WorldCollisionData TestLine, WorldCollisionData oldPOS, CLevelObject *pDanObject)
{
	// early escape if they are still jumping
	if (player->m_Velocity.tfY > 0.0f)
	  	return;
	 
	if (player->GetState() == Falling_state)
	{
		CEventHandler::GetInstance()->PlayerHitLevelObject(player, pDanObject);
		return;
	}
 	
	float y, m, x, b;
	float dY;
	// keeps the player from sliding back a forth.
	float fShifter = (TestLine.StartLine.tfX - oldPOS.StartLine.tfX);
 
	// get the player's position
	NEW_VEC4D(ObjectPos);
	getPosVec4DT(&ObjectPos, player->m_PandO);

	// Need to create the Y=M(x)+B formula. 
	// I gots the Slope. (Y1-Y0) / ( X1-X0) 
	// I need to Make B.. B = Y-M(x)
 
	// lets use the points from the TestLine.Endline to be Y and X for solving the equation shits
	y = TestLine.StartLine.tfY;
	x = TestLine.StartLine.tfX;
	m = (TestLine.EndLine.tfY - TestLine.StartLine.tfY) / (TestLine.EndLine.tfX - TestLine.StartLine.tfX);
	b = y - (float)(m*(x));

	// got the shits i need. lets plug in the player's X to find it's Y and put that bastard there.
	dY = m*(ObjectPos.tfX) + b;
  
 	// set the player's Y to the new Y position
	ObjectPos.tfY = dY;
	player->m_Velocity.tfY = 0.0f;
  
	//correct the ssliding 


	ObjectPos.tfX += fShifter;

	setPosVec4DT(ObjectPos, player->m_PandO);
	
	CEventHandler::GetInstance()->PlayerHitLevelObject(player, pDanObject);

}
//ground clamping for powerup items
void CCollisionDetector::GroundClamps2(CPowerup *object, WorldCollisionData TestLine)
{


	float angle;
	angle = atan2f((TestLine.EndLine.tfY-TestLine.StartLine.tfY), 
				   (TestLine.EndLine.tfX-TestLine.StartLine.tfX));
	angle = SPF_RAD_TO_DEG*angle;

	if (object->m_Velocity.tfY > 0.0f)
		return;

	Vec4DT ObjectPos;
	getPosVec4DT(&ObjectPos, object->m_PandO);



	// Need to create the Y=M(x)+B formula. 
	// I gots the Slope. (Y1-Y0) / ( X1-X0) 
	// I need to Make B.. B = Y-M(x)
	// next just plug the X component of the Player into the formalua and the the Y position on that line.

	float y, m, x, b;
	float dY;

	// lets use the points from the TestLine.Endline to be Y and X for solving the equation shits
	y = TestLine.EndLine.tfY;
	x = TestLine.EndLine.tfX;
	m = (TestLine.EndLine.tfY - TestLine.StartLine.tfY) / (TestLine.EndLine.tfX - TestLine.StartLine.tfX);
	b = y - (float)(m*(x));

	// got the shits i need. lets plug in the player's X to find it's Y and put that bastard there.
	dY = m*(ObjectPos.tfX) + b;
 
 	if ( angle  <= -80 && angle >= -100)
	{
 		
 	 	ObjectPos.tfX = TestLine.EndLine.tfX + GET_ENTIRE_RADIUS(object) * 0.5f;
		setPosVec4DT(ObjectPos, object->m_PandO);
 		return;
	}
	
	object->m_Velocity.tfY = 0.0f;
	ObjectPos.tfY =  dY + (GET_POWERUP_RADIUS(object) * .5f);   
	setPosVec4DT(ObjectPos, object->m_PandO);
	object->m_bHitGround = true;

}