#include <stdio.h>
#include <cstring>
#include <memory.h>

#include "..\..\..\ParticleEngine\ParticleEngine.h"
#include "..\..\..\MathLibrary\SPFMath.h"

EmitterStuffT emitterArray[Num_ParticleEffects];

void main()
{
	FILE *pFile;

	/*
	Feathers_ParticleEffect,
    PlayerLand_ParticleEffect,
	PlayerDeath_ParticleEffect,
	PlayerSplash_ParticleEffect,
	PlayerHit_ParticleEffect,
	ItemHit_ParticleEffect,
	PlayerSpawn_ParticleEffect,
	HealthPickup_ParticleEffect,
	*/

	//Feathers_ParticleEffect
	{
		emitterArray[Feathers_ParticleEffect].bAntiGravity = false;
		emitterArray[Feathers_ParticleEffect].bEmitterGrav = false;
		emitterArray[Feathers_ParticleEffect].bParticleGrav = true;
		emitterArray[Feathers_ParticleEffect].bSinValues = false;
		emitterArray[Feathers_ParticleEffect].fAlpha = 1.0f;
		emitterArray[Feathers_ParticleEffect].fCreationPeriod = 1.0f;
		emitterArray[Feathers_ParticleEffect].fEmitterLife = 1.0f;
		emitterArray[Feathers_ParticleEffect].fFadeSpeed = 0.8f;
		emitterArray[Feathers_ParticleEffect].fMaxParticleSize = 1.0f;
		emitterArray[Feathers_ParticleEffect].fMinParticleSize = 0.5f;
		emitterArray[Feathers_ParticleEffect].fParticleLife = 1.0f;
		emitterArray[Feathers_ParticleEffect].iBitmapID = 0;
		emitterArray[Feathers_ParticleEffect].iNumStartParticles = 20;
		Vec4DT maxVel = { 5.0f, 8.0f, 15.0f };
		memcpy(&emitterArray[Feathers_ParticleEffect].tMaxParticleVelocity, &maxVel, sizeof(Vec4DT));
		Vec4DT minVel = { -5.0f, 0.0f, -15.0f };
		memcpy(&emitterArray[Feathers_ParticleEffect].tMinParticleVelocity, &minVel, sizeof(Vec4DT));
		strcpy(emitterArray[Feathers_ParticleEffect].strBmpFileName, "ResourceFiles\\textures\\Feather.tga");
	}

	//PlayerLand_ParticleEffect
	{
		emitterArray[PlayerLand_ParticleEffect].bAntiGravity = false;
		emitterArray[PlayerLand_ParticleEffect].bEmitterGrav = false;
		emitterArray[PlayerLand_ParticleEffect].bParticleGrav = true;
		emitterArray[PlayerLand_ParticleEffect].bSinValues = false;
		emitterArray[PlayerLand_ParticleEffect].fAlpha = 1.0f;
		emitterArray[PlayerLand_ParticleEffect].fCreationPeriod = 1.0f;
		emitterArray[PlayerLand_ParticleEffect].fEmitterLife = 1.0f;
		emitterArray[PlayerLand_ParticleEffect].fFadeSpeed = 3.0f;
		emitterArray[PlayerLand_ParticleEffect].fMaxParticleSize = 0.4f;
		emitterArray[PlayerLand_ParticleEffect].fMinParticleSize = 0.25f;
		emitterArray[PlayerLand_ParticleEffect].fParticleLife = 1.0f;
		emitterArray[PlayerLand_ParticleEffect].iBitmapID = 0;
		emitterArray[PlayerLand_ParticleEffect].iNumStartParticles = 15;
		Vec4DT maxVel = { 3.0f, 3.0f, 5.0f };
		memcpy(&emitterArray[PlayerLand_ParticleEffect].tMaxParticleVelocity, &maxVel, sizeof(Vec4DT));
		Vec4DT minVel = { -3.0f, 1.0f, -5.0f };
		memcpy(&emitterArray[PlayerLand_ParticleEffect].tMinParticleVelocity, &minVel, sizeof(Vec4DT));
		strcpy(emitterArray[PlayerLand_ParticleEffect].strBmpFileName, "ResourceFiles\\textures\\defaultparticle.tga");
	}

	//PlayerDeath_ParticleEffect
	{
		emitterArray[PlayerDeath_ParticleEffect].bAntiGravity = false;
		emitterArray[PlayerDeath_ParticleEffect].bEmitterGrav = false;
		emitterArray[PlayerDeath_ParticleEffect].bParticleGrav = false;
		emitterArray[PlayerDeath_ParticleEffect].bSinValues = false;
		emitterArray[PlayerDeath_ParticleEffect].fAlpha = 1.0f;
		emitterArray[PlayerDeath_ParticleEffect].fCreationPeriod = .054f;
		emitterArray[PlayerDeath_ParticleEffect].fEmitterLife = 0.7f;
		emitterArray[PlayerDeath_ParticleEffect].fFadeSpeed = 1.2f;
		emitterArray[PlayerDeath_ParticleEffect].fMaxParticleSize = 2.3f;
		emitterArray[PlayerDeath_ParticleEffect].fMinParticleSize = 1.3f;
		emitterArray[PlayerDeath_ParticleEffect].fParticleLife = 1.0f;
		emitterArray[PlayerDeath_ParticleEffect].iBitmapID = 0;
		emitterArray[PlayerDeath_ParticleEffect].iNumStartParticles = 1;
		Vec4DT maxVel = { 0.0f, 0.0f, 13.0f };
		memcpy(&emitterArray[PlayerDeath_ParticleEffect].tMaxParticleVelocity, &maxVel, sizeof(Vec4DT));
		Vec4DT minVel = { 0.0f, 0.0f, -13.0f };
		memcpy(&emitterArray[PlayerDeath_ParticleEffect].tMinParticleVelocity, &minVel, sizeof(Vec4DT));
		strcpy(emitterArray[PlayerDeath_ParticleEffect].strBmpFileName, "ResourceFiles\\textures\\Death.tga");
	}

	//PlayerSplash_ParticleEffect
	{
		emitterArray[PlayerSplash_ParticleEffect].bAntiGravity = false;
		emitterArray[PlayerSplash_ParticleEffect].bEmitterGrav = false;
		emitterArray[PlayerSplash_ParticleEffect].bParticleGrav = false;
		emitterArray[PlayerSplash_ParticleEffect].bSinValues = false;
		emitterArray[PlayerSplash_ParticleEffect].fAlpha = 1.0f;
		emitterArray[PlayerSplash_ParticleEffect].fCreationPeriod = 1.0f;
		emitterArray[PlayerSplash_ParticleEffect].fEmitterLife = 1.0f;
		emitterArray[PlayerSplash_ParticleEffect].fFadeSpeed = 0.8f;
		emitterArray[PlayerSplash_ParticleEffect].fMaxParticleSize = 0.5f;
		emitterArray[PlayerSplash_ParticleEffect].fMinParticleSize = 0.5f;
		emitterArray[PlayerSplash_ParticleEffect].fParticleLife = 1.0f;
		emitterArray[PlayerSplash_ParticleEffect].iBitmapID = 0;
		emitterArray[PlayerSplash_ParticleEffect].iNumStartParticles = 49;
		Vec4DT maxVel = { 20.0f, 20.0f, 20.0f };
		memcpy(&emitterArray[PlayerSplash_ParticleEffect].tMaxParticleVelocity, &maxVel, sizeof(Vec4DT));
		Vec4DT minVel = { -20.0f, -20.0f, -20.0f };
		memcpy(&emitterArray[PlayerSplash_ParticleEffect].tMinParticleVelocity, &minVel, sizeof(Vec4DT));
		strcpy(emitterArray[PlayerSplash_ParticleEffect].strBmpFileName, "ResourceFiles\\textures\\defaultparticle.tga");
	}

	//PlayerHit_ParticleEffect
	{
		emitterArray[PlayerHit_ParticleEffect].bAntiGravity = false;
		emitterArray[PlayerHit_ParticleEffect].bEmitterGrav = false;
		emitterArray[PlayerHit_ParticleEffect].bParticleGrav = false;
		emitterArray[PlayerHit_ParticleEffect].bSinValues = false;
		emitterArray[PlayerHit_ParticleEffect].fAlpha = 1.0f;
		emitterArray[PlayerHit_ParticleEffect].fCreationPeriod = .1f;
		emitterArray[PlayerHit_ParticleEffect].fEmitterLife = 0.7f;
		emitterArray[PlayerHit_ParticleEffect].fFadeSpeed = 1.8f;
		emitterArray[PlayerHit_ParticleEffect].fMaxParticleSize = 1.3f;
		emitterArray[PlayerHit_ParticleEffect].fMinParticleSize = 1.3f;
		emitterArray[PlayerHit_ParticleEffect].fParticleLife = 1.0f;
		emitterArray[PlayerHit_ParticleEffect].iBitmapID = 0;
		emitterArray[PlayerHit_ParticleEffect].iNumStartParticles = 1;
		Vec4DT maxVel = { 0.0f, 0.0f, 0.0f };
		memcpy(&emitterArray[PlayerHit_ParticleEffect].tMaxParticleVelocity, &maxVel, sizeof(Vec4DT));
		Vec4DT minVel = { 0.0f, 0.0f, 0.0f };
		memcpy(&emitterArray[PlayerHit_ParticleEffect].tMinParticleVelocity, &minVel, sizeof(Vec4DT));
		strcpy(emitterArray[PlayerHit_ParticleEffect].strBmpFileName, "ResourceFiles\\textures\\Puff.tga");
	}

	//ItemHit_ParticleEffect
	{
		emitterArray[ItemHit_ParticleEffect].bAntiGravity = false;
		emitterArray[ItemHit_ParticleEffect].bEmitterGrav = false;
		emitterArray[ItemHit_ParticleEffect].bParticleGrav = false;
		emitterArray[ItemHit_ParticleEffect].bSinValues = false;
		emitterArray[ItemHit_ParticleEffect].fAlpha = 1.0f;
		emitterArray[ItemHit_ParticleEffect].fCreationPeriod = 1.0f;
		emitterArray[ItemHit_ParticleEffect].fEmitterLife = 1.0f;
		emitterArray[ItemHit_ParticleEffect].fFadeSpeed = 2.6f;
		emitterArray[ItemHit_ParticleEffect].fMaxParticleSize = 0.75f;
		emitterArray[ItemHit_ParticleEffect].fMinParticleSize = 0.65f;
		emitterArray[ItemHit_ParticleEffect].fParticleLife = 1.0f;
		emitterArray[ItemHit_ParticleEffect].iBitmapID = 0;
		emitterArray[ItemHit_ParticleEffect].iNumStartParticles = 49;
		Vec4DT maxVel = { 15.0f, 15.0f, 20.0f };
		memcpy(&emitterArray[ItemHit_ParticleEffect].tMaxParticleVelocity, &maxVel, sizeof(Vec4DT));
		Vec4DT minVel = { -15.0f, -15.0f, -20.0f };
		memcpy(&emitterArray[ItemHit_ParticleEffect].tMinParticleVelocity, &minVel, sizeof(Vec4DT));
		strcpy(emitterArray[ItemHit_ParticleEffect].strBmpFileName, "ResourceFiles\\textures\\PowerupHit.tga");
	}

	//PlayerSpawn_ParticleEffect
	{
		emitterArray[PlayerSpawn_ParticleEffect].bAntiGravity = false;
		emitterArray[PlayerSpawn_ParticleEffect].bEmitterGrav = false;
		emitterArray[PlayerSpawn_ParticleEffect].bParticleGrav = false;
		emitterArray[PlayerSpawn_ParticleEffect].bSinValues = false;
		emitterArray[PlayerSpawn_ParticleEffect].fAlpha = 1.0f;
		emitterArray[PlayerSpawn_ParticleEffect].fCreationPeriod = 1.5f;
		emitterArray[PlayerSpawn_ParticleEffect].fEmitterLife = 1.5f;
		emitterArray[PlayerSpawn_ParticleEffect].fFadeSpeed = 1.6f;
		emitterArray[PlayerSpawn_ParticleEffect].fMaxParticleSize = 1.0f;
		emitterArray[PlayerSpawn_ParticleEffect].fMinParticleSize = 0.5f;
		emitterArray[PlayerSpawn_ParticleEffect].fParticleLife = 1.5f;
		emitterArray[PlayerSpawn_ParticleEffect].iBitmapID = 0;
		emitterArray[PlayerSpawn_ParticleEffect].iNumStartParticles = 49;
		Vec4DT maxVel = { 10.0f, 10.0f, 10.0f };
		memcpy(&emitterArray[PlayerSpawn_ParticleEffect].tMaxParticleVelocity, &maxVel, sizeof(Vec4DT));
		Vec4DT minVel = { -10.0f, -10.0f, -10.0f };
		memcpy(&emitterArray[PlayerSpawn_ParticleEffect].tMinParticleVelocity, &minVel, sizeof(Vec4DT));
		strcpy(emitterArray[PlayerSpawn_ParticleEffect].strBmpFileName, "ResourceFiles\\textures\\Smoke.tga");
	}

	//JosieSpawn_ParticleEffect
	{
		emitterArray[JosieSpawn_ParticleEffect].bAntiGravity = false;
		emitterArray[JosieSpawn_ParticleEffect].bEmitterGrav = false;
		emitterArray[JosieSpawn_ParticleEffect].bParticleGrav = false;
		emitterArray[JosieSpawn_ParticleEffect].bSinValues = false;
		emitterArray[JosieSpawn_ParticleEffect].fAlpha = 1.0f;
		emitterArray[JosieSpawn_ParticleEffect].fCreationPeriod = 1.5f;
		emitterArray[JosieSpawn_ParticleEffect].fEmitterLife = 1.5f;
		emitterArray[JosieSpawn_ParticleEffect].fFadeSpeed = 1.0f;
		emitterArray[JosieSpawn_ParticleEffect].fMaxParticleSize = 1.8f;
		emitterArray[JosieSpawn_ParticleEffect].fMinParticleSize = 1.3f;
		emitterArray[JosieSpawn_ParticleEffect].fParticleLife = 1.5f;
		emitterArray[JosieSpawn_ParticleEffect].iBitmapID = 0;
		emitterArray[JosieSpawn_ParticleEffect].iNumStartParticles = 17;
		Vec4DT maxVel = { 10.0f, 10.0f, 10.0f };
		memcpy(&emitterArray[JosieSpawn_ParticleEffect].tMaxParticleVelocity, &maxVel, sizeof(Vec4DT));
		Vec4DT minVel = { -10.0f, -10.0f, -10.0f };
		memcpy(&emitterArray[JosieSpawn_ParticleEffect].tMinParticleVelocity, &minVel, sizeof(Vec4DT));
		strcpy(emitterArray[JosieSpawn_ParticleEffect].strBmpFileName, "ResourceFiles\\textures\\JosieSpawn.tga");
	}

	//CaptainSallySpawn_ParticleEffect
	{
		emitterArray[CaptainSallySpawn_ParticleEffect].bAntiGravity = false;
		emitterArray[CaptainSallySpawn_ParticleEffect].bEmitterGrav = false;
		emitterArray[CaptainSallySpawn_ParticleEffect].bParticleGrav = false;
		emitterArray[CaptainSallySpawn_ParticleEffect].bSinValues = false;
		emitterArray[CaptainSallySpawn_ParticleEffect].fAlpha = 1.0f;
		emitterArray[CaptainSallySpawn_ParticleEffect].fCreationPeriod = 1.5f;
		emitterArray[CaptainSallySpawn_ParticleEffect].fEmitterLife = 1.5f;
		emitterArray[CaptainSallySpawn_ParticleEffect].fFadeSpeed = 1.0f;
		emitterArray[CaptainSallySpawn_ParticleEffect].fMaxParticleSize = 1.8f;
		emitterArray[CaptainSallySpawn_ParticleEffect].fMinParticleSize = 1.3f;
		emitterArray[CaptainSallySpawn_ParticleEffect].fParticleLife = 1.5f;
		emitterArray[CaptainSallySpawn_ParticleEffect].iBitmapID = 0;
		emitterArray[CaptainSallySpawn_ParticleEffect].iNumStartParticles = 17;
		Vec4DT maxVel = { 10.0f, 10.0f, 10.0f };
		memcpy(&emitterArray[CaptainSallySpawn_ParticleEffect].tMaxParticleVelocity, &maxVel, sizeof(Vec4DT));
		Vec4DT minVel = { -10.0f, -10.0f, -10.0f };
		memcpy(&emitterArray[CaptainSallySpawn_ParticleEffect].tMinParticleVelocity, &minVel, sizeof(Vec4DT));
		strcpy(emitterArray[CaptainSallySpawn_ParticleEffect].strBmpFileName, "ResourceFiles\\textures\\SallySpawn.tga");
	}

	//MissElizabethSpawn_ParticleEffect
	{
		emitterArray[MissElizabethSpawn_ParticleEffect].bAntiGravity = false;
		emitterArray[MissElizabethSpawn_ParticleEffect].bEmitterGrav = false;
		emitterArray[MissElizabethSpawn_ParticleEffect].bParticleGrav = false;
		emitterArray[MissElizabethSpawn_ParticleEffect].bSinValues = false;
		emitterArray[MissElizabethSpawn_ParticleEffect].fAlpha = 1.0f;
		emitterArray[MissElizabethSpawn_ParticleEffect].fCreationPeriod = 1.5f;
		emitterArray[MissElizabethSpawn_ParticleEffect].fEmitterLife = 1.5f;
		emitterArray[MissElizabethSpawn_ParticleEffect].fFadeSpeed = 1.0f;
		emitterArray[MissElizabethSpawn_ParticleEffect].fMaxParticleSize = 1.8f;
		emitterArray[MissElizabethSpawn_ParticleEffect].fMinParticleSize = 1.3f;
		emitterArray[MissElizabethSpawn_ParticleEffect].fParticleLife = 1.5f;
		emitterArray[MissElizabethSpawn_ParticleEffect].iBitmapID = 0;
		emitterArray[MissElizabethSpawn_ParticleEffect].iNumStartParticles = 17;
		Vec4DT maxVel = { 10.0f, 10.0f, 10.0f };
		memcpy(&emitterArray[MissElizabethSpawn_ParticleEffect].tMaxParticleVelocity, &maxVel, sizeof(Vec4DT));
		Vec4DT minVel = { -10.0f, -10.0f, -10.0f };
		memcpy(&emitterArray[MissElizabethSpawn_ParticleEffect].tMinParticleVelocity, &minVel, sizeof(Vec4DT));
		strcpy(emitterArray[MissElizabethSpawn_ParticleEffect].strBmpFileName, "ResourceFiles\\textures\\MissElizabethSpawn.tga");
	}

	//SumiSpawn_ParticleEffect
	{
		emitterArray[SumiSpawn_ParticleEffect].bAntiGravity = false;
		emitterArray[SumiSpawn_ParticleEffect].bEmitterGrav = false;
		emitterArray[SumiSpawn_ParticleEffect].bParticleGrav = false;
		emitterArray[SumiSpawn_ParticleEffect].bSinValues = false;
		emitterArray[SumiSpawn_ParticleEffect].fAlpha = 1.0f;
		emitterArray[SumiSpawn_ParticleEffect].fCreationPeriod = 1.5f;
		emitterArray[SumiSpawn_ParticleEffect].fEmitterLife = 1.5f;
		emitterArray[SumiSpawn_ParticleEffect].fFadeSpeed = 1.0f;
		emitterArray[SumiSpawn_ParticleEffect].fMaxParticleSize = 1.8f;
		emitterArray[SumiSpawn_ParticleEffect].fMinParticleSize = 1.3f;
		emitterArray[SumiSpawn_ParticleEffect].fParticleLife = 1.5f;
		emitterArray[SumiSpawn_ParticleEffect].iBitmapID = 0;
		emitterArray[SumiSpawn_ParticleEffect].iNumStartParticles = 17;
		Vec4DT maxVel = { 10.0f, 10.0f, 10.0f };
		memcpy(&emitterArray[SumiSpawn_ParticleEffect].tMaxParticleVelocity, &maxVel, sizeof(Vec4DT));
		Vec4DT minVel = { -10.0f, -10.0f, -10.0f };
		memcpy(&emitterArray[SumiSpawn_ParticleEffect].tMinParticleVelocity, &minVel, sizeof(Vec4DT));
		strcpy(emitterArray[SumiSpawn_ParticleEffect].strBmpFileName, "ResourceFiles\\textures\\SumiSpawn.tga");
	}

	//HealthPickup_ParticleEffect
	{
		emitterArray[HealthPickup_ParticleEffect].bAntiGravity = true;
		emitterArray[HealthPickup_ParticleEffect].bEmitterGrav = false;
		emitterArray[HealthPickup_ParticleEffect].bParticleGrav = false;
		emitterArray[HealthPickup_ParticleEffect].bSinValues = false;
		emitterArray[HealthPickup_ParticleEffect].fAlpha = 1.0f;
		emitterArray[HealthPickup_ParticleEffect].fCreationPeriod = .1f;
		emitterArray[HealthPickup_ParticleEffect].fEmitterLife = 0.5f;
		emitterArray[HealthPickup_ParticleEffect].fFadeSpeed = 2.5f;
		emitterArray[HealthPickup_ParticleEffect].fMaxParticleSize = 0.7f;
		emitterArray[HealthPickup_ParticleEffect].fMinParticleSize = 0.5f;
		emitterArray[HealthPickup_ParticleEffect].fParticleLife = 1.0f;
		emitterArray[HealthPickup_ParticleEffect].iBitmapID = 0;
		emitterArray[HealthPickup_ParticleEffect].iNumStartParticles = 5;
		Vec4DT maxVel = { 5.0f, 10.0f, 3.0f };
		memcpy(&emitterArray[HealthPickup_ParticleEffect].tMaxParticleVelocity, &maxVel, sizeof(Vec4DT));
		Vec4DT minVel = { -5.0f, -1.0f, -3.0f };
		memcpy(&emitterArray[HealthPickup_ParticleEffect].tMinParticleVelocity, &minVel, sizeof(Vec4DT));
		strcpy(emitterArray[HealthPickup_ParticleEffect].strBmpFileName, "ResourceFiles\\textures\\heart.tga");
	}

	//CannonShoot_ParticleEffect
	{
		emitterArray[CannonShoot_ParticleEffect].bAntiGravity = false;
		emitterArray[CannonShoot_ParticleEffect].bEmitterGrav = false;
		emitterArray[CannonShoot_ParticleEffect].bParticleGrav = false;
		emitterArray[CannonShoot_ParticleEffect].bSinValues = false;
		emitterArray[CannonShoot_ParticleEffect].fAlpha = 1.0f;
		emitterArray[CannonShoot_ParticleEffect].fCreationPeriod = .5f;
		emitterArray[CannonShoot_ParticleEffect].fEmitterLife = 0.5f;
		emitterArray[CannonShoot_ParticleEffect].fFadeSpeed = 2.5f;
		emitterArray[CannonShoot_ParticleEffect].fMaxParticleSize = 1.3f;
		emitterArray[CannonShoot_ParticleEffect].fMinParticleSize = 1.0f;
		emitterArray[CannonShoot_ParticleEffect].fParticleLife = .5f;
		emitterArray[CannonShoot_ParticleEffect].iBitmapID = 0;
		emitterArray[CannonShoot_ParticleEffect].iNumStartParticles = 15;
		Vec4DT maxVel = { 8.0f, 12.0f, 5.0f };
		memcpy(&emitterArray[CannonShoot_ParticleEffect].tMaxParticleVelocity, &maxVel, sizeof(Vec4DT));
		Vec4DT minVel = { -8.0f, 0.0f, 0.0f };
		memcpy(&emitterArray[CannonShoot_ParticleEffect].tMinParticleVelocity, &minVel, sizeof(Vec4DT));
		strcpy(emitterArray[CannonShoot_ParticleEffect].strBmpFileName, "ResourceFiles\\textures\\CannonShot.tga");
	}

	/*
	Feathers_ParticleEffect,
    PlayerLand_ParticleEffect,
	PlayerDeath_ParticleEffect,
	PlayerSplash_ParticleEffect,
	PlayerHit_ParticleEffect,
	ItemHit_ParticleEffect,
	PlayerSpawn_ParticleEffect,
	HealthPickup_ParticleEffect,
	*/

	pFile = fopen("..\\..\\ParticleEffects\\Feathers.effect", "wb");
	fwrite(&emitterArray[Feathers_ParticleEffect], sizeof(EmitterStuffT), 1, pFile);
	fclose(pFile);

	pFile = fopen("..\\..\\ParticleEffects\\PlayerLand.effect", "wb");
	fwrite(&emitterArray[PlayerLand_ParticleEffect], sizeof(EmitterStuffT), 1, pFile);
	fclose(pFile);

	pFile = fopen("..\\..\\ParticleEffects\\PlayerDeath.effect", "wb");
	fwrite(&emitterArray[PlayerDeath_ParticleEffect], sizeof(EmitterStuffT), 1, pFile);
	fclose(pFile);

	pFile = fopen("..\\..\\ParticleEffects\\PlayerSplash.effect", "wb");
	fwrite(&emitterArray[PlayerSplash_ParticleEffect], sizeof(EmitterStuffT), 1, pFile);
	fclose(pFile);

	pFile = fopen("..\\..\\ParticleEffects\\PlayerHit.effect", "wb");
	fwrite(&emitterArray[PlayerHit_ParticleEffect], sizeof(EmitterStuffT), 1, pFile);
	fclose(pFile);

	pFile = fopen("..\\..\\ParticleEffects\\ItemHit.effect", "wb");
	fwrite(&emitterArray[ItemHit_ParticleEffect], sizeof(EmitterStuffT), 1, pFile);
	fclose(pFile);

	pFile = fopen("..\\..\\ParticleEffects\\PlayerSpawn.effect", "wb");
	fwrite(&emitterArray[PlayerSpawn_ParticleEffect], sizeof(EmitterStuffT), 1, pFile);
	fclose(pFile);

	pFile = fopen("..\\..\\ParticleEffects\\JosieSpawn.effect", "wb");
	fwrite(&emitterArray[JosieSpawn_ParticleEffect], sizeof(EmitterStuffT), 1, pFile);
	fclose(pFile);

	pFile = fopen("..\\..\\ParticleEffects\\CaptainSallySpawn.effect", "wb");
	fwrite(&emitterArray[CaptainSallySpawn_ParticleEffect], sizeof(EmitterStuffT), 1, pFile);
	fclose(pFile);

	pFile = fopen("..\\..\\ParticleEffects\\MissElizabethSpawn.effect", "wb");
	fwrite(&emitterArray[MissElizabethSpawn_ParticleEffect], sizeof(EmitterStuffT), 1, pFile);
	fclose(pFile);

	pFile = fopen("..\\..\\ParticleEffects\\SumiSpawn.effect", "wb");
	fwrite(&emitterArray[SumiSpawn_ParticleEffect], sizeof(EmitterStuffT), 1, pFile);
	fclose(pFile);

	pFile = fopen("..\\..\\ParticleEffects\\HealthPickup.effect", "wb");
	fwrite(&emitterArray[HealthPickup_ParticleEffect], sizeof(EmitterStuffT), 1, pFile);
	fclose(pFile);

	pFile = fopen("..\\..\\ParticleEffects\\CannonShoot.effect", "wb");
	fwrite(&emitterArray[CannonShoot_ParticleEffect], sizeof(EmitterStuffT), 1, pFile);
	fclose(pFile);

}