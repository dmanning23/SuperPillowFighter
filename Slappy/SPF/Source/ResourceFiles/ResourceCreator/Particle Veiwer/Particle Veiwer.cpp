// Particle Veiwer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <ctime>
#include "Particle Veiwer.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include "ParticleEngine.h"
#include "..\..\..\GameTimer\StopWatch.h"
#include "..\..\..\GameTimer\GameTimer.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

//My globals
GLuint textureHandle;	//used to store texture
//The emitter information to view
EmitterStuffT myEmitterStuff;
CStopWatch inputBufferTimer;	//shitty buffered input
Vec4DT PosVec = {0.0f, 0.0f, -20.0f};
Vec4DT VelVec = {10.0f, 0.0f, 0.0f};

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

#define PARTICLE_SIZE	pCurEmit->tParticles[ii].fSize
#define HALF_PART_SIZE	PARTICLE_SIZE/2.0f

/*////////////////////////////////////////////////////////////////////////
//Function:	gltLoadTGA
//
//Input:
//	szFile:	filename
//
//Output:
//	nWidth:		width of the image
//	nHeight:	height of the image
//
//Return:	
//	BYTE*:	the byte information of the image
//
//Purpose:	Loads a TGA file with transparency
//
/////////////////////////////////////////////////////////////////////////*/
BYTE* gltLoadTGA(char *szFile, int *nWidth, int *nHeight)
{
	HANDLE hFileHandle;
	unsigned long lBitSize = 0;
	short width, height;

	// Open the Bitmap file
	hFileHandle = CreateFile(szFile,GENERIC_READ,FILE_SHARE_READ,
		NULL,OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,NULL);

	// Check for open failure (most likely file does not exist).
	if(hFileHandle == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, "No HUD.tga", "no file", MB_OK);
		return FALSE;
	}

	// File is Open. Read in bitmap header information
	unsigned char caTGAHeader[18];

	DWORD dwBytes;
	ReadFile(hFileHandle,caTGAHeader,18, &dwBytes,NULL);

	// Check for a couple of simple errors	
	if(dwBytes != 18)
		return FALSE;

	// Save the size and dimensions of the bitmap
	memcpy(&width,&caTGAHeader[12],2);
	memcpy(&height,&caTGAHeader[14],2);

	lBitSize = width * height * 4;
	*nWidth = width;
	*nHeight = height;
	BYTE *pBytes = new BYTE[lBitSize];

	// Read in the bitmap bits
	ReadFile(hFileHandle,pBytes,lBitSize,&dwBytes,NULL);

	// Close the bitmap file now that we have all the data we need
	CloseHandle(hFileHandle);

	// Make RGB
	for(unsigned int i = 0; i < lBitSize; i+= 4)
	{
		// Do the swap
		BYTE bByte = pBytes[i];
		pBytes[i] = pBytes[i+2];
		pBytes[i+2] = bByte;

		// Hack together an Alpha
		//int nVal = pBytes[i] + pBytes[i+2] + pBytes[i+3];
		//nVal = nVal/3;
		//pBytes[i+3] = nVal;
	}

	return pBytes;
}

/*////////////////////////////////////////////////////////////////////////
//Function:	LoadEffect
//
//Input:	iType:	the effect to load
//
//Purpose:	Loads a particle effect into myEmitterStuff
//
/////////////////////////////////////////////////////////////////////////*/
void LoadEffect(int iType)
{
	FILE *pFile;

	switch (iType)
	{
		case Feathers_ParticleEffect:
			{
                //load file
				pFile = fopen("..\\..\\ParticleEffects\\Feathers.effect", "rb");
				fread(&myEmitterStuff, sizeof(EmitterStuffT), 1, pFile);
				fclose(pFile);
			}
			break;
		case PlayerLand_ParticleEffect:
			{
                //load file
				pFile = fopen("..\\..\\ParticleEffects\\PlayerLand.effect", "rb");
				fread(&myEmitterStuff, sizeof(EmitterStuffT), 1, pFile);
				fclose(pFile);
			}
			break;
		case PlayerDeath_ParticleEffect:
			{
                //load file
				pFile = fopen("..\\..\\ParticleEffects\\PlayerDeath.effect", "rb");
				fread(&myEmitterStuff, sizeof(EmitterStuffT), 1, pFile);
				fclose(pFile);
			}
			break;
		case PlayerSplash_ParticleEffect:
			{
                //load file
				pFile = fopen("..\\..\\ParticleEffects\\PlayerSplash.effect", "rb");
				fread(&myEmitterStuff, sizeof(EmitterStuffT), 1, pFile);
				fclose(pFile);
			}
			break;
		case PlayerHit_ParticleEffect:
			{
                //load file
				pFile = fopen("..\\..\\ParticleEffects\\PlayerHit.effect", "rb");
				fread(&myEmitterStuff, sizeof(EmitterStuffT), 1, pFile);
				fclose(pFile);
			}
			break;
		case ItemHit_ParticleEffect:
			{
                //load file
				pFile = fopen("..\\..\\ParticleEffects\\ItemHit.effect", "rb");
				fread(&myEmitterStuff, sizeof(EmitterStuffT), 1, pFile);
				fclose(pFile);
			}
			break;
		case PlayerSpawn_ParticleEffect:
			{
                //load file
				pFile = fopen("..\\..\\ParticleEffects\\PlayerSpawn.effect", "rb");
				fread(&myEmitterStuff, sizeof(EmitterStuffT), 1, pFile);
				fclose(pFile);
			}
			break;
		case HealthPickup_ParticleEffect:
			{
                //load file
				pFile = fopen("..\\..\\ParticleEffects\\HealthPickup.effect", "rb");
				fread(&myEmitterStuff, sizeof(EmitterStuffT), 1, pFile);
				fclose(pFile);
			}
			break;
	}

	//setup bitmap filename for loading
	char strFileName[64];
	memset(strFileName, 0, 64);
	strcpy(strFileName, "..\\..\\..\\");
	strcat(strFileName, myEmitterStuff.strBmpFileName);

	//load bitmap into texture handle
	BYTE *pBytes;
	int iWidth, iHeight;

	//bind texture
	glBindTexture(GL_TEXTURE_2D, textureHandle);

	pBytes = gltLoadTGA(strFileName, &iWidth, &iHeight);
	//error check file load
	if (pBytes == NULL)
	{
		MessageBox(NULL, "No Texture", "No file", MB_OK);
	}
	//give the texture to opengl
	//gluBuild2DMipmaps(GL_TEXTURE_2D, 4, iWidth, iHeight, GL_RGBA, GL_UNSIGNED_BYTE, pBytes);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, iWidth, iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pBytes);
	//set texturing parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//clean up memory
	delete [] pBytes;
}

/*////////////////////////////////////////////////////////////////////////
//Function:	DrawGLStuff
//
//Last Modified:	May 3, 04
//
//Purpose:	Do all opengl rendering
//
/////////////////////////////////////////////////////////////////////////*/
void DrawGLStuff()
{
	//clear the colorbuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//put identity maatrix on matrix stack
	glPushMatrix();

	//glTranslatef(0.0f, 0.0f, 10.0f);

	////draw a big quad
	//glBegin(GL_QUADS);
	//{
	//	glVertex3f(0.0f, 0.0f, -30.0f);
	//	glTexCoord2f(0.0f, 0.0f);

	//	glVertex3f(0.0f, -10.0f, -30.0f);
	//	glTexCoord2f(0.0f, 1.0f);

	//	glVertex3f(10.0f, -10.0f, -30.0f);
	//	glTexCoord2f(1.0f, 1.0f);

	//	glVertex3f(10.0f, 0.0f, -30.0f);
	//	glTexCoord2f(1.0f, 0.0f);		
	//}
	//glEnd();

	//draw the particles
	CEmitter *pCurEmit = CParticleEngine::GetInstance()->m_ActiveEmitterList;
	static float x;
	static float y;
	static float z;
	// Use triangle strips for rendering.
	glBegin(GL_TRIANGLE_STRIP);

	if (pCurEmit)
	{
		// Create the geometry for all particles.
		for(int ii = 0; ii < pCurEmit->iNumParticles; ii++)
		{
			// Leave this for billboard particle rendering.
			glColor4f(1.0f, 1.0f, 1.0f, pCurEmit->tParticles[ii].fAlpha);

			x = pCurEmit->tParticles[ii].tPosition.tfX;
			y = pCurEmit->tParticles[ii].tPosition.tfY;
			z = pCurEmit->tParticles[ii].tPosition.tfZ;

			// Top-Left corner.
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-HALF_PART_SIZE+x, HALF_PART_SIZE+y,z);

			if(ii != 0)
				// used to make a degenerate triangle.
				glVertex3f(-HALF_PART_SIZE+x, HALF_PART_SIZE+y, z);

			// Bottom-left corner.
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-HALF_PART_SIZE+x, -HALF_PART_SIZE+y, z);

			// Top-right corner.
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(HALF_PART_SIZE+x, HALF_PART_SIZE+y, z);

			// Bottom-right corner.
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(HALF_PART_SIZE+x, -HALF_PART_SIZE+y, z);

			// used to make a degenerate triangle.
			glVertex3f(HALF_PART_SIZE+x, -HALF_PART_SIZE+y, z);
		}
	}
	glEnd();

	//get the identity matrix back
	glPopMatrix();
}

/*////////////////////////////////////////////////////////////////////////
//Function:SetupRC
//
//Last Modified:	May 3, 04
//
//Purpose:	Setup opengl sepcifics
//
/////////////////////////////////////////////////////////////////////////*/
void SetupRC()
{
	//clear color buffer to blue
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//turn on  texturing
	glEnable(GL_TEXTURE_2D);

	////turn on alpha testing
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, .25f);
	// Alpha blending.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//enable z buffering
	glEnable(GL_DEPTH_TEST);

	////turn on lights
	//glEnable(GL_LIGHTING);

	////tunr of default ambient light
	//float noLight[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, noLight);

	////set up a light
	//glEnable(GL_LIGHT0);

	////set light intensities
	//float someLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	//float brightLight[] = { 0.65f, 0.65f, 0.65f, 1.0f };
	//glLightfv(GL_LIGHT0, GL_AMBIENT, someLight);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, brightLight);

	////set light position
	//float lightPos[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	//glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	////change color calls to material calls
	//glEnable(GL_COLOR_MATERIAL);
	//glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	//glDisable(GL_DEPTH_TEST);
	//glDisable(GL_LIGHTING);

	//set texture envirnment
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//turn on texturing
	glEnable(GL_TEXTURE_2D);

	//create one texture object
	glGenTextures(1, &textureHandle);

	srand((unsigned int)time(NULL));
}

/*////////////////////////////////////////////////////////////////////////
//Function:	ShutdownRC
//
//Last Modified:	May 3, 04
//
//Purpose:	shutdown opengl specifics
//
/////////////////////////////////////////////////////////////////////////*/
void ShutdownRC()
{
	//clean up texture handles
	glDeleteTextures(1, &textureHandle);
}

/*////////////////////////////////////////////////////////////////////////
//Function:	ResizeOpenGL
//
//Last Modified:	May 3, 04
//
//INPUT:
//	width
//	height
//
//Purpose:	resize the opengl window
//
/////////////////////////////////////////////////////////////////////////*/
void ResizeOpenGL(int iWidth, int iHeight)
{
	if (iHeight == 0)
	{
		iHeight = 1;
	}

	//tell opengl how much window to use
	glViewport(0, 0, iWidth, iHeight);

	//voodoo code
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//reset coordinate system
	//glOrtho(-100,100,-100,100,-100,100);

	float aspect = (float)iWidth / (float)iHeight;
	gluPerspective(55.0f, aspect, 0.1f, 500.0f);

	//voodoo
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/*////////////////////////////////////////////////////////////////////////
//Function:	WinMain
//
//Purpose:	The entry point of the program
//
/////////////////////////////////////////////////////////////////////////*/
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_PARTICLEVEIWER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_PARTICLEVEIWER);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_PARTICLEVEIWER);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCTSTR)IDC_PARTICLEVEIWER;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      0, 0, 500, 500, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	static HDC hDC = NULL;
	static HGLRC hRC = NULL;

	switch (message) 
	{
		case WM_CREATE:
			{
				TimerStart();
				//TempVec.tfZ = 0.0f;

				inputBufferTimer.Start(0.0f);

				PIXELFORMATDESCRIPTOR pfd = {
					sizeof(PIXELFORMATDESCRIPTOR),									//size of the data structure
						1,																//version of pixelformatdescriptor
						PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,		//bit flags
						PFD_TYPE_RGBA,													//gimme real color?
						32,																//desired depth of color buffer, always 32
						0,0,0,0,0,0,0,0,0,0,0,0,0,										//can't specify, hardware specific
						16,																//depth of the z-buffer (zbuffer precision)
						0,0,0,0,0,0,0													//things we don't care about today(?)
				};

				//get the GDI device context
				hDC = GetDC(hWnd);

				//find a pfd that matches the one we created
				int iPF = ChoosePixelFormat(hDC, &pfd);

				//set the pixel format 
				SetPixelFormat(hDC, iPF, &pfd);

				//create the opengl rendering context
				hRC = wglCreateContext(hDC);

				//bind opengl to this window
				wglMakeCurrent(hDC, hRC);

				//initialize the rest of opengl
				//extgl_Initialize();

				//do any opengl specific initialization
				SetupRC();

				ResizeOpenGL(500,500);

				CParticleEngine::GetInstance()->Init();
			}
			break;
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			//TODO
			// Parse the menu selections:
			switch (wmId)
			{
			case ID_EFFECT_FEATHERS:
				{
					LoadEffect(Feathers_ParticleEffect);
				}
				break;
			case ID_EFFECT_PLAYERLAND:
				{
					LoadEffect(PlayerLand_ParticleEffect);
				}
				break;
			case ID_EFFECT_PLAYERDEATH:
				{
					LoadEffect(PlayerDeath_ParticleEffect);
				}
				break;
			case ID_EFFECT_PLAYERSPLASH:
				{
					LoadEffect(PlayerSplash_ParticleEffect);
				}
				break;
			case ID_EFFECT_PLAYERHIT:
				{
					LoadEffect(PlayerHit_ParticleEffect);
				}
				break;
			case ID_EFFECT_ITEMHIT:
				{
					LoadEffect(ItemHit_ParticleEffect);
				}
				break;
			case ID_EFFECT_PLAYERSPAWN:
				{
					LoadEffect(PlayerSpawn_ParticleEffect);
				}
				break;
			case ID_EFFECT_HEALTHPICKUP:
				{
					LoadEffect(HealthPickup_ParticleEffect);
				}
				break;
			case IDM_ABOUT:
				DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				break;
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_PAINT:
			{
				TimerUpdate();
				CParticleEngine::GetInstance()->Update();

				//Do OpenGL drawing
				DrawGLStuff();

				//add a particle effect
				if (GetAsyncKeyState(VK_SPACE) && inputBufferTimer.GetRemainingTime() <= 0.0f)
				{
					CParticleEngine::GetInstance()->CreateEmitter(&PosVec, &VelVec, &myEmitterStuff);
					inputBufferTimer.Start(2.0f);
					//MessageBox(NULL, "Pop", "Fuck you", MB_OK);
				}

				//do buffer swap
				SwapBuffers(hDC);
			}
			break;
		case WM_DESTROY:
			{
				CParticleEngine::GetInstance()->Shutdown();

				//do any opengl specific shutdown
				ShutdownRC();

				wglMakeCurrent(hDC, NULL);		//release opengl rendering context
				wglDeleteContext(hRC);			//delete the opengl Rendering Context

				//release device context
				ReleaseDC(hWnd, hDC);

				PostQuitMessage(0);
			}
		case WM_SIZE:
			{
				int iWidth = LOWORD(lParam);
				int iHeight = HIWORD(lParam);

				//call resize function
				ResizeOpenGL(iWidth, iHeight);
			}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}