#ifndef _LEVELOBJECT_H_
#define _LEVELOBJECT_H_
/*////////////////////////////////////////////////////////////////////////
//File:	LevelObject.h
//
//Author:	Dan Manning
//
//Creation Date:	8-17-4
//
//Purpose:	class definition of CLevelObject.  The level object only uses
//the functionality of the base object.
//
/////////////////////////////////////////////////////////////////////////*/
#include "baseobject.h"

#define LEVELOBJ_STRING_LENGTH 64
#define NO_WATER	-100000.0f		// The value that is set to the water level when no water is desired.

//Enumeration of all possible types of level objects
enum
{
	Boat_levelobj,
	Whale_levelobj,
	Cannon_levelobj,
    RubberDuck_levelobj,
	Soap_levelobj,
	Faucet_levelobj,
	Handle_levelobj,
    WaterGun_levelobj,
	FerrisWheel_levelobj,
	RubberDuckWheel_levelobj,
	Num_levelobj
};

class CLevelObject : public CBaseObject
{
	public:

		//Physics variables
		//magnitude of wave
		float m_fMagnitude;

		//time period of wave
		float m_fWavePeriod;

		//max bob
		float m_fMaxBob;

		//min bob
		float m_fMinBob;
		
		//amount of bob (how fast)
		float m_fBobPeriod;

		//Sound variable
		//index of the sound this object makes when a player lands on it
		int m_iPlayerLandSoundIndex;
		//index of the sound this object makes when it emits a powerup
		int m_iEmitPowerupSoundIndex;

		//Filenames of object sounds
		char *m_strPlayerLandFileName;
		char *m_strEmitPowerupFileName;

		//index of model
		int m_iModelIndex;

		// Number of objects attached to this object.
		int m_numAttachedObjects;

		//object types to attach to optional points
		int *m_pOptionalAttachedObjects;

		//optional attachment points for other level objects
		Vec4DT *m_pOptionalAttachmentPoints;

		//whether or not this level object has an emitter
		bool m_bHasEmitter;

		//Emitter position
		//This is the position of an emitter attached to this object
		Vec4DT m_EmitterPos;

		//The level object that this one is attached to, used to update position
		//points to 0 if there is no owner
		CLevelObject *m_pOwner;

		//This variable holds which optional attachment point to index 
		//in the owner to properly place this object
		int m_iAttachmentPointIndex;

		//holds number of lines of this object
		int m_iNumLines;

		//holds the index of the particle effect to play when this object shoots powerups
		int m_iParticleIndex;

		// Holds the left and right extents of this level object.
		Vec4DT m_LeftMostPoint, m_RightMostPoint;

		/*////////////////////////////////////////////////////////////////////////
		//Function:	Constructor
		//
		//Input:	
		//iObjType:	the type of the object to create as enumerated in levelobject.h
		//
		//Purpose:	Construct a level object of the correct type
		//
		/////////////////////////////////////////////////////////////////////////*/
		CLevelObject(int iObjType);

		/*////////////////////////////////////////////////////////////////////////
		//Function:	Destructor
		//	
		//Purpose:	Clean up all memory allocated to the level object
		//
		/////////////////////////////////////////////////////////////////////////*/
		~CLevelObject(void);
};

#endif