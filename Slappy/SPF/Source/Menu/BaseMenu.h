#ifndef _CBASEMENU_H_
#define _CBASEMENU_H_
/*////////////////////////////////////////////////////////////////////////
//File:	BaseMenu.h
//
//Author:	Dan Manning
//
//Creation Date:	9-18-04
//
//Purpose:	The base class that all menus inherit from
//
/////////////////////////////////////////////////////////////////////////*/
#include "..\HUD\BmpFonts.h"

//enumeration of all the menu types
enum { 
	MAINMENU = 0, 
	OPTIONMENU, 
	CREDITMENU, 
	PLAYERSELECTMENU, 
	GETTHEHELLOUTOFMYGAME,
	STARTGAMENOWBITCH,
	NUMOFMENUS 
};

//Enumeration of all messages to send to a menu
enum {
	//Left messages
	P1Left_MenuMessage,
	P2Left_MenuMessage,
	P3Left_MenuMessage,
	P4Left_MenuMessage,

	//Right Messages
	P1Right_MenuMessage,
	P2Right_MenuMessage,
	P3Right_MenuMessage,
	P4Right_MenuMessage,

	//Up messages
	P1Up_MenuMessage,
	P2Up_MenuMessage,
	P3Up_MenuMessage,
	P4Up_MenuMessage,

	//Down messages
	P1Down_MenuMessage,
	P2Down_MenuMessage,
	P3Down_MenuMessage,
	P4Down_MenuMessage,

	//attack messages
	P1Attack_MenuMessage,
	P2Attack_MenuMessage,
	P3Attack_MenuMessage,
	P4Attack_MenuMessage,
	
	//jump messages
	P1Jump_MenuMessage,
	P2Jump_MenuMessage,
	P3Jump_MenuMessage,
	P4Jump_MenuMessage,

	Num_MenuMessages
};

struct MenuItemDataT
{
    // Items current state, usually on or off
    //int iItemState;

    char ScreenText[20];

    // TopLeft location of item.
    float fItemLocationX;
    float fItemLocationY;

	//TopLeft location of text
	float fTextLocationX;
	float fTextLocationY;
};

class CBaseMenu
{
	protected:

		//the id of the background image
		int m_iBackgroundImage;
		//the id of the pillow
		int m_iPillowImage;
		//the id of the dark pillow
		int m_iDarkPillowImage;

		//Hold an index of an active selection
		int m_iActiveSelection;
		
		/*////////////////////////////////////////////////////////////////////////
		//Function:	DrawBG
		//
		//Purpose:	Draw the background image of this menu
		//
		/////////////////////////////////////////////////////////////////////////*/
		virtual void DrawBG();

		/*////////////////////////////////////////////////////////////////////////
		//Function:	DrawPillow
		//
		//Purpose:	Draw a pillow image
		//
		/////////////////////////////////////////////////////////////////////////*/
		virtual void DrawPillow(float fTopLeftPointX, float fTopLeftPointY, bool bDarkened = false);

	public:

		CBaseMenu(void);
		virtual ~CBaseMenu(void) = 0;
		
		/*////////////////////////////////////////////////////////////////////////
		//Function:	Init
		//
		//Purpose:	Load all the bitmaps and stuff for a menu
		//
		/////////////////////////////////////////////////////////////////////////*/
		virtual void Init() = 0;

		/*////////////////////////////////////////////////////////////////////////
		//Function:	Draw
		//
		//Purpose:	Draw the menu page
		//
		/////////////////////////////////////////////////////////////////////////*/
		virtual void Draw() = 0;

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
		virtual int TakeInput(int iInput) = 0;
};

#endif