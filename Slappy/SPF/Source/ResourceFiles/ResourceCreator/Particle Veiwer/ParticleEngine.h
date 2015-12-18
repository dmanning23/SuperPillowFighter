#ifndef _PARTICLEENGINE_H_
#define _PARTICLEENGINE_H_
/*////////////////////////////////////////////////////////////////////////
//File:	ParticleEngine.h
//
//Author:	Dan Manning
//
//Creation Date:	8-24-4
//
//Purpose:	Class definition for the particle engine.
//	
/////////////////////////////////////////////////////////////////////////*/
#include "..\..\..\MathLibrary\spfMath.h"
#include "..\..\..\GameTimer\StopWatch.h"

#define MAX_PARTICLES 100
#define MAX_FILENAME 64

//forward declaration of base class
class CBaseObject;

//Enumeration of all particle effects
enum ParticleEffects
{
	Feathers_ParticleEffect,
    PlayerLand_ParticleEffect,
	PlayerDeath_ParticleEffect,
	PlayerSplash_ParticleEffect,
	PlayerHit_ParticleEffect,
	ItemHit_ParticleEffect,
	PlayerSpawn_ParticleEffect,
	HealthPickup_ParticleEffect,
	Num_ParticleEffects
};

/*
This structure is used by the particle engine and renderer module 
to create particle effects.  The list of these structures are sorted by the renderer,
then put into the geometry structure before being inserted into the display list.
*/
struct ParticleT
{
	//Position of the particle.
	Vec4DT tPosition;
	//Velocity of this particle
	Vec4DT tVelocity;
	//The lifespan of this particle
	//How long it has been alive
	float fLifespan;
	//The size of the square in meters that defines this particle.(length of one side)
	float fSize;
	//The alpha value of this particular particle
	float fAlpha;
	//Bool to tell if this particle is active
	//Used by emitter to tell if particle is free or not
	bool bActive;
	//Pad to even this structure out to four byte alignment
	char pad[3];
};

//This struct holds all the emitter stuff that is loaded from the resource files
struct EmitterStuffT
{
	//Maximum range of particle velocity
	Vec4DT tMaxParticleVelocity;
	//Minimum range of particle velocity
	Vec4DT tMinParticleVelocity;
	//The id of the bitmap that this particle uses.
	int iBitmapID;
	//The number of particles this emitter starts with
	int iNumStartParticles;
	//time to live of this emitter
	float fEmitterLife;
	//time to live of particles
	float fParticleLife;
	//Maximum size of a particle
	float fMaxParticleSize;
	//Minimum size of a particle
	float fMinParticleSize;
	//time delta to create particles
	//This emitter will create a particle whenever this delta expires
	float fCreationPeriod;
	//starting alpha value of all particles created by this emitter
	float fAlpha;
	//speed to add alpha to particles
	//Must be between 0.0 and 1.0
	float fFadeSpeed;
	//Whether or not to apply sin values to this particle
	//Used to create the feather effect
	bool bSinValues;
	//whether gravity is applied to this emitter
	bool bEmitterGrav;
	//whether gravity is applied to the particles of this emitter
	bool bParticleGrav;
	//whether this emitter's particles will defy gravity and float up
	bool bAntiGravity;
	//the filename of this particle
	char strBmpFileName[MAX_FILENAME];
};

//This class is used to manage the emitters in the game
class CEmitter
{
	private:

		//Stopwatch used to time particle creation
		CStopWatch CreationTimer;

		//Stopwatch used to time the life of this emitter
		CStopWatch EmitterTimer;

		//function to add a particle to an emitter
		void AddParticle();

	public:

		//The emitter stuff for this emitter
		//points to the array of emitter... stuffs
		EmitterStuffT *m_EmitterStuff;

		//These variables need to be set or read by other modules
		//the velocity of this emitter
		Vec4DT tVelocity;

		//the position of this emitter
		Vec4DT tPosition;

		//The number of particles in this emitter
		int iNumParticles;

		//the array of particles
		ParticleT tParticles[MAX_PARTICLES];

		//add linked list functionality to this class
		CEmitter *m_pPrev;
		CEmitter *m_pNext;

		CEmitter(EmitterStuffT *myEmitterStuff) { m_EmitterStuff = myEmitterStuff; }
		virtual ~CEmitter() {}

		friend class CParticleEngine;
};

//This is the actual particle engine module, which controls all the emitters in the game
class CParticleEngine
{
	private:

		//Singleton stuff
		static CParticleEngine *m_pInstance;
		CParticleEngine(void) {}
		CParticleEngine(const CParticleEngine &CParticleEngineRef) {}
		CParticleEngine &operator=(const CParticleEngine &CParticleEngineRef) {}

		//The array of particle emitterstuff templates
		EmitterStuffT m_EmitterTemplates[Num_ParticleEffects];

		//Float to hold the old time from the last frame and provide constant velocity
		float m_fOldTime;

	public:
	
		virtual ~CParticleEngine(void) {}

		//The list of active emitters
		CEmitter *m_ActiveEmitterList;

		//Singleton shits
		static CParticleEngine *GetInstance();
		static DeleteInstance();

		//Initialize the particle engine
		void Init();

		//Load the textures for the particle engine
		void LoadTextures();

		//Cleanup the particle engine
		void CleanUp();

		//shutdown the particle engine
		void Shutdown();

		/*////////////////////////////////////////////////////////////////////////
		//Function:	CreateEmitter
		//
		//Input:	
		//	iEmitterType:	the id of the emitter to create
		//	Position:		the start position of this emitter
		//					NOTE***	If you use an attachobject, this variable will be ignored!!!
		//	Velocity:		the start velocity of this emitter
		//					NOTE***	If you use an attachobject, this variable will be ignored!!!/
		//	pAttachObject:	The object to attach this emitter to.  Pass 0 to ignore
		//					If this is not 0, the emitter will follow the specified object
		//
		//Purpose:	Provide a way for other modules to add particle effects to the game
		//
		/////////////////////////////////////////////////////////////////////////*/
		void CreateEmitter(Vec4DT *Position, Vec4DT *Velocity, EmitterStuffT *emitterType);

		//Called every frame to update the particle engine
		void Update();
};

#endif