////////////////////////////////////////////////////////
//
// Name: "Menu.h"
//
// Author: Daron Keeler (DK)
//
// Creation Date: August 11, 2004
//
// Purpose: The base menu will allow users to change options, and start their game.
//
////////////////////////////////////////////////////////
#ifndef _MENU_H_
#define _MENU_H_

#include "BaseMenu.h"
#include "CharSelect.h"
#include "CreditsMenu.h"
#include "MainMenu.h"
#include "OptionsMenu.h"

/*
	MAINMENU = 0, 
	OPTIONMENU, 
	CREDITMENU, 
	PLAYERSELECTMENU, 
	GETTHEHELLOUTOFMYGAME,
	STARTGAMENOWBITCH,
*/

#define TIMERDELAY 0.14f

class CMenu
{
    private:

        // Holds which "page" or screen the menu is on
        int m_iCurMenuScreen;

        // main game data
        GameDataT *m_GameData;

        static CMenu* pInstance;

		CStopWatch InputStop;

		////Array of basemenus, to hold all the menuscreens
		////Fuck it, there are four fucking menus
		//CBaseMenu **m_Menus;
		CMainMenu m_MainMenu;
		CCharSelectMenu m_CharSelectMenu;
		COptionsMenu m_OptionsMenu;
		CCreditsMenu m_CreditsMenu;


    private:

        ////////////////////////////////////////////////////////////////////
        //
        // Function: "Constructor"
        //
        // Last Modified: August 12, 2004
        //
        // Input: none
        //
        // Output: none
        //
        // Return: none
        //
        // Purpose: Basic constructor.
        ////////////////////////////////////////////////////////////////////
		CMenu() {}

        ////////////////////////////////////////////////////////////////////
        //
        // Function: "Destructor"
        //
        // Last Modified: August 12, 2004
        //
        // Input: none
        //
        // Output: none
        //
        // Return: none
        //
        // Purpose: Basic destructor.
        ////////////////////////////////////////////////////////////////////
		~CMenu() {}

    public:

        ////////////////////////////////////////////////////////////////////
        //
        // Function: "GetInstance"
        //
        // Last Modified: August 12, 2004
        //
        // Input: none
        //
        // Output: none
        //
        // Return: CMenu *    a pointer to the single instance of the class
        //
        // Purpose: To get an instance of this class for singleton use.
        ////////////////////////////////////////////////////////////////////
        static CMenu* GetInstance();

        ////////////////////////////////////////////////////////////////////
        //
        // Function: "DeleteInstance"
        //
        // Last Modified: August 12, 2004
        //
        // Input: none
        //
        // Output: none
        //
        // Return: none
        //
        // Purpose: To delete an instance of this class for singleton use.
        ////////////////////////////////////////////////////////////////////
        static void DeleteInstance();

        ////////////////////////////////////////////////////////////////////
        //
        // Function: "UpdateMenu"
        //
        // Last Modified: August 12, 2004
        //
        // Input: none
        //
        // Output: none
        //
        // Return: bool
        //
        // Purpose: Updates menus and returns false, true if menu is complete and ready to be destroyed.
        ////////////////////////////////////////////////////////////////////
        bool UpdateMenu();

        ////////////////////////////////////////////////////////////////////
        //
        // Function: "InitMenu"
        //
        // Last Modified: August 12, 2004
        //
        // Input: none
        //
        // Output: none
        //
        // Return: none
        //
        // Purpose: Menu's Init function, loads bitmaps, etc and plays movies.
        ////////////////////////////////////////////////////////////////////
        void InitMenu();

        ////////////////////////////////////////////////////////////////////
        //
        // Function: "ShutdownMenu"
        //
        // Last Modified: August 12, 2004
        //
        // Input: none
        //
        // Output: none
        //
        // Return: GameDataT *  Returns the data needed to set up the game.
        //
        // Purpose: Safely releases everything, and returns final game data.
        ////////////////////////////////////////////////////////////////////
        GameDataT *ShutdownMenu();

		/*////////////////////////////////////////////////////////////////////////
		//Function:	DrawGameLoading
		//
		//Purpose:	This function will draw the game loading screen exactly once.
		//			It should be left up there until the game starts
		//
		/////////////////////////////////////////////////////////////////////////*/
		
	private:

		/*////////////////////////////////////////////////////////////////////////
		//Function:	DrawCurrentMenu
		//
		//Purpose:	Stupid hack to draw the current menu
		//
		/////////////////////////////////////////////////////////////////////////*/
		void DrawCurrentMenu();

		/*////////////////////////////////////////////////////////////////////////
		//Function:	SendInputToCurrentMenu
		//
		//Input:
		//	iInputType:	the input enumeration to send to the current menu
		//
		//Return:
		//	The next menu screen to go to
		//
		//Purpose:	Stupid hack to send input to the current menu
		//
		/////////////////////////////////////////////////////////////////////////*/
		int SendInputToCurrentMenu(int iInputType);
};  
#endif