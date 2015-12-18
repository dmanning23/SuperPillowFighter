////////////////////////////////////////////////////////
//
// Name: "InputInterface.h"
//
// Author: Daron Keeler (DK)
//
// Creation Date: August 8, 2004
//
// Purpose: The input interface will use the input manager to control the game.
//
////////////////////////////////////////////////////////

#ifndef _INPUTINTERFACE_H_
#define _INPUTINTERFACE_H_

#include "SubModules\\Input.h"

// Needed later for accessing the players.
#include "..\\ObjectManager\\ObjectManager.h"

#define INPUT_MENUMODE 0

#define INPUT_GAMEMODE 1

#define INPUT_JOY1 0
#define INPUT_JOY2 1
#define INPUT_JOY3 2
#define INPUT_JOY4 3


// These need to be initialized to certain buttons.
enum {INPUT_BLOCK = 0, INPUT_ATTACK, INPUT_JUMP};

class CInputInterface
{
private:

    // Instance to input manager
    //CInput * Input;

    // Instance to interface
    static CInputInterface* pInstance;

public:

    // keeps track of AVAILABLE controllers, to limit menu from choosing 3 players when 1 controller is plugged in etc.
    // bool m_bPlayerValidity[4];

    // Keep track of how many players were chosen to be used. Must be less than or equal to m_iPlayerValidity
    // int m_iActivePlayers;

    // Flag to keep track of menu mode or game mode.
    int m_iTypeFlag;

	// Keeps track of number of available controllers for the menu
	// int m_iMenuControllers;

private:
    ////////////////////////////////////////////////////////////////////
    //
    // Function: "Constructor"
    //
    // Last Modified: August 8, 2004
    //
    // Input: none
    //
    // Output: none
    //
    // Return: none
    //
    // Purpose: Basic constructor.
    ////////////////////////////////////////////////////////////////////
    CInputInterface();

    ////////////////////////////////////////////////////////////////////
    //
    // Function: "Copy Constructor"
    //
    // Last Modified: August 8, 2004
    //
    // Input: const CInput &    (Not Used)
    //
    // Output: none
    //
    // Return: none
    //
    // Purpose: Disables the Copy Constructors
    ////////////////////////////////////////////////////////////////////
    CInputInterface(const CInputInterface &c) {}

    ////////////////////////////////////////////////////////////////////
    //
    // Function: "Assignment Operator"
    //
    // Last Modified: August 8, 2004
    //
    // Input: const CInputInterface &    (Not Used)
    //
    // Output: none
    //
    // Return: CInputInterface &         (Not Used)
    //
    // Purpose: Disables the Assignment Operator for CInput.
    ////////////////////////////////////////////////////////////////////
    CInputInterface & operator = (const CInputInterface&i) {}

public:

    ////////////////////////////////////////////////////////////////////
    //
    // Function: "Destructor"
    //
    // Last Modified: August 8, 2004
    //
    // Input: none
    //
    // Output: none
    //
    // Return: none
    //
    // Purpose: Basic Destructor for the CInputInterface class.
    ////////////////////////////////////////////////////////////////////
    ~CInputInterface() {}

    ////////////////////////////////////////////////////////////////////
    //
    // Function: "GetInstance"
    //
    // Last Modified: August 8, 2004
    //
    // Input: none
    //
    // Output: none
    //
    // Return: CInputInterface *    a pointer to the single instance of the class
    //
    // Purpose: To get an instance of this class for singleton use.
    ////////////////////////////////////////////////////////////////////
    static CInputInterface* GetInstance();

    ////////////////////////////////////////////////////////////////////
    //
    // Function: "DeleteInstance"
    //
    // Last Modified: August 8, 2004
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
    // Function: "InitInputInterface"
    //
    // Last Modified: August 8, 2004
    //
    // Input: HWND      The handle to pass to the InputManager.
    //        HINSTANCE The instance to to pass to the InputManager.
    //
    // Output: none
    //
    // Return: void
    //
    // Purpose: Initializes all required variables and pointers.
    ////////////////////////////////////////////////////////////////////
    void InitInputInterface(HWND hWnd, HINSTANCE hInst);

    ////////////////////////////////////////////////////////////////////
    //
    // Function: "InitGameInput"
    //
    // Last Modified: August 12, 2004
    //
    // Input: none   
    //
    // Output: none
    //
    // Return: void
    //
    // Purpose: Initializes players. Attaches players to joysticks.
    ////////////////////////////////////////////////////////////////////
    void InitGameInput();

    ////////////////////////////////////////////////////////////////////
    //
    // Function: "UpdateInput"
    //
    // Last Modified: August 8, 2004
    //
    // Input: none
    //
    // Output: none
    //
    // Return: void
    //
    // Purpose: Updates all input and manages things properly.
    ////////////////////////////////////////////////////////////////////
    void UpdateInput();

    ////////////////////////////////////////////////////////////////////
    //
    // Function: "ShutDownInputInterface"
    //
    // Last Modified: August 8, 2004
    //
    // Input: none
    //
    // Output: none
    //
    // Return: void
    //
    // Purpose: Shuts down and releases all required components.
    ////////////////////////////////////////////////////////////////////
    void ShutDownInputInterface();

	////////////////////////////////////////////////////////////////////
    //
    // Function: "ResetGameInput"
    //
    // Last Modified: Sept 17, 2004
    //
    // Input: none
    //
    // Output: none
    //
    // Return: void
    //
    // Purpose: Shuts down and releases all required components to go to menu.
    ////////////////////////////////////////////////////////////////////
    void ResetGameInput();


};

#endif