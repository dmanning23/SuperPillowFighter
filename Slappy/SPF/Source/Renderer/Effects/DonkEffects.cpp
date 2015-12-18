//////////////////////////////////////////////////////////////////////////////////////////
// File Name:		DonkEffects.cpp
//
// Author:			Kevin Cecelski
//
// Date Created:	Monday, September 13, 2004
//
// Purpose:			This defines the effects class.
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "donkeffects.h"
#include "../displayDonkey.h"
#include "../../GameManager/GameManager.h"

//////////////////////////////////////////////////////////////////////////////////////////
// Function:	CDonkEffects()
//
// Purpose:		This is the default constructor.
//
// Last Modified:  Monday, September 13, 2004
//
// Input:		None.
//
// Output:		None.
//
// Returns:		None.
//	
//////////////////////////////////////////////////////////////////////////////////////////
CDonkEffects::CDonkEffects(void)
{
}

//////////////////////////////////////////////////////////////////////////////////////////
// Function:	~CDonkEffects
//
// Purpose:		This is the destructor.
//
// Last Modified:  Monday, September 13, 2004
//
// Input:		None.
//
// Output:		None.
//
// Returns:		None.
//
//////////////////////////////////////////////////////////////////////////////////////////
CDonkEffects::~CDonkEffects(void)
{
}

//////////////////////////////////////////////////////////////////////////////////////////
// Function:	drawBackground()
//
// Purpose:		This draws the background images for the current level.
//
// Last Modified:  Monday, September 13, 2004
//
// Input:		None.
//
// Output:		None.
//
// Returns:		None.
//
//////////////////////////////////////////////////////////////////////////////////////////
void CDonkEffects::drawBackground()
{
	// Hard-coded CRAP!
	if(CGameManager::GetInstance()->m_pGameData->iLevelChoice == FERRIS_WHEEL_LEVEL)
	{
		// For the ferris wheel.
		glDisable(GL_DEPTH_TEST);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		CBitmapManager::GetInstance()->BindTexture(
		CLevelManager::GetInstance()->m_pBackTexs[0].tiBitmapID);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f+m_fWaterTexMovement/10.0f , 0.0f);
			glVertex3f(-310.0f, 210.0f, -400.0f);
			glTexCoord2f(0.0f+m_fWaterTexMovement/10.0f, 1.0f);
			glVertex3f(-310.0f, -310.0f, -400.0f);
			glTexCoord2f(1.0f+m_fWaterTexMovement/10.0f, 1.0f);
			glVertex3f(310.0f,  -310.0f, -400.0f);
			glTexCoord2f(1.0f+m_fWaterTexMovement/10.0f, 0.0f);
			glVertex3f(310.0f, 210.0f, -400.0f);
		glEnd();
	}
	else
	{
	// For the boat level.
	glDisable(GL_DEPTH_TEST);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	CBitmapManager::GetInstance()->BindTexture(
		CLevelManager::GetInstance()->m_pBackTexs[0].tiBitmapID);
    glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-310.0f, 210.0f, -400.0f);
		glTexCoord2f(0.0f, 0.5f);
		glVertex3f(-310.0f, 2.0f, -400.0f);
		glTexCoord2f(1.0f, 0.5f);
		glVertex3f(310.0f,  2.0f, -400.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(310.0f, 210.0f, -400.0f);

		glColor4f(0.6f, 0.6f, 0.6f, 0.6f);
		glTexCoord2f(0.0f, 0.5f);
		glVertex3f(-310.0f, 2.0f, -400.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-310.0f, -395.0f, -300.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(310.0f,  -395.0f, -300.0f);
		glTexCoord2f(1.0f, 0.5f);
		glVertex3f(310.0f, 2.0f, -400.0f);

	glEnd();
	}

	glEnable(GL_DEPTH_TEST);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Function:	drawWater()
//
// Purpose:		This draws the water effect for the current level.
//
// Last Modified:  Monday, September 13, 2004
//
// Input:		None.
//
// Output:		None.
//
// Returns:		None.
//
//////////////////////////////////////////////////////////////////////////////////////////
void CDonkEffects::drawWater()
{
	// Get the change in time.
	float fCur = GetGameTimer();
	float elaps = fCur - m_fLastTime;

	// Find the distance the water traveled in that time.
	m_fWaterTexMovement += (elaps * 2);

	// Find the level of the water.
	float fWaterLevel = CLevelManager::GetInstance()->m_fWaterLevel;

	// Make sure this level needs water.
	if(fWaterLevel == NO_WATER)
	{
		// Start over.
		m_fLastTime = fCur;

		return;
	}

	// Draw the water.
	CBitmapManager::GetInstance()->BindTexture(CLevelManager::GetInstance()->iWaterTexture);
	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f+m_fWaterTexMovement, 0.0f);
		glVertex3f(-280.0f, fWaterLevel, -400.0f);
		glTexCoord2f(0.0f+m_fWaterTexMovement, 50.0f);
		glVertex3f(-280.0f, fWaterLevel, -CDisplayDonkey::GetInstance()->getCameraPos().tfZ);
		glTexCoord2f(50.0f+m_fWaterTexMovement, 50.0f);
		glVertex3f(280.0f, fWaterLevel, -CDisplayDonkey::GetInstance()->getCameraPos().tfZ);
		glTexCoord2f(50.0f+m_fWaterTexMovement, 0.0f);
		glVertex3f(280.0f, fWaterLevel, -400.0f);
	glEnd();

	// Start over.
	m_fLastTime = fCur;
}

void CDonkEffects::startEffects()
{
	m_fLastTime = GetGameTimer();
	m_fWaterTexMovement = 0.0f;
}
