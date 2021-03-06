#ifndef _CCREDITSMENU_H_
#define _CCREDITSMENU_H_
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

class CCreditsMenu : public CBaseMenu
{
	protected:
		
		

	public:

		CCreditsMenu(void) {}
		virtual ~CCreditsMenu(void) {}
		
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