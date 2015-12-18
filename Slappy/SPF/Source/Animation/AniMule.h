///////////////////////////////////////////////////////////////
// File Name:     AniMule.h
// 
// Author:        Matt Strand
//
// Date Created:  8/26/2004
//
// Purpose:       This file... madness I tell you!  I mean to
//                that it defines the CAniMule class.
///////////////////////////////////////////////////////////////

#ifndef _ANI_MULE_H_
#define _ANI_MULE_H_

#include "../Mathlibrary/spfMath.h"
#include "../MemManager/memmanager.h"
#include "../logger/logger.h"

#include "../ObjectManager/ObjectManager.h"

class CAniMule
{
private:	// Private variables.

	// Singleton instance of this class.
	static CAniMule *m_pInstance;

	// Used for logging.
	CLogger m_outLog;

	// Number of players held.
	int m_iNumPlayers;

	// Has this instance of the AniMule been initialized?
	bool m_bInit;

	// Original geometry for the characters.
	struct BindT
	{
		int tiPId;
		GeometryT *tpOrigGeom;
		//AnimationDataT *tpOrigAnim;
	};

	BindT *m_pBinds;

	float m_fAnimCurTime;

public:		// Public variables.

private:	// Private functions.

	// Overridden functions for the purpose of singleton.
	CAniMule(const CAniMule &cam) {}
	CAniMule &operator=(const CAniMule &cam) {}
	CAniMule(void) { m_bInit = false; }
	~CAniMule() { m_pInstance->shutdown(); }

	// Modify the player's geomotry.
	void modifyVerts(AJointT *pJoint, int iBind, int iFrame);

	// Shut this whole thing down.  Called during delete instance.
	void shutdown(void);

	// Attach the player's pillow to the joint that represents the hand.
	void attachPillow(CPlayer *pPlayer, AJointT *pJoint);

	// Attach the player's power-up to the joint that represents the throwing hand.
	void attachPowerUp(CPlayer *pPlayer, AJointT *pJoint);

public:		// Public functions.

	// Get an instance of this singleton.
	static CAniMule *GetInstance()
	{
		if(!m_pInstance)
		{
			m_pInstance = (CAniMule *)MemMalloc(sizeof(CAniMule));
			m_pInstance->CAniMule::CAniMule();
		}

        return m_pInstance;
	}

	// Delete an instance of this singleton. (Shutdown)
	void DeleteInstance()
	{
		if(m_pInstance)
		{
			m_pInstance->CAniMule::~CAniMule();
			MemFree(m_pInstance);
			m_pInstance = 0;
		}
	}

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
	void initialize(void);

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
	void update(void);
};

#endif