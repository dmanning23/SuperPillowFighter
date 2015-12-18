/*////////////////////////////////////////////////////////////////////////
//File:	GameManager.cpp
//
//Author:	Dan Manning
//
//Creation Date:	8-19-4
//
//Purpose:	Function definitions of GameManager class
//
/////////////////////////////////////////////////////////////////////////*/
#include "gamemanager.h"
#include "..\MemManager\MemManager.h"
#include "..\objectmanager\objectmanager.h"
#include "..\objectmanager\objectfactory.h"
#include "..\Renderer\DisplayDonkey.h"
#include "..\Input\InputInterface.h"
#include "..\GameTimer\GameTimer.h"
#include "..\LevelManager\LevelManager.h"
#include "..\Physics\PhysicsModuleInterface.h"
#include "..\Hud\hud.h"
#include "..\Profiler\Profiler.h"
#include "..\Sound\Sound.h"
#include "..\AI\AIInterface.h"
#include "..\Animation\Animule.h"
#include "..\Physics\EventHandler.h"
#include "..\Physics\CollisionDetector.h"

//the only instance of the game manager
CGameManager *CGameManager::m_pInstance = 0;

extern int g_iWindowWidth;
extern int g_iWindowHeight;

/*////////////////////////////////////////////////////////////////////////
//Function:	constructor
//
//Purpose:	Set up the game manager and all member variables
//
/////////////////////////////////////////////////////////////////////////*/
CGameManager::CGameManager(void) : m_GameStateMachine(Num_GameStates, Num_GameActions, Menu_Mode)
{
	//set up state machine
	m_GameStateMachine.SetEntry(Menu_Mode, Done_GameAction, Game_Mode);
	m_GameStateMachine.SetEntry(Game_Mode, Done_GameAction, GameOver_Mode);
	m_GameStateMachine.SetEntry(Game_Mode, Pause_GameAction, Pause_Mode);
	m_GameStateMachine.SetEntry(Pause_Mode, Done_GameAction, Game_Mode);
	m_GameStateMachine.SetEntry(GameOver_Mode, Done_GameAction, Menu_Mode);

	m_pGameData = 0;

	memset(&GameDebugInfo, 0, sizeof(DebugStructure));
}

CGameManager::~CGameManager(void)
{
}

/*////////////////////////////////////////////////////////////////////////
//Function:	GetInstance
//
//Return:	poitner to the only instance of the game manager
//
/////////////////////////////////////////////////////////////////////////*/
CGameManager *CGameManager::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = (CGameManager*)MemMalloc(sizeof(CGameManager));
		m_pInstance->CGameManager::CGameManager();
	}
	return m_pInstance;
}

/*////////////////////////////////////////////////////////////////////////
//Function:	DeleteInstance
//
//Purpose:	Clean up singleton memory
//
/////////////////////////////////////////////////////////////////////////*/
void CGameManager::DeleteInstance()
{
	m_pInstance->CGameManager::~CGameManager();
	MemFree(m_pInstance);
	m_pInstance = 0;
}

/*////////////////////////////////////////////////////////////////////////
//Function:	InitProgram
//
//Purpose:	Initializes all systems needed to run menu mode
//
/////////////////////////////////////////////////////////////////////////*/
void CGameManager::InitProgram(HWND hWnd, HINSTANCE hInstance, int ix, int iy, bool bWindowedMode, bool bArcadeMode)
{
	//Initialize in this order:

	// Grab the window! - STRAND
	m_hWnd = hWnd;

	//grab resolution
	m_iWindowWidth = ix;
	m_iWindowHeight = iy;
	m_bWindowedMode = bWindowedMode;
	m_bArcadeMode = bArcadeMode;

	//Renderer
	//	Init in orthographic
	//CDisplayDonkey::GetInstance()->initialize(hWnd, ix, iy);
	//CDisplayDonkey::GetInstance()->setOrthographic(true);

	////BitmapManager
	//CBitmapManager::GetInstance()->Initialize();

	//Input
	CInputInterface::GetInstance()->InitInputInterface(hWnd, hInstance);

	//Sound
	//	Load sounds
	//	Load song
	CSound::GetInstance()->InitSound();

	//ParticleEngine
	//	Load particle textures
	CParticleEngine::GetInstance()->Init();
	CParticleEngine::GetInstance()->LoadTextures();

	//Menu
	//	Load menu textures
	CMenu::GetInstance()->InitMenu();

	//GameTimer
	//	Start game timer
	TimerStart();

}

/*////////////////////////////////////////////////////////////////////////
//Function:	InitRound
//
//Purpose:	Initializes all systems needed to run a round
//
/////////////////////////////////////////////////////////////////////////*/
void CGameManager::InitRound()
{
	//flush bitmaps
	CBitmapManager::GetInstance()->cleanup();

	//flush sounds
	CSound::GetInstance()->StopAllSounds();
	CSound::GetInstance()->ClearBuffers();

	//flush current particles
	CParticleEngine::GetInstance()->CleanUp();
	CParticleEngine::GetInstance()->LoadTextures();

	//load event sounds
	CEventHandler::GetInstance()->Init();

	//LevelManager
	//	intitialize
	//	load level
	CLevelManager::GetInstance()->LoadLevel(m_pGameData->iLevelChoice);

	//ObjectFactory
	//	initialize
	//	create players
	//	load items
	CObjectFactory::GetInstance()->LoadItems();
	for (int i = 0; i < MENU_MAXPLAYERS; i++)
	{
		if (m_pGameData->PlayerInfo[i].bIsActive)
		{
			CObjectFactory::GetInstance()->CreatePlayer(&(m_pGameData->PlayerInfo[i]));
		}
	}
	
	//ObjectManager
	//	initialize
	CObjectManager::GetInstance()->Init();

	//ObjectFactory
	//	shutdown
	CObjectFactory::DeleteInstance();

	//AI
	//	intialize
	AIInterface::GetInstance()->InitAI();

	//INPUT
	//	setup for game
	CInputInterface::GetInstance()->InitGameInput();

	//initialize profiler for a new game
	InitializeProfiler();

	

	//start animation engine
	CAniMule::GetInstance()->initialize();

	//Init Physics
	CPhysicsModule::GetInstance()->InitLevelData();

	//restart game timer
	TimerStart();

	//HUD
	//	initialize
	//	load hud textures
	CHUD::GetInstance()->InitHUD();

	CDisplayDonkey::GetInstance()->newGame();

	//start the powerup timer
	CObjectManager::GetInstance()->SetPowerupTimer();

	//start stopwatch for time attack mode
	m_TimeAttackStopWatch.Start(TIMEATTACK_LENGTH);

	//start up background sounds
	CLevelManager::GetInstance()->StartAmbientSounds();
}

/*////////////////////////////////////////////////////////////////////////
//Function:	Update
//
//Purpose:	Called every frame to update all necessary modules
//
/////////////////////////////////////////////////////////////////////////*/
bool CGameManager::Update()
{
	switch (m_GameStateMachine.GetCurrentState())
	{
		case Menu_Mode:
			{
				if (RunMenu())
				{
					//time to exit the program
					return true;
				}

				//check for game start
				if (m_pGameData)
				{
					InitRound();
					m_GameStateMachine.SendStateMessage(Done_GameAction);
				}
				break;
			}
		case Game_Mode:
			{
				RunGame();
				if (CheckForGameOver())
				{
					CSound::GetInstance()->StopMusic();
					CSound::GetInstance()->LoadMusic("ResourceFiles\\Music\\credit.ogg");

					CInputInterface::GetInstance()->m_iTypeFlag = INPUT_MENUMODE;
				}
				break;
			}
		case Pause_Mode:
			{
				if (RunPause())
				{
					//check for unpause
					if (CHUD::GetInstance()->bQuit == true)
					{
						UnPause();
											
						m_GameStateMachine.SendStateMessage(Done_GameAction);
					}
					else 
                        UnPause();
				}
				break;
			}
		case GameOver_Mode:
			{
				if (RunGameOver())
				{
					//write gameover mode
					//go back to menu mode
					RoundShutdown();
					//switch to menu mode
					
					//restart the game timer
					TimerStart();

					CMenu::GetInstance()->InitMenu();
					//m_GameStateMachine.SendStateMessage(Done_GameAction);
					//MemFree(m_pGameData);
					//m_pGameData = 0;
					m_GameStateMachine.SendStateMessage(Done_GameAction);

					
				}
				break;
			}
	}
	return false;
}

/*////////////////////////////////////////////////////////////////////////
//Function:	CheckForGameOver
//
//Return:	true:	game is over
//			false:	game is not yet over
//
//Purpose:	This function checks if the game is over.  If the game mode 
//is Knockout, it will check how many kills each player has.  If the game
//mode is Time Attack, it will check the GameTimer to see if the specified 
//amount of time has passed.  If the game mode is Survival, it will check 
//if there is only one player left alive.  This function will return true 
//if the game is over, and false if the game is still running.
//
/////////////////////////////////////////////////////////////////////////*/
bool CGameManager::CheckForGameOver()
{
	switch (m_pGameData->iGameType)
	{
		case TIMEATTACK:
			{
				//check if time attack stopwatch has expired
				if (m_TimeAttackStopWatch.GetRemainingTime() <= 0.0f)
				{
					m_GameStateMachine.SendStateMessage(Done_GameAction);
					return true;
				}
				break;
			}
		case KNOCKOUT:
			{
				//check if any players have... 10 points
				for (int i = 0; i < CObjectManager::GetInstance()->m_iPlayerArraySize; i++)
				{
					if (CObjectManager::GetInstance()->m_PlayerArray[i]->m_iScore >= KNOCKOUT_LENGTH)
					{
						m_GameStateMachine.SendStateMessage(Done_GameAction);
						return true;
					}
				}
				break;
			}
		//case SURVIVAL:
		//	{
		//		//check if only one player remains
		//		int iNumPlayers = 0;
		//		for (int i = 0; i < CObjectManager::GetInstance()->m_iPlayerArraySize; i++)
		//		{
		//			//check for dead players
		//			if (CObjectManager::GetInstance()->m_PlayerArray[i]->m_iScore < 0)
		//			{
		//				CObjectManager::GetInstance()->KillPlayer(CObjectManager::GetInstance()->m_PlayerArray[i]);
		//			}

		//			if (CObjectManager::GetInstance()->m_PlayerArray[i])
		//			{
		//				iNumPlayers++;
		//			}
		//		}

		//		if (iNumPlayers <= 1)
		//		{
		//			m_GameStateMachine.SendStateMessage(Done_GameAction);
		//			return true;
		//		}

		//		break;
		//	}
	}
	return false;
}

/*////////////////////////////////////////////////////////////////////////
//Function:	Pause
//
//Purpose:	This function will pause the game while the Game Manager is 
//in Game mode.  This function will pause the Sound Module, pause the 
//GameTimer, and changes the state of the Game Manager to Paused.
//
/////////////////////////////////////////////////////////////////////////*/
void CGameManager::Pause()
{
	//pause game timer
	TimerPause();

	//	pause all sounds
	CSound::GetInstance()->TogglePause(true);

	//pause game manager
	m_GameStateMachine.SendStateMessage(Pause_GameAction);
}

/*////////////////////////////////////////////////////////////////////////
//Function:	UnPause
//
//Purpose:	This function will unpause the Game Manager when it is in 
//paused mode.  This function will unpause the Sound Module, unpause the 
//GameTimer, and changes the state of the GameManager to game mode.
//
/////////////////////////////////////////////////////////////////////////*/
void CGameManager::UnPause()
{
	//	unpause all sounds
	CSound::GetInstance()->TogglePause(false);

	//unpause game timer
	TimerUnpause();

	//unpause game manager
	m_GameStateMachine.SendStateMessage(Done_GameAction);
}

/*////////////////////////////////////////////////////////////////////////
//Function:	RoundShutdown
//
//Purpose:	This function is called when a round is over and after the 
//GameOver game mode is done.  After this function is done, the Game Manager 
//goes back into Menu Mode.  This function does the following things: unload 
//level(shutdown Level Object), shutdown Object Manager, shutdown Animation, 
//shutdown Physics, shutdown HUD, cleanup BitmapMananger, cleanup Game Manager, 
//cleanup Particle Engine, cleanup Sound, and switched to MenuMode.  
//Cleaning up the Game Manager includes zeroing out the memory of the GameData.
//
/////////////////////////////////////////////////////////////////////////*/
void CGameManager::RoundShutdown()
{
	//dump profiler info
	ProfilerDump();

	//LEVELMANAGER
	//	shutdown
	CLevelManager::GetInstance()->UnLoadLevel();
	CLevelManager::DeleteInstance();

	//CHUD
	//shutdown
	CHUD::GetInstance()->ShutDownHUD();
	CHUD::DeleteInstance();

	//ANIMATION
	//	shutdown
	CAniMule::GetInstance()->DeleteInstance();

	//AI
	//	shutdown
	AIInterface::GetInstance()->ShutDownAI();
	AIInterface::DeleteInstance();

	//PHYSICS
	//	shutdown
	//CPhysicsModule::DeleteInst();
	//CCollisionDetector::DeleteInst();

	//BITMAPMANAGER
	//	cleanup
	CBitmapManager::GetInstance()->cleanup();

	//GAMEMANAGER
	//free memory for game data structure
	MemFree(m_pGameData);
	m_pGameData = 0;

	//INPUT RESET
	CInputInterface::GetInstance()->ResetGameInput();

	//PARTICLE ENGINE
	//	cleanup
	CParticleEngine::GetInstance()->CleanUp();
	CParticleEngine::GetInstance()->LoadTextures();

	//PHYSICS
	CPhysicsModule::GetInstance()->KillLevelData();
	CPhysicsModule::DeleteInst();
	CEventHandler::DeleteInstance();
	CCollisionDetector::DeleteInst();

	//Sound
	//	cleanup
	CSound::GetInstance()->StopAllSounds();
	CSound::GetInstance()->ClearBuffers();
	CSound::DeleteInstance();
	CSound::GetInstance()->InitSound();

	//OBJECTMANAGER
	//	shutdown
	CObjectManager::GetInstance()->Shutdown();
	CObjectManager::DeleteInstance();
}

/*////////////////////////////////////////////////////////////////////////
//Function:	GameShutdown
//
//Purpose:	This function is called before the game exits back to Windows.  
//This function will do the following things:  shutdown sound, shutdown 
//Particle Engine, shutdown Bitmap Manager, shutdown Input, shutdown AI, 
//and shutdown Menus.  This function will only be called while the game 
//is in Menu Mode.   If the Windows message WM_DESTROY is passed through 
//winproc, RoundShutdown will be called, and then this function.  The Game 
//Manager will need to be shutdown after this function exits. 
//
/////////////////////////////////////////////////////////////////////////*/
void CGameManager::ProgramShutdown()
{
	//make sure everything else is shutdown
	//RoundShutdown();

	//Shutdown Menus
	CMenu::GetInstance()->ShutdownMenu();
	CMenu::DeleteInstance();

	//Sound
	//	shutdown
	CSound::DeleteInstance();

	//PARTICLE ENGINE
	CParticleEngine::GetInstance()->CleanUp();
	CParticleEngine::DeleteInstance();

	//INPUT
	//	shutdown
	CInputInterface::GetInstance()->ShutDownInputInterface();
	CInputInterface::DeleteInstance();

	//BITMAP MANAGER
	//	shutdown
	CBitmapManager::GetInstance()->Shutdown();
	CBitmapManager::DeleteInstance();

	//RENDERER
	//	shutdown
	CDisplayDonkey::GetInstance()->shutdown();
	CDisplayDonkey::DeleteInstance();
}

/*////////////////////////////////////////////////////////////////////////
//Function:	ResolutionToggle
//
//Purpose:	This function is called in the options menu.  It checks to see
//what resoultion we are currently at, and changes it based upon that.
//
/////////////////////////////////////////////////////////////////////////*/
void CGameManager::ResolutionToggle()
{
	int iWidth, iHeight, iDiffW, iDiffH;

	// Are we in 800x600?  Yes if the window is smaller or equal to 800 pixels.
	// If we're in 800x600 then get the window size difference for 800x600
	// but set the new height and width to 1024x768.
	// Otherwise, do the opposite.
	if(800 - m_iWindowWidth >= 0)
	{
		iWidth = 1024;
		iHeight = 768;

		iDiffW = 800 - m_iWindowWidth;
		iDiffH = 600 - m_iWindowHeight;
	}
	else
	{
		iWidth = 800;
		iHeight = 600;

		iDiffW = 1024 - m_iWindowWidth;
		iDiffH = 768 - m_iWindowHeight;
	}

	// Set the information here to reflect what is actually there.
	m_iWindowWidth = iWidth - iDiffW;
	m_iWindowHeight = iHeight - iDiffH;

	// Set the info that restricts resizing.
	g_iWindowWidth = iWidth;
	g_iWindowHeight = iHeight;
	
	// Change what the Donkey is drawing to.
	CDisplayDonkey::GetInstance()->changeResolution(m_iWindowWidth, m_iWindowHeight);

	// Change the size of the window.
	SetWindowPos(m_hWnd, HWND_TOP, 0, 0, iWidth, iHeight, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
}

/*////////////////////////////////////////////////////////////////////////
//Function:	WindowToggle
//
//Purpose:	This function is called in the options menu.  It checks to see
//what window mode we are in, and changes it based upon that.
//
/////////////////////////////////////////////////////////////////////////*/
void CGameManager::WindowToggle()
{
	// Set the new window attributes based upon what mode we're in.
	if(m_bWindowedMode)
	{
		SetWindowLong(m_hWnd, GWL_STYLE, WS_POPUP | WS_SYSMENU | WS_VISIBLE);
		ShowCursor(FALSE);

		HWND hDesktopWnd;
		HDC hDesktopDC;
		hDesktopWnd = GetDesktopWindow();

		//get handle to desktop window
		hDesktopDC = GetDC(hDesktopWnd);

		//Get screen size of desktop window
		g_iWindowWidth = GetDeviceCaps(hDesktopDC, HORZRES);
		g_iWindowHeight = GetDeviceCaps(hDesktopDC, VERTRES);

		//release desktop device context
		ReleaseDC(hDesktopWnd, hDesktopDC);

		// Apply the changes!
		SetWindowPos(m_hWnd, HWND_TOP, 0, 0, g_iWindowWidth, g_iWindowHeight, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
		SetWindowPos(m_hWnd, HWND_TOP, 0, 0, g_iWindowWidth, g_iWindowHeight, SWP_NOZORDER | SWP_FRAMECHANGED);

		// Change what the Donkey is drawing to.
		CDisplayDonkey::GetInstance()->changeResolution(g_iWindowWidth, g_iWindowHeight);
	}
	else
	{
		SetWindowLong(m_hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
		ShowCursor(TRUE);

		// Change the global resize restrictions.
		g_iWindowWidth = m_iWindowWidth + 8;
		g_iWindowHeight = m_iWindowHeight + 27;

		// Apply the changes!
		SetWindowPos(m_hWnd, HWND_TOP, (GetSystemMetrics(SM_CXSCREEN)>>1) - (g_iWindowWidth>>1),	//	Window Start Point (x, y). 
										(GetSystemMetrics(SM_CYSCREEN)>>1) - (g_iWindowHeight>>1), g_iWindowWidth, g_iWindowHeight, SWP_NOZORDER | SWP_FRAMECHANGED);

		// Change what the Donkey is drawing to.
		CDisplayDonkey::GetInstance()->changeResolution(m_iWindowWidth, m_iWindowHeight);
	}	

	// Toggle the window mode bool.
	m_bWindowedMode = !m_bWindowedMode;
}

/*////////////////////////////////////////////////////////////////////////
//Function:	RunMenu
//
//Purpose:	updates the game while in menu mode
//
/////////////////////////////////////////////////////////////////////////*/
bool CGameManager::RunMenu()
{
	//update time
	TimerUpdate();

	//INPUT
	//	update
	CInputInterface::GetInstance()->UpdateInput();

	//MENU
	//update menus - check for return value to exit program
	if (CMenu::GetInstance()->UpdateMenu())
	{
		m_pGameData = CMenu::GetInstance()->ShutdownMenu();
		//DEBUGGING
		//m_pGameData = (GameDataT*)MemMalloc(sizeof(GameDataT));
		//memset(m_pGameData, 0, sizeof(GameDataT));
		//m_pGameData->iGameType = KNOCKOUT;
		//if (m_pGameData)
		//{
		//	m_pGameData->iLevelChoice = FERRIS_WHEEL_LEVEL;
		//}

		////set up player 1
		//m_pGameData->PlayerInfo[0].iCharacter = Josie_Player;
		//m_pGameData->PlayerInfo[0].iInputType =  1;
		//m_pGameData->PlayerInfo[0].bIsActive = true;
		//m_pGameData->PlayerInfo[0].PaletteColor[0] = 0.5f;
		//m_pGameData->PlayerInfo[0].PaletteColor[1] = 0.5f;
		//m_pGameData->PlayerInfo[0].PaletteColor[2] = 1.0f;

 
		////set up player 2
		//m_pGameData->PlayerInfo[1].iCharacter = CaptainSally_Player;
		//m_pGameData->PlayerInfo[1].iInputType = AI_input;
		//m_pGameData->PlayerInfo[1].bIsActive = true;
		//m_pGameData->PlayerInfo[1].PaletteColor[0] = 1.0f;
		//m_pGameData->PlayerInfo[1].PaletteColor[1] = 0.5f;
		//m_pGameData->PlayerInfo[1].PaletteColor[2] = 0.5f;


		////	set up player 3
		//m_pGameData->PlayerInfo[2].iCharacter = MissElizabeth_Player;
		//m_pGameData->PlayerInfo[2].iInputType = AI_input;
		//m_pGameData->PlayerInfo[2].bIsActive = true;
		//m_pGameData->PlayerInfo[2].PaletteColor[0] = 0.5f;
		//m_pGameData->PlayerInfo[2].PaletteColor[1] = 1.0f;
		//m_pGameData->PlayerInfo[2].PaletteColor[2] = 0.5f;

		////	set up player 4
		//m_pGameData->PlayerInfo[3].iCharacter = Sumi_Player;
		//m_pGameData->PlayerInfo[3].iInputType = AI_input;
		//m_pGameData->PlayerInfo[3].bIsActive = true;
		//m_pGameData->PlayerInfo[3].PaletteColor[0] = 1.0f;
		//m_pGameData->PlayerInfo[3].PaletteColor[1] = 1.0f;
		//m_pGameData->PlayerInfo[3].PaletteColor[2] = 0.0f;

		CInputInterface::GetInstance()->m_iTypeFlag = INPUT_GAMEMODE;

		if (!m_pGameData)
		{
			//time to exit the program
			return true;
		}
		else
		{
			//TODO
			//stop all the sounds from playing

			return false;
		}
	}

	//PARTICLE ENGINE
	//	update
	CParticleEngine::GetInstance()->Update();

	//RENDERER
	//	render menus
	//	render particles
	//?

	//Sound
	//	update
	CSound::GetInstance()->UpdateSound();

	CDisplayDonkey::GetInstance()->Render();
	return false;
}

/*////////////////////////////////////////////////////////////////////////
//Function:	RunGame
//
//Purpose:	updates the game while in game mode
//
/////////////////////////////////////////////////////////////////////////*/
void CGameManager::RunGame()
{
	
 
 
	//update time
	StartProfiler(TimerUpdate_profiler);
	TimerUpdate();
	//MemCheck();

	//Sound
	//	update
	StartProfiler(CSound_Update_profiler);
	CSound::GetInstance()->UpdateSound();
	//MemCheck();

	//INPUT
	//	update
	StartProfiler(CInputInterface_UpdateInput_profiler);
	CInputInterface::GetInstance()->UpdateInput();
	//MemCheck();

	//AI
	//	update
	StartProfiler(AI_Update_profiler);
	AIInterface::GetInstance()->UpdateAI();
	//MemCheck();

	//PHSYICS
	//	update
	StartProfiler(CPhysicsModule_UpdatePhysics_profiler);
	CPhysicsModule::GetInstance()->UpdatePhysics();
	//MemCheck();

	//OBJECTMANAGER
	//	update
	StartProfiler(CObjectManager_Update_profiler);
	CObjectManager::GetInstance()->Update();
	//MemCheck();

	//ANIMATION
	//	update
	StartProfiler(Animation_Update_profiler);
	CAniMule::GetInstance()->update();
	//MemCheck();

	//PARTICLE
	//	update
	StartProfiler(ParticleEngine_Update_profiler);
	CParticleEngine::GetInstance()->Update();
	//MemCheck();

	//RENDERER
	//	render world
	//	switch to orthographic mode
	CDisplayDonkey::GetInstance()->setOrthographic(false);

	StartProfiler(CDisplayDonkey_drawWorld_profiler);
	CDisplayDonkey::GetInstance()->drawWorld();

	CDisplayDonkey::GetInstance()->setOrthographic(true);
	//MemCheck();

	//HUD
	//	update
	//	render
	StartProfiler(CHUD_UpdateHUD_profiler);
	CHUD::GetInstance()->UpdateHUD();
	//MemCheck();

	StartProfiler(CDisplayDonkey_Render_profiler);
	CDisplayDonkey::GetInstance()->Render();
	//MemCheck();


	if (CInput::GetInstance()->GetBuffKey(DIK_ESCAPE))
	{
		if (m_GameStateMachine.GetCurrentState() == Game_Mode)
			Pause();
	}
}

/*////////////////////////////////////////////////////////////////////////
//Function:	RunPause
//
//Purpose:	updates the game while in pause mode
//
/////////////////////////////////////////////////////////////////////////*/
bool CGameManager::RunPause()
{
	CSound::GetInstance()->UpdateMusic();

	//INPUT
	//	update
	CInputInterface::GetInstance()->UpdateInput();

	//RENDERER
	//	draw world
	//	switch to orthographic
	CDisplayDonkey::GetInstance()->setOrthographic(false);
	CDisplayDonkey::GetInstance()->drawWorld();
	CDisplayDonkey::GetInstance()->setOrthographic(true);

	//HUD
	//	update
	//	render
	
	if (CHUD::GetInstance()->UpdatePausedHUD()) 
 		return true;

	
	CDisplayDonkey::GetInstance()->Render();
	return false;

}

/*////////////////////////////////////////////////////////////////////////
//Function:	RunGameOver
//
//Purpose:	updates the game while in GameOver mode
//
/////////////////////////////////////////////////////////////////////////*/
bool CGameManager::RunGameOver()
{
	//INPUT
	//	update
	CInputInterface::GetInstance()->UpdateInput();

	//update sound
	CSound::GetInstance()->UpdateSound();

	//RENDERER
	//	draw world
	//	switch to orthographic
	CDisplayDonkey::GetInstance()->setOrthographic(false);
	CDisplayDonkey::GetInstance()->drawWorld();
	CDisplayDonkey::GetInstance()->setOrthographic(true);

	//HUD
	//	update
	//	render
	if (CHUD::GetInstance()->UpdateEndHUD())
	{
		return true;
	}

	CDisplayDonkey::GetInstance()->Render();

	return false;
}