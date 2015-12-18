/*////////////////////////////////////////////////////////////////////////
//File:	CreditsMenu.cpp
//
//Author:	Dan Manning
//
//Creation Date:	9-18-04
//
//Purpose:	The Credits menu class.
//
/////////////////////////////////////////////////////////////////////////*/
#include "charselect.h"
#include "..\BitmapManager\BitmapManager.h"
#include "..\MemManager\MemManager.h"
#include "..\LevelManager\LevelManager.h"
#include "..\GameManager\GameManager.h"
#include "..\Input\SubModules\Input.h"

#include "mainmenu.h"

/*////////////////////////////////////////////////////////////////////////
//Function:	Draw
//
//Purpose:	Draw the menu page
//
/////////////////////////////////////////////////////////////////////////*/
void CCharSelectMenu::Draw()
{
	//First, draw the background
	DrawBG();

	//Draw the menu
	for (int i = 0; i < PLAYERSELECTMENU_NUMOFITEMS; i++)
	{
		switch (i)
		{
			case PLAYERSELECTMENU_MODE:
			{
				//draw the pillow, dark if index equals active selection
				DrawPillow(m_MenuItems[i].fItemLocationX, m_MenuItems[i].fItemLocationY, (i == this->m_iActiveSelection ? true : false));

				//write the text
				CBmpFonts::GetInstance()->WriteWithFonts(m_MenuItems[i].ScreenText, m_MenuItems[i].fTextLocationX, m_MenuItems[i].fTextLocationY, 1);

				//write whichever mode it is
				switch (m_pGameData->iGameType)
				{
					case TIMEATTACK:
					{
						CBmpFonts::GetInstance()->WriteWithFonts("Time", m_MenuItems[i].fTextLocationX, m_MenuItems[i].fTextLocationY + .03f, 1);
						CBmpFonts::GetInstance()->WriteWithFonts("Attack", m_MenuItems[i].fTextLocationX, m_MenuItems[i].fTextLocationY + .06f, 1);
					}
					break;

					case KNOCKOUT:
					{
						CBmpFonts::GetInstance()->WriteWithFonts("Knockout", m_MenuItems[i].fTextLocationX, m_MenuItems[i].fTextLocationY + .03f, 1);
					}
					break;
				}
			}
			break;

			case PLAYERSELECTMENU_PLAYERS:
			{
				//draw some different stuff for the character select

				// Initialize these temps to some number.
				float TmpFontLoc = 0.0f;

				float fOffset = 0.38f;

				//temp variable to fix image stretching
				float fStretchFix = ((float)CGameManager::GetInstance()->m_iWindowHeight / (float)CGameManager::GetInstance()->m_iWindowWidth);

				// Draw the four player selects.
				for(int j = 0; j < MENU_MAXPLAYERS; j++)
				{
					CBitmapManager::GetInstance()->BindTexture(m_iCharBackgrounds[j]);

					glBegin(GL_QUADS);
							glTexCoord2f(0.0f, 0.0f);
							glVertex2f(m_CharSelectMenuItems[j].fItemLocationX, m_CharSelectMenuItems[j].fItemLocationY);

							glTexCoord2f(0.0f, 1.0f);
							glVertex2f(m_CharSelectMenuItems[j].fItemLocationX, m_CharSelectMenuItems[j].fItemLocationY + fOffset);

							glTexCoord2f(1.0f, 1.0f);
							glVertex2f(m_CharSelectMenuItems[j].fItemLocationX + (fStretchFix * fOffset), m_CharSelectMenuItems[j].fItemLocationY + fOffset);

							glTexCoord2f(1.0f, 0.0f);
							glVertex2f(m_CharSelectMenuItems[j].fItemLocationX + (fStretchFix * fOffset), m_CharSelectMenuItems[j].fItemLocationY);
					glEnd();

					// if its going to be used, draw it.
					if(m_pGameData->PlayerInfo[j].bIsActive)
					{
						// bind the proper character.
						CBitmapManager::GetInstance()->BindTexture(m_iCharPics[m_pGameData->PlayerInfo[j].iCharacter]);

						glBegin(GL_QUADS);
							glTexCoord2f(0.0f, 0.0f);
							glVertex2f(m_CharSelectMenuItems[j].fItemLocationX, m_CharSelectMenuItems[j].fItemLocationY);

							glTexCoord2f(0.0f, 1.0f);
							glVertex2f(m_CharSelectMenuItems[j].fItemLocationX, m_CharSelectMenuItems[j].fItemLocationY + fOffset);

							glTexCoord2f(1.0f, 1.0f);
							glVertex2f(m_CharSelectMenuItems[j].fItemLocationX + (fStretchFix * fOffset), m_CharSelectMenuItems[j].fItemLocationY + fOffset);

							glTexCoord2f(1.0f, 0.0f);
							glVertex2f(m_CharSelectMenuItems[j].fItemLocationX + (fStretchFix * fOffset), m_CharSelectMenuItems[j].fItemLocationY);
						glEnd();

						//if a bot, write "AI" on the picture
						if (m_pGameData->PlayerInfo[j].iInputType == AI_input)
						{
							CBmpFonts::GetInstance()->setFontColor(1.0f, 0.0f, 0.0f);
							CBmpFonts::GetInstance()->WriteWithFonts("AI", (m_CharSelectMenuItems[j].fItemLocationX + .05f), (m_CharSelectMenuItems[j].fItemLocationY + .05f), 2);
							CBmpFonts::GetInstance()->setFontColor(1.0f, 1.0f, 1.0f);
						}
					}

					//draw the player location
					for (int k = 0; k < MENU_MAXPLAYERS; k++)
					{
						if (m_CharSelect[k].iIndex == j && m_CharSelect[k].pMyPlayerData != 0)
						{
							CBmpFonts::GetInstance()->setFontColor(1.0f, 0.0f, 0.0f);
							CBmpFonts::GetInstance()->WriteJustInts((k + 1), m_CharSelectMenuItems[j].fItemLocationX, m_CharSelectMenuItems[j].fItemLocationY, 2);
							CBmpFonts::GetInstance()->setFontColor(1.0f, 1.0f, 1.0f);
						}
					}

					//if this is the active selection, write "Select Your Character" underneath it
					if (m_iActiveSelection == PLAYERSELECTMENU_PLAYERS)
					{
						CBmpFonts::GetInstance()->setFontColor(1.0f, 0.0f, 0.0f);
						CBmpFonts::GetInstance()->WriteWithFonts("Select your character", 0.21f, 0.65f, 2);
						CBmpFonts::GetInstance()->setFontColor(1.0f, 1.0f, 1.0f);
					}
				}
			}
			break;

			case PLAYERSELECTMENU_LEVELS:
			{
				//draw the pillow, dark if index equals active selection
				DrawPillow(m_MenuItems[i].fItemLocationX, m_MenuItems[i].fItemLocationY, (i == this->m_iActiveSelection ? true : false));

				//write the text
				CBmpFonts::GetInstance()->WriteWithFonts(m_MenuItems[i].ScreenText, m_MenuItems[i].fTextLocationX, m_MenuItems[i].fTextLocationY, 1);

				//TODO
				//this has to be changed to support the levels that we have

				//write the selected level
				switch (m_pGameData->iLevelChoice)
				{
					case PIRATEBOAT_LEVEL:
					{
						CBmpFonts::GetInstance()->WriteWithFonts("Boat", m_MenuItems[i].fTextLocationX, m_MenuItems[i].fTextLocationY + .03f, 1);
					}
					break;

					case FERRIS_WHEEL_LEVEL:
					{
						CBmpFonts::GetInstance()->WriteWithFonts("Ferris Wheel", m_MenuItems[i].fTextLocationX, m_MenuItems[i].fTextLocationY + .03f, 1);
					}
					break;

					//case TEST_LEVEL:
					//{
					//	CBmpFonts::GetInstance()->WriteWithFonts("Test", m_MenuItems[i].fTextLocationX, m_MenuItems[i].fTextLocationY + .03f, 1);
					//}
					//break;
				}
			}
			break;

			default:
			{
				//draw the pillow, dark if index equals active selection
				DrawPillow(m_MenuItems[i].fItemLocationX, m_MenuItems[i].fItemLocationY, (i == this->m_iActiveSelection ? true : false));

				//write the text
				CBmpFonts::GetInstance()->WriteWithFonts(m_MenuItems[i].ScreenText, m_MenuItems[i].fTextLocationX, m_MenuItems[i].fTextLocationY, 1);
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
void CCharSelectMenu::Init()
{
	//load images
	m_iBackgroundImage = CBitmapManager::GetInstance()->LoadImage("ResourceFiles\\textures\\Pinkback.tga", true);
	m_iPillowImage = CBitmapManager::GetInstance()->LoadImage("ResourceFiles\\textures\\Pillow.tga", true);
	m_iDarkPillowImage = CBitmapManager::GetInstance()->LoadImage("ResourceFiles\\textures\\PillowDark.tga", true);

	//load character images and backgrounds
	m_iCharPics[Josie_Player] = CBitmapManager::GetInstance()->LoadImage("ResourceFiles\\textures\\JosieDone.tga", true);
	m_iCharPics[CaptainSally_Player] = CBitmapManager::GetInstance()->LoadImage("ResourceFiles\\textures\\CaptainSallyDone.tga", true);
	m_iCharPics[MissElizabeth_Player] = CBitmapManager::GetInstance()->LoadImage("ResourceFiles\\textures\\MissElizabethDone.tga", true);
	m_iCharPics[Sumi_Player] = CBitmapManager::GetInstance()->LoadImage("ResourceFiles\\textures\\SumiDone.tga", true);

	//Image IDs for the background images
	m_iCharBackgrounds[0] = CBitmapManager::GetInstance()->LoadImage("ResourceFiles\\textures\\Player1BG.tga", true);
	m_iCharBackgrounds[1] = CBitmapManager::GetInstance()->LoadImage("ResourceFiles\\textures\\Player2BG.tga", true);
	m_iCharBackgrounds[2] = CBitmapManager::GetInstance()->LoadImage("ResourceFiles\\textures\\Player3BG.tga", true);
	m_iCharBackgrounds[3] = CBitmapManager::GetInstance()->LoadImage("ResourceFiles\\textures\\Player4BG.tga", true);

	//allocate memory to the gamadatat pointer
	m_pGameData = (GameDataT*)MemMalloc(sizeof(GameDataT));
	memset(m_pGameData, 0, sizeof(GameDataT));

	memset(m_CharSelect, 0, sizeof(CharSelectDataT) * 4);

	//set colors of player data structures
	m_pGameData->PlayerInfo[0].PaletteColor[0] = 0.5f;
	m_pGameData->PlayerInfo[0].PaletteColor[1] = 0.5f;
	m_pGameData->PlayerInfo[0].PaletteColor[2] = 1.0f;

	m_pGameData->PlayerInfo[1].PaletteColor[0] = 1.0f;
	m_pGameData->PlayerInfo[1].PaletteColor[1] = 0.5f;
	m_pGameData->PlayerInfo[1].PaletteColor[2] = 0.5f;

	m_pGameData->PlayerInfo[2].PaletteColor[0] = 0.5f;
	m_pGameData->PlayerInfo[2].PaletteColor[1] = 1.0f;
	m_pGameData->PlayerInfo[2].PaletteColor[2] = 0.5f;

	m_pGameData->PlayerInfo[3].PaletteColor[0] = 1.0f;
	m_pGameData->PlayerInfo[3].PaletteColor[1] = 1.0f;
	m_pGameData->PlayerInfo[3].PaletteColor[2] = 0.0f;

    // set menu's active state
	m_iActiveSelection = PLAYERSELECTMENU_PLAYERS;

	//set positions of all the menu items
    float TempX = 0.42f;
    float TempY = 0.01f;

    // loop through and set each item in the main menu
    for(int i = 0; i < PLAYERSELECTMENU_NUMOFITEMS; i++)
    {
		//set the location of the menu item and text
		m_MenuItems[i].fItemLocationX = TempX;
		m_MenuItems[i].fTextLocationX = (TempX + .03f);
		m_MenuItems[i].fItemLocationY = TempY;
		m_MenuItems[i].fTextLocationY = (TempY + .03f);

		if (i == PLAYERSELECTMENU_PLAYERS)
		{
			//the x position where the pics start
			float fTempX = 0.02f;

			//the spacing of the pics
			float fXOffset = 0.23f;

			for (int j = 0; j < 4; j++)
			{
				//set up the character selection menu items
				m_CharSelectMenuItems[j].fItemLocationX = fTempX;
				m_CharSelectMenuItems[j].fItemLocationY = (TempY + .04f);
				m_CharSelectMenuItems[j].fTextLocationX = fTempX;
				m_CharSelectMenuItems[j].fTextLocationY = (TempY + .04f);

				fTempX += fXOffset;
			}

			TempY += 0.5f;
		}
		else
		{
			TempY += 0.11f;
		}
	}

    // Set up the menu's text
	strcpy(m_MenuItems[PLAYERSELECTMENU_BACK].ScreenText, "Back");
	strcpy(m_MenuItems[PLAYERSELECTMENU_MODE].ScreenText, "Mode");
	strcpy(m_MenuItems[PLAYERSELECTMENU_PLAYERS].ScreenText, "Player Select");
	strcpy(m_MenuItems[PLAYERSELECTMENU_LEVELS].ScreenText, "Level");
	strcpy(m_MenuItems[PLAYERSELECTMENU_DONE].ScreenText, "Start");
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
int CCharSelectMenu::TakeInput(int iInput)
{
	if (iInput >= P1Up_MenuMessage && iInput <= P4Up_MenuMessage)
	{
		//decrement current active state
		if (m_iActiveSelection == PLAYERSELECTMENU_BACK)
		{
            m_iActiveSelection = PLAYERSELECTMENU_DONE;
		}
		else
		{
			m_iActiveSelection--;
		}
	}
	else if (iInput >= P1Down_MenuMessage && iInput <= P4Down_MenuMessage)
	{
		//increment current active selection
		if (m_iActiveSelection == PLAYERSELECTMENU_DONE)
		{
			m_iActiveSelection = PLAYERSELECTMENU_BACK;
		}
		else
		{
			m_iActiveSelection++;
		}
	}
	//parse the left and right messages
	else if (iInput >= P1Left_MenuMessage && iInput <= P4Left_MenuMessage)
	{
		if (m_iActiveSelection == PLAYERSELECTMENU_PLAYERS)
		{
			//parse left for the char select
			ParseCharSelect(iInput);
		}
	}
	else if (iInput >= P1Right_MenuMessage && iInput <= P4Right_MenuMessage)
	{
		if (m_iActiveSelection == PLAYERSELECTMENU_PLAYERS)
		{
			//parse the right message for char select
			ParseCharSelect(iInput);
		}
	}

	//parse button presses
	else if (iInput >= P1Attack_MenuMessage && iInput <= P4Jump_MenuMessage)
	{
		if (m_iActiveSelection == PLAYERSELECTMENU_BACK)
		{
			return MAINMENU;
		}
		//do the mode select
		else if (m_iActiveSelection == PLAYERSELECTMENU_MODE)
		{
			if (m_pGameData->iGameType == KNOCKOUT)
			{
				m_pGameData->iGameType = TIMEATTACK;
			}
			else
			{
				m_pGameData->iGameType++;
			}
		}
		//do the player select
		else if (m_iActiveSelection == PLAYERSELECTMENU_PLAYERS)
		{
			//parse action messages for the char select menu
			ParseCharSelect(iInput);
		}

		//TODO
		//change this to match the levels we have

		//do the level select
		else if (m_iActiveSelection == PLAYERSELECTMENU_LEVELS)
		{
			if (m_pGameData->iLevelChoice == PIRATEBOAT_LEVEL)
			{
				m_pGameData->iLevelChoice = FERRIS_WHEEL_LEVEL;
			}
			else if (m_pGameData->iLevelChoice == FERRIS_WHEEL_LEVEL)
			{
				m_pGameData->iLevelChoice = PIRATEBOAT_LEVEL;
			}
		}
		//start the game
		else if (m_iActiveSelection == PLAYERSELECTMENU_DONE)
		{
			//make sure game data holds valid information

			//find the number of active players
			int iNumPlayers = 0;
			for (int i = 0; i < 4; i++)
			{
				if (m_pGameData->PlayerInfo[i].bIsActive)
				{
					iNumPlayers++;
				}
			}

			//if less than 2 players, add an ai
			while (iNumPlayers < 2)
			{
				//find the first inactive player, and set it up as a bot
				for (int i = 0; i < 4; i++)
				{
					if (!m_pGameData->PlayerInfo[i].bIsActive)
					{
						m_pGameData->PlayerInfo[i].bIsActive = true;
						m_pGameData->PlayerInfo[i].iCharacter = Josie_Player;
						m_pGameData->PlayerInfo[i].iInputType = AI_input;
						iNumPlayers++;
						break;
					}
				}
			}
			return STARTGAMENOWBITCH;
		}
	}

	return -1;
}

/*////////////////////////////////////////////////////////////////////////
//Function:	ParseCharSelect
//
//Input:
//	iInputType:	the button being passed in
//
//Purpose:	Provide a centralized way to parse input for the character select
//
/////////////////////////////////////////////////////////////////////////*/
void CCharSelectMenu::ParseCharSelect(int iInputType)
{
	//The player index that this input applies to
	int iPlayerIndex = iInputType;

	//This variable holds which input was recieved
	//The values correspond as follows:
	enum { Left = 0, Right, Up, Down, Attack, Jump };
	int iInputValue;

	//check for left
	if (iInputType >= P1Left_MenuMessage && iInputType <= P4Left_MenuMessage)
	{
		iInputValue = Left;
		iPlayerIndex = (iInputType - P1Left_MenuMessage);
	}

	//check for right
	else if (iInputType >= P1Right_MenuMessage && iInputType <= P4Right_MenuMessage)
	{
		iInputValue = Right;
		iPlayerIndex = (iInputType - P1Right_MenuMessage);
	}

	//check for up
	else if (iInputType >= P1Up_MenuMessage && iInputType <= P4Up_MenuMessage)
	{
		iInputValue = Up;
		iPlayerIndex = (iInputType - P1Up_MenuMessage);
	}

	//check for down
	else if (iInputType >= P1Down_MenuMessage && iInputType <= P4Down_MenuMessage)
	{
		iInputValue = Down;
		iPlayerIndex = (iInputType - P1Down_MenuMessage);
	}

	//check for attack
	else if (iInputType >= P1Attack_MenuMessage && iInputType <= P4Attack_MenuMessage)
	{
		iInputValue = Attack;
		iPlayerIndex = (iInputType - P1Attack_MenuMessage);
	}

	//check for jump
	else if (iInputType >= P1Jump_MenuMessage && iInputType <= P4Jump_MenuMessage)
	{
		iInputValue = Jump;
		iPlayerIndex = (iInputType - P1Jump_MenuMessage);
	}

	//if this player is not yet activated, activate them and exit
	if (m_CharSelect[iPlayerIndex].pMyPlayerData == 0)
	{
		//check for arcade mode
		if (CGameManager::GetInstance()->m_bArcadeMode)
		{
			//check that the player isn't greater than player one
			if (iPlayerIndex == 0)
			{
				m_CharSelect[iPlayerIndex].pMyPlayerData = &m_pGameData->PlayerInfo[iPlayerIndex];
				m_CharSelect[iPlayerIndex].pCurrentPlayerSelect = &m_pGameData->PlayerInfo[iPlayerIndex];
				m_CharSelect[iPlayerIndex].iIndex = iPlayerIndex;

				//set up the player
				m_CharSelect[iPlayerIndex].pMyPlayerData->bIsActive = true;

				//set up the players controller
				m_CharSelect[iPlayerIndex].pMyPlayerData->iInputType = ArcadeKeyMap1_input;
				m_CharSelect[iPlayerIndex].pMyPlayerData->iCharacter = Josie_Player;
			}
			else if (iPlayerIndex == 1)
			{
				m_CharSelect[iPlayerIndex].pMyPlayerData = &m_pGameData->PlayerInfo[iPlayerIndex];
				m_CharSelect[iPlayerIndex].pCurrentPlayerSelect = &m_pGameData->PlayerInfo[iPlayerIndex];
				m_CharSelect[iPlayerIndex].iIndex = iPlayerIndex;

				//set up the player
				m_CharSelect[iPlayerIndex].pMyPlayerData->bIsActive = true;

				//set up the players controller
				m_CharSelect[iPlayerIndex].pMyPlayerData->iInputType = ArcadeKeyMap2_input;
				m_CharSelect[iPlayerIndex].pMyPlayerData->iCharacter = Josie_Player;
			}

			//check if anyone else is pointing at this playerdata
			GoHome(iPlayerIndex);

			return;
		}
		else
		{
			m_CharSelect[iPlayerIndex].pMyPlayerData = &m_pGameData->PlayerInfo[iPlayerIndex];
			m_CharSelect[iPlayerIndex].pCurrentPlayerSelect = &m_pGameData->PlayerInfo[iPlayerIndex];
			m_CharSelect[iPlayerIndex].iIndex = iPlayerIndex;

			//set up the player
			m_CharSelect[iPlayerIndex].pMyPlayerData->bIsActive = true;

			//set up the players controller
			if (CInput::GetInstance()->m_iNumJoysticks > 0)
			{
				//Activate the player's controller
				m_CharSelect[iPlayerIndex].pMyPlayerData->iInputType = (iPlayerIndex + Joystick1_input);
			}
			else
			{
				//One player on the keyboard
				m_CharSelect[iPlayerIndex].pMyPlayerData->iInputType = Keyboard_input;
			}

			//check if anyone else is pointing at this playerdata
			GoHome(iPlayerIndex);

			return;
		}
	}

	switch (iInputValue)
	{
		case Left:
		{	
			//find a player data to the left that is turned off

			//while it doesn't point to my player
			do
			{	
				//check if pointing at last index
				if (m_CharSelect[iPlayerIndex].pCurrentPlayerSelect == &m_pGameData->PlayerInfo[0])
				{
					//point at first index
					m_CharSelect[iPlayerIndex].pCurrentPlayerSelect = &m_pGameData->PlayerInfo[3];
					m_CharSelect[iPlayerIndex].iIndex = 3;
				}
				else
				{
					//increment to the next index
					m_CharSelect[iPlayerIndex].iIndex--;
					m_CharSelect[iPlayerIndex].pCurrentPlayerSelect = &m_pGameData->PlayerInfo[m_CharSelect[iPlayerIndex].iIndex];
				}

				//else if the current index is already a player, continue
				if (m_CharSelect[iPlayerIndex].pCurrentPlayerSelect->bIsActive &&
					m_CharSelect[iPlayerIndex].pCurrentPlayerSelect->iInputType != AI_input)
				{
					continue;
				}

				//if this is the players index, quit
				if (m_CharSelect[iPlayerIndex].pCurrentPlayerSelect == m_CharSelect[iPlayerIndex].pMyPlayerData ||
					m_CharSelect[iPlayerIndex].pCurrentPlayerSelect->bIsActive == false ||
					(m_CharSelect[iPlayerIndex].pCurrentPlayerSelect->bIsActive &&
					m_CharSelect[iPlayerIndex].pCurrentPlayerSelect->iInputType == AI_input)
					)
				{
					//check if any other players are pointing at this player
					bool BadShits = false;
					for (int i = 0; i < 4; i++)
					{
						if (m_CharSelect[i].pCurrentPlayerSelect == m_CharSelect[iPlayerIndex].pCurrentPlayerSelect &&
							i != iPlayerIndex)
						{
							BadShits = true;
						}
					}
					if (BadShits)
					{
						continue;
					}
					else
					{
						break;
					}				}
				
			}	while (m_CharSelect[iPlayerIndex].pCurrentPlayerSelect != m_CharSelect[iPlayerIndex].pMyPlayerData);

		}
		break;

		case Right:
		{
			//find a player data to the right that is turned off

			//while it doesn't point to my player
			do
			{	
				//check if pointing at last index
				if (m_CharSelect[iPlayerIndex].pCurrentPlayerSelect == &m_pGameData->PlayerInfo[3])
				{
					//point at first index
					m_CharSelect[iPlayerIndex].pCurrentPlayerSelect = &m_pGameData->PlayerInfo[0];
					m_CharSelect[iPlayerIndex].iIndex = 0;
				}
				else
				{
					//increment to the next index
					m_CharSelect[iPlayerIndex].iIndex++;
					m_CharSelect[iPlayerIndex].pCurrentPlayerSelect = &m_pGameData->PlayerInfo[m_CharSelect[iPlayerIndex].iIndex];
				}

				//else if the current index is already a player, continue
				if (m_CharSelect[iPlayerIndex].pCurrentPlayerSelect->bIsActive &&
					m_CharSelect[iPlayerIndex].pCurrentPlayerSelect->iInputType != AI_input)
				{
					continue;
				}

				//if this is the players index, quit
				if (m_CharSelect[iPlayerIndex].pCurrentPlayerSelect == m_CharSelect[iPlayerIndex].pMyPlayerData ||
					m_CharSelect[iPlayerIndex].pCurrentPlayerSelect->bIsActive == false ||
					(m_CharSelect[iPlayerIndex].pCurrentPlayerSelect->bIsActive &&
					m_CharSelect[iPlayerIndex].pCurrentPlayerSelect->iInputType == AI_input)
					)
				{
					//check if any other players are pointing at this player
					bool BadShits = false;
					for (int i = 0; i < 4; i++)
					{
						if (m_CharSelect[i].pCurrentPlayerSelect == m_CharSelect[iPlayerIndex].pCurrentPlayerSelect &&
							i != iPlayerIndex)
						{
							BadShits = true;
						}
					}
					if (BadShits)
					{
						continue;
					}
					else
					{
						break;
					}
				}
				
			}	while (m_CharSelect[iPlayerIndex].pCurrentPlayerSelect != m_CharSelect[iPlayerIndex].pMyPlayerData);
		}
		break;

		case Up:
		{
			//It should never get this message
		}
		break;

		case Down:
		{
			//It should never get this message
		}
		break;

		case Attack:
		{
			//Either flip on the ai of the current player data, or switch characters
			
			//check if the player is already turned on
			if (!m_CharSelect[iPlayerIndex].pCurrentPlayerSelect->bIsActive)
			{
				//turn it on and set it to AI
				m_CharSelect[iPlayerIndex].pCurrentPlayerSelect->bIsActive = true;
				m_CharSelect[iPlayerIndex].pCurrentPlayerSelect->iInputType = AI_input;
				m_CharSelect[iPlayerIndex].pCurrentPlayerSelect->iCharacter = Josie_Player;

				GoHome(iPlayerIndex);
			}
			else
			{
				//else increment the player character
				if (m_CharSelect[iPlayerIndex].pCurrentPlayerSelect->iCharacter == CaptainSally_Player)
				{
					m_CharSelect[iPlayerIndex].pCurrentPlayerSelect->iCharacter = Josie_Player;
				}
				else
				{
					m_CharSelect[iPlayerIndex].pCurrentPlayerSelect->iCharacter = CaptainSally_Player;
				}
			}
		}
		break;

		case Jump:
		{
			//Either flip on the ai of the current player data, or switch characters
			
			//check if the player is already turned on
			if (!m_CharSelect[iPlayerIndex].pCurrentPlayerSelect->bIsActive)
			{
				//turn it on and set it to AI
				m_CharSelect[iPlayerIndex].pCurrentPlayerSelect->bIsActive = true;
				m_CharSelect[iPlayerIndex].pCurrentPlayerSelect->iInputType = AI_input;
				m_CharSelect[iPlayerIndex].pCurrentPlayerSelect->iCharacter = Josie_Player;

				GoHome(iPlayerIndex);
			}
			else
			{
				//else increment the player character
				if (m_CharSelect[iPlayerIndex].pCurrentPlayerSelect->iCharacter == CaptainSally_Player)
				{
					m_CharSelect[iPlayerIndex].pCurrentPlayerSelect->iCharacter = Josie_Player;
				}
				else
				{
					m_CharSelect[iPlayerIndex].pCurrentPlayerSelect->iCharacter = CaptainSally_Player;
				}
			}
		}
		break;
	}
}

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
void CCharSelectMenu::GoHome(int iIndex)
{
	//loop through players
	for (int i = 0; i < 4; i++)
	{
		if (m_CharSelect[i].pCurrentPlayerSelect == &m_pGameData->PlayerInfo[iIndex])
		{
			m_CharSelect[i].pCurrentPlayerSelect = m_CharSelect[i].pMyPlayerData;
			
			//find the players home
			for (int j = 0; j < 4; j++)
			{
				if (m_CharSelect[i].pMyPlayerData == &m_pGameData->PlayerInfo[j])
				{
					m_CharSelect[i].iIndex = j;
					break;
				}
			}
		}
	}
}