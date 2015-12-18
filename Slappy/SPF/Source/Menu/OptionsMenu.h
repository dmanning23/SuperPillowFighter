#ifndef _COPTIONSMENU_H_
#define _COPTIONSMENU_H_
/*////////////////////////////////////////////////////////////////////////
//File:	OptionsMenu.h
//
//Author:	Dan Manning
//
//Creation Date:	9-18-04
//
//Purpose:	The Options menu class.
//
/////////////////////////////////////////////////////////////////////////*/
#include "basemenu.h"

class COptionsMenu : public CBaseMenu
{
	protected:
		
		//The enumerations for all the selections in this state
		enum { 
			OPTIONMENU_VOLUME = 0, 
			OPTIONMENU_MUSICVOLUME,
			OPTIONMENU_RESOLUTION, 
			OPTIONMENU_WINDOWMODE, 
			OPTIONMENU_BACK, 
			OPTIONMENU_NUMOFITEMS 
		};

		//The Menu Item data structures for the main menu
		MenuItemDataT m_MainMenuData[OPTIONMENU_NUMOFITEMS];

		//the "ding" sound to play when the player changes volume
		int m_iDingSound;

		//Id of the Title.tga picture
		int m_iTitleTarg;

	public:

		COptionsMenu(void) {}
		virtual ~COptionsMenu(void) {}
		
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
};

#endif