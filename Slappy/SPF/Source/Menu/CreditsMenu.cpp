#include "creditsmenu.h"

#include "mainmenu.h"
#include "..\\BitmapManager\\BitmapManager.h"
#include "..\Sound\Sound.h"

/*////////////////////////////////////////////////////////////////////////
//Function:	Draw
//
//Purpose:	Draw the menu page
//
/////////////////////////////////////////////////////////////////////////*/
void CCreditsMenu::Draw()
{
	DrawBG();

	
	CBmpFonts::GetInstance()->setFontColor(0.05f, 0.05f, 0.45f);
	// main credits screen
	CBmpFonts::GetInstance()->WriteWithFonts("Credits", 0.35f, 0.03f, 2);
    
	// Our team credits
	// EDITED: 09/21/04 1:28 AM - STRAND
	// Allan and I were not included for some FUCKED up reason.
	CBmpFonts::GetInstance()->WriteWithFonts("Programming Team", 0.0f, 0.1f, 2); 
	CBmpFonts::GetInstance()->WriteWithFonts("Project Lead   Dan Manning", 0.0f, 0.15f, 1);
	CBmpFonts::GetInstance()->WriteWithFonts("Tech Lead      Kevin Cecelski", 0.0f, 0.19f, 1);
	CBmpFonts::GetInstance()->WriteWithFonts("Animation      Matt Strand", 0.0f, 0.23f, 1);
	CBmpFonts::GetInstance()->WriteWithFonts("Physics        Allan Bittan", 0.0f, 0.27f, 1);
	CBmpFonts::GetInstance()->WriteWithFonts("Outsource Mgr  Daron Keeler", 0.0f, 0.31f, 1);

	// Sounds
	CBmpFonts::GetInstance()->WriteWithFonts("Sounds", 0.4f, 0.35f, 2);	
	CBmpFonts::GetInstance()->WriteWithFonts("Sound Effects  Aaron Gandia", 0.4f, 0.4f, 1);
	CBmpFonts::GetInstance()->WriteWithFonts("Music          Mike Cecelski", 0.4f, 0.44f, 1);
	CBmpFonts::GetInstance()->WriteWithFonts("Voices         Andry B Gandia", 0.4f, 0.48f, 1);

	// 2D Artists
	CBmpFonts::GetInstance()->WriteWithFonts("2D Artists", 0.0f, 0.50f, 2);
	CBmpFonts::GetInstance()->WriteWithFonts("Characters     Ashley Schoeller", 0.0f, 0.55f, 1);
	CBmpFonts::GetInstance()->WriteWithFonts("Backgrounds    Dan Rosemund", 0.0f, 0.59f, 1);
	CBmpFonts::GetInstance()->WriteWithFonts("Concept Art    John", 0.0f, 0.63f, 1);
	
	// 3D Artists
	CBmpFonts::GetInstance()->WriteWithFonts("3D Artists", 0.4f, 0.68f, 2);
	CBmpFonts::GetInstance()->WriteWithFonts("Characters     Jaime Fiore", 0.4f, 0.73f, 1); 
	CBmpFonts::GetInstance()->WriteWithFonts("Level Models   Devin Nickoloff", 0.4f, 0.77f, 1);
	CBmpFonts::GetInstance()->WriteWithFonts("Level Models   James T McCurry", 0.4f, 0.81f, 1);
	CBmpFonts::GetInstance()->WriteWithFonts("Powerups       Jacob Meyer", 0.4f, 0.85f, 1);

	// Final Thanks
	//CBmpFonts::GetInstance()->WriteWithFonts("ALL OF THE FULL SAIL STAFF", 0.1f, 0.85f, 2);
	
				
	// Press any button
	CBmpFonts::GetInstance()->WriteWithFonts("Press Selection Button To Return To Main Menu", 0.25f, 0.95f, 0);
	CBmpFonts::GetInstance()->setFontColor(1.0f, 1.0f, 1.0f);
	CBmpFonts::GetInstance()->WriteWithFonts("a", 1.25f, 0.95f, 0);
	



}

/*////////////////////////////////////////////////////////////////////////
//Function:	Init
//
//Purpose:	Load all the bitmaps and stuff for a menu
//
/////////////////////////////////////////////////////////////////////////*/
void CCreditsMenu::Init()
{
	m_iBackgroundImage = CBitmapManager::GetInstance()->LoadImage("ResourceFiles\\textures\\DuckyBack.tga", true);
}

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
int CCreditsMenu::TakeInput(int iInput)
{
	if(iInput >= P1Attack_MenuMessage && iInput <= P4Jump_MenuMessage)
	{
		CSound::GetInstance()->StopMusic();
		CSound::GetInstance()->LoadMusic("ResourceFiles\\Music\\intro_loop.ogg");
		return MAINMENU;
	}

	return -1;
}