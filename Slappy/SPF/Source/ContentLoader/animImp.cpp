///////////////////////////////////////////////////////////////
// File Name:     animImp.cpp
// 
// Author:        Matt Strand
//
// Date Created:  8/24/2004
//
// Purpose:       This file implements all importer functions.
//                Purpose is to load and unload animations for
//                players.
///////////////////////////////////////////////////////////////
#include "animImp.h"
#include <stdio.h>
#include <string.h>
#include "..\MathLibrary\spfMath.h"
#include "..\MemManager\memmanager.h"
#include "..\Logger\logger.h"
#include "..\Entities\player.h"

///////////////////////////////////////////////////////////////
// Function: loadAnim()
//
// Purpose: Opens multiple files to load all animations for
//          for a player.
//
// Last Modified: 9/08/2004
//
// Input: *pPlayer    - The player to load the animations to.
//        iAnimID     - The type of animations to load.
//
// Output: pPlayer::m_pAnimationData - Filled out.
//
// Returns: True - If success.
//
// Notes: Animation type IDs can be found in ANIM_TYPES enum.
///////////////////////////////////////////////////////////////
bool loadAnim(CPlayer *pPlayer, int iAnimID)
{
	int i, j, k/*, l*/;	// For loops

	// Let's start logging.
	CLogger logOut;
	logOut.Init("animationImporter.txt");

	// Get enough memory for my animation madness.
	pPlayer->m_pAnimationData = (AnimationDataT *)MemMalloc(sizeof(AnimationDataT));

	// Set up Animation Info.
	pPlayer->m_pAnimationData->tfPrevTime = 0.0f;
	pPlayer->m_pAnimationData->tfStartTime = 0.0f;
	pPlayer->m_pAnimationData->tfElapsedTime = 0.0f;
	pPlayer->m_pAnimationData->tiPrevFrame = 0;
	pPlayer->m_pAnimationData->tiPrevState = 0;

	// Get enough memory for every animation.
	pPlayer->m_pAnimationData->tpAnimations = (AnimT *)MemMalloc(sizeof(AnimT) * NUM_ANIMS - 1);
	memset(pPlayer->m_pAnimationData->tpAnimations, 0, sizeof(AnimT) * NUM_ANIMS - 1);

	// Loop through once for every animation.
	for(i = 0; i < NUM_ANIMS; i++)
	{
		// Get enough memory for full path.
		int iPathLen = (int)strlen(pcAnimPath) + (int)strlen(pcIdToFileName[iAnimID][i]);
		char *filePathName = (char *)MemMalloc(sizeof(char) * (iPathLen + 1));

		// Copy the full path for the animtion.
		strcpy(filePathName, pcAnimPath);
		strcat(filePathName, pcIdToFileName[iAnimID][i]);

		logOut << "Attempting to load animation: \"" << filePathName << "\"\n";

		// Open the animation.
		FILE *ifFile;
		ifFile = fopen(filePathName, "rb");

		if(!ifFile)
		{
			// There was an error opening the file.
			logOut << "\tThere was an error loading animation: \"" << filePathName << "\"\n";
			MemFree(filePathName);
			continue;
		}

		// Get the file version number.
		int iVersion;
		fread(&iVersion, sizeof(int), 1, ifFile);

		// Based on what version the file is, it will load differently.
		switch(iVersion)
		{
			case 1002:
				{
					// Fill up the basic animation data.
					fread(&pPlayer->m_pAnimationData->tpAnimations[i].tiNumFrames, sizeof(int), 1, ifFile);
					fread(&pPlayer->m_pAnimationData->tpAnimations[i].tiJointCount, sizeof(int), 1, ifFile);
					fread(&pPlayer->m_pAnimationData->tpAnimations[i].tiTotalTime, sizeof(int), 1, ifFile);

					// Make some room for frames of this animation.
					pPlayer->m_pAnimationData->tpAnimations[i].tpKeyInfo = (KeyframeT *)MemMalloc(sizeof(KeyframeT) * pPlayer->m_pAnimationData->tpAnimations[i].tiNumFrames);

					// Go through every frame
					for(j = 0; j < pPlayer->m_pAnimationData->tpAnimations[i].tiNumFrames; j++)
					{
						// Fill up basic frame data.
						fread(&pPlayer->m_pAnimationData->tpAnimations[i].tpKeyInfo[j].tiFrame, sizeof(int), 1, ifFile);
						fread(&pPlayer->m_pAnimationData->tpAnimations[i].tpKeyInfo[j].tfTime, sizeof(float), 1, ifFile);

						// Make some room for the joints of this frame.
						pPlayer->m_pAnimationData->tpAnimations[i].tpKeyInfo[j].tpJointInfo = (AJointT *)MemMalloc(sizeof(AJointT) * pPlayer->m_pAnimationData->tpAnimations[i].tiJointCount);

						// Go through every joint.
						for(k = 0; k < pPlayer->m_pAnimationData->tpAnimations[i].tiJointCount; k++)
						{
							// Fill up basic joint information.
							fread(&pPlayer->m_pAnimationData->tpAnimations[i].tpKeyInfo[j].tpJointInfo[k].tiId, sizeof(int), 1, ifFile);
							fread(&pPlayer->m_pAnimationData->tpAnimations[i].tpKeyInfo[j].tpJointInfo[k].tiNumChild, sizeof(int), 1, ifFile);
							fread(&pPlayer->m_pAnimationData->tpAnimations[i].tpKeyInfo[j].tpJointInfo[k].tiNumParent, sizeof(int), 1, ifFile);

							// Make some room for the parent and children indeces coming in.
							if(pPlayer->m_pAnimationData->tpAnimations[i].tpKeyInfo[j].tpJointInfo[k].tiNumChild > 0)
								pPlayer->m_pAnimationData->tpAnimations[i].tpKeyInfo[j].tpJointInfo[k].tpiChildren = (int *)MemMalloc(sizeof(int) * pPlayer->m_pAnimationData->tpAnimations[i].tpKeyInfo[j].tpJointInfo[k].tiNumChild);
							else
								pPlayer->m_pAnimationData->tpAnimations[i].tpKeyInfo[j].tpJointInfo[k].tpiChildren = 0;
							if(pPlayer->m_pAnimationData->tpAnimations[i].tpKeyInfo[j].tpJointInfo[k].tiNumParent > 0)
								pPlayer->m_pAnimationData->tpAnimations[i].tpKeyInfo[j].tpJointInfo[k].tpiParents = (int *)MemMalloc(sizeof(int) * pPlayer->m_pAnimationData->tpAnimations[i].tpKeyInfo[j].tpJointInfo[k].tiNumParent);
							else
								pPlayer->m_pAnimationData->tpAnimations[i].tpKeyInfo[j].tpJointInfo[k].tpiParents = 0;

							// Read in Orientation data.
							//fread(&pPlayer->m_pAnimationData->tpAnimations[i].tpKeyInfo[j].tpJointInfo[k].tLOrientation, sizeof(QuaternionT), 1, ifFile);

							// Get joint parent/child data.							
							fread(pPlayer->m_pAnimationData->tpAnimations[i].tpKeyInfo[j].tpJointInfo[k].tpiChildren, sizeof(int), pPlayer->m_pAnimationData->tpAnimations[i].tpKeyInfo[j].tpJointInfo[k].tiNumChild, ifFile);
							fread(pPlayer->m_pAnimationData->tpAnimations[i].tpKeyInfo[j].tpJointInfo[k].tpiParents, sizeof(int), pPlayer->m_pAnimationData->tpAnimations[i].tpKeyInfo[j].tpJointInfo[k].tiNumParent, ifFile);

							//fread(&pPlayer->m_pAnimationData->tpAnimations[i].tpKeyInfo[j].tpJointInfo[k].tLRotation, sizeof(QuaternionT), 1, ifFile);
							//fread(&pPlayer->m_pAnimationData->tpAnimations[i].tpKeyInfo[j].tpJointInfo[k].tLSOrientation, sizeof(QuaternionT), 1, ifFile);
							//fread(&pPlayer->m_pAnimationData->tpAnimations[i].tpKeyInfo[j].tpJointInfo[k].tWRotation, sizeof(Vec4DT), 1, ifFile);
							//fread(&pPlayer->m_pAnimationData->tpAnimations[i].tpKeyInfo[j].tpJointInfo[k].tWScale, sizeof(Vec4DT), 1, ifFile);
							//fread(&pPlayer->m_pAnimationData->tpAnimations[i].tpKeyInfo[j].tpJointInfo[k].tWTranslation, sizeof(Vec4DT), 1, ifFile);

							// Get Matrix
							fread(&pPlayer->m_pAnimationData->tpAnimations[i].tpKeyInfo[j].tpJointInfo[k].tLocalMat.rowVecs[0], sizeof(float), 4, ifFile);
							fread(&pPlayer->m_pAnimationData->tpAnimations[i].tpKeyInfo[j].tpJointInfo[k].tLocalMat.rowVecs[1], sizeof(float), 4, ifFile);
							fread(&pPlayer->m_pAnimationData->tpAnimations[i].tpKeyInfo[j].tpJointInfo[k].tLocalMat.rowVecs[2], sizeof(float), 4, ifFile);
							fread(&pPlayer->m_pAnimationData->tpAnimations[i].tpKeyInfo[j].tpJointInfo[k].tLocalMat.rowVecs[3], sizeof(float), 4, ifFile);
						}

						// Concatenate matrices.
						NEW_MAT4X4(blankMat1);
						NEW_MAT4X4(blankMat2);
						makeMatrix(&pPlayer->m_pAnimationData->tpAnimations[i].tpKeyInfo[j].tpJointInfo[0], blankMat1/*, blankMat2*/, pPlayer->m_pAnimationData->tpAnimations[i].tpKeyInfo[j]);
						unloadHeritage(pPlayer->m_pAnimationData->tpAnimations[i].tpKeyInfo[j].tpJointInfo, pPlayer->m_pAnimationData->tpAnimations[i].tiJointCount);
					}
				}
				break;

			case 2002:
				{

					int iTemp;
					float fTemp;

					// Fill up the basic animation data.
					fread(&iTemp, sizeof(int), 1, ifFile);
					fread(&pPlayer->m_pAnimationData->tBind.tiJointCount, sizeof(int), 1, ifFile);
					fread(&iTemp, sizeof(int), 1, ifFile);

					// Fill up basic frame data.
					fread(&fTemp, sizeof(int), 1, ifFile);
					fread(&fTemp, sizeof(float), 1, ifFile);

					// Make some room for the joints of this frame.
					pPlayer->m_pAnimationData->tBind.tpJointInfo = (AJointT *)MemMalloc(sizeof(AJointT) * pPlayer->m_pAnimationData->tBind.tiJointCount);

					// Go through every joint.
					for(k = 0; k < pPlayer->m_pAnimationData->tBind.tiJointCount; k++)
					{
						// Fill up basic joint information.
						fread(&pPlayer->m_pAnimationData->tBind.tpJointInfo[k].tiId, sizeof(int), 1, ifFile);
						fread(&pPlayer->m_pAnimationData->tBind.tpJointInfo[k].tiNumChild, sizeof(int), 1, ifFile);

						fread(&pPlayer->m_pAnimationData->tBind.tpJointInfo[k].tiNumParent, sizeof(int), 1, ifFile);

						// Make some room for the parent and children indeces coming in.
						if(pPlayer->m_pAnimationData->tBind.tpJointInfo[k].tiNumChild > 0)
							pPlayer->m_pAnimationData->tBind.tpJointInfo[k].tpiChildren = (int *)MemMalloc(sizeof(int) * pPlayer->m_pAnimationData->tBind.tpJointInfo[k].tiNumChild);
						else
							pPlayer->m_pAnimationData->tBind.tpJointInfo[k].tpiChildren = 0;
						if(pPlayer->m_pAnimationData->tBind.tpJointInfo[k].tiNumParent > 0)
							pPlayer->m_pAnimationData->tBind.tpJointInfo[k].tpiParents = (int *)MemMalloc(sizeof(int) * pPlayer->m_pAnimationData->tBind.tpJointInfo[k].tiNumParent);
						else
							pPlayer->m_pAnimationData->tBind.tpJointInfo[k].tpiParents = 0;

						// Get joint parent/child data.							
						fread(pPlayer->m_pAnimationData->tBind.tpJointInfo[k].tpiChildren, sizeof(int), pPlayer->m_pAnimationData->tBind.tpJointInfo[k].tiNumChild, ifFile);
						fread(pPlayer->m_pAnimationData->tBind.tpJointInfo[k].tpiParents, sizeof(int), pPlayer->m_pAnimationData->tBind.tpJointInfo[k].tiNumParent, ifFile);

						// Get Matrix
						fread(&pPlayer->m_pAnimationData->tBind.tpJointInfo[k].tLocalMat.rowVecs[0], sizeof(float), 4, ifFile);
						fread(&pPlayer->m_pAnimationData->tBind.tpJointInfo[k].tLocalMat.rowVecs[1], sizeof(float), 4, ifFile);
						fread(&pPlayer->m_pAnimationData->tBind.tpJointInfo[k].tLocalMat.rowVecs[2], sizeof(float), 4, ifFile);
						fread(&pPlayer->m_pAnimationData->tBind.tpJointInfo[k].tLocalMat.rowVecs[3], sizeof(float), 4, ifFile);
					}

					// Concatenate matrices.
					NEW_MAT4X4(blankMat1);
					NEW_MAT4X4(blankMat2);
					makeInvMatrix(&pPlayer->m_pAnimationData->tBind.tpJointInfo[0], blankMat2, pPlayer->m_pAnimationData->tBind.tpJointInfo);
					unloadHeritage(pPlayer->m_pAnimationData->tBind.tpJointInfo, pPlayer->m_pAnimationData->tBind.tiJointCount);
				}
				break;
		}

		logOut << "\tSuccess loading animation: " << filePathName << "\n";

		fclose(ifFile);

		// Free up the temporary data.
		MemFree(filePathName);
	}

	return true;
}

///////////////////////////////////////////////////////////////
// Function: unloadHeritage()
//
// Purpose: Releases all memory allocated for joint parents and
//          chidlren.
//
// Last Modified:9/18/2004
//
// Input: *pJoint    - Pointer to the joint array.
//        iNumJoints - Number of joints in the array.
//
// Output: pPlayer::m_pAnimationData - Partial Release.
//
// Returns: None.
//
// Notes: This is release memory that was allocated but is not
//        used later in the game other than for debugging.
//        Calling this makes debugging joints impossible.
///////////////////////////////////////////////////////////////
void unloadHeritage(AJointT *pJoint, const int iNumJoints)
{
	int i;

	for(i = 0; i < iNumJoints; i++)
	{
		MemFree(pJoint[i].tpiChildren);
		MemFree(pJoint[i].tpiParents);

		//memset(pJoint[i].tpiChildren, 0, pJoint[i].tiNumChild);
		//memset(pJoint[i].tpiParents, 0, pJoint[i].tiNumParent);
	}
}

///////////////////////////////////////////////////////////////
// Function: unloadAnim()
//
// Purpose: Releases all memory allocated for animations.
//
// Last Modified: 8/24/2004
//
// Input: *pPlayer    - The player whos animations are being
//                      unloaded
//
// Output: pPlayer::m_pAnimationData - Released.
//
// Returns: None.
//
// Notes: Pass in player, and have all their animations relased
///////////////////////////////////////////////////////////////
void unloadAnim(CPlayer *pPlayer)
{
	int i, j/*, k*/;	// For loops

	if(!pPlayer->m_pAnimationData)
		return;

	// Go through all the animations!
	for(i = 0; i < NUM_ANIMS - 1; i++)
	{
		// Make sure there's something there.
		if(pPlayer->m_pAnimationData->tpAnimations[i].tiJointCount > 0)
		{
			// Go through every frame!
			for(j = 0; j < pPlayer->m_pAnimationData->tpAnimations[i].tiNumFrames; j++)
			{
				// Go through every joint!
				//for(k = 0; k < pPlayer->m_pAnimationData->tpAnimations[i].tiJointCount; k++)
				//{
				//	// Get rid of the arrays of children/parents.
				//	if(pPlayer->m_pAnimationData->tpAnimations[i].tpKeyInfo[j].tpJointInfo[k].tiNumChild > 0)
				//		MemFree(pPlayer->m_pAnimationData->tpAnimations[i].tpKeyInfo[j].tpJointInfo[k].tpiChildren);
				//	if(pPlayer->m_pAnimationData->tpAnimations[i].tpKeyInfo[j].tpJointInfo[k].tiNumParent > 0)
				//		MemFree(pPlayer->m_pAnimationData->tpAnimations[i].tpKeyInfo[j].tpJointInfo[k].tpiParents);
				//}
				// Get rid of joints.
				if(pPlayer->m_pAnimationData->tpAnimations[i].tpKeyInfo[j].tpJointInfo)
					MemFree(pPlayer->m_pAnimationData->tpAnimations[i].tpKeyInfo[j].tpJointInfo);
			}
			// Get rid of keyframes.
			if(pPlayer->m_pAnimationData->tpAnimations[i].tpKeyInfo)
				MemFree(pPlayer->m_pAnimationData->tpAnimations[i].tpKeyInfo);
		}
	}
	// Get rid of the pointer to all animation data.
	if(pPlayer->m_pAnimationData->tpAnimations)
		MemFree(pPlayer->m_pAnimationData->tpAnimations);

	// Get rid of the pointer to all this.
	if(pPlayer->m_pAnimationData)
		MemFree(pPlayer->m_pAnimationData);
}

///////////////////////////////////////////////////////////////
// Function: makeMatrix()
//
// Purpose: Concatenates the matrices from parents to chidlren.
//
// Last Modified: 9/18/2004
//
// Input: *pJoint    - Pointer to the joint to change.
//        &parentMat - The parent's final matrix.
//        &frame     - What keyframe we're operating in.
//
// Output: pPlayer::m_pAnimationData - Joint matrices modified.
//
// Returns: None.
//
// Notes: Called only once after a frame is loaded.
///////////////////////////////////////////////////////////////
void makeMatrix(AJointT *pJoint, const Matrix4x4T &parentMat, const KeyframeT &frame)
{
	int i;	// For loops

	MMultMat4x4T(&pJoint->tConcMat, pJoint->tLocalMat, parentMat);	

	for(i = 0; i < pJoint->tiNumChild; i++)
		makeMatrix(&frame.tpJointInfo[pJoint->tpiChildren[i] - 1], pJoint->tConcMat, frame);
}

///////////////////////////////////////////////////////////////
// Function: makeInvMatrix()
//
// Purpose: Concatenates the inverted matrices from parents to
//          chidlren.
//
// Last Modified: 9/18/2004
//
// Input: *pJoint       - Pointer to the joint to change.
//        &parentInvMat - The parent's final inverted matrix.
//        &frame        - What keyframe we're operating in.
//
// Output: pPlayer::m_pAnimationData->tBind - Joint matrices
//                                            modified.
//
// Returns: None.
//
// Notes: Called only once after bind is loaded.
//        Recursive.
///////////////////////////////////////////////////////////////
void makeInvMatrix(AJointT *pJoint, const Matrix4x4T &parentInvMat, AJointT *joints)
{
	int i;	// For loops

	MMultMat4x4T(&pJoint->tConcMat, parentInvMat, pJoint->tLocalMat);	

	for(i = 0; i < pJoint->tiNumChild; i++)
		makeInvMatrix(&(joints[pJoint->tpiChildren[i] - 1]), pJoint->tConcMat, joints);
}

//void Quat2Eul(QuaternionT *resultQuat, const QuaternionT &input)
//{
//	float fScale, fTemp;
//
//	fTemp = (float)acos(input.tfW) * 2;
//
//	if(fTemp == 0.0f)
//		fScale = 0.0f;
//	else
//		fScale = (float)sin(fTemp / 2.0f);
//
//	if(fScale == 0.0f)
//	{
//		resultQuat->tfX = 0.0f;
//		resultQuat->tfY = 0.0f;
//		resultQuat->tfZ = 0.0f;
//	}
//	else
//	{
//		resultQuat->tfX = input.tfX / fScale;
//		resultQuat->tfY = input.tfY / fScale;
//		resultQuat->tfZ = input.tfZ / fScale;
//	}
//
//	resultQuat->tfW = (fTemp * (360 / 2)) / (float)SPF_PI;
//}