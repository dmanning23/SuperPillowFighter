////////////////////////////////////////////////////////
//
// Name: "HUD.h"
//
// Author: Daron Keeler (DK)
//
// Creation Date: August 22, 2004
//
// Purpose: The HUD interface
//
////////////////////////////////////////////////////////

#ifndef _HUD_H_
#define _HUD_H_

#include "..\\Logger\\Logger.h"
#include "..\\ObjectManager\\ObjectManager.h"


#define HUD_TOTALBMPS 5
#define HUD_MAXCHARS 8
#define PLAYER_STATE_DEBUG_POS			0.70f
#define PLAYER_STATE_DEBUG_FONT_SIZE	0

struct HUDDataT
{
/*#ifdef _DEBUG
    // Holds ai index for debug purposes.
    int iAIIndex;
#endif*/

    // All player's scores and data to print to screen.
    int iScores[HUD_MAXCHARS];
    int iDamage[HUD_MAXCHARS];

    // Which character portrait to draw.
    int iPortrait[HUD_MAXCHARS];

    // The portrait's left X value.
    float fXLoc[HUD_MAXCHARS];

    // Current game time
    float GameTimer;
};

class CHUD
{
private:

    // HUD Data
    HUDDataT HUDData;
    // Player count
    int m_iPlayerCount;
	float m_fStartTime;

	// FPS counter.
	float m_fLast;
	int m_iOutfps, m_iFps;

    // Instance to interface
    static CHUD* pInstance;

    // Logger
    CLogger m_LogMe;

    // Saves current font
    float m_fCurFontLocX;
    float m_fCurFontLocY;
        
    int m_HUDBmps[HUD_TOTALBMPS];
	int m_iPortraitBckCircle;


	// Used for pausing and ending
	bool bEnding;
	
	// Is there a TIE?!
	bool bTie;

	// Holds the int to the player array, going to be who won.
	int m_iWinner;

	CStopWatch HUDStop;

    ////////////////////////////////////////////////////////////////////
    //
    // Function: Overwritten Constructors
    //
    // Last Modified: August 22, 2004
    //
    // Input: none
    //
    // Output: none
    //
    // Return: none
    //
    // Purpose: all HUD constructors. These are for singleton use.
    ////////////////////////////////////////////////////////////////////
    CHUD() {}
    CHUD(const CHUD &c) {}
    CHUD & operator =(const CHUD &c) {} 

    ////////////////////////////////////////////////////////////////////
    //
    // Function: "GetData"
    //
    // Last Modified: August 22, 2004
    //
    // Input: none
    //
    // Output: none
    //
    // Return: none
    //
    // Purpose: Gets data to print each frame.
    ////////////////////////////////////////////////////////////////////
    void GetData();

    ////////////////////////////////////////////////////////////////////
    //
    // Function: "WriteAlphas"
    //
    // Last Modified: August 26, 2004
    //
    // Input:   int LetterToDraw    which letter to send to Drawfont
    //
    // Output: none
    //
    // Return: none
    //
    // Purpose: Writes all numbers.
    ////////////////////////////////////////////////////////////////////
    int WriteAlphas(char LetterToDraw);

    ////////////////////////////////////////////////////////////////////
    //
    // Function: "WriteNumerals"
    //
    // Last Modified: August 26, 2004
    //
    // Input:   int NumberToDraw    which number to send to Drawfont
    //
    // Output: none
    //
    // Return: none
    //
    // Purpose: Writes all numbers.
    ////////////////////////////////////////////////////////////////////
    int WriteNumerals(int NumberToDraw);

    ////////////////////////////////////////////////////////////////////
    //
    // Function: "DrawFont"
    //
    // Last Modified: August 26, 2004
    //
    // Input: none
    //
    // Output: none
    //
    // Return: none
    //
    // Purpose: Draws indexes to screen..
    ////////////////////////////////////////////////////////////////////
    void DrawFont(int iIndex, float CurX, float CurY, float fSize);





public:

	bool bQuit;
    ////////////////////////////////////////////////////////////////////
    //
    // Function: Destructor
    //
    // Last Modified: August 22, 2004
    //
    // Input: none
    //
    // Output: none
    //
    // Return: none
    //
    // Purpose: Basic HUD destructor
    ////////////////////////////////////////////////////////////////////
    ~CHUD() {}

    ////////////////////////////////////////////////////////////////////
    //
    // Function: "GetInstance"
    //
    // Last Modified: August 22, 2004
    //
    // Input: none
    //
    // Output: none
    //
    // Return: CHUD *    a pointer to the single instance of the class
    //
    // Purpose: To get an instance of this class for singleton use.
    ////////////////////////////////////////////////////////////////////
    static CHUD* GetInstance();

    ////////////////////////////////////////////////////////////////////
    //
    // Function: "DeleteInstance"
    //
    // Last Modified: August 22, 2004
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
    // Function: "InitHUD"
    //
    // Last Modified: August 22, 2004
    //
    // Input: none
    //
    // Output: none
    //
    // Return: none
    //
    // Purpose: Initialize all HUD parameters
    ////////////////////////////////////////////////////////////////////
    void InitHUD();

    ////////////////////////////////////////////////////////////////////
    //
    // Function: "UpdateHUD"
    //
    // Last Modified: August 22, 2004
    //
    // Input: none
    //
    // Output: none
    //
    // Return: none
    //
    // Purpose: Update main game HUD.
    ////////////////////////////////////////////////////////////////////
    void UpdateHUD();

    ////////////////////////////////////////////////////////////////////
    //
    // Function: "UpdatePausedHUD"
    //
    // Last Modified: August 22, 2004
    //
    // Input: none
    //
    // Output: none
    //
    // Return: none
    //
    // Purpose: Update Paused HUD mode.
    ////////////////////////////////////////////////////////////////////
    bool UpdatePausedHUD();

    ////////////////////////////////////////////////////////////////////
    //
    // Function: "UpdateEndHUD"
    //
    // Last Modified: August 22, 2004
    //
    // Input: none
    //
    // Output: none
    //
    // Return: bool         true if player responded
    //
    // Purpose: Update the ending until player presses select or attack.
    ////////////////////////////////////////////////////////////////////
    bool UpdateEndHUD();

    ////////////////////////////////////////////////////////////////////
    //
    // Function: "ShutDownHUD"
    //
    // Last Modified: August 22, 2004
    //
    // Input: none
    //
    // Output: none
    //
    // Return: none
    //
    // Purpose: Shuts Down the HUD
    ////////////////////////////////////////////////////////////////////
    void ShutDownHUD();

};

#endif 