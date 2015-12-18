#ifndef _CCHARSELECTMENU_H_
#define _CCHARSELECTMENU_H_
/*////////////////////////////////////////////////////////////////////////
//File:	CreditsMenu.h
//
//Author:	Dan Manning
//
//Creation Date:	9-18-04
//
//Purpose:	The Credits menu class.
//
/////////////////////////////////////////////////////////////////////////*/
#include "basemenu.h"
#include "..\Entities\Player.h"

//the maximum number of players
#define MENU_MAXPLAYERS 4

//Enumeration of possible game types
enum {
	TIMEATTACK = 0, 
	KNOCKOUT
};

//structure used to select characters
struct CharSelectDataT
{
	//The character that this player will control in the game
	PlayerDataT *pMyPlayerData;

	//index of my player data
	int iIndex;

    //The character that this player is selecting (doesn't have to be their own)
	PlayerDataT *pCurrentPlayerSelect;
};

// Main game data struct.
struct GameDataT
{
    // Holds the current game type, enumerated above
    int iGameType;
    // Array of four players, each one holding player data
    PlayerDataT PlayerInfo[MENU_MAXPLAYERS];
    // Enumerated value that holds which level was selected.
    int iLevelChoice;
};

class CCharSelectMenu : public CBaseMenu
{
	protected:

		//character select structures
		CharSelectDataT m_CharSelect[4];
		
		//Image ids for character selection
		int m_iCharPics[4];

		//Image IDs for the background images
		int m_iCharBackgrounds[4];

		//enuemration of possible states of player menu
		enum {
			PLAYERSELECTMENU_BACK = 0, 
			PLAYERSELECTMENU_MODE,
			PLAYERSELECTMENU_PLAYERS, 
			PLAYERSELECTMENU_LEVELS, 
			PLAYERSELECTMENU_DONE, 
			PLAYERSELECTMENU_NUMOFITEMS
		};

		//menu items
		MenuItemDataT m_MenuItems[PLAYERSELECTMENU_NUMOFITEMS];

		//Character select menu items
		MenuItemDataT m_CharSelectMenuItems[4];

		//the game data for the new game
		GameDataT *m_pGameData;

		/*////////////////////////////////////////////////////////////////////////
		//Function:	ParseCharSelect
		//
		//Input:
		//	iInputType:	the button being passed in
		//
		//Purpose:	Provide a centralized way to parse input for the character select
		//
		/////////////////////////////////////////////////////////////////////////*/
		void ParseCharSelect(int iInputType);

	public:

		CCharSelectMenu(void) {}
		virtual ~CCharSelectMenu(void) {}
		
		/*////////////////////////////////////////////////////////////////////////
		//Function:	Init
		//
		//Purpose:	Load all the bitmaps and stuff for a menu
		//
		/////////////////////////////////////////////////////////////////////////*/
		virtual void Init();
		
		/*////////////////////////////////////////////////////////////////////////
		//Function:	Draw
		//
		//Purpose:	Draw the menu page
		//
		/////////////////////////////////////////////////////////////////////////*/
		virtual void Draw();

		/*////////////////////////////////////////////////////////////////////////
		//Function:	TakeInput
		//
		//Input:	
		//	iInput:	the message to send to the menu
		//
		//Return:
		//	int:	an enumeration of the menu to go to
		//			Will be -1 to stay on this menu
		//
		//Purpose:	Pass an input message to the menu
		//
		/////////////////////////////////////////////////////////////////////////*/
		virtual int TakeInput(int iInput);

		/*////////////////////////////////////////////////////////////////////////
		//Function:	GetGameData
		//
		//Return:
		//	GameDataT *: the completed game data structure
		//
		//Purpose:	Provide access to m_pGameData
		//
		/////////////////////////////////////////////////////////////////////////*/
		GameDataT *GetGameData() { return m_pGameData; }

		/*////////////////////////////////////////////////////////////////////////
		//Function:	GoHome
		//
		//Input:
		//	iIndex:	the index of the player data to check
		//
		//Purpose:	Checks the playerdata array in the gamedata structure
		//			If anyone points to it, they are sent to point at their
		//			onw playerdata
		//
		/////////////////////////////////////////////////////////////////////////*/
		void GoHome(int iIndex);
};

#endif