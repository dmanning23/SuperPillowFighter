#ifndef _OBJECTFACTORY_H_
#define _OBJECTFACTORY_H_
/*////////////////////////////////////////////////////////////////////////
//File:	ObjectFactory.h
//
//Author:	Dan Manning
//
//Creation Date:	8-15-4
//
//Purpose:	Class and function definitions for CObjectFactory
//
/////////////////////////////////////////////////////////////////////////*/
#include "..\Entities\Player.h"
#include "..\Entities\Pillow.h"
#include "..\Entities\Powerup.h"
#include "..\Entities\LevelObject.h"

#ifdef _DEBUG
#include "..\Logger\Logger.h"
#endif

//Struct to add linked list functionality to the CBaseObject class
struct BaseNode
{
	BaseNode *pNext;
	CBaseObject *myObject;
};

class CObjectFactory
{
	private:

		//singleton functionality of object Factory
		static CObjectFactory *m_pInstance;

		/*////////////////////////////////////////////////////////////////////////
		//Function:	default constructor
		//
		//Purpose:	initailizes the object factory and logger (debug mode)
		//
		/////////////////////////////////////////////////////////////////////////*/
		CObjectFactory(void);

		CObjectFactory(const CObjectFactory &CObjectFactoryRef) {}
		CObjectFactory &operator=(const CObjectFactory &CObjectFactoryRef) {}

#ifdef _DEBUG
		CLogger ObjLogger;
#endif

	public:
		
		/*////////////////////////////////////////////////////////////////////////
		//Function:	destructor
		//
		//Purpose:	This will check that all the memory allocated to this class
		//is deleted properly.  After this function is called, DeleteInstance will need to 
		//be called.
		//
		/////////////////////////////////////////////////////////////////////////*/
		virtual ~CObjectFactory(void);

		//Player Collision Data
		float Center_Point;
		float Center_Radius;
		float Entire_Radius;
		float Top_Center_Point;
		float Top_Radius;
		float Bottom_Center_Point;
		float Bottom_Radius;
		//Item Collision Data
		float Item_Radius;
		//Pillow Collision Data
		float Pillow_Radius;

		//lists of game objects
		BaseNode *m_PlayerList;
		BaseNode *m_PillowList;
		BaseNode *m_LevelObjectList;

		//sizes of lists
		int m_iNumPlayers;
		int m_iNumLevelObjects;

		//array of powerup templates
		CPowerup **m_PowerupTemplates;

		/*////////////////////////////////////////////////////////////////////////
		//Function:	Create
		//
		//Input:	
		//iType:	the type to create, as enumerated in BaseObject.h
		//
		//Return:	CBaseObject:	pointer to the base object created
		//
		//Purpose:	Provide functionality for a way to create game objects.
		//
		//NOTE:	This function WILL NOT load geometry OR collision data.
		//
		/////////////////////////////////////////////////////////////////////////*/
		CBaseObject *Create(int iType);

		/*////////////////////////////////////////////////////////////////////////
		//Function:	CreatePlayer
		//
		//Input:	
		//PlayerStuff:	the PlayerDataT for the player to create
		//
		//Purpose:	This function creates a player object based on the player 
		//data passed in.  This function will call the content loader to load 
		//the geometry, the animation engine to load animation data, and will 
		//open the correct player.txt file and load Bob Dole's stats and collision 
		//data.  This function will also load and initialize each player's pillow. 
		//
		/////////////////////////////////////////////////////////////////////////*/
		void CreatePlayer(PlayerDataT *pPlayerData);

		/*////////////////////////////////////////////////////////////////////////
		//Function:	CreateLevelObject
		//
		//Input:	
		//	iType:	the type of level object to create, as enumerated in levelobject.h
		//
		//Return:
		//	CLevelObject*:	a pointer to the created level object
		//
		//Purpose:	This function will create and set up a level object of the 
		//			defined type.
		//
		/////////////////////////////////////////////////////////////////////////*/
		CLevelObject *CreateLevelObject(int iType);

		/*////////////////////////////////////////////////////////////////////////
		//Function:	LoadItems
		//
		//Purpose:	This function will load all the items into the templates array.
		//This function will also load all the collision data and geometry for the 
		//item.
		//
		/////////////////////////////////////////////////////////////////////////*/
		void LoadItems();

		/*////////////////////////////////////////////////////////////////////////
		//Function:	Shutdown
		//
		//Purpose:	Clean up all memory allocated for object factory
		//
		/////////////////////////////////////////////////////////////////////////*/
		void Shutdown();

		/*////////////////////////////////////////////////////////////////////////
		//Function:	GetInstance
		//
		//Return:	static pointer to the only instance of the object factory
		//
		//Purpose:	Provide other modules with a way to access the functionality 
		//of the object factory
		//
		/////////////////////////////////////////////////////////////////////////*/
		static CObjectFactory *GetInstance();

		/*////////////////////////////////////////////////////////////////////////
		//Function:	DeleteInstance
		//
		//Purpose:	clean up all memory used for the object factory singleton
		//
		/////////////////////////////////////////////////////////////////////////*/
		static void DeleteInstance();
};

#endif