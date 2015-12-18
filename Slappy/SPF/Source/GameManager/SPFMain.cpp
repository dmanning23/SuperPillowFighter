//	WinMain.cpp : Basic Windows Shell.

#include <windows.h>						//	Needed for Windows Applications.
#include <ctime>
#include "GameManager.h"
#include "..\MemManager\MemManager.h"
#include "..\GameTimer\\GameTimer.h"
#include "..\Renderer\DisplayDonkey.h"
#include "..\BitmapManager\BitmapManager.h"
#include "..\GameTimer\StopWatch.h"
#include "..\Sound\Sound.h"

#define CLASSNAME "Super Pillow Fighter"				//	Window Class Name.
//#define WINDOW_WIDTH	640					//	Window Width.
//#define WINDOW_HEIGHT	480					//	Window Height.

HWND            main_hwnd;					//	The Main Window Handle.
int g_iWindowWidth;
int g_iWindowHeight;
bool g_bWindowedMode;
bool g_bArcadeMode;

//Function to load window data from a file
void LoadWindowData();

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//	This is the main message handler of the system

	//	What is the message 
	switch(msg)
	{
		case WM_CREATE: 
		{
			//	Do initialization stuff here
			return(0);
		}
		break;

		case WM_PAINT:
		{
			return(0);
		}
		break;

		//Grab the resize message
		case WM_GETMINMAXINFO:
		{
			((MINMAXINFO*) lparam)->ptMinTrackSize.y = g_iWindowHeight;
			((MINMAXINFO*) lparam)->ptMinTrackSize.x = g_iWindowWidth;
			((MINMAXINFO*) lparam)->ptMaxTrackSize.y = g_iWindowHeight;
			((MINMAXINFO*) lparam)->ptMaxTrackSize.x = g_iWindowWidth;
		}
		break;

		case WM_DESTROY: 
		{
			//	Kill the application			
			PostQuitMessage(0);
			return(0);
		}
		break;

		default:
		break;
	}

	//	Process any messages that we didn't take care of 
	return (DefWindowProc(hwnd, msg, wparam, lparam));
}

/***********/
/* WINMAIN */
/***********/

int WINAPI WinMain(	HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow)
{
	WNDCLASS winclass;		//	This will hold the class we create
	MSG		 msg;			//	Generic message

	LoadWindowData();

	//	First fill in the window class stucture
	winclass.style			= CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc	= WindowProc;
	winclass.cbClsExtra		= 0;
	winclass.cbWndExtra		= 0;
	winclass.hInstance		= hinstance;
	winclass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground	= (HBRUSH)NULL;
	winclass.lpszMenuName	= NULL; 
	winclass.lpszClassName	= CLASSNAME;

	//	Register the window class
	if (!RegisterClass(&winclass))
		return(0);

	if (g_bWindowedMode)
	{	
		//Windowed mode
		//	Create the window
		if (!(main_hwnd = CreateWindow(	CLASSNAME,											//	Class Name.
										"Super Pillow Fighter",										//	Title of the Window.
										WS_OVERLAPPEDWINDOW | WS_VISIBLE,					//	Window Flags.
										(GetSystemMetrics(SM_CXSCREEN)>>1) - (g_iWindowWidth>>1),	//	Window Start Point (x, y). 
										(GetSystemMetrics(SM_CYSCREEN)>>1) - (g_iWindowHeight>>1),
										g_iWindowWidth,										//	Width of Window.
										g_iWindowHeight,										//	Height of Window.
										NULL,												//	Handle to parent.
										NULL,												//	Handle to menu.
										hinstance,											//	Application Instance.
										NULL)))												//	Creation parms.
			return(0);
	}
	else
	{
		//Full screen mode

		ShowCursor(FALSE);

		//Get screen size of desktop window
		//Get handle to desktop window
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

		//	Create the window
		if (!(main_hwnd = CreateWindow(	CLASSNAME,											//	Class Name.
										"Super Pillow Fighter",										//	Title of the Window.
										WS_POPUP | WS_VISIBLE,					//	Window Flags.
										(GetSystemMetrics(SM_CXSCREEN)>>1) - (g_iWindowWidth>>1),	//	Window Start Point (x, y). 
										(GetSystemMetrics(SM_CYSCREEN)>>1) - (g_iWindowHeight>>1),
										g_iWindowWidth,										//	Width of Window.
										g_iWindowHeight,										//	Height of Window.
										NULL,												//	Handle to parent.
										NULL,												//	Handle to menu.
										hinstance,											//	Application Instance.
										NULL)))												//	Creation parms.
			return(0);
	}

	//Initialize game
	srand((unsigned int)time(0));
	MemInit(50000000);
	RECT rClient;
	GetClientRect(main_hwnd, &rClient);
	
	//Pre-initialize the bitmap manager and renderer
	CDisplayDonkey::GetInstance()->initialize(main_hwnd, rClient.right, rClient.bottom);
	CDisplayDonkey::GetInstance()->setOrthographic(true);
	CBitmapManager::GetInstance()->Initialize();
	TimerStart();

	//Draw startup screens
	CStopWatch LoadTimer;
	//Draw Carbon13
	int Carbon13 = CBitmapManager::GetInstance()->LoadImage("ResourceFiles\\textures\\carbon_13.tga", true);
	//clear screen to white
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	CBitmapManager::GetInstance()->BindTexture(Carbon13);
	glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);
			glVertex2f(0.0f, 0.0f);

			glTexCoord2f(0.0f, 1.0f);
			glVertex2f(0.0f, 1.0f);

			glTexCoord2f(1.0f, 1.0f);
			glVertex2f(1.0f, 1.0f);

			glTexCoord2f(1.0f, 0.0f);
			glVertex2f(1.0f, 0.0f);
	glEnd();
	//swap the buffers
	CDisplayDonkey::GetInstance()->Render();
	//wait five seconds
	LoadTimer.Start(5.0f);
	do
	{
		if (LoadTimer.GetRemainingTime() <= 0.0f)
		{
			break;
		}
		TimerUpdate();
	}	while (true);
	TimerStart();

	//Draw Slappy Games
	int SlappyGames = CBitmapManager::GetInstance()->LoadImage("ResourceFiles\\textures\\smileytooth1.tga", true);
	//clear screen to white
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	CBitmapManager::GetInstance()->BindTexture(SlappyGames);
	glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);
			glVertex2f(0.0f, 0.0f);

			glTexCoord2f(0.0f, 1.0f);
			glVertex2f(0.0f, 1.0f);

			glTexCoord2f(1.0f, 1.0f);
			glVertex2f(1.0f, 1.0f);

			glTexCoord2f(1.0f, 0.0f);
			glVertex2f(1.0f, 0.0f);
	glEnd();
	//swap the buffers
	CDisplayDonkey::GetInstance()->Render();
	//wait five seconds
	LoadTimer.Start(5.0f);
	do
	{
		if (LoadTimer.GetRemainingTime() <= 0.0f)
		{
			break;
		}
		TimerUpdate();
	}	while (true);
	TimerStart();

	//Initialize everything else
	CGameManager::GetInstance()->InitProgram(main_hwnd, hinstance, rClient.right, rClient.bottom, g_bWindowedMode, g_bArcadeMode);

	//	Enter main event loop
	while(1)
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{ 
			//	Test if this is a quit
			if (msg.message == WM_QUIT)
				break;
		
			//	Translate any accelerator keys
			TranslateMessage(&msg);

			//	Send the message to the window proc
			DispatchMessage(&msg);
		}
		if (CGameManager::GetInstance()->Update())
		{
			break;
		}

		////dirty dirty hack
		//if (GetAsyncKeyState(VK_ESCAPE))
		//{
		//	break;
		//}
	}

	//Shutdown program
	CGameManager::GetInstance()->ProgramShutdown();
	CGameManager::DeleteInstance();
	MemShutDown();

	//	Return to Windows like this
	return (int)(msg.wParam);
}

void LoadWindowData()
{
	//load the window data from the file
	FILE *pFile = fopen("ResourceFiles\\Window.data", "rb");
	fread(&g_bWindowedMode, sizeof(bool), 1, pFile);
	fread(&g_iWindowWidth, sizeof(int), 1, pFile);
	fread(&g_iWindowHeight, sizeof(int), 1, pFile);
	fread(&g_bArcadeMode, sizeof(bool), 1, pFile);
	fclose(pFile);
}