///////////////////////////////////////////////////////////////
// File Name:     animExp.h
// 
// Author:        Matt Strand
//
// Date Created:  8/17/2004
//
// Purpose:       This file declares all exporter functions
//                and defines.
///////////////////////////////////////////////////////////////
#ifndef _ANIM_EXP_H_
#define _ANIM_EXP_H_

// Maya Includes
#include <maya/MLibrary.h>
#include <maya/MIOStream.h>
#include <maya/MFileIO.h>
#include <maya/MGlobal.h>
#include <maya/MItDag.h>
#include <maya/MDagPath.h>
#include <maya/MFnDagNode.h>
#include <maya/MAnimControl.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MQuaternion.h>
#include <maya/MFnIkJoint.h>
#include <maya/MVector.h>
#include <maya/MMatrix.h>

// Macro for initializing the Maya API.
#define loadMaya() MLibrary::initialize("AnimationExporter", false)

// Macro for shutting down the Maya API.
#define unloadMaya() MLibrary::cleanup(0)

// Macro for returning an error code.
#define returnError(n) if(n) return n		// If 'n' specifies an error, return it.
#define processError(n, v) if(!n) return v	// if 'n' fails, return specifed error code 'v'.

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
//        exporter.
///////////////////////////////////////////////////////////////
int exportAnimations(const char *pcReadIn, const char *pcFileOut, bool bAnim);

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
int getAnim(void);

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
// Notes: Reserved Error Codes 51 - 100.
///////////////////////////////////////////////////////////////
int getFrame(int iFrameIndex);

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
int getJointCount(void);

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
// Notes: Function IDs Joints and their parents/children for
//        later refrencing to compare against.
//        Reserved Error Codes 201 - 250.
///////////////////////////////////////////////////////////////
int getJointInfo(void);

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
///////////////////////////////////////////////////////////////
int openFile(const char *pcReadIn);

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
int saveFile(const char *pcFileOut, bool bAnim);

///////////////////////////////////////////////////////////////
// Function: shutdown()
//
// Purpose: To return all memory used during operations.
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
int shutdown();

struct QuaternionT
{
	float tdX, tdY, tdZ, tdW;
};

void Quat2Eul(QuaternionT *resultQuat, const QuaternionT &input);

#endif