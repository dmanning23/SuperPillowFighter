//////////////////////////////////////////////////////////////////////////////////////////
//File Name:		geometryImp.h
// 
//Author:			Kevin Cecelski
//
//Date Created:		8/15/2004
//
//Purpose:			This defines the geometry importer interface.
//
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef	_GEOMETRY_IMP_H_
#define _GEOMETRY_IMP_H_

#include <stdio.h>
#include <string.h>
#include "..\MathLibrary\spfMath.h"
#include "..\Renderer\geometryTypes.h"
#include "..\Entities\baseobject.h"
#include "..\BitmapManager\bitmapManager.h"
#include "..\MemManager\memmanager.h"
#include "..\Logger\logger.h"
#include "..\Renderer\VertBufCtrl\VertBufCtrl.h"

// Interface 
#define MAX_STRING	256

// Ids for all the models in the game.
enum MODEL_IDS{
	JOSIE_MODEL = 0,
	PIRATESHIP_MODEL,
	CANNON_MODEL,
	POWERUP_BALL,
	DUCK_MODEL,
	PEAR_MODEL,
    CONE_MODEL,
	GUN_MODEL,
	PIE_MODEL,
	TURKEY_MODEL,
	PINEAPPLE_MODEL,
	JOSIESPILLOW_MODEL,
	CAPTAINSALLY_MODEL,
	SALLYPILLOW_MODE,
    FERRIS_WHEEL_MODEL,
	NUM_MODELS,
};

// Associates each model ID with a filename.
static const char *strIdToFileName[NUM_MODELS] = {
	"josie.mef",
	"pirateship.mef",
	"cannon.mef",
	"bitball.mef",
	"rubduck.mef",
	"pear.mef",
	"cone.mef",
	"gun.mef",
	"pie.mef", 
	"turkey.mef",
	"pineapple.mef",
	"josiePillow.mef",
	"captainSally.mef",
	"sallyPillow.mef",
	"ferriswheel.mef",
};

// Path to the model directory.
static const char *strModelPath = "ResourceFiles//models//";

// Path to the texture image directory.
static const char *strTexImgPath = "ResourceFiles//textures//";

//////////////////////////////////////////
// Function: LoadGeometry()
//
// Purpose:	Loads geometry from a file.
//
// Last Modified: Monday, August 30, 2004
//
// Input: *pBaseObject		- The object that will store the geometry.
//		  iModelID			- The id of the model to load.
//
// Output: None.
//
// Returns: true if successful, false otherwise.
//
//////////////////////////////////////////
bool LoadGeometry(CBaseObject *pBaseObject, int iModelID);

#endif
