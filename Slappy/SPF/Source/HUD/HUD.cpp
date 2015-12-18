////////////////////////////////////////////////////////
//
// Name: "HUD.cpp"
//
// Author: Daron Keeler (DK)
//
// Creation Date: August 22, 2004
//
// Purpose: The HUD interface
//
////////////////////////////////////////////////////////

#include "HUD.h"
#include "..\\MemManager\\MemManager.h"
#include "..\\ObjectManager\\ObjectManager.h"
#include "..\\GameTimer\\GameTimer.h"
#include "..\\Input\\SubModules\\Input.h"
#include "..\\BitmapManager\\BitmapManager.h"
#include "..\\Entities\\Player.h"
#include "..\\GameManager\\GameManager.h"
#include "..\\Renderer\\DisplayDonkey.h"
#include "..\\AI\\AIInterface.h"
#include "..\\Sound\\Sound.h"
#include "BmpFonts.h"

CHUD *CHUD::pInstance = 0;

CHUD *CHUD::GetInstance()
{
	// Make one if it hasnt been done.
	if(!pInstance)
		//    pInstance = new CInputInterface;
	{
		pInstance = (CHUD*)MemMalloc(sizeof(CHUD));
		pInstance->CHUD::CHUD();
	}

	// Return the address of the instance.
	return pInstance;
}

void CHUD::DeleteInstance()
{
	// delete the instance
	if(pInstance)
	{
		// delete pInstance;
		pInstance->CHUD::~CHUD();
		MemFree(pInstance);

		pInstance = NULL;
	}
}

void CHUD::GetData()
{
	for(int i = 0; i < CObjectManager::GetInstance()->m_iPlayerArraySize; i++)
	{
		if(CObjectManager::GetInstance()->m_PlayerArray[i])
		{
			HUDData.iScores[i] = CObjectManager::GetInstance()->m_PlayerArray[i]->m_iScore;
			HUDData.iDamage[i] = CObjectManager::GetInstance()->m_PlayerArray[i]->m_iDamage;     
		}
	}
	HUDData.GameTimer = GetGameTimer();
}

void CHUD::InitHUD()
{
	// init the logger
	m_LogMe.Init("InputLog.txt");

	for(int i = 0; i < HUD_MAXCHARS; i++)
	{
		HUDData.iScores[i] = -1;
		HUDData.iDamage[i] = -1;   
		HUDData.fXLoc[i] = 0.0f;
	}

	// Load the four portraits
	m_HUDBmps[Josie_Player] = CBitmapManager::GetInstance()->LoadImage("ResourceFiles\\textures\\JosiePortrait.tga", true);
	m_HUDBmps[CaptainSally_Player] = CBitmapManager::GetInstance()->LoadImage("ResourceFiles\\textures\\SallyPortrait.tga", true);
	m_HUDBmps[MissElizabeth_Player] = m_HUDBmps[CaptainSally_Player];
	m_HUDBmps[Sumi_Player] = m_HUDBmps[CaptainSally_Player];
	m_iPortraitBckCircle = CBitmapManager::GetInstance()->LoadImage("ResourceFiles\\textures\\portraitBckCircle.tga", true);

	float TempX = 1.0f/10.0f;
	TempX -= 0.02f;

	CBmpFonts::GetInstance()->InitFonts();

	bEnding = false;
	bQuit = false;

	m_iPlayerCount = 0;
	// Set hud data struct to nothing.
	for(int i = 0; i < CObjectManager::GetInstance()->m_iPlayerArraySize; i++)
	{
		if(CObjectManager::GetInstance()->m_PlayerArray[i])
		{
			HUDData.iScores[i] = CObjectManager::GetInstance()->m_PlayerArray[i]->m_iScore;
			HUDData.iDamage[i] = CObjectManager::GetInstance()->m_PlayerArray[i]->m_iDamage;
			m_iPlayerCount++;

			// load portrait in based on character type.
			HUDData.iPortrait[i] = m_HUDBmps[CObjectManager::GetInstance()->m_PlayerArray[i]->m_pPlayerData->iCharacter];
			// Set their X coords.
			HUDData.fXLoc[i] = TempX;
			TempX += 0.21f;
		}
	}

	HUDStop.Start(0.3f);
	HUDData.GameTimer = GetGameTimer();
	m_fStartTime =  TIMEATTACK_LENGTH;

	// Start FPS ccounter.
	m_fLast = GetGameTimer();		// The current time.
	// The number of frames per second for the most recent sample.
	m_iOutfps = 0;
	// Number of frames.
	m_iFps = 0;

	m_LogMe << "HUD Initialized";
}

void CHUD::UpdateHUD()
{
	GetData();

	// Draw portraits
	for(int i = 0; i < m_iPlayerCount; i++)
	{
		if(!CObjectManager::GetInstance()->m_PlayerArray[i])
			continue;

		// Draw the background circle and player stats in the color of the character.
		float *pfColor = CObjectManager::GetInstance()->m_PlayerArray[i]->m_pPlayerData->PaletteColor;
		CBmpFonts::GetInstance()->setFontColor(pfColor[0], pfColor[1], pfColor[2], 0.9f);
		glColor4f(pfColor[0], pfColor[1], pfColor[2], 0.7f);
		CBitmapManager::GetInstance()->BindTexture(m_iPortraitBckCircle);

		//temp variable to fix hud stretching
		float fHudFix = ((float)CGameManager::GetInstance()->m_iWindowHeight / (float)CGameManager::GetInstance()->m_iWindowWidth);

		// Used to find where the "cursor" is on the screen.
		float EndPt;

		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f((HUDData.fXLoc[i]), 0.82f);

		glTexCoord2f(0.0f, 1.0f);
		glVertex2f((HUDData.fXLoc[i]), 0.97f);

		glTexCoord2f(1.0f, 1.0f);
		glVertex2f((HUDData.fXLoc[i] + (fHudFix * .15f)), 0.97f);

		glTexCoord2f(1.0f, 0.0f);
		glVertex2f((HUDData.fXLoc[i] + (fHudFix * .15f)), 0.82f);
		glEnd(); 

		// Just put together for now, will be changed when real fonts are located.
		//Score
		EndPt = CBmpFonts::GetInstance()->WriteWithFonts("X", HUDData.fXLoc[i]+0.11f, 0.87f, 0);
		CBmpFonts::GetInstance()->WriteJustInts(HUDData.iScores[i], EndPt, 0.86f);

		//Damage
		EndPt = CBmpFonts::GetInstance()->WriteJustInts(HUDData.iDamage[i], HUDData.fXLoc[i]+0.11f, 0.9f, 2);
		CBmpFonts::GetInstance()->WriteWithFonts("%", EndPt, 0.9f, 1);

		// Back to white and opaque.
		CBmpFonts::GetInstance()->setFontColor(1.0f, 1.0f, 1.0f);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		// Draw the portrait.
		CBitmapManager::GetInstance()->BindTexture(HUDData.iPortrait[i]);
		glBegin(GL_QUADS);

		glTexCoord2f(0.0f, 0.0f);
		glVertex2f((HUDData.fXLoc[i]), 0.8f);

		glTexCoord2f(0.0f, 1.0f);
		glVertex2f((HUDData.fXLoc[i]), 0.95f);

		glTexCoord2f(1.0f, 1.0f);
		glVertex2f((HUDData.fXLoc[i] + (fHudFix * .15f)), 0.95f);

		glTexCoord2f(1.0f, 0.0f);
		glVertex2f((HUDData.fXLoc[i] + (fHudFix * .15f)), 0.8f);

		glEnd(); 
	}

	// DEBUG TEST ONLY
	if(CInput::GetInstance()->GetBuffKey(DIK_T))
	{
		m_LogMe << "\nTimer - " << (int)HUDData.GameTimer << '\n';

		for(int i = 0; i < m_iPlayerCount; i++)
		{
			m_LogMe << "Score - " << HUDData.iScores[i] << ", Damage - " << HUDData.iDamage[i] << '\n';
		}
	}

	// Crappy FPS counter.		
	m_iFps++;	// Another frame has been processed.

	// Sample the frame rate every 1/4 second.
	if( (HUDData.GameTimer - m_fLast) > 0.25f )
	{
		// Adjust for the sampling rate.
		m_iOutfps = m_iFps*4;

		// Start over.
		m_iFps = 0;

		// Start over.
		m_fLast = HUDData.GameTimer;
	}

	// Shall we draw the time?
	// Print out the time left.
	m_fStartTime = TIMEATTACK_LENGTH-HUDData.GameTimer;

	if(CGameManager::GetInstance()->m_pGameData->iGameType == TIMEATTACK)
	{
		// Get the number of seconds.
		int iSeconds = int(m_fStartTime)%60;
		int iMinutes = int(m_fStartTime/60.0f);

		if (iSeconds <= 0 && iMinutes <= 0)
		{
			iSeconds = 0;
			iMinutes = 0;
		}

		//change the color
		if (iMinutes <= 0 && iSeconds <= 10)
		{
			CBmpFonts::GetInstance()->setFontColor(1.0f, 0.0f, 0.0f);
		}
		else
		{
			CBmpFonts::GetInstance()->setFontColor(1.0f, 1.0f, 1.0f);
		}

		// Write the minutes.
		
		float EndPt = 0;
		if(iSeconds < 5 && iMinutes == 0)
			EndPt = CBmpFonts::GetInstance()->WriteJustInts(iMinutes, ((1.0f-HUD_FONTWIDTH*7.0f)/2.0f), 0.0f,2);
		else
			EndPt = CBmpFonts::GetInstance()->WriteJustInts(iMinutes, ((1.0f-HUD_FONTWIDTH*4.0f)/2.0f), 0.0f,2);

		EndPt = CBmpFonts::GetInstance()->WriteWithFonts(":", EndPt, 0.0f, 2);


		// Write the seconds.
		if(iSeconds < 10)
		{
			// Write a leading zero if the time is below 10 seconds...
			EndPt = CBmpFonts::GetInstance()->WriteJustInts(0, EndPt, 0.0f, 2);
			EndPt = CBmpFonts::GetInstance()->WriteJustInts(iSeconds, EndPt, 0.0f, 2);
		}
		else
			EndPt = CBmpFonts::GetInstance()->WriteJustInts(iSeconds, EndPt, 0.0f, 2);	// Draw the seconds.

		// Draw milliseconds if the time is below 5 seconds.
		if(iSeconds < 5 && iMinutes == 0)
		{
			//iSeconds = int(m_fStartTime)%60;
            int fMilliseconds = int((m_fStartTime - (float)iSeconds)*100);
			if (fMilliseconds <= 0)
			{
				fMilliseconds = 0;
			}

			EndPt = CBmpFonts::GetInstance()->WriteWithFonts(":", EndPt, 0.0f, 2);
			if(fMilliseconds < 10)
			{
				// Write a leading zero if the time is below 10 seconds...
				EndPt = CBmpFonts::GetInstance()->WriteJustInts(0, EndPt, 0.0f, 2);
				EndPt = CBmpFonts::GetInstance()->WriteJustInts(fMilliseconds, EndPt, 0.0f, 2);
			}
			else
				EndPt = CBmpFonts::GetInstance()->WriteJustInts(fMilliseconds, EndPt, 0.0f, 2);	// Draw the milliseconds.
   		}
		CBmpFonts::GetInstance()->setFontColor(1.0f, 1.0f, 1.0f);
	}

	float iPlayerPos;

	// AI DEBUG INFO
	if(CGameManager::GetInstance()->GameDebugInfo.AIStates)
	{
		for(int i = 0; i < CObjectManager::GetInstance()->m_iPlayerArraySize; i++)
		{
			//WriteJustInts(i, HUDData.fXLoc[i], 0.7, 1);

			switch(AIInterface::GetInstance()->GetBotGoal(i))
			{
				// nothing
			case -1:  
				CBmpFonts::GetInstance()->WriteWithFonts("AI: NONE", HUDData.fXLoc[i], 0.75, 1);
				break;

			case AI_FINDGOAL:
				CBmpFonts::GetInstance()->WriteWithFonts("AI: Find", HUDData.fXLoc[i], 0.75, 1);
				break;

			case AI_ATTACKPLAYER:
				CBmpFonts::GetInstance()->WriteWithFonts("AI: Attack", HUDData.fXLoc[i], 0.75, 1);
				break;

			case AI_GETITEM:
				CBmpFonts::GetInstance()->WriteWithFonts("AI: Item", HUDData.fXLoc[i], 0.75, 1);
				break;

			case AI_RECOVER:
				CBmpFonts::GetInstance()->WriteWithFonts("AI: Recover", HUDData.fXLoc[i], 0.75, 1);
				break;

			};

			// Write player states.
			if(!CObjectManager::GetInstance()->m_PlayerArray[i])
				continue;
			iPlayerPos = PLAYER_STATE_DEBUG_POS + ((i%2)*0.1f);
			switch(CObjectManager::GetInstance()->m_PlayerArray[i]->GetState())
			{
			case Standing_state:
				CBmpFonts::GetInstance()->WriteWithFonts("PL: Standing", HUDData.fXLoc[i], iPlayerPos, PLAYER_STATE_DEBUG_FONT_SIZE);
				break;

			case Walking_state:
				CBmpFonts::GetInstance()->WriteWithFonts("PL: Walking_state", HUDData.fXLoc[i], iPlayerPos, PLAYER_STATE_DEBUG_FONT_SIZE);
				break;

			case Jumping_state:
				CBmpFonts::GetInstance()->WriteWithFonts("PL: Jumping_state", HUDData.fXLoc[i], iPlayerPos, PLAYER_STATE_DEBUG_FONT_SIZE);
				break;

			case DoubleJumping_state:
				CBmpFonts::GetInstance()->WriteWithFonts("PL: DoubleJumping_state", HUDData.fXLoc[i], iPlayerPos, PLAYER_STATE_DEBUG_FONT_SIZE);
				break;

			case Attack_state:
				CBmpFonts::GetInstance()->WriteWithFonts("PL: Attack_state", HUDData.fXLoc[i], iPlayerPos, PLAYER_STATE_DEBUG_FONT_SIZE);
				break;

			case ForwardAttack_state:
				CBmpFonts::GetInstance()->WriteWithFonts("PL: ForwardAttack_state", HUDData.fXLoc[i], iPlayerPos, PLAYER_STATE_DEBUG_FONT_SIZE);
				break;

			case SpecialAttack_state:
				CBmpFonts::GetInstance()->WriteWithFonts("PL: SpecialAttack_state", HUDData.fXLoc[i], iPlayerPos, PLAYER_STATE_DEBUG_FONT_SIZE);
				break;

			case UpAttack_state:
				CBmpFonts::GetInstance()->WriteWithFonts("PL: UpAttack_state", HUDData.fXLoc[i], iPlayerPos, PLAYER_STATE_DEBUG_FONT_SIZE);
				break;

			case AirSpecialAttack_state:
				CBmpFonts::GetInstance()->WriteWithFonts("PL: AirSpecialAttack_state", HUDData.fXLoc[i], iPlayerPos, PLAYER_STATE_DEBUG_FONT_SIZE);
				break;

			case AirDownAttack_state:
				CBmpFonts::GetInstance()->WriteWithFonts("PL: AirDownAttack_state", HUDData.fXLoc[i], iPlayerPos, PLAYER_STATE_DEBUG_FONT_SIZE);
				break;

			case AirForwardAttack_state:
				CBmpFonts::GetInstance()->WriteWithFonts("PL: AirForwardAttack_state", HUDData.fXLoc[i], iPlayerPos, PLAYER_STATE_DEBUG_FONT_SIZE);
				break;

			case Throwing_state:
				CBmpFonts::GetInstance()->WriteWithFonts("PL: Throwing_state", HUDData.fXLoc[i], iPlayerPos, PLAYER_STATE_DEBUG_FONT_SIZE);
				break;

			case Blocking_state:
				CBmpFonts::GetInstance()->WriteWithFonts("PL: Blocking_state", HUDData.fXLoc[i], iPlayerPos, PLAYER_STATE_DEBUG_FONT_SIZE);
				break;

			case Falling_state:
				CBmpFonts::GetInstance()->WriteWithFonts("PL: Falling_state", HUDData.fXLoc[i], iPlayerPos, PLAYER_STATE_DEBUG_FONT_SIZE);
				break;

			case Dead_state:
				CBmpFonts::GetInstance()->WriteWithFonts("PL: Dead_state", HUDData.fXLoc[i], iPlayerPos, PLAYER_STATE_DEBUG_FONT_SIZE);
				break;

			case PhysicsUp_state:
				CBmpFonts::GetInstance()->WriteWithFonts("PL: PhysicsUp_state", HUDData.fXLoc[i], iPlayerPos, PLAYER_STATE_DEBUG_FONT_SIZE);
				break;

			case PhysicsLeft_state:
				CBmpFonts::GetInstance()->WriteWithFonts("PL: PhysicsLeft_state", HUDData.fXLoc[i], iPlayerPos, PLAYER_STATE_DEBUG_FONT_SIZE);
				break;

			case PhysicsRight_state:
				CBmpFonts::GetInstance()->WriteWithFonts("PL: PhysicsRight_state", HUDData.fXLoc[i], iPlayerPos, PLAYER_STATE_DEBUG_FONT_SIZE);
				break;

			case AttackTransition_state:
				CBmpFonts::GetInstance()->WriteWithFonts("PL: AttackTransition_state", HUDData.fXLoc[i], iPlayerPos, PLAYER_STATE_DEBUG_FONT_SIZE);
				break;

			};
		}
	}


	// Print out the current frame rate.
	static char strOut[25];
	if(CGameManager::GetInstance()->GameDebugInfo.FPS)
	{
		sprintf(strOut, "FPS: %i", m_iOutfps);
		CBmpFonts::GetInstance()->WriteWithFonts(strOut,0.0f, 0.0f);
		sprintf(strOut, "Polys: %i", CObjectManager::GetInstance()->m_iNumVerts);
		CBmpFonts::GetInstance()->WriteWithFonts(strOut,0.0f, 0.03f);
		//	sprintf(strOut, "Parts: %i", NUM_PARTICLES);
		//	CHUD::GetInstance()->WriteWithFonts(strOut,0.0f, 0.06f);
	}
}

bool CHUD::UpdateEndHUD()
{
	
	// Start the shutdown HUD
	if(!bEnding)
	{		
		int iTemp = -1;
		// Find the winner.
		for(int i = 0; i < CObjectManager::GetInstance()->m_iPlayerArraySize; i++)
		{
			// See if there was a tie
			if(CObjectManager::GetInstance()->m_PlayerArray[i]->m_iScore == iTemp)
				bTie = true;

			// See if this guy was better than the last
			if(CObjectManager::GetInstance()->m_PlayerArray[i]->m_iScore > iTemp)
			{
				iTemp = CObjectManager::GetInstance()->m_PlayerArray[i]->m_iScore;
				m_iWinner = i;
				bTie = false;
			}

		}

		bEnding = true;

		// Call Kevin's End function
		if(!bTie)
		{
			CDisplayDonkey::GetInstance()->DrawEnding(m_iWinner);
			CSound::GetInstance()->PlaySoundEffect((CObjectManager::GetInstance()->m_PlayerArray[m_iWinner]->m_iUniqueSounds[VICTORY_SOUND]));
		}
	}

	if(bTie)
		CBmpFonts::GetInstance()->WriteWithFonts("ONLY A DRAW", 0.34f, 0.1f, 2);
	else
	{
		float fXLoc = 0.30f;
		// Draw the You win picture and zoom it in.
		fXLoc = CBmpFonts::GetInstance()->WriteWithFonts("Player ", fXLoc, 0.1f, 2);
		fXLoc = CBmpFonts::GetInstance()->WriteJustInts(m_iWinner+1, fXLoc, 0.1f, 2);
		fXLoc = CBmpFonts::GetInstance()->WriteWithFonts(" Wins", fXLoc, 0.1f, 2);
	}
	
	CBmpFonts::GetInstance()->WriteWithFonts("Press Attack To Continue", 0.29f, 0.7f, 1);
	
	UpdateHUD();

	// This will happen for 2 seconds and then CHECK INPUT to return to the main menu.
		// check the input to leave.
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

			if (CInput::GetInstance()->GetBuffKey(DIK_LSHIFT))
			{
				return true;
			}

			if (CInput::GetInstance()->GetBuffKey(DIK_W))
			{
				return true;
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
						//check the attack button
						if (CInput::GetInstance()->JoyPush(1, i))
						{
							return true;
						}
					}
				}
			}
			else
			{
				if (CInput::GetInstance()->GetBuffKey(DIK_A))
				{
					return true;
				}
			}
		}
	
	
		return false;
	
}

bool CHUD::UpdatePausedHUD()
{
	// TODO: Check the input to follow through a bool.

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
			if (CInput::GetInstance()->GetBuffKey(DIK_NUMPAD8))
			{
				bQuit = !bQuit;
			}
			if (CInput::GetInstance()->GetBuffKey(DIK_NUMPAD2))
			{
				bQuit = !bQuit;
			}

			if (CInput::GetInstance()->GetBuffKey(DIK_LSHIFT))
			{
				return true;
			}
		

			//check player 2's inputs
			if (CInput::GetInstance()->GetBuffKey(DIK_R))
			{
				bQuit = !bQuit;
			}
			if (CInput::GetInstance()->GetBuffKey(DIK_F))
			{
				bQuit = !bQuit;
			}
			
			if (CInput::GetInstance()->GetBuffKey(DIK_W))
			{
				return true;
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

						//check the up direction
						if (CInput::GetInstance()->GetJoyDir(i) == JOYSTICK_UP)
						{
							// Swap the bool
							bQuit = !bQuit;
						}

						//check the down direction
						if (CInput::GetInstance()->GetJoyDir(i) == JOYSTICK_DOWN)
						{
							bQuit = !bQuit;
						}

						//check the attack button
						if (CInput::GetInstance()->JoyPush(1, i))
						{
							return true;
						}
					}
				}
			}
			else
			{
				//check the keyboard
				if (CInput::GetInstance()->GetBuffKey(DIK_UP))
				{
					bQuit = !bQuit;
				}
				if (CInput::GetInstance()->GetBuffKey(DIK_DOWN))
				{
					bQuit = !bQuit;
				}
				if (CInput::GetInstance()->GetBuffKey(DIK_A) || CInput::GetInstance()->GetBuffKey(DIK_RETURN))
				{
					return true;
				}
			}
		
		}
	
	// To render the box, we need to draw the basic black box
	// Write continue and Quit with BmpFonts
	// Possibly change their colors based on which one is selected.
	
	if(bQuit)
		CBmpFonts::GetInstance()->setFontColor(1.f, 1.0f, 1.0f, 0.5f);
	else
		CBmpFonts::GetInstance()->setFontColor(1.0f, 1.0f, 1.0f);

	CBmpFonts::GetInstance()->WriteWithFonts("Continue", 0.45f, 0.45f, 2);

	if(bQuit)
		CBmpFonts::GetInstance()->setFontColor(1.0f, 1.0f, 1.0f);
	else
		CBmpFonts::GetInstance()->setFontColor(1.0f, 1.0f, 1.0f, 0.5f);

	CBmpFonts::GetInstance()->WriteWithFonts("Exit", 0.45f, 0.5f, 2);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	
	return false;
}

void CHUD::ShutDownHUD()
{

	CBmpFonts::GetInstance()->ShutDownFonts();
	CBmpFonts::DeleteInstance();

	m_LogMe << "Shutdown HUD";
	m_LogMe.Shutdown();
}

