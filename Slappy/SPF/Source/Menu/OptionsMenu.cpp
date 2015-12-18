/*////////////////////////////////////////////////////////////////////////
//File:	OptionsMenu.cpp
//
//Author:	Dan Manning
//
//Creation Date:	9-18-04
//
//Purpose:	The Options menu class.
//
/////////////////////////////////////////////////////////////////////////*/
#include "optionsmenu.h"
#include "..\Sound\Sound.h"
#include "..\BitmapManager\BitmapManager.h"
#include "..\GameManager\GameManager.h"
#include "..\Renderer\DisplayDonkey.h"

/*////////////////////////////////////////////////////////////////////////
//Function:	Draw
//
//Purpose:	Draw the menu page
//
/////////////////////////////////////////////////////////////////////////*/
void COptionsMenu::Draw()
{
	//draw the background
	DrawBG();

	float fOffset = 1.23f;
	float fX = 0.05f;
	float fY = -0.28f;

	//temp variable to fix image stretching
	float fStretchFix = ((float)CGameManager::GetInstance()->m_iWindowHeight / (float)CGameManager::GetInstance()->m_iWindowWidth);
	CBitmapManager::GetInstance()->BindTexture(m_iTitleTarg);
	glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);
			glVertex2f(fX, fY);

			glTexCoord2f(0.0f, 1.0f);
			glVertex2f(fX, fY + fOffset);

			glTexCoord2f(1.0f, 1.0f);
			glVertex2f(fX + (fStretchFix * fOffset), fY + fOffset);

			glTexCoord2f(1.0f, 0.0f);
			glVertex2f(fX + (fStretchFix * fOffset), fY);
	glEnd();

	//draw all the menu items
	for (int i = 0; i < OPTIONMENU_NUMOFITEMS; i++)
	{
		//draw the pillow, dark if index equals active selection
		DrawPillow(m_MainMenuData[i].fItemLocationX, m_MainMenuData[i].fItemLocationY, (i == this->m_iActiveSelection ? true : false));

		//write the text
		CBmpFonts::GetInstance()->WriteWithFonts(m_MainMenuData[i].ScreenText, m_MainMenuData[i].fTextLocationX, m_MainMenuData[i].fTextLocationY);

		//draw volume, resolution, windowed mode
		switch (i)
		{
			case OPTIONMENU_VOLUME:
			{
				//get and draw the current volume
				int iCurVolume = (int)(CSound::GetInstance()->GetVolume() * 100.0f);
				//bah, hackery
				float fVolLoc = CBmpFonts::GetInstance()->WriteJustInts(iCurVolume, m_MainMenuData[i].fTextLocationX, (m_MainMenuData[i].fTextLocationY + .03f), 1);
				CBmpFonts::GetInstance()->WriteWithFonts("%", fVolLoc, (m_MainMenuData[i].fTextLocationY + .03f), 1);
			}
			break;

			case OPTIONMENU_MUSICVOLUME:
			{
				//get and draw the current volume
				int iCurVolume = (int)(CSound::GetInstance()->GetMusicVolume() * 100.0f);
				//bah, hackery
				float fVolLoc = CBmpFonts::GetInstance()->WriteJustInts(iCurVolume, m_MainMenuData[i].fTextLocationX, (m_MainMenuData[i].fTextLocationY + .03f), 1);
				CBmpFonts::GetInstance()->WriteWithFonts("%", fVolLoc, (m_MainMenuData[i].fTextLocationY + .03f), 1);
			}
			break;

			case OPTIONMENU_RESOLUTION:
			{
				//fix the resolution

				//get and draw the current resolution
				int iX = CGameManager::GetInstance()->m_iWindowWidth;
				int iY = CGameManager::GetInstance()->m_iWindowHeight;
				float fVolLoc = CBmpFonts::GetInstance()->WriteJustInts(iX, m_MainMenuData[i].fTextLocationX, (m_MainMenuData[i].fTextLocationY + .03f), 1);
				fVolLoc = CBmpFonts::GetInstance()->WriteWithFonts("x", fVolLoc, (m_MainMenuData[i].fTextLocationY + .03f), 1);
				CBmpFonts::GetInstance()->WriteJustInts(iY, fVolLoc, (m_MainMenuData[i].fTextLocationY + .03f), 1);
			}
			break;

			case OPTIONMENU_WINDOWMODE:
			{
				//get and draw the window mode
				bool bWindowMode = CGameManager::GetInstance()->m_bWindowedMode;
				if (bWindowMode)
				{
					CBmpFonts::GetInstance()->WriteWithFonts("True", m_MainMenuData[i].fTextLocationX, (m_MainMenuData[i].fTextLocationY + .03f), 1);
				}
				else
				{
					CBmpFonts::GetInstance()->WriteWithFonts("False", m_MainMenuData[i].fTextLocationX, (m_MainMenuData[i].fTextLocationY + .03f), 1);
				}
			}
			break;
		}
	}
}

/*////////////////////////////////////////////////////////////////////////
//Function:	Init
//
//Purpose:	Load all the bitmaps and stuff for a menu
//
/////////////////////////////////////////////////////////////////////////*/
void COptionsMenu::Init()
{
	//load bitmaps
	m_iBackgroundImage = CBitmapManager::GetInstance()->LoadImage("ResourceFiles\\textures\\Pinkback.tga", true);
	m_iPillowImage = CBitmapManager::GetInstance()->LoadImage("ResourceFiles\\textures\\Pillow.tga", true);
	m_iDarkPillowImage = CBitmapManager::GetInstance()->LoadImage("ResourceFiles\\textures\\PillowDark.tga", true);
	m_iTitleTarg = CBitmapManager::GetInstance()->LoadImage("ResourceFiles\\textures\\Title.tga", true);

	//set up all data for the options menu
	m_iActiveSelection = OPTIONMENU_VOLUME;

    //temp coords to set position of objects   
	float TempX = 0.42f;
    float TempY = 0.33f;

    // loop through and set each item in option menu.
    for(int i = 0; i < OPTIONMENU_NUMOFITEMS; i++)
    {
		//set the location of the menu item and text
		m_MainMenuData[i].fItemLocationX = TempX;
		m_MainMenuData[i].fTextLocationX = (TempX + .03f);
		m_MainMenuData[i].fItemLocationY = TempY;
		m_MainMenuData[i].fTextLocationY = (TempY + .03f);

        TempY += 0.13f;
    }

    strcpy(m_MainMenuData[OPTIONMENU_VOLUME].ScreenText,"Volume");
	strcpy(m_MainMenuData[OPTIONMENU_MUSICVOLUME].ScreenText, "Music Volume");
    strcpy(m_MainMenuData[OPTIONMENU_RESOLUTION].ScreenText, "Resolution");
    strcpy(m_MainMenuData[OPTIONMENU_WINDOWMODE].ScreenText, "Window Mode");
    strcpy(m_MainMenuData[OPTIONMENU_BACK].ScreenText, "Back");

	//load up the sound
	m_iDingSound = CSound::GetInstance()->LoadSound("ResourceFiles\\Sounds\\Josie_Puddin.wav");
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
int COptionsMenu::TakeInput(int iInput)
{
	if (iInput >= P1Up_MenuMessage && iInput <= P4Up_MenuMessage)
	{
		//decrement current active state
		if (m_iActiveSelection == OPTIONMENU_VOLUME)
		{
            m_iActiveSelection = OPTIONMENU_BACK;
		}
		else
		{
			m_iActiveSelection--;
		}
	}
	else if (iInput >= P1Down_MenuMessage && iInput <= P4Down_MenuMessage)
	{
		//increment current active selection
		if (m_iActiveSelection == OPTIONMENU_BACK)
		{
			m_iActiveSelection = OPTIONMENU_VOLUME;
		}
		else
		{
			m_iActiveSelection++;
		}
	}
	//parse the left and right messages
	else if (iInput >= P1Left_MenuMessage && iInput <= P4Left_MenuMessage)
	{
		switch (m_iActiveSelection)
		{
			case OPTIONMENU_VOLUME:
			{
				// Get the current volume and lower it a little
				if(CSound::GetInstance()->GetVolume() <= 0.1f)
				{
					CSound::GetInstance()->ChangeSFXVolume(0.0f);
				}
				else if(CSound::GetInstance()->GetVolume() > 0.1f)
				{
					CSound::GetInstance()->ChangeSFXVolume((CSound::GetInstance()->GetVolume() - 0.1f));
				}
				CSound::GetInstance()->PlaySoundEffect(m_iDingSound);
			}
			break;

			case OPTIONMENU_MUSICVOLUME:
			{
				// Get the current volume and lower it a little
				if(CSound::GetInstance()->GetMusicVolume() <= 0.1f)
				{
					CSound::GetInstance()->ChangeMusicVolume(0.0f);
				}
				else if(CSound::GetInstance()->GetMusicVolume() > 0.1f)
				{
					CSound::GetInstance()->ChangeMusicVolume((CSound::GetInstance()->GetMusicVolume() - 0.1f));
				}
				//CSound::GetInstance()->PlaySoundEffect(m_iDingSound);
			}
			break;

			case OPTIONMENU_RESOLUTION:
			{
				if(CGameManager::GetInstance()->m_bWindowedMode)
					CGameManager::GetInstance()->ResolutionToggle();
			}
			break;

			case OPTIONMENU_WINDOWMODE:
			{
				CGameManager::GetInstance()->WindowToggle();
			}
			break;
		}
	}
	else if (iInput >= P1Right_MenuMessage && iInput <= P4Right_MenuMessage)
	{
		switch (m_iActiveSelection)
		{
			case OPTIONMENU_VOLUME:
			{
				// Get the current volume and make it raise a little
				if(CSound::GetInstance()->GetVolume() >= 0.9f)
				{
					CSound::GetInstance()->ChangeSFXVolume(1.0f);
				}
				else if (CSound::GetInstance()->GetVolume() < 0.9f)
				{
					CSound::GetInstance()->ChangeSFXVolume((CSound::GetInstance()->GetVolume() + 0.1f));
				}
				CSound::GetInstance()->PlaySoundEffect(m_iDingSound);
			}
			break;

			case OPTIONMENU_MUSICVOLUME:
			{
				// Get the current volume and lower it a little
				if(CSound::GetInstance()->GetMusicVolume() >= 0.9f)
				{
					CSound::GetInstance()->ChangeMusicVolume(1.0f);
				}
				else if(CSound::GetInstance()->GetMusicVolume() < 0.9f)
				{
					CSound::GetInstance()->ChangeMusicVolume((CSound::GetInstance()->GetMusicVolume() + 0.1f));
				}
				//CSound::GetInstance()->PlaySoundEffect(m_iDingSound);
			}
			break;

			case OPTIONMENU_RESOLUTION:
			{
				if(CGameManager::GetInstance()->m_bWindowedMode)
					CGameManager::GetInstance()->ResolutionToggle();
			}
			break;

			case OPTIONMENU_WINDOWMODE:
			{
				CGameManager::GetInstance()->WindowToggle();
			}
			break;
		}
	}
	else if (iInput >= P1Attack_MenuMessage && iInput <= P4Jump_MenuMessage)
	{
		if (m_iActiveSelection == OPTIONMENU_BACK)
		{
			return MAINMENU;
		}
	}

	return -1;
}