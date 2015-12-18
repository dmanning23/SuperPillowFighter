#include "stdafx.h"
#include <memory.h>
#include "particleengine.h"
#include "..\..\..\GameTimer\GameTimer.h"
#include "..\..\..\ParticleEngine\RandomNumbers.h"

#define PARTICLE_GRAVITY 9.8f

//function to add a particle to an emitter
void CEmitter::AddParticle()
{
	//create a new particle
	if (iNumParticles < MAX_PARTICLES)
	{
		//place the particle at the emitter position
		memcpy(&tParticles[iNumParticles].tPosition, &tPosition, sizeof(Vec4DT));

		//set the particle velocity
		//This will set the particle velocity to a random value between the
		//min particle speed and max particle speed
		tParticles[iNumParticles].tVelocity.tfX = RandomFloat(m_EmitterStuff->tMinParticleVelocity.tfX, m_EmitterStuff->tMaxParticleVelocity.tfX);
		tParticles[iNumParticles].tVelocity.tfY = RandomFloat(m_EmitterStuff->tMinParticleVelocity.tfY, m_EmitterStuff->tMaxParticleVelocity.tfY);
		tParticles[iNumParticles].tVelocity.tfZ = RandomFloat(m_EmitterStuff->tMinParticleVelocity.tfZ, m_EmitterStuff->tMaxParticleVelocity.tfZ);

		//set the lifespan to 0.0f
		tParticles[iNumParticles].fLifespan = 0.0f;

		//set the size of the particle
		tParticles[iNumParticles].fSize = RandomFloat(m_EmitterStuff->fMinParticleSize, m_EmitterStuff->fMaxParticleSize);

		//set the initial alpha value of this particle
		tParticles[iNumParticles].fAlpha = m_EmitterStuff->fAlpha;

		//set the particle to active;
		tParticles[iNumParticles].bActive = true;

		iNumParticles++;
	}
}

CParticleEngine *CParticleEngine::m_pInstance = 0;

//Singleton shits
CParticleEngine *CParticleEngine::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = (CParticleEngine*)malloc(sizeof(CParticleEngine));
		m_pInstance->CParticleEngine::CParticleEngine();
	}
	return m_pInstance;
}

CParticleEngine::DeleteInstance()
{
	m_pInstance->CParticleEngine::~CParticleEngine();
	free(m_pInstance);
}

//Initialize the particle engine
void CParticleEngine::Init()
{
	////load up templates
	//FILE *pFile;

	//for (int i = 0; i < Num_ParticleEffects; i++)
	//{
	//	switch (i)
	//	{
	//		case Feathers_ParticleEffect:
	//			{
	//				pFile = fopen("ResourceFiles\\ParticleEffects\\Feathers.effect", "rb");
	//				break;
	//			}
	//		//case PlayerLand_ParticleEffect:
	//		//	{
	//		//		pFile = fopen("ResourceFiles\\ParticleEffects\\PlayerLand.effect", "rb");
	//		//		break;
	//		//	}
	//		//case PlayerDeath_ParticleEffect:
	//		//	{
	//		//		pFile = fopen("ResourceFiles\\ParticleEffects\\PlayerDeath.effect", "rb");
	//		//		break;
	//		//	}
	//		//case PlayerSplash_ParticleEffect:
	//		//	{
	//		//		pFile = fopen("ResourceFiles\\ParticleEffects\\PlayerSplash.effect", "rb");
	//		//		break;
	//		//	}
	//		//case PlayerHit_ParticleEffect:
	//		//	{
	//		//		pFile = fopen("ResourceFiles\\ParticleEffects\\PlayerHit.effect", "rb");
	//		//		break;
	//		//	}
	//		//case ItemHit_ParticleEffect:
	//		//	{
	//		//		pFile = fopen("ResourceFiles\\ParticleEffects\\ItemHit.effect", "rb");
	//		//		break;
	//		//	}
	//		//case PlayerSpawn_ParticleEffect:
	//		//	{
	//		//		pFile = fopen("ResourceFiles\\ParticleEffects\\PlayerSpawn.effect", "rb");
	//		//		break;
	//		//	}
	//		//case HealthPickup_ParticleEffect:
	//		//	{
	//		//		pFile = fopen("ResourceFiles\\ParticleEffects\\HealthPickup.effect", "rb");
	//		//		break;
	//		//	}
	//	}

	//	fread(&m_EmitterTemplates[i], sizeof(EmitterStuffT), 1, pFile);
	//	fclose(pFile);
	//}

	//set up initial values
	m_ActiveEmitterList = 0;
	m_fOldTime = GetGameTimer();
}

//Load the textures for the particle engine
void CParticleEngine::LoadTextures()
{
	for (int i = 0; i < Num_ParticleEffects; i++)
	{
		//m_EmitterTemplates[i].iBitmapID = CBitmapManager::GetInstance()->LoadImage(m_EmitterTemplates[i].strBmpFileName, true);
	}
}

//Cleanup the particle engine
void CParticleEngine::CleanUp()
{
	//clear out active emitters
	CEmitter *pIter = m_ActiveEmitterList;
	while (pIter)
	{
		CEmitter *pTemp = pIter->m_pNext;
		pIter->CEmitter::~CEmitter();
		free(pIter);
		pIter = pTemp;
	}
	m_ActiveEmitterList = 0;
}

//shutdown the particle engine
void CParticleEngine::Shutdown()
{
	CleanUp();
}

/*////////////////////////////////////////////////////////////////////////
//Function:	CreateEmitter
//
//Input:	
//	iEmitterType:	the id of the emitter to create
//	Position:		the start position of this emitter
//	Velocity:		the start velocity of this emitter
//	pAttachObject:	The object to attach this emitter to.  Pass 0 to ignore
//					If this is not 0, the emitter will follow the specified object
//
//Purpose:	Provide a way for other modules to add particle effects to the game
//
/////////////////////////////////////////////////////////////////////////*/
void CParticleEngine::CreateEmitter(Vec4DT *Position, Vec4DT *Velocity, EmitterStuffT *emitterType)
{
	//create new particle emitter
	CEmitter *pTemp = (CEmitter*)malloc(sizeof(CEmitter));
	memset(pTemp, 0, sizeof(CEmitter));
	memset(pTemp->tParticles, 0, (MAX_PARTICLES * sizeof(ParticleT)));
	pTemp->CEmitter::CEmitter(emitterType);
	//pTemp->pMyBaseObject = pAttachObject;

	////position emitter
	//if (pTemp->pMyBaseObject)
	//{
	//	getPosVec4DT(&pTemp->tPosition, pAttachObject->m_PandO);
	//	memcpy(&pTemp->tVelocity, &zeroVector, sizeof(Vec4DT));
	//}
	//else
	//{
		memcpy(&pTemp->tPosition, Position, sizeof(Vec4DT));
		memcpy(&pTemp->tVelocity, Velocity, sizeof(Vec4DT));
	//}

	//add to list of emitters, but use insertion sort to put contexts together
	bool bAdded = false;
	for (CEmitter *pIter = m_ActiveEmitterList; pIter; pIter = pIter->m_pNext)
	{
		if (pTemp->m_EmitterStuff->iBitmapID == pIter->m_EmitterStuff->iBitmapID)
		{
			if (pIter->m_pNext)
			{
				pIter->m_pNext->m_pPrev = pTemp;
			}
			pTemp->m_pNext = pIter->m_pNext;
			pIter->m_pNext = pTemp;
			pTemp->m_pPrev = pIter;
			bAdded = true;
			break;
		}
	}
	if (!bAdded)
	{
		//no emitters with this context, add to head of list
		pTemp->m_pNext = m_ActiveEmitterList;
		if (m_ActiveEmitterList)
		{
			if (m_ActiveEmitterList->m_pPrev)
			{
				m_ActiveEmitterList->m_pPrev->m_pNext = pTemp;
			}
			m_ActiveEmitterList->m_pPrev = pTemp;
		}
		m_ActiveEmitterList = pTemp;
	}

	//add the start particles
	for (int i = 0; i < pTemp->m_EmitterStuff->iNumStartParticles; i++)
	{
		pTemp->AddParticle();
	}

	//start timers
	pTemp->EmitterTimer.Start(pTemp->m_EmitterStuff->fEmitterLife);
	pTemp->CreationTimer.Start(pTemp->m_EmitterStuff->fCreationPeriod);
}

//Called every frame to update the particle engine
void CParticleEngine::Update()
{
	//float to hold the elapsed time from last frame
	float fElapsedTime = GetGameTimer() - m_fOldTime;
	if (fElapsedTime > 0.050f)
	{
		fElapsedTime = 0.050f;
	}

	//update all emitters
	CEmitter *pIter = m_ActiveEmitterList;
	while (pIter)
	{
		//add elapsed time to emitter check if this emitter has expired
		if (pIter->EmitterTimer.GetRemainingTime() <= 0.0f)
		{
			//remove the emitter from the list
			if (pIter->m_pPrev)
			{
				pIter->m_pPrev->m_pNext = pIter->m_pNext;
			}
			if (pIter->m_pNext)
			{
				pIter->m_pNext->m_pPrev = pIter->m_pPrev;
			}
			if (m_ActiveEmitterList == pIter)
			{
				m_ActiveEmitterList = pIter->m_pNext;
			}
			CEmitter *pTemp = pIter->m_pNext;
			pIter->CEmitter::~CEmitter();
			free(pIter);
			pIter = pTemp;
			continue;
		}

		//add elapsed time to particles and check for expiration
		//temp vec used to scale the particles velocity by the elapsed time
		Vec4DT scaledVec;
		for (int i = 0; i < MAX_PARTICLES; i++)
		{
			if (pIter->tParticles[i].bActive)
			{
				pIter->tParticles[i].fLifespan += fElapsedTime;

				//check if particle life has expired
				if (pIter->tParticles[i].fLifespan >= pIter->m_EmitterStuff->fParticleLife)
				{
					pIter->tParticles[i].bActive = false;

					//check if the array size is maxed out
					if (pIter->iNumParticles < MAX_PARTICLES)
					{
						//move all the rest of the particles up one
						memcpy(&pIter->tParticles[0], &pIter->tParticles[1], (sizeof(ParticleT) * pIter->iNumParticles));
					}
					else
					{
						//move the particles up one and set the last one to inactive
						memcpy(&pIter->tParticles[0], &pIter->tParticles[1], (sizeof(ParticleT) * (pIter->iNumParticles - 1)));

						pIter->tParticles[pIter->iNumParticles - 1].bActive = false;
					}

					//decrement the number of particles
					pIter->iNumParticles--;
				}
			
				//update particle position
				//get the velocity vector scaled by the elapsed time
				sMultVec4DT(&scaledVec, fElapsedTime, pIter->tParticles[i].tVelocity);
				addVec4DT(&pIter->tParticles[i].tPosition, scaledVec, pIter->tParticles[i].tPosition);

				//add gravity to velocity
				if (pIter->m_EmitterStuff->bParticleGrav)
				{
					pIter->tParticles[i].tVelocity.tfY -= (PARTICLE_GRAVITY * fElapsedTime);
				}
				if (pIter->m_EmitterStuff->bAntiGravity)
				{
					pIter->tParticles[i].tVelocity.tfY += (PARTICLE_GRAVITY * fElapsedTime);
				}

				//add sin value to velocity
				if (pIter->m_EmitterStuff->bSinValues)
				{
					pIter->tParticles[i].tPosition.tfX += ((float)sin(fElapsedTime) * 5.0f);
				}

				//add alpha value to particle
				pIter->tParticles[i].fAlpha -= (pIter->m_EmitterStuff->fFadeSpeed * fElapsedTime);
				if (pIter->tParticles[i].fAlpha < 0.0f)
				{
					pIter->tParticles[i].fAlpha = 0.0f;
				}
			}
		}
		
		////check if this should be attached to a base object
		//if (pIter->pMyBaseObject)
		//{
		//	getPosVec4DT(&pIter->tPosition, pIter->pMyBaseObject->m_PandO);
		//}
		//else
		//{
			//update emitter position
			sMultVec4DT(&scaledVec, fElapsedTime, pIter->tVelocity);
			addVec4DT(&pIter->tPosition, scaledVec, pIter->tPosition);

			//add gravity
			if (pIter->m_EmitterStuff->bEmitterGrav)
			{
				pIter->tVelocity.tfY -= (PARTICLE_GRAVITY * fElapsedTime);
			}
		//}

		//check if another particle needs to be emitted
		if (pIter->CreationTimer.GetRemainingTime() <= 0.0f)
		{
			pIter->AddParticle();
			pIter->CreationTimer.Start(pIter->m_EmitterStuff->fCreationPeriod);
		}

		pIter = pIter->m_pNext;
	}

	m_fOldTime = GetGameTimer();
}