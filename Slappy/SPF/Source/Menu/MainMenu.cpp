#include "mainmenu.h"
#include "..\BitmapManager\BitmapManager.h"
#include "..\GameManager\GameManager.h"

CMainMenu::CMainMenu(void) : CBaseMenu()
{
}

CMainMenu::~CMainMenu(void)
{
}

/*////////////////////////////////////////////////////////////////////////
//Function:	Draw
//
//Purpose:	Draw the menu page
//
/////////////////////////////////////////////////////////////////////////*/
void CMainMenu::Draw()
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
	for (int i = 0; i < MAINMENU_NUMOFITEMS; i++)
	{
		if (CGameManager::GetInstance()->m_bArcadeMode)
		{
			//remove the options and exit menu items from arcade mode
			if (i == MAINMENU_OPTIONS ||
				i == MAINMENU_EXIT)
			{
				continue;
			}
		}

		//TODO
		//tweak the text placement, only if there is time for polish

		//draw the pillow, dark if index equals active selection
		DrawPillow(m_MainMenuData[i].fItemLocationX, m_MainMenuData[i].fItemLocationY, (i == this->m_iActiveSelection ? true : false));

		//write the text
		CBmpFonts::GetInstance()->WriteWithFonts(m_MainMenuData[i].ScreenText, m_MainMenuData[i].fTextLocationX, m_MainMenuData[i].fTextLocationY, 1);
	}
}

/*////////////////////////////////////////////////////////////////////////
//Function:	Init
//
//Purpose:	Load all the bitmaps and stuff for a menu
//
/////////////////////////////////////////////////////////////////////////*/
void CMainMenu::Init()
{
	//load bitmaps
	m_iBackgroundImage = CBitmapManager::GetInstance()->LoadImage("ResourceFiles\\textures\\Pinkback.tga", true);
	m_iPillowImage = CBitmapManager::GetInstance()->LoadImage("ResourceFiles\\textures\\Pillow.tga", true);
	m_iDarkPillowImage = CBitmapManager::GetInstance()->LoadImage("ResourceFiles\\textures\\PillowDark.tga", true);
	m_iTitleTarg = CBitmapManager::GetInstance()->LoadImage("ResourceFiles\\textures\\Title.tga", true);

	//load up all the data for the main menu
    // set menu's active state
    m_iActiveSelection = MAINMENU_START;
    float TempX = 0.42f;
    float TempY = 0.45f;

	if (CGameManager::GetInstance()->m_bArcadeMode)
	{
		TempY += 0.13f;
	}

    // loop through and set each item in the main menu
    for(int i = 0; i < MAINMENU_NUMOFITEMS; i++)
    {
		//set the location of the menu item and text
		m_MainMenuData[i].fItemLocationX = TempX;
		m_MainMenuData[i].fTextLocationX = (TempX + .03f);
		m_MainMenuData[i].fItemLocationY = TempY;
		m_MainMenuData[i].fTextLocationY = (TempY + .03f);

        TempY += 0.13f;
    }

    // Set up the menu's text
	strcpy(m_MainMenuData[MAINMENU_START].ScreenText, "Start");
	strcpy(m_MainMenuData[MAINMENU_OPTIONS].ScreenText, "Options");
	strcpy(m_MainMenuData[MAINMENU_CREDITS].ScreenText, "Credits");
	strcpy(m_MainMenuData[MAINMENU_EXIT].ScreenText, "Exit");
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
int CMainMenu::TakeInput(int iInput)
{
	if (iInput >= P1Up_MenuMessage && iInput <= P4Up_MenuMessage)
	{
		//decrement current active state
		if (m_iActiveSelection == MAINMENU_START)
		{
            m_iActiveSelection = MAINMENU_EXIT;
		}
		else
		{
			m_iActiveSelection--;
		}

		//remove the options and exit menu items from arcade mode
		if (CGameManager::GetInstance()->m_bArcadeMode)
		{
			//remove the options and exit menu items from arcade mode
			if (m_iActiveSelection == MAINMENU_OPTIONS)
			{
				m_iActiveSelection--;
			}

			if (m_iActiveSelection == MAINMENU_EXIT)
			{
				m_iActiveSelection--;
			}
		}
	}
	else if (iInput >= P1Down_MenuMessage && iInput <= P4Down_MenuMessage)
	{
		//increment current active selection
		if (m_iActiveSelection == MAINMENU_EXIT)
		{
			m_iActiveSelection = MAINMENU_START;
		}
		else
		{
			m_iActiveSelection++;
		}

		//remove the options and exit menu items from arcade mode
		if (CGameManager::GetInstance()->m_bArcadeMode)
		{
			//remove the options and exit menu items from arcade mode
			if (m_iActiveSelection == MAINMENU_OPTIONS)
			{
				m_iActiveSelection++;
			}

			if (m_iActiveSelection == MAINMENU_EXIT)
			{
				m_iActiveSelection = MAINMENU_START;
			}
		}
	}

	else if (iInput >= P1Attack_MenuMessage && iInput <= P4Jump_MenuMessage)
	{
		//remove the options and exit menu items from arcade mode
		if (CGameManager::GetInstance()->m_bArcadeMode)
		{
			//remove the options and exit menu items from arcade mode
			if (m_iActiveSelection == MAINMENU_OPTIONS)
			{
				m_iActiveSelection = MAINMENU_CREDITS;
			}

			if (m_iActiveSelection == MAINMENU_EXIT)
			{
				m_iActiveSelection = MAINMENU_START;
			}
		}

		//find out the right state to go to
		switch (m_iActiveSelection)
		{
			case MAINMENU_START:
			{
				return PLAYERSELECTMENU;
			}
			break;

			case MAINMENU_OPTIONS:
			{
				return OPTIONMENU;
			}
			break;

			case MAINMENU_CREDITS:
			{
				return CREDITMENU;
			}
			break;

			case MAINMENU_EXIT:
			{
				return GETTHEHELLOUTOFMYGAME;
			}
			break;
		}
	}

	return -1;
}

void CMainMenu::DrawBG()
{
	CBaseMenu::DrawBG();
}