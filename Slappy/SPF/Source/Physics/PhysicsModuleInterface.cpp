#include "PhysicsModuleInterface.h"
#include "EventHandler.h"



#define MoveMemPointerForwardx(NumberOfBytes, LevelCounter) CObjectManager::GetInstance()->m_LevelObjectArray[LevelCounter]->CollisionData = (void*)((char*)CObjectManager::GetInstance()->m_LevelObjectArray[LevelCounter]->CollisionData + NumberOfBytes);
#define MoveMemPointerBackwardx(NumberOfBytes, LevelCounter) CObjectManager::GetInstance()->m_LevelObjectArray[LevelCounter]->CollisionData = (void*)((char*)CObjectManager::GetInstance()->m_LevelObjectArray[LevelCounter]->CollisionData - NumberOfBytes);

#define POWERUP_SPEED 2.0f

CPhysicsModule *CPhysicsModule::pInstance = 0;
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
void CPhysicsModule::DeleteInst()
{
	// delete the instance
	if(pInstance)
	{
		//delete pInstance;
		pInstance->CPhysicsModule::~CPhysicsModule();
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
CPhysicsModule *CPhysicsModule::GetInstance(void)
{
	if (pInstance == 0)
	{
		pInstance = (CPhysicsModule*)MemMalloc(sizeof(CPhysicsModule));
		pInstance->CPhysicsModule::CPhysicsModule();
	}
	return pInstance;
}


CPhysicsModule::CPhysicsModule(void)
{
	m_fCurrentTime = m_fElapseTime = m_fOldTime = 0;
	CollisionDetector = CCollisionDetector::GetInstance();
}


CPhysicsModule::~CPhysicsModule(void)
{
}


void CPhysicsModule::ApplyForceToPlayer(CPlayer* player)
{
	// get the position, and store the it in PlayerPosition vector.
	NEW_VEC4D(PlayerPosition);
	getPosVec4DT(&PlayerPosition, player->m_PandO);

	if (player->GetState() == PhysicsUp_state)
		player->m_Velocity.tfY = player->m_fJumpHeight;

	switch (player->m_iMovement)
	{

		//left
	case 1:
		{
			player->m_Velocity.tfX -= player->m_fWalkSpeed * m_fElapseTime;
			if (player->m_Velocity.tfX > 57.0f)
				player->m_Velocity.tfX = 57.0f;
			Vec4DT look = {-1.0f, 0.0f, 0.0f};
			Vec4DT right = {0.0f, 0.0f, 1.0f};
			setLookAtVec4DT(look, player->m_PandO);
			setRightVec4DT(right, player->m_PandO);
		}

		break;

		// right
	case 2:
		{
			player->m_Velocity.tfX += player->m_fWalkSpeed * m_fElapseTime;
			if (player->m_Velocity.tfX > 57.0f)
				player->m_Velocity.tfX = 57.0f;
			Vec4DT look = {1.0f, 0.0f, 0.0f};
			Vec4DT right = {0.0f, 0.0f, -1.0f};
			setLookAtVec4DT(look, player->m_PandO);
			setRightVec4DT(right, player->m_PandO);
		}
		break;
	}
}

void CPhysicsModule::ApplyForceToPowerUps()
{
	// Im making a temp pointer to the head of the list of powerups
	CPowerup* Temp;
	Temp = CObjectManager::GetInstance()->m_ActivePowerupList;
	Vec4DT PowerUpPosition;

	// used for which way the player is facing.
	Vec4DT LookAtVec;
	getLookAtVec4DT(&LookAtVec, CObjectManager::GetInstance()->m_PlayerArray[0]->m_PandO);

	while (Temp)
	{
		switch (Temp->GetState())
		{
		case Emitted_state:
			{
				// Calculate New Velocity
				Temp->m_Velocity.tfY -= (GRAVITY * m_fElapseTime);
				Temp->m_Velocity.tfX *= (.99f);
				
				// lets make the power ups travel on the Z-plane
				Temp->m_Velocity.tfZ -= ((GRAVITY) * m_fElapseTime);




				// Get Old Position of PowerUp

				getPosVec4DT(&PowerUpPosition, Temp->m_PandO);

				PowerUpPosition.tfY += (Temp->m_Velocity.tfY * m_fElapseTime);
				PowerUpPosition.tfX += (Temp->m_Velocity.tfX * m_fElapseTime);


				PowerUpPosition.tfZ += (Temp->m_Velocity.tfZ * m_fElapseTime);
				if (PowerUpPosition.tfZ < 0.0f)
					PowerUpPosition.tfZ = 0.0f;
 
				// rotate the powerup untill it hits the ground.
				if (!Temp->m_bHitGround)
 					rotXMat4x4T(&Temp->m_PandO,((GetGameTimer() * 7)));
 			



				// Set the new Position of the Powerup
				setPosVec4DT(PowerUpPosition, Temp->m_PandO);

			}
			break;

		case Thrown_state:
			{

				// are the velocities stored inside the powerup themselves? if not, we' just gotta tweak the velocity 
				// here.
				//Temp->m_Velocity.tfX += POWERUP_SPEED * LookAtVec.tfX;

				getPosVec4DT(&PowerUpPosition, Temp->m_PandO);
				PowerUpPosition.tfX += (Temp->m_Velocity.tfX * m_fElapseTime);


				rotZMat4x4T(&Temp->m_PandO,((GetGameTimer() * 9)));

				setPosVec4DT(PowerUpPosition, Temp->m_PandO);
				//Temp->SendStateMessage(Thrown_state);
			}
			break;
		}
		Temp = Temp->m_pNext;
	}
}

void CPhysicsModule::ApplyBobbing()
{	
	
	Vec4DT levelPos;
	for (int i = 0; i < CObjectManager::GetInstance()->m_iLevelObjectArraySize; i++)
	{
		// HACK FOR THE FERRIS WHEEL SHITS
 		if (CObjectManager::GetInstance()->m_LevelObjectArray[i]->m_fMaxBob == 0.0f)
 			continue;

		getPosVec4DT(&levelPos, CObjectManager::GetInstance()->m_LevelObjectArray[i]->m_PandO);

		// m_fMaxBob is the WavePeriod of the Sine-Wave
		// m_fMinBob is the Magnitude of the Sine-Wave
 		levelPos.tfY = SPF_COS(GetGameTimer() * CObjectManager::GetInstance()->m_LevelObjectArray[i]->m_fMaxBob ) * 
 			CObjectManager::GetInstance()->m_LevelObjectArray[i]->m_fMinBob;
	
 		setPosVec4DT(levelPos, CObjectManager::GetInstance()->m_LevelObjectArray[i]->m_PandO);
	}
}

void CPhysicsModule::ApplyRotation()
{
	Vec4DT levelPos;
	for (int i = 0; i < CObjectManager::GetInstance()->m_iLevelObjectArraySize; i++)
	{

		getPosVec4DT(&levelPos, CObjectManager::GetInstance()->m_LevelObjectArray[i]->m_PandO);

		m_oldPando[i] = CObjectManager::GetInstance()->m_LevelObjectArray[i]->m_PandO;

		rotZMat4x4T(&CObjectManager::GetInstance()->m_LevelObjectArray[i]->m_PandO, 
			(SPF_COS(GetGameTimer() * CObjectManager::GetInstance()->m_LevelObjectArray[i]->m_fWavePeriod) *
			CObjectManager::GetInstance()->m_LevelObjectArray[i]->m_fMagnitude) );

		setPosVec4DT(levelPos, CObjectManager::GetInstance()->m_LevelObjectArray[i]->m_PandO);
	}
}



void CPhysicsModule::ApplyForceToWorldObjects()
{
 	ApplyBobbing();
	ApplyRotation();
}


void CPhysicsModule::UpdatePhysics()
{

	CPowerup* TempPowerUp;
	TempPowerUp = CObjectManager::GetInstance()->m_ActivePowerupList;

	// get the time
	m_fCurrentTime = GetGameTimer();
	m_fElapseTime = m_fCurrentTime - m_fOldTime;

	// HACK to keep the bastards on the board.
	if (m_fElapseTime > 0.050f)
		m_fElapseTime = 0.050f;

	// temp used so we can do the Matrix Multiply correctly
	Vec4DT Temp;
	Vec4DT PlayerOldPosition;


	// Apply changes to world objects.
	ApplyForceToPowerUps(); 
	ApplyForceToWorldObjects();


	// always apply gravity to objects in air. //////////////////////////////////////
	for (int a = 0; a < CObjectManager::GetInstance()->m_iPlayerArraySize; a++)
	{
		ApplyForceToPlayer(CObjectManager::GetInstance()->m_PlayerArray[a]);

		Vec4DT PlayerPosition;
		// take away gravity from their velocity
		CObjectManager::GetInstance()->m_PlayerArray[a]->m_Velocity.tfY -= (GRAVITY * m_fElapseTime);
		// take away "wind friction" from their X velocity
		CObjectManager::GetInstance()->m_PlayerArray[a]->m_Velocity.tfX *= (.9901f);

		//Terminal velocity baby
		if (CObjectManager::GetInstance()->m_PlayerArray[a]->m_Velocity.tfY < -19.6)
			CObjectManager::GetInstance()->m_PlayerArray[a]->m_Velocity.tfY = -19.6f;

		// get current player's position
		getPosVec4DT(&PlayerPosition, CObjectManager::GetInstance()->m_PlayerArray[a]->m_PandO);

		// store their old position.
		PlayerOldPosition = PlayerPosition;

		// update their position
		PlayerPosition.tfY += (CObjectManager::GetInstance()->m_PlayerArray[a]->m_Velocity.tfY * m_fElapseTime);
		PlayerPosition.tfX += (CObjectManager::GetInstance()->m_PlayerArray[a]->m_Velocity.tfX * m_fElapseTime);

 		// set it
		setPosVec4DT(PlayerPosition, CObjectManager::GetInstance()->m_PlayerArray[a]->m_PandO);

		//Check for out of bounds
		//LEFT_BOUND
		if (PlayerPosition.tfX < CLevelManager::GetInstance()->m_fBoundaries[LEFT_BOUND])
		{
			CEventHandler::GetInstance()->PlayerHitBoundary(CObjectManager::GetInstance()->m_PlayerArray[a], LEFT_BOUND);
		}
		//RIGHT_BOUND
		if (PlayerPosition.tfX > CLevelManager::GetInstance()->m_fBoundaries[RIGHT_BOUND])
		{
			CEventHandler::GetInstance()->PlayerHitBoundary(CObjectManager::GetInstance()->m_PlayerArray[a], RIGHT_BOUND);
		}
		//TOP_BOUND
		if (PlayerPosition.tfY > CLevelManager::GetInstance()->m_fBoundaries[TOP_BOUND])
		{
			CEventHandler::GetInstance()->PlayerHitBoundary(CObjectManager::GetInstance()->m_PlayerArray[a], TOP_BOUND);
		}
		//BOTTOM_BOUND
		if (PlayerPosition.tfY < CLevelManager::GetInstance()->m_fBoundaries[BOTTOM_BOUND])
		{
			CEventHandler::GetInstance()->PlayerHitBoundary(CObjectManager::GetInstance()->m_PlayerArray[a], BOTTOM_BOUND);
		}

		// iterate thru all of the level objects
		for (int iLevelCounter = 0; iLevelCounter < CObjectManager::GetInstance()->m_iLevelObjectArraySize; iLevelCounter++)
		{
			// copy over the number of lines from the CollisionData memory chunck
			memcpy(&m_iNumLines, CObjectManager::GetInstance()->m_LevelObjectArray[iLevelCounter]->CollisionData, sizeof(int));
			// move the pointer forward
			MoveMemPointerForwardx(sizeof(int),iLevelCounter);
			// this is a temp varible for the line collision line.
			CCollisionDetector::WorldCollisionData TestLine;
			// iterate thru the number of lines in the world
			for (int j = 0; j < m_iNumLines; j++)
			{

				// set that shit so that its the right data. for the FERRIS WHEEL
				if (CObjectManager::GetInstance()->m_LevelObjectArray[iLevelCounter]->m_pOwner)
				{
					//place this object at the attachment point of the owner, 
					//as indexed by the iattached index number
					Vec4DT TempVec;
					//get a pointer to owner
					CLevelObject *pOwner = CObjectManager::GetInstance()->m_LevelObjectArray[iLevelCounter]->m_pOwner;
					MatVecMult(&TempVec, pOwner->m_PandO, pOwner->m_pOptionalAttachmentPoints[CObjectManager::GetInstance()->m_LevelObjectArray[iLevelCounter]->m_iAttachmentPointIndex]);
					//add offset
					setPosVec4DT(TempVec, CObjectManager::GetInstance()->m_LevelObjectArray[iLevelCounter]->m_PandO);
				}


				TempPowerUp = CObjectManager::GetInstance()->m_ActivePowerupList;
				//copy the line data from the chunck of memory 
				//covert to world space 
				//move the pointer to the next data set of line shits

				// ACCESS the Collision Data ARRAY style.
				TestLine =  (((CCollisionDetector::WorldCollisionData*)	CObjectManager::GetInstance()->m_LevelObjectArray[iLevelCounter]->CollisionData)[j]);

				// needs me some old data
				OldTestLine = TestLine;

NEW_VEC4D(newPOS);
NEW_VEC4D(oldPOS);

				// apply the rotation to the collision lines
				MatVecMult(&Temp, CObjectManager::GetInstance()->m_LevelObjectArray[iLevelCounter]->m_PandO, TestLine.StartLine);
				TestLine.StartLine = Temp;

				MatVecMult(&Temp, CObjectManager::GetInstance()->m_LevelObjectArray[iLevelCounter]->m_PandO, TestLine.EndLine);
				TestLine.EndLine = Temp;

				getPosVec4DT(&newPOS, CObjectManager::GetInstance()->m_LevelObjectArray[iLevelCounter]->m_PandO);

				// apply the rotation to the Old collision lines
				Temp = zeroVector;
				MatVecMult(&Temp, m_oldPando[iLevelCounter], OldTestLine.StartLine);
				OldTestLine.StartLine = Temp;

				MatVecMult(&Temp, m_oldPando[iLevelCounter], OldTestLine.EndLine);
				OldTestLine.EndLine = Temp;

				getPosVec4DT(&oldPOS, m_oldPando[iLevelCounter]);

 				// WE HAVE COLLISION WITH THE GROUND BITCH! 
				if (CollisionDetector->LineCircleTest(CObjectManager::GetInstance()->m_PlayerArray[a], TestLine) )
				{

					// send the action to the statemachine stating we have collided wit da gr-zound.
					// the clamps the clamps to the ground
					// take away all velocity from the bastards
					CollisionDetector->GroundClamps2(CObjectManager::GetInstance()->m_PlayerArray[a], 
						TestLine, 
						OldTestLine, 
						CObjectManager::GetInstance()->m_LevelObjectArray[iLevelCounter]);
				}
				//// NOW LETS CHECK OUT THE POWERUPS!
				while (TempPowerUp)
				{
					CPowerup *pNext = TempPowerUp->m_pNext;
					//	//test if player is colliding with a powerup and if they are picking it up
					if (CollisionDetector->CircleToCircleFromEntireToBottomTest(CObjectManager::GetInstance()->m_PlayerArray[a], TempPowerUp))
					{
						if (CEventHandler::GetInstance()->PowerupHitPlayer(TempPowerUp, 
							CObjectManager::GetInstance()->m_PlayerArray[a]))
						{
							TempPowerUp = 0;
						}
					}

					if (TempPowerUp)
					{
						TempPowerUp = TempPowerUp->m_pNext;
					}
					else
					{
						TempPowerUp = pNext;
					}
				}							
			}
			// move the pointer back for the next gal
			MoveMemPointerBackwardx(sizeof(int), iLevelCounter);
		} 
	}

	//*************************
	// DO POWERUPS NOW
	//********************

	// iterate thru all of the level objects
	for (int iLevelCounter = 0; iLevelCounter < CObjectManager::GetInstance()->m_iLevelObjectArraySize; iLevelCounter++)
	{
		// copy over the number of lines from the CollisionData memory chunck
		memcpy(&m_iNumLines, CObjectManager::GetInstance()->m_LevelObjectArray[iLevelCounter]->CollisionData, sizeof(int));
		// move the pointer forward
		MoveMemPointerForwardx(sizeof(int), iLevelCounter);
		// this is a temp varible for the line collision line.
		CCollisionDetector::WorldCollisionData TestLine;
		// iterate thru the number of lines in the world
		for (int j = 0; j < m_iNumLines; j++)
		{
			TempPowerUp = CObjectManager::GetInstance()->m_ActivePowerupList;
			//copy the line data from the chunck of memory 
			//covert to world space 
			//move the pointer to the next data set of line shits

			// ACCESS the Collision Data ARRAY style.
			TestLine =  (((CCollisionDetector::WorldCollisionData*) 
				CObjectManager::GetInstance()->m_LevelObjectArray[iLevelCounter]->CollisionData)[j]);

			// needs me some old data
			OldTestLine = TestLine;

			// apply the rotation to the collision lines
			MatVecMult(&Temp, CObjectManager::GetInstance()->m_LevelObjectArray[iLevelCounter]->m_PandO, TestLine.StartLine);
			TestLine.StartLine = Temp;

			MatVecMult(&Temp, CObjectManager::GetInstance()->m_LevelObjectArray[iLevelCounter]->m_PandO, TestLine.EndLine);
			TestLine.EndLine = Temp;

			// apply the rotation to the Old collision lines
			Temp = zeroVector;
			MatVecMult(&Temp, m_oldPando[iLevelCounter], OldTestLine.StartLine);
			OldTestLine.StartLine = Temp;

			MatVecMult(&Temp, m_oldPando[iLevelCounter], OldTestLine.EndLine);
			OldTestLine.EndLine = Temp;

			//// NOW LETS CHECK OUT THE POWERUPS!
			while (TempPowerUp)
			{
				if (TempPowerUp->GetState() == Held_state)
				{
					TempPowerUp = TempPowerUp->m_pNext;
					continue;
				}

				//check if the powerup is still on the board
				NEW_VEC4D(PowerupPos);
				getPosVec4DT(&PowerupPos, TempPowerUp->m_PandO);
				if (PowerupPos.tfX < CLevelManager::GetInstance()->m_fBoundaries[LEFT_BOUND] ||
					PowerupPos.tfX > CLevelManager::GetInstance()->m_fBoundaries[RIGHT_BOUND] ||
					PowerupPos.tfY > CLevelManager::GetInstance()->m_fBoundaries[TOP_BOUND] ||
					PowerupPos.tfY < CLevelManager::GetInstance()->m_fBoundaries[BOTTOM_BOUND])
				{
					CPowerup *pNextPowerup = TempPowerUp->m_pNext;
					CObjectManager::GetInstance()->KillPowerup(TempPowerUp);
					TempPowerUp = pNextPowerup;
					continue;
				}

				if (CollisionDetector->LineCircleTest(TempPowerUp, TestLine))
				{
					CEventHandler::GetInstance()->PowerupHitLevelObject(TempPowerUp, 
						CObjectManager::GetInstance()->m_LevelObjectArray[iLevelCounter]);
					CollisionDetector->GroundClamps2(TempPowerUp, TestLine);
				}

				CPowerup *pNext = TempPowerUp->m_pNext;

				if (TempPowerUp)
				{
					TempPowerUp = TempPowerUp->m_pNext;
				}
				else
				{
					TempPowerUp = pNext;
				}
			}	

		}
		// move the pointer back for the next gal
		MoveMemPointerBackwardx(sizeof(int), iLevelCounter);
	} 




	/*************************************************************************/
	// test collisions: 1) WITH PLAYERS TO PLAYERS, 2) PLAYERS WITH PILLOWS  //
	/*************************************************************************/
	for (int i = 0; i < CObjectManager::GetInstance()->m_iPlayerArraySize; i++)
	{
		for(int j = i+1; j < CObjectManager::GetInstance()->m_iPlayerArraySize; j ++)
		{
			if (CollisionDetector->CircleToCircleFromEntireToBottomTest((CPlayer*)(CObjectManager::GetInstance()->m_PlayerArray[i]), 
				(CPlayer*)(CObjectManager::GetInstance()->m_PlayerArray[j])))
			{

				CollisionDetector->Elastic(CObjectManager::GetInstance()->m_PlayerArray[i], CObjectManager::GetInstance()->m_PlayerArray[j]);
			}
		}
	}

	// TEST COLLISION WITH PILLOWS.
	for (int i = 0; i < CObjectManager::GetInstance()->m_iPlayerArraySize; i++)
	{
		for (int j = 0; j < CObjectManager::GetInstance()->m_iPlayerArraySize; j++)
		{
			//test for collisions with pillows
			if (CollisionDetector->CircleToCircleFromEntireToBottomTest(CObjectManager::GetInstance()->m_PlayerArray[i], 
				CObjectManager::GetInstance()->m_PillowArray[j]))
			{
				CEventHandler::GetInstance()->PillowHitPlayer(CObjectManager::GetInstance()->m_PillowArray[j], CObjectManager::GetInstance()->m_PlayerArray[i]);
			}
		}
	}

	// save old time.
	m_fOldTime = m_fCurrentTime;
}

void CPhysicsModule::InitLevelData()
{
	m_oldPando = (Matrix4x4T *)MemMalloc(CObjectManager::GetInstance()->m_iLevelObjectArraySize * sizeof(Matrix4x4T));
}

void CPhysicsModule::KillLevelData()
{
	if (m_oldPando)
		MemFree(m_oldPando);
	
	m_oldPando = 0;
}


