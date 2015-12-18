////////////////////////////////////////////////////////
//
// Name: "Menu.cpp"
//
// Author: Daron Keeler (DK)
//
// Creation Date: August 12, 2004
//
// Purpose: The base menu will allow users to change options, and start their game.
//
////////////////////////////////////////////////////////
#include "Menu.h"
#include "..\\hud\\hud.h"
#include "..\\hud\\BmpFonts.h"
#include "..\\LevelManager\\Levelmanager.h"
#include "..\\Sound\\Sound.h"
#include "..\MemManager\MemManager.h"
#include "..\GameManager\GameManager.h"
#include "..\Input\SubModules\Input.h"
#include "..\ParticleEngine\RandomNumbers.h"
#include "..\Renderer\DisplayDonkey.h"

CMenu *CMenu::pInstance = 0;

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
CMenu *CMenu::GetInstance()
{
    // Make one if it hasnt been done.
    if(!pInstance)
    {
        //pInstance = new CMenu;
        pInstance = (CMenu*)MemMalloc(sizeof(CMenu));
        pInstance->CMenu::CMenu();
    }
    // Return the address of the instance.
    return pInstance;
}

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
void CMenu::DeleteInstance()
{
    // delete the instance
    if(pInstance)
    {
        //delete pInstance;
        pInstance->CMenu::~CMenu();
        MemFree(pInstance);
		pInstance = NULL;
    }
}

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
bool CMenu::UpdateMenu()
{
	//In arcade mode, the escape key will exit the game from the menus
	if (CGameManager::GetInstance()->m_bArcadeMode)
	{
		if (CInput::GetInstance()->GetKey(DIK_ESCAPE))
		{
			m_iCurMenuScreen = GETTHEHELLOUTOFMYGAME;
		}
	}

	//run the correct menu
	switch (m_iCurMenuScreen)
	{
		case GETTHEHELLOUTOFMYGAME:
		{
			m_GameData = 0;
			return true;
		}
		break;

		case STARTGAMENOWBITCH:
		{
			m_GameData = m_CharSelectMenu.GetGameData();
			return true;
		}
		break;

		default:
		{
			DrawCurrentMenu();
		}
	}
	
	//Check all the input
	//check if it is arcade mode
	//Temp variable to hold the returned menu mode
	int iNextMenuMode = -1;

	if (CGameManager::GetInstance()->m_bArcadeMode)
	{
		//p1
		//up		numpad8
		//down		numpad2
		//left		numpad4
		//right		numpad6
		//attack	leftshift
		//jump		z

		//p2
		//up		r
		//down		f
		//left		d
		//right		g
		//attack	w
		//jump		e

		//check player 1's inputs
		if (CInput::GetInstance()->GetKey(DIK_NUMPAD8))
		{
			iNextMenuMode = SendInputToCurrentMenu(P1Up_MenuMessage);
		}
		if (CInput::GetInstance()->GetKey(DIK_NUMPAD2))
		{
			iNextMenuMode = SendInputToCurrentMenu(P1Down_MenuMessage);
		}
		if (CInput::GetInstance()->GetKey(DIK_NUMPAD4))
		{
			iNextMenuMode = SendInputToCurrentMenu(P1Left_MenuMessage);
		}
		if (CInput::GetInstance()->GetKey(DIK_NUMPAD6))
		{
			iNextMenuMode = SendInputToCurrentMenu(P1Right_MenuMessage);
		}
		if (CInput::GetInstance()->GetKey(DIK_LSHIFT))
		{
			iNextMenuMode = SendInputToCurrentMenu(P1Attack_MenuMessage);
		}
		if (CInput::GetInstance()->GetKey(DIK_Z))
		{
			iNextMenuMode = SendInputToCurrentMenu(P1Jump_MenuMessage);
		}

		//check player 2's inputs
		if (CInput::GetInstance()->GetKey(DIK_R))
		{
			iNextMenuMode = SendInputToCurrentMenu(P2Up_MenuMessage);
		}
		if (CInput::GetInstance()->GetKey(DIK_F))
		{
			iNextMenuMode = SendInputToCurrentMenu(P2Down_MenuMessage);
		}
		if (CInput::GetInstance()->GetKey(DIK_D))
		{
			iNextMenuMode = SendInputToCurrentMenu(P2Left_MenuMessage);
		}
		if (CInput::GetInstance()->GetKey(DIK_G))
		{
			iNextMenuMode = SendInputToCurrentMenu(P2Right_MenuMessage);
		}
		if (CInput::GetInstance()->GetKey(DIK_W))
		{
			iNextMenuMode = SendInputToCurrentMenu(P2Attack_MenuMessage);
		}
		if (CInput::GetInstance()->GetKey(DIK_E))
		{
			iNextMenuMode = SendInputToCurrentMenu(P2Jump_MenuMessage);
		}
	}
	else
	{
		//check how many controllers there are
		int iNumControllers = CInput::GetInstance()->m_iNumJoysticks;
		if (iNumControllers > 0)
		{
			//loop through number of current controllers 
			//and send data to the current menu
			for (int i = 0; i < iNumControllers; i++)
			{
				//make sure we don't go over 4
				if (iNumControllers <= 4)
				{
					//adding i to the player 1's message will set it to the correct player

					//check the left direction
					if (CInput::GetInstance()->GetJoyDir(i) == JOYSTICK_LEFT)
					{
						iNextMenuMode = SendInputToCurrentMenu(i + P1Left_MenuMessage);
					}

					//check the right direction
					if (CInput::GetInstance()->GetJoyDir(i) == JOYSTICK_RIGHT)
					{
						iNextMenuMode = SendInputToCurrentMenu(i + P1Right_MenuMessage);
					}

					//check the up direction
					if (CInput::GetInstance()->GetJoyDir(i) == JOYSTICK_UP)
					{
						iNextMenuMode = SendInputToCurrentMenu(i + P1Up_MenuMessage);
					}

					//check the down direction
					if (CInput::GetInstance()->GetJoyDir(i) == JOYSTICK_DOWN)
					{
						iNextMenuMode = SendInputToCurrentMenu(i + P1Down_MenuMessage);
					}

					//check the attack button
					if (CInput::GetInstance()->JoyPush(1, i))
					{
						iNextMenuMode = SendInputToCurrentMenu(i + P1Attack_MenuMessage);
					}

					//check the jump button
					if (CInput::GetInstance()->JoyPush(2, i))
					{
						iNextMenuMode = SendInputToCurrentMenu(i + P1Jump_MenuMessage);
					}
				}
			}
		}
		else
		{
			//check the keyboard
			if (CInput::GetInstance()->GetKey(DIK_UP))
			{
				iNextMenuMode = SendInputToCurrentMenu(P1Up_MenuMessage);
			}
			if (CInput::GetInstance()->GetKey(DIK_DOWN))
			{
				iNextMenuMode = SendInputToCurrentMenu(P1Down_MenuMessage);
			}
			if (CInput::GetInstance()->GetKey(DIK_LEFT))
			{
				iNextMenuMode = SendInputToCurrentMenu(P1Left_MenuMessage);
			}
			if (CInput::GetInstance()->GetKey(DIK_RIGHT))
			{
				iNextMenuMode = SendInputToCurrentMenu(P1Right_MenuMessage);
			}
			if (CInput::GetInstance()->GetKey(DIK_A))
			{
				iNextMenuMode = SendInputToCurrentMenu(P1Attack_MenuMessage);
			}
			if (CInput::GetInstance()->GetKey(DIK_W))
			{
				iNextMenuMode = SendInputToCurrentMenu(P1Jump_MenuMessage);
			}
			if (CInput::GetInstance()->GetKey(DIK_RETURN))
			{
				iNextMenuMode = SendInputToCurrentMenu(P1Attack_MenuMessage);
			}
		}
	}

	//set the current menu change
	if (iNextMenuMode >= 0)
	{
		if (iNextMenuMode == CREDITMENU)
		{
			CSound::GetInstance()->StopMusic();
			CSound::GetInstance()->LoadMusic("ResourceFiles\\Music\\credit.ogg");
		}
		m_iCurMenuScreen = iNextMenuMode;
	}

	return false;
}

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
void CMenu::InitMenu()
{
	//restart the input timer
	InputStop.Start(TIMERDELAY);

	//intialize all the menus
	m_MainMenu.Init();
	m_CharSelectMenu.Init();
	m_OptionsMenu.Init();
	m_CreditsMenu.Init();

	//make sure game data points to null
	m_GameData = 0;

	//set the initial state
	m_iCurMenuScreen = MAINMENU;

	//set up bitmap fonts
	CBmpFonts::GetInstance()->InitFonts();

	//load the main menu song
	CSound::GetInstance()->LoadMusic("ResourceFiles\\Music\\intro_loop.ogg");
}

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
GameDataT *CMenu::ShutdownMenu()
{
	//check if there is any game data
	if (!m_GameData)
	{
		GameDataT *TempGamePointer = m_CharSelectMenu.GetGameData();
		MemFree(TempGamePointer);
	}
	else
	{
		//release all the music
		CSound::GetInstance()->ReleaseMusic();

		//Draw the "Game Loading" screen

		//get a random int
		int NumPictures = 8;
		int iPicIndex = RandomInt(0, NumPictures);

		//depending on the int returned, load an image
		int iBmpIndex = 0;
		switch (iPicIndex)
		{
			case 0:
			{
				iBmpIndex = CBitmapManager::GetInstance()->LoadImage("ResourceFiles\\textures\\JosieDone.tga", true);
			}
			break;

			case 1:
			{
				iBmpIndex = CBitmapManager::GetInstance()->LoadImage("ResourceFiles\\textures\\JosieLose.tga", true);
			}
			break;

			case 2:
			{
				iBmpIndex = CBitmapManager::GetInstance()->LoadImage("ResourceFiles\\textures\\JosieWins.tga", true);
			}
			break;

			case 3:
			{
				iBmpIndex = CBitmapManager::GetInstance()->LoadImage("ResourceFiles\\textures\\CaptainSallyDone.tga", true);
			}
			break;

			case 4:
			{
				iBmpIndex = CBitmapManager::GetInstance()->LoadImage("ResourceFiles\\textures\\CaptainSallyWins.tga", true);
			}
			break;

			case 5:
			{
				iBmpIndex = CBitmapManager::GetInstance()->LoadImage("ResourceFiles\\textures\\CaptainSallyLose.tga", true);
			}
			break;

			case 6:
			{
				iBmpIndex = CBitmapManager::GetInstance()->LoadImage("ResourceFiles\\textures\\MissElizabethDone.tga", true);
			}
			break;

			case 7:
			{
				iBmpIndex = CBitmapManager::GetInstance()->LoadImage("ResourceFiles\\textures\\MissElizabethLose.tga", true);
			}
			break;

			case 8:
			{
				iBmpIndex = CBitmapManager::GetInstance()->LoadImage("ResourceFiles\\textures\\SumiDone.tga", true);
			}
			break;
		}

		//clear screen to white
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		//draw the image at an arbitrary location
		float fOffset = 0.8f;
		//temp variable to fix image stretching
		float fStretchFix = ((float)CGameManager::GetInstance()->m_iWindowHeight / (float)CGameManager::GetInstance()->m_iWindowWidth);

		CBitmapManager::GetInstance()->BindTexture(iBmpIndex);

		glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f);
				glVertex2f(0.45f, 0.1f);

				glTexCoord2f(0.0f, 1.0f);
				glVertex2f(0.45f, 0.1f + fOffset);

				glTexCoord2f(1.0f, 1.0f);
				glVertex2f(0.45f + (fStretchFix * fOffset), 0.1f + fOffset);

				glTexCoord2f(1.0f, 0.0f);
				glVertex2f(0.45f + (fStretchFix * fOffset), 0.1f);
		glEnd();

		//write text in black
		CBmpFonts::GetInstance()->setFontColor(0.0f, 0.0f, 0.0f, 1.0f);

		//LOADING...
		CBmpFonts::GetInstance()->WriteWithFonts("Loading", 0.1f, 0.1f, 2);

		//Directions - KNOCK ALL THE OTHER PLAYERS OFF THE BOARD
		CBmpFonts::GetInstance()->WriteWithFonts("KNOCK ALL THE OTHER", 0.05f, 0.18f, 1);
		CBmpFonts::GetInstance()->WriteWithFonts("PLAYERS OFF THE BOARD", 0.05f, 0.22f, 1);

		//Depending on game mode
		if (m_GameData->iGameType == TIMEATTACK)
		{
			//Time attack - THE PLAYER WITH THE MOST KILLS AFTER TWO MINUTES WINS
			CBmpFonts::GetInstance()->WriteWithFonts("TIME ATTACK MODE", 0.05f, 0.30f, 1);
			CBmpFonts::GetInstance()->WriteWithFonts("THE PLAYER WITH THE MOST", 0.05f, 0.34f, 1);
			CBmpFonts::GetInstance()->WriteWithFonts("POINTS AFTER TWO MINUTES", 0.05f, 0.38f, 1);
			CBmpFonts::GetInstance()->WriteWithFonts("WINS", 0.05f, 0.42f, 1);
		}
		else
		{
			//Knockout - THE FIRST PLAYER TO REACH TEN KILLS WINS
			CBmpFonts::GetInstance()->WriteWithFonts("KNOCKOUT MODE", 0.05f, 0.30f, 1);
			CBmpFonts::GetInstance()->WriteWithFonts("THE FIRST PLAYER TO", 0.05f, 0.34f, 1);
			CBmpFonts::GetInstance()->WriteWithFonts("REACH TEN POINTS WINS", 0.05f, 0.38f, 1);
		}

		//mention items


		//check for keyboard mode
		if (m_GameData->PlayerInfo[0].iInputType == Keyboard_input)
		{
			//Knockout - THE FIRST PLAYER TO REACH TEN KILLS WINS
			CBmpFonts::GetInstance()->WriteWithFonts("KEYBOARD MAP", 0.05f, 0.55f, 1);
			CBmpFonts::GetInstance()->WriteWithFonts("ATTACK: A", 0.05f, 0.59f, 1);
			CBmpFonts::GetInstance()->WriteWithFonts("JUMP: W", 0.05f, 0.63f, 1);
			CBmpFonts::GetInstance()->WriteWithFonts("MOVEMENT: DIRECTION KEYS", 0.05f, 0.67f, 1);
		}

		//swap the buffers
		CDisplayDonkey::GetInstance()->Render();

		CBmpFonts::GetInstance()->setFontColor(1.0f, 1.0f, 1.0f, 1.0f);
	}

	//Clean out bitmap fonts
	CBmpFonts::DeleteInstance();
	return m_GameData;
}

/*////////////////////////////////////////////////////////////////////////
//Function:	DrawCurrentMenu
//
//Purpose:	Stupid hack to draw the current menu
//
/////////////////////////////////////////////////////////////////////////*/
void CMenu::DrawCurrentMenu()
{
	switch (m_iCurMenuScreen)
	{
		case MAINMENU:
		{
			m_MainMenu.Draw();
		}
		break;
		case OPTIONMENU:
		{
			m_OptionsMenu.Draw();
		}
		break;
		case CREDITMENU:
		{
			m_CreditsMenu.Draw();
		}
		break;
		case PLAYERSELECTMENU:
		{
			m_CharSelectMenu.Draw();
		}
		break;
	}
}

/*////////////////////////////////////////////////////////////////////////
//Function:	SendInputToCurrentMenu
//
//Input:
//	iInputType:	the input enumeration to send to the current menu
//
//Purpose:	Stupid hack to send input to the current menu
//
/////////////////////////////////////////////////////////////////////////*/
int CMenu::SendInputToCurrentMenu(int iInputType)
{
	if (InputStop.GetRemainingTime() > 0.0f)
	{
		return -1;
	}

	int iNextMenu = 0;
	switch (m_iCurMenuScreen)
	{
		case MAINMENU:
		{
			iNextMenu = m_MainMenu.TakeInput(iInputType);
		}
		break;
		case OPTIONMENU:
		{
			iNextMenu = m_OptionsMenu.TakeInput(iInputType);
		}
		break;
		case CREDITMENU:
		{
			iNextMenu = m_CreditsMenu.TakeInput(iInputType);
		}
		break;
		case PLAYERSELECTMENU:
		{
			iNextMenu = m_CharSelectMenu.TakeInput(iInputType);
		}
		break;
	}

	InputStop.Start(TIMERDELAY);
	return iNextMenu;
}