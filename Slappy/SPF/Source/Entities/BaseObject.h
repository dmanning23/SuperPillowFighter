#ifndef _CBASEOBJECT_H_
#define _CBASEOBJECT_H_
/*////////////////////////////////////////////////////////////////////////
//File:	BashObject.h
//
//Author:	Dan Manning
//
//Creation Date:	8-9-4
//
//Purpose:	This function has all the enums and function definitions for the
// CBaseObject abc.
//
/////////////////////////////////////////////////////////////////////////*/

#include "..\MathLibrary\spfMath.h"
#include "..\Renderer\geometryTypes.h"  

//Enumeration of all object types
enum ObjectTypes
{
	Player_obj,
	Level_obj,
    Powerup_obj,
	Pillow_obj,
	Num_Objects
};

class CBaseObject
{
	public:

		CBaseObject(void) {}
		virtual ~CBaseObject(void) = 0 {}

		// Position and orientation of the object.
		Matrix4x4T		m_PandO;

		//This variable points to all collision data for this object
		void *CollisionData;

		//This variable points to the geometry of this object
		//GeometryDataT *GeometryData;
		GeometryT		*m_pGeometryData;

		//This variable will hold the type of this object
		int m_iType;
};
#endif