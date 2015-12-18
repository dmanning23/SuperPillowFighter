///////////////////////////////////////////////////////////////
// File Name:     AniMule.cpp
// 
// Author:        Matt Strand
//
// Date Created:  8/26/2004
//
// Purpose:       This file initializes and updates player
//                animations.
///////////////////////////////////////////////////////////////

#include "AniMule.h"
#include "../GameTimer/GameTimer.h"
#include "../ContentLoader/animImp.h"

// Initialize the only pointer to this class.
CAniMule *CAniMule::m_pInstance = 0;

///////////////////////////////////////////////////////////////
// Function: initialize()
//
// Purpose: Sets up all the bind info for the players.
//
// Last Modified: 9/18/2004
//
// Input: NONE
//
// Output: *m_pBinds                        - Filled out.
//         CPlayer::m_pAnimationData->tBind - Released.
//
// Returns: NONE
//
// Notes: Must be called before update can be used.  Update
//        return immediately otherwise.  Release bind matrices
//        for this player.
///////////////////////////////////////////////////////////////
void CAniMule::initialize(void)
{
	int i/*, k*/;	// For loops
	unsigned int j;	// For loops

	Vec4DT tempVec;
	Matrix4x4T matrixResult, oneMat, twoMat, threeMat;

	// Get an instance to the ObjectManager.
	CObjectManager *pObjManInst = CObjectManager::GetInstance();

	// Get enough memory to hold all the Bind info pointers.
	m_pBinds = (BindT *)MemMalloc(sizeof(BindT) * pObjManInst->m_iPlayerArraySize);

	// Set up how many players that we are storing.
	m_iNumPlayers = pObjManInst->m_iPlayerArraySize;

	// Go through every player.
	for(i = 0; i < m_iNumPlayers; i++)
	{
		// Get enough memory for the geometry data.
		m_pBinds[i].tpOrigGeom = (GeometryT *)MemMalloc(sizeof(GeometryT));

		// Set this player's ID.
		m_pBinds[i].tiPId = pObjManInst->m_PlayerArray[i]->m_pPlayerData->iCharacter;

		// Set up the initial data of for the geometry.
		m_pBinds[i].tpOrigGeom->tfRGBAColor[0] = pObjManInst->m_PlayerArray[i]->m_pGeometryData->tfRGBAColor[1];
		m_pBinds[i].tpOrigGeom->tfRGBAColor[1] = pObjManInst->m_PlayerArray[i]->m_pGeometryData->tfRGBAColor[2];
		m_pBinds[i].tpOrigGeom->tfRGBAColor[2] = pObjManInst->m_PlayerArray[i]->m_pGeometryData->tfRGBAColor[3];
		m_pBinds[i].tpOrigGeom->tfRGBAColor[3] = pObjManInst->m_PlayerArray[i]->m_pGeometryData->tfRGBAColor[4];
		m_pBinds[i].tpOrigGeom->tuNumVerts = pObjManInst->m_PlayerArray[i]->m_pGeometryData->tuNumVerts;
		m_pBinds[i].tpOrigGeom->tuTexID = pObjManInst->m_PlayerArray[i]->m_pGeometryData->tuTexID;

		// Get enough memory to hold all the vertices for this player.
		m_pBinds[i].tpOrigGeom->tpVertices = (VertexT *)MemMalloc(sizeof(VertexT) * m_pBinds[i].tpOrigGeom->tuNumVerts);

		// Go through all the verts and copy their contents.
		for(j = 0; j < m_pBinds[i].tpOrigGeom->tuNumVerts; j++)
		{
			// Set up all the vertice data.
			m_pBinds[i].tpOrigGeom->tpVertices[j].tfTex[0] = pObjManInst->m_PlayerArray[i]->m_pGeometryData->tpVertices[j].tfTex[0];
			m_pBinds[i].tpOrigGeom->tpVertices[j].tfTex[1] = pObjManInst->m_PlayerArray[i]->m_pGeometryData->tpVertices[j].tfTex[1];
			m_pBinds[i].tpOrigGeom->tpVertices[j].tNormal = pObjManInst->m_PlayerArray[i]->m_pGeometryData->tpVertices[j].tNormal;
			m_pBinds[i].tpOrigGeom->tpVertices[j].tPosition = pObjManInst->m_PlayerArray[i]->m_pGeometryData->tpVertices[j].tPosition;
			m_pBinds[i].tpOrigGeom->tpVertices[j].tuJoints[0] = pObjManInst->m_PlayerArray[i]->m_pGeometryData->tpVertices[j].tuJoints[0];
			m_pBinds[i].tpOrigGeom->tpVertices[j].tuJoints[1] = pObjManInst->m_PlayerArray[i]->m_pGeometryData->tpVertices[j].tuJoints[1];
			m_pBinds[i].tpOrigGeom->tpVertices[j].tuJoints[2] = pObjManInst->m_PlayerArray[i]->m_pGeometryData->tpVertices[j].tuJoints[2];

			// Move the vertex position by the concatenated inverse matrix of the joints that affect it.
			// This is done so that later we can simply multiply the concatenated matrices of
			//  the correct joints without first having to do this.

			// Move a blank matrix based on the first joint's inverse concatenation matrix scaled by the weight for that joint.
			sMultMat4x4T(&oneMat, 
						m_pBinds[i].tpOrigGeom->tpVertices[j].tuJoints[0].tfWeight,
						pObjManInst->m_PlayerArray[i]->m_pAnimationData->tBind.tpJointInfo[m_pBinds[i].tpOrigGeom->tpVertices[j].tuJoints[0].tiJointId - 1].tConcMat);

			// Move a blank matrix based on the second joint's inverse concatenation matrix scaled by the weight for that joint.
			sMultMat4x4T(&twoMat, 
						m_pBinds[i].tpOrigGeom->tpVertices[j].tuJoints[1].tfWeight,
						pObjManInst->m_PlayerArray[i]->m_pAnimationData->tBind.tpJointInfo[m_pBinds[i].tpOrigGeom->tpVertices[j].tuJoints[1].tiJointId - 1].tConcMat);

			// Move a blank matrix based on the third joint's inverse concatenation matrix scaled by the weight for that joint.
			if(m_pBinds[i].tpOrigGeom->tpVertices[j].tuJoints[2].tiJointId)
				sMultMat4x4T(&threeMat, 
							m_pBinds[i].tpOrigGeom->tpVertices[j].tuJoints[2].tfWeight,
							pObjManInst->m_PlayerArray[i]->m_pAnimationData->tBind.tpJointInfo[m_pBinds[i].tpOrigGeom->tpVertices[j].tuJoints[2].tiJointId - 1].tConcMat);

			// Add them together!
			addMat4x4T(&matrixResult, oneMat, twoMat);
			if(m_pBinds[i].tpOrigGeom->tpVertices[j].tuJoints[2].tiJointId)
				addMat4x4T(&matrixResult, matrixResult, threeMat);

			// Multiply the vertex position by the result.
			MatVecMult(&tempVec,
				matrixResult,
				m_pBinds[i].tpOrigGeom->tpVertices[j].tPosition);

			// Set the position to this new value.
			m_pBinds[i].tpOrigGeom->tpVertices[j].tPosition = tempVec;
		}

		// Unload all inverse matrices.
		if(pObjManInst->m_PlayerArray[i]->m_pAnimationData->tBind.tpJointInfo)
			MemFree(pObjManInst->m_PlayerArray[i]->m_pAnimationData->tBind.tpJointInfo);

		//for(k = 0; k < pObjManInst->m_PlayerArray[i]->m_pAnimationData->tBind.tiJointCount; k++)
		//{
		//	MemFree(pObjManInst->m_PlayerArray[i]->m_pAnimationData->tBind.tpJointInfo[k].tpiChildren);
		//	MemFree(pObjManInst->m_PlayerArray[i]->m_pAnimationData->tBind.tpJointInfo[k].tpiChildren);
		//}
	}

	m_bInit = true;
}

// Shut this whole thing down.  Called during delete instance.
void CAniMule::shutdown(void)
{
	if(!m_bInit)
		return;

	int i;	// For loops
	if(m_pBinds)
	{
		for(i = 0; i < m_iNumPlayers; i++)
		{
			if(m_pBinds[i].tpOrigGeom)
			{
				if(m_pBinds[i].tpOrigGeom->tpVertices)
					MemFree(m_pBinds[i].tpOrigGeom->tpVertices);
				
				MemFree(m_pBinds[i].tpOrigGeom);
			}
		}

		MemFree(m_pBinds);
	}		

    m_iNumPlayers = 0;
	m_bInit = false;
}

///////////////////////////////////////////////////////////////
// Function: update()
//
// Purpose: Plays the correct animation based on current state.
//
// Last Modified: 9/18/2004
//
// Input: NONE
//
// Output: CPlayer::m_pGeometryData - Vertices modified.
//
// Returns: NONE
//
// Notes: Call as many times as you like.
///////////////////////////////////////////////////////////////
void CAniMule::update(void)
{
	if(!m_bInit)
	{
		return;
	}

	// Get an instance to the ObjectManager.
	CObjectManager *pObjManInst = CObjectManager::GetInstance();

	m_fAnimCurTime = GetGameTimer();

	int i, j, k/*, l, m, n*/;	// For loops
	int iState;
	float fTime;

	// Go through the entire list of players.
	for(i = 0; i < pObjManInst->m_iPlayerArraySize; i++)
	{
		// Is there a player there?
		if(pObjManInst->m_PlayerArray[i] == NULL)
			continue;

		// Check to see what play this is and what information we will be using to change it.
		for(j = 0; j < m_iNumPlayers; j++)
		{
			// If this bind info is correct, keep j.
			if(m_pBinds[j].tiPId == pObjManInst->m_PlayerArray[i]->m_pPlayerData->iCharacter)
				break;

			// No valid information found for this player.  Get the hell out of here!
			if((j + 1) == m_iNumPlayers)
				return;
		}

		// Checks current state and sets the correct animation state.
		// Also sets the right fTime for the animation.
		// fTime determines the factor of speed increase for the animation
		//  based upon this player's attack speed.  Certain animations are longer
		//  than others, but with this, all the attack animations are the same.
		// Converts PlayerStates to states recognized by the animation system.
		switch(pObjManInst->m_PlayerArray[i]->GetState())
		{
			case Standing_state:
				{
					iState = STANDING;
					fTime = 1.0f;
				}
				break;
			case Walking_state:
				{
					iState = WALKING;
					fTime = 1.0f;
				}
				break;
			case Jumping_state:
				{
					iState = JUMPING;
					fTime = 1.0f;
				}
				break;
			case DoubleJumping_state:
				{
					iState = JUMPING;
					fTime = 1.0f;

					// If we're coming for jumping to double jumping, start animation over.
					if(pObjManInst->m_PlayerArray[i]->m_iPreviousState == Jumping_state)
						pObjManInst->m_PlayerArray[i]->m_pAnimationData->tiPrevFrame = 0;
				}
				break;
			case Falling_state:
				{
					iState = FALLING;
					fTime = float((pObjManInst->m_PlayerArray[i]->m_pAnimationData->tpAnimations[iState].tiTotalTime / 1000.0f) / FALLING_TIME);
				}
				break;
			case Attack_state:
				{
					iState = ATTACK;
					fTime = float((pObjManInst->m_PlayerArray[i]->m_pAnimationData->tpAnimations[iState].tiTotalTime / 1000.0f) / pObjManInst->m_PlayerArray[i]->m_fAttackSpeed);
				}
				break;
			case ForwardAttack_state:
				{
					iState = ATTACK_FORWARD;
					fTime = float((pObjManInst->m_PlayerArray[i]->m_pAnimationData->tpAnimations[iState].tiTotalTime / 1000.0f) / pObjManInst->m_PlayerArray[i]->m_fAttackSpeed);
				}
				break;
			case SpecialAttack_state:
				{
					iState = ATTACK_SPECIAL;
					fTime = float((pObjManInst->m_PlayerArray[i]->m_pAnimationData->tpAnimations[iState].tiTotalTime / 1000.0f) / pObjManInst->m_PlayerArray[i]->m_fAttackSpeed);
				}
				break;
			case UpAttack_state:
				{
					iState = ATTACK_UP;
					fTime = float((pObjManInst->m_PlayerArray[i]->m_pAnimationData->tpAnimations[iState].tiTotalTime / 1000.0f) / pObjManInst->m_PlayerArray[i]->m_fAttackSpeed);
				}
				break;
			case AirSpecialAttack_state:
				{
					iState = AIR_ATTACK_SPECIAL;
					fTime = float((pObjManInst->m_PlayerArray[i]->m_pAnimationData->tpAnimations[iState].tiTotalTime / 1000.0f) / pObjManInst->m_PlayerArray[i]->m_fAttackSpeed);
				}
				break;
			case AirForwardAttack_state:
				{
					iState = AIR_ATTACK_FORWARD;
					fTime = float((pObjManInst->m_PlayerArray[i]->m_pAnimationData->tpAnimations[iState].tiTotalTime / 1000.0f) / pObjManInst->m_PlayerArray[i]->m_fAttackSpeed);
				}
				break;
			case AirDownAttack_state:
				{
					iState = AIR_ATTACK_DOWN;
					fTime = float((pObjManInst->m_PlayerArray[i]->m_pAnimationData->tpAnimations[iState].tiTotalTime / 1000.0f) / pObjManInst->m_PlayerArray[i]->m_fAttackSpeed);
				}
				break;
			case AirUpAttack_state:
				{
					iState = AIR_ATTACK_UP;
					fTime = float((pObjManInst->m_PlayerArray[i]->m_pAnimationData->tpAnimations[iState].tiTotalTime / 1000.0f) / pObjManInst->m_PlayerArray[i]->m_fAttackSpeed);
				}
				break;
			case Throwing_state:
				{
					iState = THROWING;
					fTime = float((pObjManInst->m_PlayerArray[i]->m_pAnimationData->tpAnimations[iState].tiTotalTime / 1000.0f) / pObjManInst->m_PlayerArray[i]->m_fAttackSpeed);
				}
				break;
		}

		// If this frame is different from the last, handle it here.
		if(iState != pObjManInst->m_PlayerArray[i]->m_pAnimationData->tiPrevState)
		{
			// When an air attack was the previous thing played,
			//  the player returns to last frame of jump animation.
			// Time is also set to right amount here as to not screw things
			//  up since we should not really be jumping to certain frames of
			//  animations like this.
			switch(pObjManInst->m_PlayerArray[i]->m_pAnimationData->tiPrevState)
			{
				case AIR_ATTACK_SPECIAL:
					{
						pObjManInst->m_PlayerArray[i]->m_pAnimationData->tiPrevFrame = pObjManInst->m_PlayerArray[i]->m_pAnimationData->tpAnimations[JUMPING].tiNumFrames - 1;
						pObjManInst->m_PlayerArray[i]->m_pAnimationData->tfStartTime = pObjManInst->m_PlayerArray[i]->m_pAnimationData->tfPrevTime - 
																						(pObjManInst->m_PlayerArray[i]->m_pAnimationData->tpAnimations[JUMPING].tpKeyInfo[
																							pObjManInst->m_PlayerArray[i]->m_pAnimationData->tiPrevFrame].tfTime * 1000.0f);
					}
					break;
				case AIR_ATTACK_FORWARD:
					{
						pObjManInst->m_PlayerArray[i]->m_pAnimationData->tiPrevFrame = pObjManInst->m_PlayerArray[i]->m_pAnimationData->tpAnimations[JUMPING].tiNumFrames - 1;
						pObjManInst->m_PlayerArray[i]->m_pAnimationData->tfStartTime = pObjManInst->m_PlayerArray[i]->m_pAnimationData->tfPrevTime - 
																						(pObjManInst->m_PlayerArray[i]->m_pAnimationData->tpAnimations[JUMPING].tpKeyInfo[
																							pObjManInst->m_PlayerArray[i]->m_pAnimationData->tiPrevFrame].tfTime * 1000.0f);
					}
					break;
				case AIR_ATTACK_DOWN:
					{
						pObjManInst->m_PlayerArray[i]->m_pAnimationData->tiPrevFrame = pObjManInst->m_PlayerArray[i]->m_pAnimationData->tpAnimations[JUMPING].tiNumFrames - 1;
						pObjManInst->m_PlayerArray[i]->m_pAnimationData->tfStartTime = pObjManInst->m_PlayerArray[i]->m_pAnimationData->tfPrevTime - 
																						(pObjManInst->m_PlayerArray[i]->m_pAnimationData->tpAnimations[JUMPING].tpKeyInfo[
																							pObjManInst->m_PlayerArray[i]->m_pAnimationData->tiPrevFrame].tfTime * 1000.0f);
					}
					break;
				case AIR_ATTACK_UP:
					{
						pObjManInst->m_PlayerArray[i]->m_pAnimationData->tiPrevFrame = pObjManInst->m_PlayerArray[i]->m_pAnimationData->tpAnimations[JUMPING].tiNumFrames - 1;
						pObjManInst->m_PlayerArray[i]->m_pAnimationData->tfStartTime = pObjManInst->m_PlayerArray[i]->m_pAnimationData->tfPrevTime - 
																						(pObjManInst->m_PlayerArray[i]->m_pAnimationData->tpAnimations[JUMPING].tpKeyInfo[
																							pObjManInst->m_PlayerArray[i]->m_pAnimationData->tiPrevFrame].tfTime * 1000.0f);
					}
					break;
				case FALLING:
					{
						pObjManInst->m_PlayerArray[i]->m_pAnimationData->tiPrevFrame = pObjManInst->m_PlayerArray[i]->m_pAnimationData->tpAnimations[JUMPING].tiNumFrames - 1;
						pObjManInst->m_PlayerArray[i]->m_pAnimationData->tfStartTime = pObjManInst->m_PlayerArray[i]->m_pAnimationData->tfPrevTime - 
																						(pObjManInst->m_PlayerArray[i]->m_pAnimationData->tpAnimations[JUMPING].tpKeyInfo[
																							pObjManInst->m_PlayerArray[i]->m_pAnimationData->tiPrevFrame].tfTime * 1000.0f);
					}
					break;
				default:
					pObjManInst->m_PlayerArray[i]->m_pAnimationData->tiPrevFrame = 0;
					break;
			}
		}

		// Essentially zero out the animation tracking data.
		if(pObjManInst->m_PlayerArray[i]->m_pAnimationData->tiPrevFrame == 0)
		{
			pObjManInst->m_PlayerArray[i]->m_pAnimationData->tfStartTime = m_fAnimCurTime;
			pObjManInst->m_PlayerArray[i]->m_pAnimationData->tfPrevTime = m_fAnimCurTime;
			pObjManInst->m_PlayerArray[i]->m_pAnimationData->tfElapsedTime = 0.0f;
		}

		// Get in the correct frame.
		for(k = pObjManInst->m_PlayerArray[i]->m_pAnimationData->tiPrevFrame; k < pObjManInst->m_PlayerArray[i]->m_pAnimationData->tpAnimations[iState].tiNumFrames; k++)
		{
			// If it is time, or passed time, to play the next frame of an animation... do it!
			if((m_fAnimCurTime - pObjManInst->m_PlayerArray[i]->m_pAnimationData->tfStartTime) >= (pObjManInst->m_PlayerArray[i]->m_pAnimationData->tpAnimations[iState].tpKeyInfo[k].tfTime / 1000.0f) / fTime)
			{
				// Modify the verts for this animation and keyframe.
				modifyVerts(pObjManInst->m_PlayerArray[i]->m_pAnimationData->tpAnimations[iState].tpKeyInfo[k].tpJointInfo, i, k);

				// Attach the pillow to the player's hand.
				attachPillow(pObjManInst->m_PlayerArray[i], 
					pObjManInst->m_PlayerArray[i]->m_pAnimationData->tpAnimations[iState].tpKeyInfo[k].tpJointInfo);
				attachPowerUp(pObjManInst->m_PlayerArray[i], 
					pObjManInst->m_PlayerArray[i]->m_pAnimationData->tpAnimations[iState].tpKeyInfo[k].tpJointInfo);

				// Update animation tracking data.
				pObjManInst->m_PlayerArray[i]->m_pAnimationData->tiPrevFrame = pObjManInst->m_PlayerArray[i]->m_pAnimationData->tpAnimations[iState].tpKeyInfo[k].tiFrame;
				pObjManInst->m_PlayerArray[i]->m_pAnimationData->tfPrevTime = m_fAnimCurTime;
				pObjManInst->m_PlayerArray[i]->m_pAnimationData->tfElapsedTime += m_fAnimCurTime;
				pObjManInst->m_PlayerArray[i]->m_pAnimationData->tiPrevState = iState;

				break;
			}
		}

		// If we were just doing the last frame, go back to the first frame.  Looping.
		if(pObjManInst->m_PlayerArray[i]->m_pAnimationData->tiPrevFrame == pObjManInst->m_PlayerArray[i]->m_pAnimationData->tpAnimations[iState].tiNumFrames)
		{
			// For cases that make the prevFrame equal 0, the animation loops.
			// Every other frame's last frame is decremented by one so that it pauses on the last frame of animation.
			switch(iState)
			{
				case STANDING:
					pObjManInst->m_PlayerArray[i]->m_pAnimationData->tiPrevFrame = 0;
					break;
				case WALKING:
					pObjManInst->m_PlayerArray[i]->m_pAnimationData->tiPrevFrame = 0;
					break;
				/*case FALLING:
					pObjManInst->m_PlayerArray[i]->m_pAnimationData->tiPrevFrame = 0;
					break;*/
				default:
					pObjManInst->m_PlayerArray[i]->m_pAnimationData->tiPrevFrame -= 1;
					break;
			}
		}
	}
}

// Modify the player's geomotry.
void CAniMule::modifyVerts(AJointT *pJoint, int iBind, int iFrame)
{
	unsigned int j;

	// Get an instance to the ObjectManager.
	CObjectManager *pObjManInst = CObjectManager::GetInstance();

	Vec4DT tempVec;
	Matrix4x4T matrixResult, oneMat, twoMat, threeMat;

	// Go through all the verts.
	for(j = 0; j < m_pBinds[iBind].tpOrigGeom->tuNumVerts; j++)
	{
		// Get bind position of this vert.
		tempVec = m_pBinds[iBind].tpOrigGeom->tpVertices[j].tPosition;

		// Move a blank matrix based on the first joint's concatenation matrix scaled by the weight for that joint.
		sMultMat4x4T(&oneMat, 
					m_pBinds[iBind].tpOrigGeom->tpVertices[j].tuJoints[0].tfWeight,
					pJoint[m_pBinds[iBind].tpOrigGeom->tpVertices[j].tuJoints[0].tiJointId - 1].tConcMat);

		// Move a blank matrix based on the second joint's concatenation matrix scaled by the weight for that joint.
		sMultMat4x4T(&twoMat, 
					m_pBinds[iBind].tpOrigGeom->tpVertices[j].tuJoints[1].tfWeight,
					pJoint[m_pBinds[iBind].tpOrigGeom->tpVertices[j].tuJoints[1].tiJointId - 1].tConcMat);

		// Move a blank matrix based on the third joint's concatenation matrix scaled by the weight for that joint.
		if(m_pBinds[iBind].tpOrigGeom->tpVertices[j].tuJoints[2].tiJointId)
			sMultMat4x4T(&threeMat, 
						m_pBinds[iBind].tpOrigGeom->tpVertices[j].tuJoints[2].tfWeight,
						pJoint[m_pBinds[iBind].tpOrigGeom->tpVertices[j].tuJoints[2].tiJointId - 1].tConcMat);

		// Add those matrices together!
		addMat4x4T(&matrixResult, oneMat, twoMat);
		if(m_pBinds[iBind].tpOrigGeom->tpVertices[j].tuJoints[2].tiJointId)
			addMat4x4T(&matrixResult, matrixResult, threeMat);

		// Multiply that matrix by the original bind position into the new position for the vert.
		MatVecMult(&pObjManInst->m_PlayerArray[iBind]->m_pGeometryData->tpVertices[j].tPosition,
			matrixResult,
			tempVec);
	}
}

// Attach the player's pillow to the joint that represents the hand.
void CAniMule::attachPillow(CPlayer *pPlayer, AJointT *pJoint)
{
	Vec4DT tempVec;
	NEW_MAT4X4(rotMat);

	// Get the position of the player.
	getPosVec4DT(&tempVec, pPlayer->m_PandO);

	// Rotate the joint matrix based on the direction the player is facing.
	if(pPlayer->m_PandO.rowVecs[2].tfX == -1.0f)
		rotYMat4x4T(&rotMat, SPF_HALF_PI);		// Facing left.
	if(pPlayer->m_PandO.rowVecs[2].tfX == 1.0f)
		rotYMat4x4T(&rotMat, -SPF_HALF_PI);		// Facing right
    
	// Now rotate that matrix.
	if(pPlayer->m_pPlayerData->iCharacter == CaptainSally_Player)
		MMultMat4x4T(&pPlayer->m_pPillow->m_PandO, pJoint[18].tConcMat, rotMat);	// Captain Sally.
	else
		MMultMat4x4T(&pPlayer->m_pPillow->m_PandO, pJoint[34].tConcMat, rotMat);	// Josie.

	// Add the player's position to complete the pillow placement.
	translateMat4x4T(&pPlayer->m_pPillow->m_PandO, tempVec.tfX, tempVec.tfY, tempVec.tfZ);
}

// Attach the player's power-up to the joint that represents the throwing hand.
void CAniMule::attachPowerUp(CPlayer *pPlayer, AJointT *pJoint)
{
	Vec4DT tempVec;
	NEW_MAT4X4(rotMat);

	// Is the player holding anything ?
	if(!pPlayer->m_pPowerup)
		return;		// No, do nothing.

	// Get the position of the player.
	getPosVec4DT(&tempVec, pPlayer->m_PandO);

	// Rotate the joint matrix based on the direction the player is facing.
	if(pPlayer->m_PandO.rowVecs[2].tfX == -1.0f)
		rotYMat4x4T(&rotMat, SPF_HALF_PI);		// Facing left.
	if(pPlayer->m_PandO.rowVecs[2].tfX == 1.0f)
		rotYMat4x4T(&rotMat, -SPF_HALF_PI);		// Facing right
    
	// Now rotate that matrix.
	MMultMat4x4T(&pPlayer->m_pPowerup->m_PandO, pJoint[38].tConcMat, rotMat);

	// Now rotate that matrix.
	if(pPlayer->m_pPlayerData->iCharacter == CaptainSally_Player)
		MMultMat4x4T(&pPlayer->m_pPowerup->m_PandO, pJoint[22].tConcMat, rotMat);	// Captain Sally.
	else
		MMultMat4x4T(&pPlayer->m_pPowerup->m_PandO, pJoint[38].tConcMat, rotMat);	// Josie.

	// Add the player's position to complete the power-up placement.
	translateMat4x4T(&pPlayer->m_pPowerup->m_PandO, tempVec.tfX, tempVec.tfY, tempVec.tfZ);
}
