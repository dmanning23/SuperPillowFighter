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

#include "BmpFonts.h"
#include "..\\MemManager\\MemManager.h"
#include "..\\BitmapManager\\BitmapManager.h"

CBmpFonts *CBmpFonts::pInstance = 0;

CBmpFonts *CBmpFonts::GetInstance()
{
    // Make one if it hasnt been done.
    if(!pInstance)
        //    pInstance = new CInputInterface;
    {
        pInstance = (CBmpFonts*)MemMalloc(sizeof(CBmpFonts));
        pInstance->CBmpFonts::CBmpFonts();
    }

    // Return the address of the instance.
    return pInstance;
}

void CBmpFonts::DeleteInstance()
{
    // delete the instance
    if(pInstance)
    {
        // delete pInstance;
        pInstance->CBmpFonts::~CBmpFonts();
        MemFree(pInstance);

        pInstance = NULL;
    }
}

void CBmpFonts::InitFonts()
{
    m_iTotalTextures[0] = -1;
    // Load the font
    m_iTotalTextures[0] = CBitmapManager::GetInstance()->LoadImage("ResourceFiles\\textures\\BmpFontsNEW.tga", true);

	// Set the initial color to white.
	m_fColor[0] = 1.0f;
	m_fColor[1] = 1.0f;
	m_fColor[2] = 1.0f;
	m_fColor[3] = 1.0f;
}

void CBmpFonts::ShutDownFonts()
{

}

float CBmpFonts::WriteWithFonts(char *SentenceToWrite, float XLoc, float YLoc, int iSize)
{    
    int ArraySize = (int)strlen(SentenceToWrite);

    if(ArraySize > 48)
        return 0;

    char TempArray[50];
    TempArray[ArraySize+1] = '\0';

    // save the passed in values
    m_fCurFontLocX = XLoc;
    m_fCurFontLocY = YLoc;

    float fSize;

    switch(iSize)
    {
    case FONTSIZE_SMALL:
        fSize = SMALLFONT;
        break;
    case FONTSIZE_MEDIUM:
        fSize = MEDIUMFONT;
        break;
    case FONTSIZE_LARGE:
        fSize = LARGEFONT;
        break;
    };

    // BIND THE FONT TEXTURE

    CBitmapManager::GetInstance()->BindTexture(m_iTotalTextures[0]);


    for(int i = 0; i < ArraySize; i++)
    {
        // Make it all uppercase
        if((int)SentenceToWrite[i] >= 97 && (int)SentenceToWrite[i] <= 122) 
           TempArray[i] = (char)toupper((int)SentenceToWrite[i]);
        else
            TempArray[i] = SentenceToWrite[i];

       // Draw the letters
        if((int)TempArray[i] >= 65 && (int)TempArray[i] <= 90)
        {
            DrawFont(WriteAlphas(TempArray[i]), m_fCurFontLocX, m_fCurFontLocY, fSize);
            m_fCurFontLocX += (fSize*0.6f);
            continue;
        }
        if((int)TempArray[i] >= 48 && (int)TempArray[i] <= 57)
        {
            DrawFont(WriteNumerals(TempArray[i]), m_fCurFontLocX, m_fCurFontLocY, fSize); 
            m_fCurFontLocX += (fSize*0.6f);
            continue;
        }
        if((int)TempArray[i] == 32)
        {
            // Just move the temp X right one space.
            m_fCurFontLocX += (fSize*0.6f);
            continue;
        }       
        if(TempArray[i] == char(37))
        {
            // index into the array specifically and draw the percent.     It's 36
            DrawFont(36, m_fCurFontLocX, m_fCurFontLocY, fSize);
         
            m_fCurFontLocX += (fSize*0.6f);
            continue;
        }
        if(TempArray[i] == char(58))
        {

            // index into the array specifically and draw the colon.     It's 37
            DrawFont(37, m_fCurFontLocX, m_fCurFontLocY, fSize);

            m_fCurFontLocX += (fSize*0.6f);
            continue;
        }
    }

    return m_fCurFontLocX;
}

float CBmpFonts::WriteJustInts(int NumToDraw, float XLoc, float YLoc, int iSize)
{
    char TempArray[20];
    

    itoa(NumToDraw, TempArray, 10);

     // save the passed in values
    m_fCurFontLocX = XLoc;
    m_fCurFontLocY = YLoc;

    float fSize;

    switch(iSize)
    {
    case FONTSIZE_SMALL:
        fSize = SMALLFONT;
        break;
    case FONTSIZE_MEDIUM:
        fSize = MEDIUMFONT;
        break;
    case FONTSIZE_LARGE:
        fSize = LARGEFONT;
        break;
    };

    // BIND THE FONT TEXTURE

    CBitmapManager::GetInstance()->BindTexture(m_iTotalTextures[0]);


    // loop through and draw each individual number
    for(int i = 0; i < 20; i++)
    {
        if(TempArray[i] == '\0')
            break;
        DrawFont(TempArray[i]-22, XLoc, YLoc, fSize);
        m_fCurFontLocX += (fSize*0.6f);
    }

    return m_fCurFontLocX;

}

int CBmpFonts::WriteAlphas(char LetterToDraw)
{
    return ((int)LetterToDraw - 65);
}

int CBmpFonts::WriteNumerals(int NumberToDraw)
{
    return NumberToDraw - 22;
}             

void CBmpFonts::DrawFont(int iIndex, float CurX, float CurY, float fSize)
{
    glBegin(GL_QUADS);
	   glColor4fv(m_fColor);	
       glTexCoord2f((float)((iIndex % 8) * HUD_FONTWIDTH), (float)(iIndex / 8) * HUD_FONTHEIGHT);
       glVertex2f(m_fCurFontLocX, m_fCurFontLocY);

       glTexCoord2f((float)((iIndex % 8) * HUD_FONTWIDTH), (float)(((iIndex / 8) * HUD_FONTHEIGHT) + HUD_FONTHEIGHT));
       glVertex2f(m_fCurFontLocX, m_fCurFontLocY + fSize);

       glTexCoord2f((float)(((iIndex % 8) * HUD_FONTWIDTH) + HUD_FONTWIDTH), (float)(((iIndex / 8) * HUD_FONTHEIGHT) + HUD_FONTHEIGHT));
       glVertex2f(m_fCurFontLocX + (fSize *0.6f), m_fCurFontLocY + fSize);

       glTexCoord2f((float)(((iIndex % 8) * HUD_FONTWIDTH) + HUD_FONTWIDTH), (float)(iIndex / 8) * HUD_FONTHEIGHT);
       glVertex2f(m_fCurFontLocX + (fSize *0.6f), m_fCurFontLocY);
    glEnd();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

}

void CBmpFonts::setFontColor(float fRed, float fGreen, float fBlue, float fAlpha)
{
    // Set the colors. 
	m_fColor[0] = fRed;
	m_fColor[1] = fGreen;
	m_fColor[2] = fBlue;

	// Set the alpha.
	m_fColor[3] = fAlpha;
}
