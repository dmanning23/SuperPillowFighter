////////////////////////////////////////////////////////
//
// Name: "Input.h"
//
// Author: Daron Keeler (DK)
//
// Creation Date: August 4, 2004
//
// Purpose: The input manager will control all input from 
//          the keyboard or controllers.
//
////////////////////////////////////////////////////////

#ifndef _INPUT_H_
#define _INPUT_H_


#include "..\\..\\Logger\\Logger.h"

// Force DirectInput to use an earlier version
#define DIRECTINPUT_VERSION 0x0800

// include DirectInput
#include <dinput.h>        
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

// include error report
// #include <dxerr9.h>
// #pragma comment (lib, "dxerr9.lib")

// Releases DX pointers.
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) if (p) { p->Release(); p = NULL; }
#endif

// Buffer size for input
#define INPUT_BUFF_SIZE 10
   
// Number of Joysticks
#define NUM_JOYSTICKS 4

// Used to differ between player joysticks.
enum {InputP1 = 0, InputP2, InputP3, InputP4 };

enum {JOYSTICK_LEFT = 0, JOYSTICK_RIGHT, JOYSTICK_UP, JOYSTICK_DOWN, JOYSTICK_UPRIGHT, JOYSTICK_DOWNRIGHT, JOYSTICK_DOWNLEFT, 
    JOYSTICK_UPLEFT, JOYSTICK_NEUTRAL};

struct JoyDataT
{
    DIJOYSTATE2 JoyState;          // Holds joystick's data.
    LPDIRECTINPUTDEVICE8 Joystick;  // The main Joystick device.
    GUID JoyGUID;                 // The joystick's GUID
    bool JoyUnplug;               // Test to see if joystick unplugged.
    bool JoyButtons[128];          // Buffer input use.                         
    bool JoyFound;                // Was a joystick found?
};

class CInput
{
    // for all member names, di stands for direct input.
    private:

        // The main object.
        LPDIRECTINPUT8     m_diObj;         

        // Keyboard object
        LPDIRECTINPUTDEVICE8 m_diKeyboard;   

        // Main instance handle. Passed in during Init.
        HINSTANCE         m_hInst;           

        // Main Window Handle. Passed in during Init.
        HWND           m_hWin;              

        // Receives buffered data.
        DIDEVICEOBJECTDATA m_ObjData[INPUT_BUFF_SIZE];  

        // Holds data for the keyboard.
        char m_szKeyboard[256];       

        // Holds which key is currently active. (pressed) 
        BYTE m_Keys[256];                    

        // Data for Keyboard Layout
        HKL m_kblayout;          

        // Instance pointer, singleton use.
        static CInput* pInstance;

        // Logger
        CLogger m_LogMe;

        // used to save results and log.
        HRESULT m_hResult;

        // save key elements.
        DWORD  m_Elements;

        // The joystick structures
        JoyDataT m_JoyDat[NUM_JOYSTICKS];

        


    public:
        
        // Number of joysticks
        int m_iNumJoysticks;

    private:
        ////////////////////////////////////////////////////////////////////
        //
        // Function: "Constructor"
        //
        // Last Modified: August 4, 2004
        //
        // Input: none
        //
        // Output: none
        //
        // Return: none
        //
        // Purpose: Basic constructor.
        ////////////////////////////////////////////////////////////////////
        CInput();

        ////////////////////////////////////////////////////////////////////
        //
        // Function: "Copy Constructor"
        //
        // Last Modified: August 4, 2004
        //
        // Input: const CInput &    (Not Used)
        //
        // Output: none
        //
        // Return: none
        //
        // Purpose: Disables the Copy Constructors
        ////////////////////////////////////////////////////////////////////
        CInput(const CInput &c) {}

        ////////////////////////////////////////////////////////////////////
        //
        // Function: "Assignment Operator"
        //
        // Last Modified: August 4, 2004
        //
        // Input: const CInput &    (Not Used)
        //
        // Output: none
        //
        // Return: CInput &         (Not Used)
        //
        // Purpose: Disables the Assignment Operator for CInput.
        ////////////////////////////////////////////////////////////////////
        CInput & operator = (const CInput&i) {}

    public:

        ////////////////////////////////////////////////////////////////////
        //
        // Function: "Destructor"
        //
        // Last Modified: August 4, 2004
        //
        // Input: none
        //
        // Output: none
        //
        // Return: none
        //
        // Purpose: Basic Destructor for the CInput class.
        ////////////////////////////////////////////////////////////////////
        ~CInput() { }

        ////////////////////////////////////////////////////////////////////
        //
        // Function: "GetInstance"
        //
        // Last Modified: August 4, 2004
        //
        // Input: none
        //
        // Output: none
        //
        // Return: CInput *    a pointer to the single instance of the class
        //
        // Purpose: To get an instance of this class for singleton use.
        ////////////////////////////////////////////////////////////////////
        static CInput* GetInstance();

        ////////////////////////////////////////////////////////////////////
        //
        // Function: "DeleteInstance"
        //
        // Last Modified: August 4, 2004
        //
        // Input: none
        //
        // Output: none
        //
        // Return: none
        //
        // Purpose: To delete an instance of this class for singleton use.
        ////////////////////////////////////////////////////////////////////
        static void DeleteInstance();

        ////////////////////////////////////////////////////////////////////
        //
        // Function: "InitInput"
        //
        // Last Modified: August 4, 2004
        //
        // Input: HWND      The handle to use in the CInput class.
        //        HINSTANCE The instance to use throughout the CInput class.
        //
        // Output: none
        //
        // Return: void
        //
        // Purpose: Initializes all required variables and pointers.
        ////////////////////////////////////////////////////////////////////
        void InitInput(HWND hWnd, HINSTANCE hInst);
            
        ////////////////////////////////////////////////////////////////////
        //
        // Function: "ShutDownInput"
        //
        // Last Modified: August 4, 2004
        //
        // Input: none
        //
        // Output: none
        //
        // Return: void
        //
        // Purpose: Shuts down and releases all required components.
        ////////////////////////////////////////////////////////////////////
        void ShutDownInput();

        ////////////////////////////////////////////////////////////////////
        //
        // Function: "InitKeyBoard"
        //
        // Last Modified: August 5, 2004
        //
        // Input: none
        //
        // Output: none
        //
        // Return: void
        //
        // Purpose: Initializes keyboard for use.
        ////////////////////////////////////////////////////////////////////
        void InitKeyBoard();

        ////////////////////////////////////////////////////////////////////
        //
        // Function: "GetKey"
        //
        // Last Modified: August 5, 2004
        //
        // Input: const DWORD   the key to check
        //
        // Output: none
        //
        // Return: bool     Was it pressed or not?
        //
        // Purpose: Takes in a chosen key and returns if it was pressed.
        ////////////////////////////////////////////////////////////////////
        bool GetKey(const DWORD dwKey);

        ////////////////////////////////////////////////////////////////////
        //
        // Function: "GetBuffKey"
        //
        // Last Modified: August 5, 2004
        //
        // Input: const DWORD   the key to check
        //
        // Output: none
        //
        // Return: bool     Was it pressed or not
        //
        // Purpose: Takes in a chosen key and returns if it was pressed. 
        //          Uses buffered input.
        ////////////////////////////////////////////////////////////////////
        bool GetBuffKey(const DWORD dwKey);

        ////////////////////////////////////////////////////////////////////
        //
        // Function: "UpdateKeyBoard"
        //
        // Last Modified: August 5, 2004
        //
        // Input: none
        //
        // Output: none
        //
        // Return: bool     false if unable to get keyboard state
        //
        // Purpose: This function reads the keyboard state each frame.
        ////////////////////////////////////////////////////////////////////
        bool UpdateKeyBoard();

        ////////////////////////////////////////////////////////////////////
        //
        // Function: "UpdateBuffKeyBoard"
        //
        // Last Modified: August 5, 2004
        //
        // Input: none
        //
        // Output: none
        //
        // Return: bool     false if unable to get keyboard state
        //
        // Purpose: This function reads the keyboard state each frame.
        //          Using buffered input.
        ////////////////////////////////////////////////////////////////////
        bool UpdateBuffKeyBoard();

        ////////////////////////////////////////////////////////////////////
        //
        // Function: "CheckBuffKey"
        //
        // Last Modified: August 5, 2004
        //
        // Input: none
        //
        // Output: none
        //
        // Return: char     the key that was pressed.
        //
        // Purpose: Checks to see which key was pressed using buffered input.
        ////////////////////////////////////////////////////////////////////
        char CheckBuffKey();

        ////////////////////////////////////////////////////////////////////
        //
        // Function: "InitJoy"
        //
        // Last Modified: August 5, 2004
        //
        // Input: none
        //
        // Output: none
        //
        // Return: void
        //
        // Purpose: Initializes joysticks.
        ////////////////////////////////////////////////////////////////////
        void InitJoy();

        ////////////////////////////////////////////////////////////////////
        //
        // Function: "UpdateJoy"
        //
        // Last Modified: August 5, 2004
        //
        // Input: none
        //
        // Output: none
        //
        // Return: void
        //
        // Purpose: Updates joysticks.
        ////////////////////////////////////////////////////////////////////
        bool UpdateJoystick();

        ////////////////////////////////////////////////////////////////////
        //
        // Function: "JoyPush"
        //
        // Last Modified: August 5, 2004
        //
        // Input: int iButton   which button to check
        //        int iID       the joystick to use.        
        //
        // Output: none
        //
        // Return: bool    was it pushed or not
        //
        // Purpose: Checks to see if a certain button is pressed.
        ////////////////////////////////////////////////////////////////////
        bool JoyPush(int iButton, int iID = 0);

        ////////////////////////////////////////////////////////////////////
        //
        // Function: "JoyBuffPush"
        //
        // Last Modified: August 5, 2004
        //
        // Input: int iButton   which button to check
        //        int iID       the joystick to use.  
        //
        // Output: none
        //
        // Return: bool     was it pushed or not
        //
        // Purpose: Checks to see if a certain button is pressed using buff input.
        ////////////////////////////////////////////////////////////////////
        bool JoyBuffPush(int iButton, int iID = 0);

        ////////////////////////////////////////////////////////////////////
        //
        // Function: "GetJoyDir"
        //
        // Last Modified: August 5, 2004
        //
        // Input: 
        //        int iID    the joystick to use.  
        //
        // Output: none
        //
        // Return: int         enumerated direction that was pressed.
        //
        // Purpose: Takes in a direction and returns true or false based on whether
        //          or not that direction is currently active.
        ////////////////////////////////////////////////////////////////////
        int GetJoyDir(int iID = 0); 

        ////////////////////////////////////////////////////////////////////
        //
        // Function: "EnumJoystickCallback"
        //
        // Last Modified: August 5, 2004
        //
        // Input:   const DIDEVICEINSTANCE  * pDid   the instance to use
        //          void * pV                       void pointer
        //
        // Output: none
        //
        // Return: bool
        //
        // Purpose: this function enumerates a joystick, finds instances.
        ////////////////////////////////////////////////////////////////////
        friend BOOL CALLBACK EnumJoystickCallback(const DIDEVICEINSTANCE *pDid, void * pV);

};

#endif