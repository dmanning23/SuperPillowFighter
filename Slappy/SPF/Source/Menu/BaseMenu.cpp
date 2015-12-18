/*////////////////////////////////////////////////////////////////////////
//File:	BaseMenu.cpp
//
//Author:	Dan Manning
//
//Creation Date:	9-18-04
//
//Purpose:	The base class that all menus inherit from
//
/////////////////////////////////////////////////////////////////////////*/
#include "basemenu.h"
#include "..\BitmapManager\BitmapManager.h"

CBaseMenu::CBaseMenu(void)
{
}

CBaseMenu::~CBaseMenu(void)
{
}

/*////////////////////////////////////////////////////////////////////////
//Function:	DrawBG
//
//Purpose:	Draw the background image of this menu
//
/////////////////////////////////////////////////////////////////////////*/
void CBaseMenu::DrawBG()
{
	CBitmapManager::GetInstance()->BindTexture(m_iBackgroundImage);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(0.0f, 0.0f);

        glTexCoord2f(0.0f, 1.0f);
        //glTexCoord2f(0.0f, 0.75f);
        glVertex2f(0.0f, 1.0f);

        glTexCoord2f(1.0f, 1.0f);
        //glTexCoord2f(1.0f, 0.75f);
        glVertex2f(1.0f, 1.0f);

        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(1.0f, 0.0f);
    glEnd();
}

/*////////////////////////////////////////////////////////////////////////
//Function:	DrawPillow
//
//Purpose:	Draw a pillow image
//
/////////////////////////////////////////////////////////////////////////*/
void CBaseMenu::DrawPillow(float fTopLeftPointX, float fTopLeftPointY, bool bDarkened)
{
	if(!bDarkened)
    {
        // Bind the pillow
		CBitmapManager::GetInstance()->BindTexture(m_iPillowImage);
        // NOTE:: Sizes are subject to change
        glBegin(GL_QUADS);

           glTexCoord2f(0.0f, 0.0f);
           glVertex2f(fTopLeftPointX, fTopLeftPointY);

           glTexCoord2f(0.0f, 1.0f);
           glVertex2f(fTopLeftPointX, fTopLeftPointY+0.15f);

           glTexCoord2f(1.0f, 1.0f);
           glVertex2f(fTopLeftPointX+0.2f, fTopLeftPointY+0.15f);

           glTexCoord2f(1.0f, 0.0f);
           glVertex2f(fTopLeftPointX+0.2f, fTopLeftPointY);
                                                         
        glEnd();
    }
    else
    {
        // Bind the pillow
		CBitmapManager::GetInstance()->BindTexture(m_iDarkPillowImage);
        // NOTE:: Sizes are subject to change
        glBegin(GL_QUADS);

           glTexCoord2f(0.0f, 0.0f);
           glVertex2f(fTopLeftPointX, fTopLeftPointY);

           glTexCoord2f(0.0f, 1.0f);
           glVertex2f(fTopLeftPointX, fTopLeftPointY+0.15f);

           glTexCoord2f(1.0f, 1.0f);
           glVertex2f(fTopLeftPointX+0.2f, fTopLeftPointY+0.15f);

           glTexCoord2f(1.0f, 0.0f);
           glVertex2f(fTopLeftPointX+0.2f, fTopLeftPointY);
                                                         
        glEnd();
    }
}