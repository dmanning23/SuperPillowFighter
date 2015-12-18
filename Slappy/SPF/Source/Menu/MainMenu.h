#ifndef _CMAINMENU_H_
#define _CMAINMENU_H_
/*////////////////////////////////////////////////////////////////////////
//File:	MainMenu.h
//
//Author:	Dan Manning
//
//Creation Date:	9-18-04
//
//Purpose:	The main menu class.
//
/////////////////////////////////////////////////////////////////////////*/
#include "basemenu.h"

class CMainMenu : public CBaseMenu
{
	protected:
		
		//The enumerations for all the selections in this state
		enum { 
			MAINMENU_START = 0, 
			MAINMENU_OPTIONS, 
			MAINMENU_CREDITS, 
			MAINMENU_EXIT, 
			MAINMENU_NUMOFITEMS
		};

		//The Menu Item data structures for the main menu
		MenuItemDataT m_MainMenuData[MAINMENU_NUMOFITEMS];

		//Id of the Title.tga picture
		int m_iTitleTarg;

	public:

		CMainMenu(void);
		virtual ~CMainMenu(void);
		
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

		virtual void DrawBG();
};

#endif
