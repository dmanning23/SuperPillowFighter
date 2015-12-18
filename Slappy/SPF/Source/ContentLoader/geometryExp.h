//////////////////////////////////////////////////////////////////////////////////////////
//File Name:		geometryExp.h
// 
//Author:			Kevin Cecelski
//
//Date Created:		8/11/2004
//
//Purpose:			This file defines the interface for the maya exporter.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef _GEOMETRY_EXP_H_
#define _GEOMETRY_EXP_H_

// Maya stuff
#include <maya/MLibrary.h>
#include <maya/MIOStream.h>
#include <maya/MFileIO.h>
#include <maya/mglobal.h>
#include <maya/MItDag.h>
#include <maya/MDagPath.h>
#include <maya/MFnDagNode.h>
#include <maya/MFnMesh.h>
#include <maya/MFloatPointArray.h>
#include <maya/mpoint.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MItGeometry.h>
#include <maya/MFnSkinCluster.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MDagPathArray.h>

#include "../MathLibrary/spfMath.h"			// For math.
#include "../Renderer/geometryTypes.h"		// For rendering data.
#include "../MemManager/MemManager.h"	// Memory management.

// Maximum size for a string.
#define MAX_STRING	256

// Macro for initializing the maya api.
#define startMaya()	MLibrary::initialize("export", false)

// Macro for shutting down the maya api.
#define exitMaya()	MLibrary::cleanup(0)

//////////////////////////////////////////
// Function: readGeometry()
//
// Purpose:	Reads geometry information from a 
//			Maya file.
//
// Last Modified: 8/15/2004
//
// Input: *strMayaFile		 - The name of the maya file.
//
// Output: None.
//
// Returns: true if successful, false otherwise.
//
//////////////////////////////////////////
bool readGeometry(char *strMayaFile, bool animated);

//////////////////////////////////////////
// Function: saveGeometry()
//
// Purpose:	Saves the geometry information
//			and texture filename to a file.
//
// Last Modified: 8/15/2004
//
// Input: *strSaveFile		 - The name of the file where the 
//							   data shall be saved.
//		  *strTexFileName	 - The name of the texture file
//							   that this model uses.
//
// Output: None.
//
// Returns: true if successful, false otherwise.
//
//////////////////////////////////////////
bool saveGeometry(char *strSaveFile, char *strTexFileName);

//////////////////////////////////////////
// Function: cleanGeometryExp()
//
// Purpose: Cleans up all the temporary data used
//			by the exporter.
//
// Last Modified: 8/15/2004
//
// Input: None.
//
// Output: None.
//
// Returns: None.
//
//////////////////////////////////////////
void cleanGeometryExp();

#endif