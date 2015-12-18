#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_
/*////////////////////////////////////////////////////////////////////////
//File:	GameManager.h
//
//Author:	dan Manning
//
//Creation Date:	8-19-4
//
//Purpose:	Class definition of CGameManager - manages the whole game
//
/////////////////////////////////////////////////////////////////////////*/
#include "..\\StateMachine\\State_Manager.h"
#include "..\\Menu\\Menu.h"
#include "..\\GameTimer\\StopWatch.h"

//lengths of various game types
#define TIMEATTACK_LENGTH 120.0f
#define SURVIVAL_LENGTH	3
#define KNOCKOUT_LENGTH 3

enum 
{
	Menu_Mode,
	Game_Mode,
	Pause_Mode,
	GameOver_Mode, 
	Num_GameStates
};

enum
{
	Done_GameAction,
	Pause_GameAction,
	Exit_GameAction,
	Num_GameActions
};

//Debug structure to display debugging information during debug mode
struct DebugStructure
{
	bool bCollisionData;
	bool FPS;
	bool AIStates;
	bool PlayerStates;
};

class CGameManager
{
	private:	

		//Windows variables, needed to initialize various modules
		HWND m_hWnd;
		HINSTANCE m_hInstance;

		//controls the states of the program
		CStateMachine m_GameStateMachine;

		//the only instance of the game manager
		static CGameManager *m_pInstance;

		/*////////////////////////////////////////////////////////////////////////
		//Function:	constructor
		//
		//Purpose:	Set up the game manager and all member variables
		//
		/////////////////////////////////////////////////////////////////////////*/
		CGameManager(void);

		CGameManager(const CGameManager &CGameManagerRef) : m_GameStateMachine(0,0,0) {}
		CGameManager &operator=(const CGameManager &CGameManagerRef) {}

	public:

		//resolution of the window
		int m_iWindowWidth;
		int m_iWindowHeight;
		bool m_bWindowedMode;

		//Arcade mode flag
		bool m_bArcadeMode;

		DebugStructure GameDebugInfo;

		//Used to count down time in time attack mode
		CStopWatch m_TimeAttackStopWatch;

		//pointer to all the game data
		GameDataT *m_pGameData;

		virtual ~CGameManager(void);

		/*////////////////////////////////////////////////////////////////////////
		//Function:	GetInstance
		//
		//Return:	poitner to the only instance of the game manager
		//
		/////////////////////////////////////////////////////////////////////////*/
		static CGameManager *GetInstance();

		/*////////////////////////////////////////////////////////////////////////
		//Function:	DeleteInstance
		//
		//Purpose:	Clean up singleton memory
		//
		/////////////////////////////////////////////////////////////////////////*/
		static void DeleteInstance();

		/*////////////////////////////////////////////////////////////////////////
		//Function:	InitProgram
		//
		//Purpose:	Initializes all systems needed to run menu mode
		//
		/////////////////////////////////////////////////////////////////////////*/
		void InitProgram(HWND hWnd, HINSTANCE hInstance, int ix, int iy, bool bWindowedMode, bool bArcadeMode);

		/*////////////////////////////////////////////////////////////////////////
		//Function:	InitRound
		//
		//Purpose:	Initializes all systems needed to run a round
		//
		/////////////////////////////////////////////////////////////////////////*/
		void InitRound();

		/*////////////////////////////////////////////////////////////////////////
		//Function:	Update
		//
		//Return:
		//	true:	exit program
		//
		//Purpose:	Called every frame to update all necessary modules
		//
		/////////////////////////////////////////////////////////////////////////*/
		bool Update();

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
		bool CheckForGameOver();

		/*////////////////////////////////////////////////////////////////////////
		//Function:	Pause
		//
		//Purpose:	This function will pause the game while the Game Manager is 
		//in Game mode.  This function will pause the Sound Module, pause the 
		//GameTimer, and changes the state of the Game Manager to Paused.
		//
		/////////////////////////////////////////////////////////////////////////*/
		void Pause();

		/*////////////////////////////////////////////////////////////////////////
		//Function:	UnPause
		//
		//Purpose:	This function will unpause the Game Manager when it is in 
		//paused mode.  This function will unpause the Sound Module, unpause the 
		//GameTimer, and changes the state of the GameManager to game mode.
		//
		/////////////////////////////////////////////////////////////////////////*/
		void UnPause();

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
		void RoundShutdown();

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
		void ProgramShutdown();

		/*////////////////////////////////////////////////////////////////////////
		//Function:	ResolutionToggle
		//
		//Purpose:	This function is called in the options menu.  It checks to see
		//what resoultion we are currently at, and changes it based upon that.
		//
		/////////////////////////////////////////////////////////////////////////*/
		void ResolutionToggle();

		/*////////////////////////////////////////////////////////////////////////
		//Function:	WindowToggle
		//
		//Purpose:	This function is called in the options menu.  It checks to see
		//what window mode we are in, and changes it based upon that.
		//
		/////////////////////////////////////////////////////////////////////////*/
		void WindowToggle();

	private:

		/*////////////////////////////////////////////////////////////////////////
		//Function:	RunMenu
		//
		//Purpose:	updates the game while in menu mode
		//
		/////////////////////////////////////////////////////////////////////////*/
		bool RunMenu();

		/*////////////////////////////////////////////////////////////////////////
		//Function:	RunGame
		//
		//Purpose:	updates the game while in game mode
		//
		/////////////////////////////////////////////////////////////////////////*/
		void RunGame();

		/*////////////////////////////////////////////////////////////////////////
		//Function:	RunPause
		//
		//Purpose:	updates the gaem while in pause mode
		//
		/////////////////////////////////////////////////////////////////////////*/
		bool RunPause();

		/*////////////////////////////////////////////////////////////////////////
		//Function:	RunGameOver
		//
		//Purpose:	updates the game while in GameOver mode
		//
		/////////////////////////////////////////////////////////////////////////*/
		bool RunGameOver();
};

#endif