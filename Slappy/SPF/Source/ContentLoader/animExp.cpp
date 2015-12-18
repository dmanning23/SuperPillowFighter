///////////////////////////////////////////////////////////////
// File Name:     animExp.cpp
// 
// Author:        Matt Strand
//
// Date Created:  8/17/2004
//
// Purpose:       This file implements all exporter functions
//                and defines.
///////////////////////////////////////////////////////////////
#include "animExp.h"
#include <stdio.h>
#include "spfMath.h"

struct ThreeDT
{
	float tdX, tdY, tdZ;
};

struct JointT
{
	int tiId;
	int tiNumParent, tiNumChild;
	ThreeDT tWTranslation, tWRotation, tWScale, tEffectorTran;			//  World
	QuaternionT tOrientation, tSOrientation, tRotation, tEffectorRot;	//  Local
	int *tpiParents, *tpiChildren;
	int tiNumNonJointP, tiNumNonJointC;
	float tranMat[4][4], invTranMat[4][4];
};

struct KeyframeT
{
	int tiFrame;
	float tdTime;
	JointT *tpJointInfo;
};

struct AnimT
{
	int tiNumFrames;
	int tiTotalTime;	//  Milleseconds
	int tiJointCount;
	KeyframeT *tpKeyInfo;
};

struct JJointHeritageT
{
	int tiId;
	MObject tThisJoint;
	int tiNumParent, tiNumChild;
	MObject *tpParents, *tpChildren;
	int tiNumNonJointP, tiNumNonJointC;
};

AnimT AnimInfo;
JJointHeritageT *JointHeritageInfo;

///////////////////////////////////////////////////////////////
// Function: exportAnimations()
//
// Purpose: Calls functions in the correct order to load and
//          save all information needed for animations.
//
// Last Modified: 8/21/2004
//
// Input: *pcReadIn    - The name of the Maya file to load.
//        *pcFileOut   - The name of the file to save to.
//
// Output: None.
//
// Returns: 0 if successful.  Any other number defines error.
//
// Notes: This function is the interface for the entire
//        exporter.  No reserved Error Codes.
///////////////////////////////////////////////////////////////
int exportAnimations(const char *pcReadIn, const char *pcFileOut, bool bAnim)
{
	int iError;	//  Used to catch error codes.

	//	Open the file.
	iError = openFile(pcReadIn);
	returnError(iError);

	//  Read in the Data.
	iError = getAnim();
	returnError(iError);

	//  Save all the information to file.
	iError = saveFile(pcFileOut, bAnim);
	returnError(iError);

	//  Release all memory.
	iError = shutdown();
	returnError(iError);

	return 0;
}

///////////////////////////////////////////////////////////////
// Function: getAnim()
//
// Purpose: To read in all of the animation data.
//
// Last Modified: 8/21/2004
//
// Input: None.
//
// Output: Completely fills out 'AnimInfo' global parameter.
//
// Returns: 0 if successful.  Any other number defines error.
//
// Notes: Dynamically allocates memory for every pointer in
//        'AnimInfo'.
//        Reserved Error Codes 101 - 150.
///////////////////////////////////////////////////////////////
int getAnim(void)
{
	MStatus status;
	MAnimControl ACon;
	MTime time;
	int i;	//  For loops.
	int iError;	//  Used to catch error codes.

	//  Set the animation to playback only once.
	status = ACon.setPlaybackMode(MAnimControl::kPlaybackOnce);
	processError(status, 101);

	//  Get the time of the last frame of animation.
	time = ACon.animationEndTime();
	status = time.setUnit(MTime::kMilliseconds);
	processError(status, 102);
	AnimInfo.tiTotalTime = (int)time.value();

	//  Get the number of frames for the animation.
	status = time.setUnit(MTime::kNTSCFrame);
	processError(status, 102);
	AnimInfo.tiNumFrames = (int)time.value();

	//  Making sure I have enough room fo all this new info I'll be reading in.
	AnimInfo.tpKeyInfo = (KeyframeT *)malloc(sizeof(KeyframeT) * AnimInfo.tiNumFrames);
	memset(AnimInfo.tpKeyInfo, 0, sizeof(KeyframeT) * AnimInfo.tiNumFrames);

	//  Find out how many joints we have.
	iError = getJointCount();
	returnError(iError);

	//  Allocate enough memory to hold all the joint heritage info to be read in next.
	JointHeritageInfo = (JJointHeritageT *)malloc(sizeof(JJointHeritageT) * AnimInfo.tiJointCount);
	memset(JointHeritageInfo, 0, sizeof(JJointHeritageT) * AnimInfo.tiJointCount);

	//  This pre-reads all joint info so that later when I store them I can properly
	//  number the joints and their children/parents.
	iError = getJointInfo();
	returnError(iError);

	//  Go through every frame and get it's info.
	for(i = 0; i < AnimInfo.tiNumFrames; i++)
	{
		//  Progress the animation.
		time.setValue((double)i);
		ACon.setCurrentTime(time);

		//  Set intitial Animation info.
		AnimInfo.tpKeyInfo[i].tiFrame = i + 1;
		AnimInfo.tpKeyInfo[i].tdTime = (AnimInfo.tiTotalTime / AnimInfo.tiNumFrames) * (float)i;

		//  Get enough room for the info I will reading in.
		AnimInfo.tpKeyInfo[i].tpJointInfo = (JointT *)malloc(sizeof(JointT) * AnimInfo.tiJointCount);
		memset(AnimInfo.tpKeyInfo[i].tpJointInfo, 0, sizeof(JointT) * AnimInfo.tiJointCount);

		getFrame(i);
	}

	return 0;
}

///////////////////////////////////////////////////////////////
// Function: getFrame()
//
// Purpose: to read in all the data pertaining to a frame.
//
// Last Modified: 8/21/2004
//
// Input: iFrameIndex    - The index of the frame that I'm
//                         changing.
//
// Output: Fills in 'AnimInfo.tpKeyInfo[iFrameIndex]'.
//
// Returns: 0 if successful.  Any other number defines error.
//
// Notes: Dynamically allocates pointers to parents and
//        children for each joint.
//        Reserved Error Codes 51 - 100.
///////////////////////////////////////////////////////////////
int getFrame(int iFrameIndex)
{
	MStatus									status;
	MQuaternion								tempQuat, orientQuat, rotQuat;	
	int										iObjectCount = 0;
	double									dpDat[3];
	MTransformationMatrix::RotationOrder	order;
	int j;	// For Loops
	unsigned int iCount, i;
	MMatrix	tranMat, invMat;

	//  Create the iterator for the DAG.
	MItDag dagIterator(MItDag::kDepthFirst, MFn::kJoint, &status);
	processError(status, 51);

	//  Go through the 
	for( ; !dagIterator.isDone(); dagIterator.next())
	{
		MDagPath dagPath;

		//  Get path of current location of iterator.
		status = dagIterator.getPath(dagPath);
		processError(status, 52);

		//  Create a DAG node from the path.
		MFnDagNode dagNode(dagPath, &status);
		processError(status, 53);

		//  Create a object from the DAG path's transform.
		MObject transformNode = dagPath.transform(&status);
		processError(status, 54);

		//  Create a joint from the DAG path.
		MFnIkJoint tempJoint(dagPath, &status);
		processError(status, 55);

		//  Create a DAG node to hodl transform data.
		MFnDagNode transform(transformNode, &status);
		processError(status, 53);

		//  Get world translation data.
		MTransformationMatrix matrix(transform.transformationMatrix());
		AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tWTranslation.tdX = (float)matrix.translation(MSpace::kTransform).x;
		AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tWTranslation.tdY = (float)matrix.translation(MSpace::kTransform).y;
		AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tWTranslation.tdZ = (float)matrix.translation(MSpace::kTransform).z;

		tranMat = matrix.asMatrix();
		tranMat.get(AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tranMat);

		invMat = tranMat.inverse();
		invMat.get(AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].invTranMat);

		//  Get world rotation data.
		matrix.getRotation(dpDat, order);
		switch(order)
		{
			case MTransformationMatrix::kXYZ:
				AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tWRotation.tdX = (float)dpDat[0];
				AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tWRotation.tdY = (float)dpDat[1];
				AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tWRotation.tdZ = (float)dpDat[2];
				break;
			case MTransformationMatrix::kXZY:
				AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tWRotation.tdX = (float)dpDat[0];
				AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tWRotation.tdZ = (float)dpDat[1];
				AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tWRotation.tdY = (float)dpDat[2];
				break;
			case MTransformationMatrix::kYXZ:
				AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tWRotation.tdY = (float)dpDat[0];
				AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tWRotation.tdX = (float)dpDat[1];
				AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tWRotation.tdZ = (float)dpDat[2];
				break;
			case MTransformationMatrix::kYZX:
				AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tWRotation.tdY = (float)dpDat[0];
				AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tWRotation.tdZ = (float)dpDat[1];
				AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tWRotation.tdX = (float)dpDat[2];
				break;
			case MTransformationMatrix::kZXY:
				AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tWRotation.tdZ = (float)dpDat[0];
				AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tWRotation.tdX = (float)dpDat[1];
				AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tWRotation.tdY = (float)dpDat[2];
				break;
			case MTransformationMatrix::kZYX:
				AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tWRotation.tdZ = (float)dpDat[0];
				AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tWRotation.tdY = (float)dpDat[1];
				AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tWRotation.tdX = (float)dpDat[2];
				break;
			default:
				processError(false, 56);
				break;
		}

		//  Get world scale data.
		matrix.getScale(dpDat, MSpace::kWorld);
		AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tWScale.tdX = (float)dpDat[0];
		AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tWScale.tdY = (float)dpDat[1];
		AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tWScale.tdX = (float)dpDat[2];

		//  Get orientation data.
		tempJoint.getOrientation(tempQuat);
		AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tOrientation.tdX = (float)tempQuat.x;
		AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tOrientation.tdY = (float)tempQuat.y;
		AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tOrientation.tdZ = (float)tempQuat.z;
		AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tOrientation.tdW = (float)tempQuat.w;

		//  Get scale orientation data.
		tempJoint.getScaleOrientation(tempQuat);
		AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tSOrientation.tdX = (float)tempQuat.x;
		AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tSOrientation.tdY = (float)tempQuat.y;
		AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tSOrientation.tdZ = (float)tempQuat.z;
		AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tSOrientation.tdW = (float)tempQuat.w;

		//  Get rotation data.
		tempJoint.getRotation(rotQuat);
		tempJoint.getOrientation(orientQuat);

		tempQuat = rotQuat * orientQuat;

		AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tRotation.tdX = (float)tempQuat.x;
		AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tRotation.tdY = (float)tempQuat.y;
		AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tRotation.tdZ = (float)tempQuat.z;
		AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tRotation.tdW = (float)tempQuat.w;

		//  Set the ID of this joint.
		AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tiId = iObjectCount + 1;

		//  Make sure that this joint is the same as the one we read in earlier for heritage information.
		if(JointHeritageInfo[iObjectCount].tThisJoint == tempJoint.object())
		{
			AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tiNumNonJointP = JointHeritageInfo[iObjectCount].tiNumNonJointP;
			AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tiNumNonJointC = JointHeritageInfo[iObjectCount].tiNumNonJointC;

			//  Copy over the number of parents and children to this joint.
			AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tiNumParent = JointHeritageInfo[iObjectCount].tiNumParent;
			AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tiNumChild = JointHeritageInfo[iObjectCount].tiNumChild;

			//  Set enough memory for the parents.
			AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tpiParents = (int *)malloc(sizeof(int) * AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tiNumParent);
			memset(AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tpiParents, 0, sizeof(int) * AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tiNumParent);

			//  Set enough memory for the children.
			AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tpiChildren = (int *)malloc(sizeof(int) * AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tiNumChild);
			memset(AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tpiChildren, 0, sizeof(int) * AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tiNumChild);

			//  Go through all the parents of this joint.
			iCount = 0;
			for(i = 0; i < tempJoint.parentCount(&status); i++)
			{
				//  Is the parent a Joint?
				if(tempJoint.parent(i, &status).hasFn(MFn::kJoint))	//  ERROR 57
				{
					//  Is the current parent the same as the corresponding parent of the pre-read heritage info?
					if(JointHeritageInfo[iObjectCount].tpParents[iCount] == tempJoint.parent(i, &status))
					{
						//  Go through all the joints
						for(j = 0; j < AnimInfo.tiJointCount; j++)
						{
							//  Is this the joint that is the parent?
							if(tempJoint.parent(i, &status) == JointHeritageInfo[j].tThisJoint)
							{
								//  Set this Joint's parent number to the ID of the corresponding heritage joint.
								AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tpiParents[iCount] = JointHeritageInfo[j].tiId;
								break;
							}
						}
					}
					iCount += 1;
				}
			}

			//  Go through all the children of this joint.
			iCount = 0;
			for(i = 0; i < tempJoint.childCount(&status); i++)
			{
				//  Is the child a Joint?
				if(tempJoint.child(i, &status).hasFn(MFn::kJoint))	//  ERROR 58
				{
					//  Is the current parent the same as the corresponding parent of the pre-read heritage info?
					if(JointHeritageInfo[iObjectCount].tpChildren[iCount] == tempJoint.child(i, &status))
					{
						//  Go through all the joints
						for(j = 0; j < AnimInfo.tiJointCount; j++)
						{
							//  Is this the joint that is the parent?
							if(tempJoint.child(i, &status) == JointHeritageInfo[j].tThisJoint)
							{
								//  Set this Joint's parent number to the ID of the corresponding heritage joint.
								AnimInfo.tpKeyInfo[iFrameIndex].tpJointInfo[iObjectCount].tpiChildren[iCount] = JointHeritageInfo[j].tiId;
								break;
							}
						}
					}
					iCount += 1;
				}
			}
		}

		iObjectCount += 1;
	}

	return 0;
}

///////////////////////////////////////////////////////////////
// Function: getJointCount()
//
// Purpose: To find out how many joints there are.
//
// Last Modified: 8/21/2004
//
// Input: None.
//
// Output: Fills in 'AnimInfo.tiJointCount' with correct number
//         of joints.
//
// Returns: 0 if successful.  Any other number defines error.
//
// Notes: Reserved Error Codes 151 - 200.
///////////////////////////////////////////////////////////////
int getJointCount(void)
{
	MStatus status;
	int	iObjectCount = 0;

	//  Create the iterator for the DAG.
	MItDag dagIterator(MItDag::kDepthFirst, MFn::kJoint, &status);
	processError(status, 151);

	//  Iteratre through the DAG.
	for( ; !dagIterator.isDone(); dagIterator.next())
	{
		iObjectCount += 1;
	}

	//  Hold the Object Count globally.
	AnimInfo.tiJointCount = iObjectCount;

	return 0;
}

///////////////////////////////////////////////////////////////
// Function: getJointInfo()
//
// Purpose: To get all the heirarchy information for the Joints
//
// Last Modified: 8/21/2004
//
// Input: None.
//
// Output: Fills in the 'JointHeritageInfo' global paramter.
//
// Returns: 0 if successful.  Any other number defines error.
//
// Notes: Dynamically allocates memory for every pointer in
//        'JointHeritageInfo'.  Fills in function IDs for
//        Joints and their parent/children info for later
//        refrencing to compare against.
//        Reserved Error Codes 201 - 250.
///////////////////////////////////////////////////////////////
int getJointInfo(void)
{
	MStatus status;
	int	iObjectCount = 0;
	unsigned int i;	//  For loops.
	int iCount, iTotalInt = 0;

	//  Create the iterator for the DAG.
	MItDag dagIterator(MItDag::kDepthFirst, MFn::kJoint, &status);
	processError(status, 201);

	//  Iteratre through the DAG.
	for( ; !dagIterator.isDone(); dagIterator.next())
	{
		MDagPath dagPath;

		//  Get path of current location of iterator.
		status = dagIterator.getPath(dagPath);
		processError(status, 202);

		//  Create a joint from the DAG path.
		MFnIkJoint tempJoint(dagPath, &status);
		processError(status, 203);

		JointHeritageInfo[iObjectCount].tiId = iObjectCount + 1;

		//  Go through each parent belonging to this joint and find
		//  out which ones are joints and how many.
		iCount = 0;
		iTotalInt = 0;
		for(i = 0; i < tempJoint.parentCount(&status); i++)
		{
			//  Is the parent a Joint?
			if(tempJoint.parent(i, &status).hasFn(MFn::kJoint))	//  ERROR 204
			{
				iCount += 1;
			}
			iTotalInt += 1;
		}

		//  Set the number of parents.
		JointHeritageInfo[iObjectCount].tiNumParent = iCount;
		JointHeritageInfo[iObjectCount].tiNumNonJointP = iTotalInt;

		//  Make sure I have enough memory for all the parents.
		JointHeritageInfo[iObjectCount].tpParents = (MObject *)malloc(sizeof(MObject) * JointHeritageInfo[iObjectCount].tiNumParent);
		memset(JointHeritageInfo[iObjectCount].tpParents, 0, sizeof(MObject) * JointHeritageInfo[iObjectCount].tiNumParent);

		//  Go through and set all of this Joint's parents.
		iCount = 0;
		for(i = 0; i < tempJoint.parentCount(&status); i++)
		{
			//  Is the parent a Joint?
			if(tempJoint.parent(i, &status).hasFn(MFn::kJoint))	//  ERROR 204
			{
				JointHeritageInfo[iObjectCount].tpParents[iCount] = tempJoint.parent(i, &status);
				processError(status, 204);
				iCount += 1;
			}
		}

		//  Go through each child belonging to this joint and find
		//  out which ones are joints and how many.
		iCount = 0;
		iTotalInt = 0;
		for(i = 0; i < tempJoint.childCount(&status); i++)
		{
			//  Is the child a Joint?
			if(tempJoint.child(i, &status).hasFn(MFn::kJoint))	//  ERROR 205
			{
				iCount += 1;
			}
			iTotalInt += 1;
		}

		//  Set the number of children.
		JointHeritageInfo[iObjectCount].tiNumChild = iCount;
		JointHeritageInfo[iObjectCount].tiNumNonJointC = iTotalInt;

		//  Make sure I have enough memory for all the children.
		JointHeritageInfo[iObjectCount].tpChildren = (MObject *)malloc(sizeof(MObject) * JointHeritageInfo[iObjectCount].tiNumChild);
		memset(JointHeritageInfo[iObjectCount].tpChildren, 0, sizeof(MObject) * JointHeritageInfo[iObjectCount].tiNumChild);

		//  Go through and set all of this Joint's children.
		iCount = 0;
		for(i = 0; i < tempJoint.childCount(&status); i++)
		{
			//  Is the child a Joint?
			if(tempJoint.child(i, &status).hasFn(MFn::kJoint))	//  ERROR 205
			{
				JointHeritageInfo[iObjectCount].tpChildren[iCount] = tempJoint.child(i, &status);
				processError(status, 205);
				iCount += 1;
			}
		}

		//  Create a copy of the joint so I can check for it later on against its ID.
		JointHeritageInfo[iObjectCount].tThisJoint = tempJoint.object();
		processError(status, 206);

		iObjectCount += 1;
	}

	return 0;
}

///////////////////////////////////////////////////////////////
// Function: openFileIn()
//
// Purpose: To load the specified Maya file.
//
// Last Modified: 8/19/2004
//
// Input: *pcReadIn    - The name of the Maya file to load.
//
// Output: None.
//
// Returns: 0 if successful.  Any other number defines error.
//
// Notes: Function also deletes history.
//        Reserved Error Codes 1 - 50.
///////////////////////////////////////////////////////////////
int openFile(const char *pcReadIn)
{
	MStatus status;

	// Open a new file.
	MFileIO::newFile(true);

	// Open Maya file.
	status = MFileIO::open(pcReadIn);
	processError(status, 1);

	// Delete history.
	status = MGlobal::executeCommand("delete -ch");
	processError(status, 2);

	return 0;
}

///////////////////////////////////////////////////////////////
// Function: saveFile()
//
// Purpose: To write all the data in 'AnimInfo' to file.
//
// Last Modified: 8/21/2004
//
// Input: *pcFileOut     -  The file to create and write to.
//
// Output: None.
//
// Returns: 0 if successful.  Any other number defines error.
//
// Notes: Reserved Error Codes 251 - 300.
///////////////////////////////////////////////////////////////
int saveFile(const char *pcFileOut, bool bAnim)
{
	int i, j, k, l;	// For loops.

	int iVersion;

	switch(bAnim)
	{
		case true:
			{
				iVersion = 1002;	//  Animation Versions.
			}
			break;

		case false:
			{
				iVersion = 2002;	//  Bind Versions.
			}
			break;
	}

	//  Open me up a file with the name passed in.
	FILE *ofFileOut;
	ofFileOut = fopen(pcFileOut, "wb");

	FILE *ofTextOut;
	ofTextOut = fopen("temp.txt", "w");

	if(!ofFileOut)
		processError(false, 251);

	//  Write out the file version.
	fwrite(&iVersion, sizeof(int), 1, ofFileOut);

	//  Write out the number of Frames and Joints.
	fwrite(&AnimInfo.tiNumFrames, sizeof(int), 1, ofFileOut);
	fprintf(ofTextOut, "%d\t-\t Num Frames.\n", AnimInfo.tiNumFrames);

	fwrite(&AnimInfo.tiJointCount, sizeof(int), 1, ofFileOut);
	fprintf(ofTextOut, "%d\t-\t Joint Count.\n", AnimInfo.tiJointCount);

	fwrite(&AnimInfo.tiTotalTime, sizeof(int), 1, ofFileOut);
	fprintf(ofTextOut, "%d\t-\t Total Time.\n\n", AnimInfo.tiTotalTime);

	for(i = 0; i < AnimInfo.tiNumFrames; i++)
	{
		//  Write the Frame number and the begining time of the frame.
		fwrite(&AnimInfo.tpKeyInfo[i].tiFrame, sizeof(int), 1, ofFileOut);
		fprintf(ofTextOut, "%d - Frame.\n", AnimInfo.tpKeyInfo[i].tiFrame);

		fwrite(&AnimInfo.tpKeyInfo[i].tdTime, sizeof(float), 1, ofFileOut);
		fprintf(ofTextOut, "%g - Time.\n", AnimInfo.tpKeyInfo[i].tdTime);

		for(j = 0; j < AnimInfo.tiJointCount; j++)
		{
			//  Write all the the data contained inside a joint!  This should be fun.
			fwrite(&AnimInfo.tpKeyInfo[i].tpJointInfo[j].tiId, sizeof(int), 1, ofFileOut);
			fprintf(ofTextOut, "\n  %d - ID.\n", AnimInfo.tpKeyInfo[i].tpJointInfo[j].tiId);

			fwrite(&AnimInfo.tpKeyInfo[i].tpJointInfo[j].tiNumChild, sizeof(int), 1, ofFileOut);
			fprintf(ofTextOut, "  %d - Num Child.\n", AnimInfo.tpKeyInfo[i].tpJointInfo[j].tiNumChild);
			fprintf(ofTextOut, "  %d - Num Child - Total.\n", AnimInfo.tpKeyInfo[i].tpJointInfo[j].tiNumNonJointC);

			fwrite(&AnimInfo.tpKeyInfo[i].tpJointInfo[j].tiNumParent, sizeof(int), 1, ofFileOut);
			fprintf(ofTextOut, "  %d - Num Parent.\n", AnimInfo.tpKeyInfo[i].tpJointInfo[j].tiNumParent);
			fprintf(ofTextOut, "  %d - Num Parent - Total.\n", AnimInfo.tpKeyInfo[i].tpJointInfo[j].tiNumNonJointP);

			//fwrite(&AnimInfo.tpKeyInfo[i].tpJointInfo[j].tOrientation, sizeof(QuaternionT), 1, ofFileOut);
			fwrite(AnimInfo.tpKeyInfo[i].tpJointInfo[j].tpiChildren, sizeof(int), AnimInfo.tpKeyInfo[i].tpJointInfo[j].tiNumChild, ofFileOut);
			fprintf(ofTextOut, "  Children: ");
			for(k = 0; k < AnimInfo.tpKeyInfo[i].tpJointInfo[j].tiNumChild; k++)
			{
				fprintf(ofTextOut, "%d, ", AnimInfo.tpKeyInfo[i].tpJointInfo[j].tpiChildren[k]);
			}
			fprintf(ofTextOut, "\n");

			fwrite(AnimInfo.tpKeyInfo[i].tpJointInfo[j].tpiParents, sizeof(int), AnimInfo.tpKeyInfo[i].tpJointInfo[j].tiNumParent, ofFileOut);
			fprintf(ofTextOut, "  Parents: ");
			for(k = 0; k < AnimInfo.tpKeyInfo[i].tpJointInfo[j].tiNumParent; k++)
			{
				fprintf(ofTextOut, "%d, ", AnimInfo.tpKeyInfo[i].tpJointInfo[j].tpiParents[k]);
			}
			fprintf(ofTextOut, "\n");

			switch(bAnim)
			{
				case true:
					{
						//fprintf(ofTextOut, "%d\t-\t Time.", AnimInfo.tpKeyInfo[i].tdTime);

						//QuaternionT temp;

						//fwrite(&AnimInfo.tpKeyInfo[i].tpJointInfo[j].tRotation, sizeof(QuaternionT), 1, ofFileOut);
						//fprintf(ofTextOut, "  Quaternion Rotation:\t%g, %g, %g, %g\n", AnimInfo.tpKeyInfo[i].tpJointInfo[j].tRotation.tdX,
						//															AnimInfo.tpKeyInfo[i].tpJointInfo[j].tRotation.tdY,
						//															AnimInfo.tpKeyInfo[i].tpJointInfo[j].tRotation.tdZ,
						//															AnimInfo.tpKeyInfo[i].tpJointInfo[j].tRotation.tdW);

						//Quat2Eul(&temp, AnimInfo.tpKeyInfo[i].tpJointInfo[j].tRotation);
						//fprintf(ofTextOut, "  Converted Quat Rot:\t%g, %g, %g, %g\n", temp.tdX,
						//																temp.tdY,
						//																temp.tdZ,
						//																temp.tdW);

						//fwrite(&AnimInfo.tpKeyInfo[i].tpJointInfo[j].tSOrientation, sizeof(QuaternionT), 1, ofFileOut);
						//fwrite(&AnimInfo.tpKeyInfo[i].tpJointInfo[j].tWRotation, sizeof(ThreeDT), 1, ofFileOut);
						//fprintf(ofTextOut, "  Euler Rotation:\t%g, %g, %g\n", AnimInfo.tpKeyInfo[i].tpJointInfo[j].tWRotation.tdX,
						//													AnimInfo.tpKeyInfo[i].tpJointInfo[j].tWRotation.tdY,
						//													AnimInfo.tpKeyInfo[i].tpJointInfo[j].tWRotation.tdZ);

						//fwrite(&AnimInfo.tpKeyInfo[i].tpJointInfo[j].tWScale, sizeof(ThreeDT), 1, ofFileOut);
						//fwrite(&AnimInfo.tpKeyInfo[i].tpJointInfo[j].tWTranslation, sizeof(ThreeDT), 1, ofFileOut);
						//fprintf(ofTextOut, "  Translation:\t\t%g, %g, %g\n\n", AnimInfo.tpKeyInfo[i].tpJointInfo[j].tWTranslation.tdX,
						//													AnimInfo.tpKeyInfo[i].tpJointInfo[j].tWTranslation.tdY,
						//													AnimInfo.tpKeyInfo[i].tpJointInfo[j].tWTranslation.tdZ);


						fwrite(AnimInfo.tpKeyInfo[i].tpJointInfo[j].tranMat[0], sizeof(float), 4, ofFileOut);
						fwrite(AnimInfo.tpKeyInfo[i].tpJointInfo[j].tranMat[1], sizeof(float), 4, ofFileOut);
						fwrite(AnimInfo.tpKeyInfo[i].tpJointInfo[j].tranMat[2], sizeof(float), 4, ofFileOut);
						fwrite(AnimInfo.tpKeyInfo[i].tpJointInfo[j].tranMat[3], sizeof(float), 4, ofFileOut);

						fprintf(ofTextOut, "TranMat:\n");
						for(k = 0; k < 4; k++)
						{
							fprintf(ofTextOut, "\t");
							for(l = 0; l < 4; l++)
							{
								fprintf(ofTextOut, "%g, ", AnimInfo.tpKeyInfo[i].tpJointInfo[j].tranMat[k][l]);
							}
							fprintf(ofTextOut, "\n");
						}
					}
					break;

				case false:
					{
						fwrite(AnimInfo.tpKeyInfo[i].tpJointInfo[j].invTranMat[0], sizeof(float), 4, ofFileOut);
						fwrite(AnimInfo.tpKeyInfo[i].tpJointInfo[j].invTranMat[1], sizeof(float), 4, ofFileOut);
						fwrite(AnimInfo.tpKeyInfo[i].tpJointInfo[j].invTranMat[2], sizeof(float), 4, ofFileOut);
						fwrite(AnimInfo.tpKeyInfo[i].tpJointInfo[j].invTranMat[3], sizeof(float), 4, ofFileOut);

						fprintf(ofTextOut, "InvTranMat:\n");
						for(k = 0; k < 4; k++)
						{
							fprintf(ofTextOut, "\t");
							for(l = 0; l < 4; l++)
							{
								fprintf(ofTextOut, "%g, ", AnimInfo.tpKeyInfo[i].tpJointInfo[j].invTranMat[k][l]);
							}
							fprintf(ofTextOut, "\n");
						}
					}
					break;
			}
		}
	}

	fflush(ofFileOut);	
	fflush(ofTextOut);
	fclose(ofFileOut);
	fclose(ofTextOut);

	return 0;
}

///////////////////////////////////////////////////////////////
// Function: shutdown()
//
// Purpose: To release all memory used during operations.
//
// Last Modified: 8/21/2004
//
// Input: None.
//
// Output: None.
//
// Returns: 0 if successful.  Any other number defines error.
//
// Notes: No Reserved Error Codes
///////////////////////////////////////////////////////////////
int shutdown()
{
	int i, j;	//  For loops

	//  FREE ALL MEMORY!  WEE!
	for(i = 0; i < AnimInfo.tiJointCount; i++)
	{
		free(JointHeritageInfo[i].tpChildren);
		free(JointHeritageInfo[i].tpParents);
	}

	free(JointHeritageInfo);

	for(i = 0; i < AnimInfo.tiNumFrames; i++)
	{
		for(j = 0; j < AnimInfo.tiJointCount; j++)
		{
			free(AnimInfo.tpKeyInfo[i].tpJointInfo[j].tpiChildren);
			free(AnimInfo.tpKeyInfo[i].tpJointInfo[j].tpiParents);
		}

		free(AnimInfo.tpKeyInfo[i].tpJointInfo);
	}

	free(AnimInfo.tpKeyInfo);

	return 0;
}

void Quat2Eul(QuaternionT *resultQuat, const QuaternionT &input)
{
	// () ()
	//   |   
	//  ()    - Time for to collect nuts!
	float fScale, fTemp;

	fTemp = (float)acos(input.tdW) * 2;

	if(fTemp == 0.0f)
		fScale = 0.0f;
	else
		fScale = (float)sin(fTemp / 2.0f);

	if(fScale == 0.0f)
	{
		resultQuat->tdX = 0.0f;
		resultQuat->tdY = 0.0f;
		resultQuat->tdZ = 0.0f;
	}
	else
	{
		resultQuat->tdX = input.tdX / fScale;
		resultQuat->tdY = input.tdY / fScale;
		resultQuat->tdZ = input.tdZ / fScale;
	}

	resultQuat->tdW = (fTemp * (360 / 2)) / (float)SPF_PI;
}