#ifndef _PILLOW_H_
#define _PILLOW_H_
/*////////////////////////////////////////////////////////////////////////
//File:	Pillow.h
//
//Author:	Dan Manning
//
//Creation Date:	8-12-4
//
//Purpose:	Class definition of the pillow class
//
/////////////////////////////////////////////////////////////////////////*/
#include "baseobject.h"

enum Pillow_Types
{
	Josie_Pillow,
	CaptainSally_Pillow,
	MissElizabeth_Pillow,
	Sumi_Pillow,
	Num_Pillows
};

class CPlayer;

class CPillow : public CBaseObject
{
	public:

		//this bool controls if the pillow is active (attacking) or not
		//This variable will be changed by the player it is attached to.
		bool m_bActive;

		//the owner of this pillow
		CPlayer *m_pOwner;

		CPillow(void);
		virtual ~CPillow(void);
};

#endif
