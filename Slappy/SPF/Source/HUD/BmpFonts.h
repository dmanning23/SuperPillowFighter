#ifndef _BMPFONTS_H_
#define _BMPFONTS_H_
////////////////////////////////////////////////////////
//
// Name: "BmpFonts.h"
//
// Author: Daron Keeler (DK)
//
// Creation Date: September 02, 2004
//
// Purpose: The Bitmapped Font class
//          NOTE:: All dates previous of Sept 02 are actually Sept 02, 
//            this class was created from already written functions in
//            another class.
//
////////////////////////////////////////////////////////

#define HUD_FONTWIDTH 1.0f/8.0f-0.005f
#define HUD_FONTHEIGHT 1.0f/7.0f

#define SMALLFONT 0.02f
#define MEDIUMFONT 0.03f
#define LARGEFONT 0.05f

#define BMP_TEXTURES 1

enum {FONTSIZE_SMALL = 0, FONTSIZE_MEDIUM, FONTSIZE_LARGE};

class CBmpFonts
{
private:
    // holds id's to all textures
    int m_iTotalTextures[BMP_TEXTURES];

    // Instance to interface
    static CBmpFonts* pInstance;

    // Saves current font
    float m_fCurFontLocX;
    float m_fCurFontLocY;

	// Current color and alpha for the fonts that will be displayed.
	float m_fColor[4];

    ////////////////////////////////////////////////////////////////////
    //
    // Function: Overwritten Constructors
    //
    // Last Modified: September 2, 2004
    //
    // Input: none
    //
    // Output: none
    //
    // Return: none
    //
    // Purpose: all BmpFont constructors. These are for singleton use.
    ////////////////////////////////////////////////////////////////////
    CBmpFonts() {}
    CBmpFonts(const CBmpFonts &c) {}
    CBmpFonts & operator = (const CBmpFonts &c) {}

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
    ////////////////////////////////////////////////////////////////////
    //
    // Function: setFontColor()
    //
    // Last Modified: Sept 18, 2004
    //
    // Input:	fRed		- red channel for this color.		(0.0f - 1.0f)
	//			fGreen		- green channel for this color.		(0.0f - 1.0f)
	//			fBlue		- blue channel for this color.		(0.0f - 1.0f)
	//			fAlpha		- the alpha value for the fonts.	(0.0f - 1.0f)
    //
    // Output: none
    //
    // Return: none
    //
    // Purpose: Set the color and alpha value for the following text.
    ////////////////////////////////////////////////////////////////////
	void setFontColor(float fRed, float fGreen, float fBlue, float fAlpha = 1.0f);

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
    // Purpose: Basic CBmpFonts destructor
    ////////////////////////////////////////////////////////////////////
    ~CBmpFonts() {}

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
    // Return: CBmpFonts *    a pointer to the single instance of the class
    //
    // Purpose: To get an instance of this class for singleton use.
    ////////////////////////////////////////////////////////////////////
    static CBmpFonts* GetInstance();

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
    // Function: "InitFonts"
    //
    // Last Modified: August 22, 2004
    //
    // Input: none
    //
    // Output: none
    //
    // Return: none
    //
    // Purpose: Initialize all BmpFonts parameters
    ////////////////////////////////////////////////////////////////////
    void InitFonts();

    ////////////////////////////////////////////////////////////////////
    //
    // Function: "ShutDownFonts"
    //
    // Last Modified: August 22, 2004
    //
    // Input: none
    //
    // Output: none
    //
    // Return: none
    //
    // Purpose: Shuts Down the Fonts
    ////////////////////////////////////////////////////////////////////
    void ShutDownFonts();

    ////////////////////////////////////////////////////////////////////
    //
    // Function: "WriteWithFonts"
    //
    // Last Modified: August 26, 2004
    //
    // Input:   char * SentenceToWrite  The actual words to write
    //          float XLoc              The far left of the entire sentence
    //          float YLoc              The very top of the entire sentence
    //          int iSize               Enumerated at the top, three total sizes.
    //
    // Output: none
    //
    // Return: the end point of the sentence
    //
    // Purpose: Takes in a sentence and writes it to the screen.
    ////////////////////////////////////////////////////////////////////
    float WriteWithFonts(char * SentenceToWrite, float XLoc, float YLoc, int iSize = 1);

    ////////////////////////////////////////////////////////////////////
    //
    // Function: "WriteJustInts"
    //
    // Last Modified: August 26, 2004
    //
    // Input:   int                     The number to draw.
    //          float XLoc              The far left of the entire sentence
    //          float YLoc              The very top of the entire sentence
    //          int iSize               Enumerated at the top, three total sizes.
    //
    // Output: none
    //
    // Return: the end point of the number
    //
    // Purpose: Takes a number and writes it to screen.
    ////////////////////////////////////////////////////////////////////
    float WriteJustInts(int NumToDraw, float XLoc, float YLoc, int iSize = 1);
};

#endif