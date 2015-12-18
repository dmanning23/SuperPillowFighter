#include <stdio.h>

#include "..\..\..\MathLibrary\spfmath.h"
#include "..\..\..\Entities\LevelObject.h"
#include "..\..\..\ParticleEngine\ParticleEngine.h"

#include "ObjectStats.h"

void main()
{
	//variables for writing out to a file
	int iInt;
	float fFloat;
	char *strString;
	bool bBool;
	
	//file pointer to write to
	FILE *pFile;

	//write boat info out to a file
	pFile = fopen("..\\..\\Boat.data", "wb");
	iInt = BOAT_NUMLINES;
	fwrite(&iInt, sizeof(int), 1, pFile);
	{
		Vec4DT ColLines[] = BOAT_LINES;
		fwrite(ColLines, (sizeof(Vec4DT)), (BOAT_NUMLINES * 2), pFile);
	}
	fFloat = BOAT_MAGNITUDE;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	fFloat = BOAT_WAVEPERIOD;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	fFloat = BOAT_MAXBOB;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	fFloat = BOAT_MINBOB;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	fFloat = BOAT_BOBPERIOD;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	strString = (char*)malloc(LEVELOBJ_STRING_LENGTH);
	memset(strString, 0, LEVELOBJ_STRING_LENGTH);
	strcpy(strString, BOAT_PLAYERLANDSOUNDINDEX);
	fwrite(strString, LEVELOBJ_STRING_LENGTH, 1, pFile);
	free(strString);
	strString = (char*)malloc(LEVELOBJ_STRING_LENGTH);
	memset(strString, 0, LEVELOBJ_STRING_LENGTH);
	strcpy(strString, BOAT_EMITPOWERUPSOUNDINDEX);
	fwrite(strString, LEVELOBJ_STRING_LENGTH, 1, pFile);
	free(strString);
	iInt = BOAT_MODELINDEX;
	fwrite(&iInt, sizeof(int), 1, pFile);
	{
	// Write the number of attachments.
	int fBoatNumAttach =	BOAT_NUM_ATTACH_OBJECTS;
	fwrite(&fBoatNumAttach, sizeof(int), 1, pFile);

#if(BOAT_NUM_ATTACH_OBJECTS)
	// Write their ids.
	int iAttachIDs[BOAT_NUM_ATTACH_OBJECTS] = BOAT_ATTACHEDOBJECTS;
	fwrite(iAttachIDs, sizeof(int), BOAT_NUM_ATTACH_OBJECTS, pFile);

	// Write their positions.
	Vec4DT vPositions[BOAT_NUM_ATTACH_OBJECTS] = BOAT_ATTACHEDOBJECTPOSITIONS;
	fwrite(vPositions, sizeof(Vec4DT), BOAT_NUM_ATTACH_OBJECTS, pFile);
#endif
	}
	//{
 //       int iIntArray[2] = BOAT_ATTACHEDOBJECTS;
	//	fwrite(iIntArray, sizeof(int), 2, pFile);
	//	Vec4DT AttachedPoints[] = BOAT_ATTACHEDOBJECTPOSITIONS;
	//	fwrite(AttachedPoints, sizeof(Vec4DT), 2, pFile);
	//}
    

	bBool = BOAT_HASEMITTER;
	fwrite(&bBool, sizeof(bool), 1, pFile);
	{
		Vec4DT Emitter = BOAT_EMITTERPOS;
		fwrite(&Emitter, sizeof(Vec4DT), 1, pFile);
	}
	iInt = BOAT_PARTICLE;
	fwrite(&iInt, sizeof(int), 1, pFile);

	Vec4DT BoatLeft = BOAT_LEFT_POINT;
	Vec4DT BoatRight = BOAT_RIGHT_POINT;

	// Output the left and right-most points on the boat.
	fwrite(&BoatLeft, sizeof(Vec4DT), 1, pFile);
	fwrite(&BoatRight, sizeof(Vec4DT), 1, pFile);
	
	fclose(pFile);

	//write WHALE info out to a file
	pFile = fopen("..\\..\\Whale.data", "wb");
	iInt = WHALE_NUMLINES;
	fwrite(&iInt, sizeof(int), 1, pFile);
	{
		Vec4DT ColLines[] = WHALE_LINES;
		fwrite(ColLines, (sizeof(Vec4DT)), WHALE_NUMLINES, pFile);
	}
	fFloat = WHALE_MAGNITUDE;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	fFloat = WHALE_WAVEPERIOD;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	fFloat = WHALE_MAXBOB;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	fFloat = WHALE_MINBOB;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	fFloat = WHALE_BOBPERIOD;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	strString = (char*)malloc(LEVELOBJ_STRING_LENGTH);
	memset(strString, 0, LEVELOBJ_STRING_LENGTH);
	strcpy(strString, WHALE_PLAYERLANDSOUNDINDEX);
	fwrite(strString, LEVELOBJ_STRING_LENGTH, 1, pFile);
	free(strString);
	strString = (char*)malloc(LEVELOBJ_STRING_LENGTH);
	memset(strString, 0, LEVELOBJ_STRING_LENGTH);
	strcpy(strString, WHALE_EMITPOWERUPSOUNDINDEX);
	fwrite(strString, LEVELOBJ_STRING_LENGTH, 1, pFile);
	free(strString);
	iInt = WHALE_MODELINDEX;
	fwrite(&iInt, sizeof(int), 1, pFile);
	{

	// Write the number of attachments.
	int fBoatNumAttach =	WHALE_NUM_ATTACH_OBJECTS;
	fwrite(&fBoatNumAttach, sizeof(int), 1, pFile);

#if(WHALE_NUM_ATTACH_OBJECTS)
	// Write their ids.
	int iAttachIDs[WHALE_NUM_ATTACH_OBJECTS] = WHALE_ATTACHEDOBJECTPOSITIONS;
	fwrite(iAttachIDs, sizeof(int), WHALE_NUM_ATTACH_OBJECTS, pFile);

	// Write their positions.
	Vec4DT vPositions[WHALE_NUM_ATTACH_OBJECTS] = WHALE_ATTACHEDOBJECTPOSITIONS;
	fwrite(vPositions, sizeof(Vec4DT), WHALE_NUM_ATTACH_OBJECTS, pFile);
#endif
	}
//	{
////        int iIntArray[2] = WHALE_ATTACHEDOBJECTS;
////		fwrite(iIntArray, sizeof(int), 2, pFile);
//		Vec4DT AttachedPoints[] = WHALE_ATTACHEDOBJECTPOSITIONS;
//		fwrite(AttachedPoints, sizeof(Vec4DT), 2, pFile);
//	}
	bBool = WHALE_HASEMITTER;
	fwrite(&bBool, sizeof(bool), 1, pFile);
	{
		Vec4DT Emitter = WHALE_EMITTERPOS;
		fwrite(&Emitter, sizeof(Vec4DT), 1, pFile);
	}
	iInt = WHALE_PARTICLE;
	fwrite(&iInt, sizeof(int), 1, pFile);

	Vec4DT WhaleLeft = WHALE_LEFT_POINT;
	Vec4DT WhaleRight = WHALE_RIGHT_POINT;

	// Output the left and right-most points on the whale.
	fwrite(&WhaleLeft, sizeof(Vec4DT), 1, pFile);
	fwrite(&WhaleRight, sizeof(Vec4DT), 1, pFile);

	fclose(pFile);

	//write CANNON info out to a file
	pFile = fopen("..\\..\\Cannon.data", "wb");
	iInt = CANNON_NUMLINES;
	fwrite(&iInt, sizeof(int), 1, pFile);
	{
		Vec4DT ColLines[] = CANNON_LINES;
		fwrite(ColLines, (sizeof(Vec4DT)), CANNON_NUMLINES, pFile);
	}
	fFloat = CANNON_MAGNITUDE;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	fFloat = CANNON_WAVEPERIOD;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	fFloat = CANNON_MAXBOB;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	fFloat = CANNON_MINBOB;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	fFloat = CANNON_BOBPERIOD;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	strString = (char*)malloc(LEVELOBJ_STRING_LENGTH);
	memset(strString, 0, LEVELOBJ_STRING_LENGTH);
	strcpy(strString, CANNON_PLAYERLANDSOUNDINDEX);
	fwrite(strString, LEVELOBJ_STRING_LENGTH, 1, pFile);
	free(strString);
	strString = (char*)malloc(LEVELOBJ_STRING_LENGTH);
	memset(strString, 0, LEVELOBJ_STRING_LENGTH);
	strcpy(strString, CANNON_EMITPOWERUPSOUNDINDEX);
	fwrite(strString, LEVELOBJ_STRING_LENGTH, 1, pFile);
	free(strString);
	iInt = CANNON_MODELINDEX;
	fwrite(&iInt, sizeof(int), 1, pFile);
	{
	// Write the number of attachments.
	int fBoatNumAttach =	CANNON_NUM_ATTACH_OBJECTS;
	fwrite(&fBoatNumAttach, sizeof(int), 1, pFile);

#if(CANNON_NUM_ATTACH_OBJECTS)
	// Write their ids.
	int iAttachIDs[CANNON_NUM_ATTACH_OBJECTS] = CANNON_ATTACHEDOBJECTS;
	fwrite(iAttachIDs, sizeof(int), CANNON_NUM_ATTACH_OBJECTS, pFile);

	// Write their positions.
	Vec4DT vPositions[CANNON_NUM_ATTACH_OBJECTS] = CANNON_ATTACHEDOBJECTPOSITIONS;
	fwrite(vPositions, sizeof(Vec4DT), CANNON_NUM_ATTACH_OBJECTS, pFile);
#endif
	}
	//{
 //       int iIntArray[2] = CANNON_ATTACHEDOBJECTS;
	//	fwrite(iIntArray, sizeof(int), 2, pFile);
	//	Vec4DT AttachedPoints[] = CANNON_ATTACHEDOBJECTPOSITIONS;
	//	fwrite(AttachedPoints, sizeof(Vec4DT), 2, pFile);
	//}

	bBool = CANNON_HASEMITTER;
	fwrite(&bBool, sizeof(bool), 1, pFile);
	{
		Vec4DT Emitter = CANNON_EMITTERPOS;
		fwrite(&Emitter, sizeof(Vec4DT), 1, pFile);
	}
	iInt = CANNON_PARTICLE;
	fwrite(&iInt, sizeof(int), 1, pFile);

	Vec4DT CannonLeft = CANNON_LEFT_POINT;
	Vec4DT CannonRight = CANNON_RIGHT_POINT;

	// Output the left and right-most points on the cannon.
	fwrite(&CannonLeft, sizeof(Vec4DT), 1, pFile);
	fwrite(&CannonRight, sizeof(Vec4DT), 1, pFile);

	fclose(pFile);

	//write RUBBERDUCK info out to a file
	pFile = fopen("..\\..\\RubberDuck.data", "wb");
	iInt = RUBBERDUCK_NUMLINES;
	fwrite(&iInt, sizeof(int), 1, pFile);
	{
		Vec4DT ColLines[] = RUBBERDUCK_LINES;
		fwrite(ColLines, (sizeof(Vec4DT)), (2 * RUBBERDUCK_NUMLINES), pFile);
	}
	fFloat = RUBBERDUCK_MAGNITUDE;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	fFloat = RUBBERDUCK_WAVEPERIOD;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	fFloat = RUBBERDUCK_MAXBOB;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	fFloat = RUBBERDUCK_MINBOB;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	fFloat = RUBBERDUCK_BOBPERIOD;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	strString = (char*)malloc(LEVELOBJ_STRING_LENGTH);
	memset(strString, 0, LEVELOBJ_STRING_LENGTH);
	strcpy(strString, RUBBERDUCK_PLAYERLANDSOUNDINDEX);
	fwrite(strString, LEVELOBJ_STRING_LENGTH, 1, pFile);
	free(strString);
	strString = (char*)malloc(LEVELOBJ_STRING_LENGTH);
	memset(strString, 0, LEVELOBJ_STRING_LENGTH);
	strcpy(strString, RUBBERDUCK_EMITPOWERUPSOUNDINDEX);
	fwrite(strString, LEVELOBJ_STRING_LENGTH, 1, pFile);
	free(strString);
	iInt = RUBBERDUCK_MODELINDEX;
	fwrite(&iInt, sizeof(int), 1, pFile);
	{
	// Write the number of attachments.
	int fBoatNumAttach =	RUBBERDUCK_NUM_ATTACH_OBJECTS;
	fwrite(&fBoatNumAttach, sizeof(int), 1, pFile);

#if(RUBBERDUCK_NUM_ATTACH_OBJECTS)
	// Write their ids.
	int iAttachIDs[RUBBERDUCK_NUM_ATTACH_OBJECTS] = RUBBERDUCK_ATTACHEDOBJECTS;
	fwrite(iAttachIDs, sizeof(int), RUBBERDUCK_NUM_ATTACH_OBJECTS, pFile);

	// Write their positions.
	Vec4DT vPositions[RUBBERDUCK_NUM_ATTACH_OBJECTS] = RUBBERDUCK_ATTACHEDOBJECTPOSITIONS;
	fwrite(vPositions, sizeof(Vec4DT), RUBBERDUCK_NUM_ATTACH_OBJECTS, pFile);
#endif
	}
	//{
 //       int iIntArray[2] = RUBBERDUCK_ATTACHEDOBJECTS;
	//	fwrite(iIntArray, sizeof(int), 2, pFile);
	//	Vec4DT AttachedPoints[] = RUBBERDUCK_ATTACHEDOBJECTPOSITIONS;
	//	fwrite(AttachedPoints, sizeof(Vec4DT), 2, pFile);
	//}
	bBool = RUBBERDUCK_HASEMITTER;
	fwrite(&bBool, sizeof(bool), 1, pFile);
	{
		Vec4DT Emitter = RUBBERDUCK_EMITTERPOS;
		fwrite(&Emitter, sizeof(Vec4DT), 1, pFile);
	}
	iInt = RUBBERDUCK_PARTICLE;
	fwrite(&iInt, sizeof(int), 1, pFile);

	Vec4DT RubberDuckLeft = RUBBERDUCK_LEFT_POINT;
	Vec4DT RubberDuckRight = RUBBERDUCK_RIGHT_POINT;

	// Output the left and right-most points on the duck.
	fwrite(&RubberDuckLeft, sizeof(Vec4DT), 1, pFile);
	fwrite(&RubberDuckRight, sizeof(Vec4DT), 1, pFile);

	fclose(pFile);

	//write SOAP info out to a file
	pFile = fopen("..\\..\\Soap.data", "wb");
	iInt = SOAP_NUMLINES;
	fwrite(&iInt, sizeof(int), 1, pFile);
	{
		Vec4DT ColLines[] = SOAP_LINES;
		fwrite(ColLines, (sizeof(Vec4DT)), SOAP_NUMLINES, pFile);
	}
	fFloat = SOAP_MAGNITUDE;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	fFloat = SOAP_WAVEPERIOD;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	fFloat = SOAP_MAXBOB;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	fFloat = SOAP_MINBOB;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	fFloat = SOAP_BOBPERIOD;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	strString = (char*)malloc(LEVELOBJ_STRING_LENGTH);
	memset(strString, 0, LEVELOBJ_STRING_LENGTH);
	strcpy(strString, SOAP_PLAYERLANDSOUNDINDEX);
	fwrite(strString, LEVELOBJ_STRING_LENGTH, 1, pFile);
	free(strString);
	strString = (char*)malloc(LEVELOBJ_STRING_LENGTH);
	memset(strString, 0, LEVELOBJ_STRING_LENGTH);
	strcpy(strString, SOAP_EMITPOWERUPSOUNDINDEX);
	fwrite(strString, LEVELOBJ_STRING_LENGTH, 1, pFile);
	free(strString);
	iInt = SOAP_MODELINDEX;
	fwrite(&iInt, sizeof(int), 1, pFile);
	{
	// Write the number of attachments.
	int fBoatNumAttach =	SOAP_NUM_ATTACH_OBJECTS;
	fwrite(&fBoatNumAttach, sizeof(int), 1, pFile);

#if(SOAP_NUM_ATTACH_OBJECTS)
	// Write their ids.
	int iAttachIDs[SOAP_NUM_ATTACH_OBJECTS] = SOAP_ATTACHEDOBJECTS;
	fwrite(iAttachIDs, sizeof(int), SOAP_NUM_ATTACH_OBJECTS, pFile);

	// Write their positions.
	Vec4DT vPositions[SOAP_NUM_ATTACH_OBJECTS] = SOAP_ATTACHEDOBJECTPOSITIONS;
	fwrite(vPositions, sizeof(Vec4DT), SOAP_NUM_ATTACH_OBJECTS, pFile);
#endif
	}
	//{
 //       int iIntArray[2] = SOAP_ATTACHEDOBJECTS;
	//	fwrite(iIntArray, sizeof(int), 2, pFile);
	//	Vec4DT AttachedPoints[] = SOAP_ATTACHEDOBJECTPOSITIONS;
	//	fwrite(AttachedPoints, sizeof(Vec4DT), 2, pFile);
	//}
	bBool = SOAP_HASEMITTER;
	fwrite(&bBool, sizeof(bool), 1, pFile);
	{
		Vec4DT Emitter = SOAP_EMITTERPOS;
		fwrite(&Emitter, sizeof(Vec4DT), 1, pFile);
	}
	iInt = SOAP_PARTICLE;
	fwrite(&iInt, sizeof(int), 1, pFile);

	Vec4DT SoapLeft = SOAP_LEFT_POINT;
	Vec4DT SoapRight = SOAP_RIGHT_POINT;

	// Output the left and right-most points on the soap.
	fwrite(&SoapLeft, sizeof(Vec4DT), 1, pFile);
	fwrite(&SoapRight, sizeof(Vec4DT), 1, pFile);

	fclose(pFile);

	//write FAUCET info out to a file
	pFile = fopen("..\\..\\Faucet.data", "wb");
	iInt = FAUCET_NUMLINES;
	fwrite(&iInt, sizeof(int), 1, pFile);
	{
		Vec4DT ColLines[] = FAUCET_LINES;
		fwrite(ColLines, (sizeof(Vec4DT)), FAUCET_NUMLINES, pFile);
	}
	fFloat = FAUCET_MAGNITUDE;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	fFloat = FAUCET_WAVEPERIOD;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	fFloat = FAUCET_MAXBOB;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	fFloat = FAUCET_MINBOB;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	fFloat = FAUCET_BOBPERIOD;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	strString = (char*)malloc(LEVELOBJ_STRING_LENGTH);
	memset(strString, 0, LEVELOBJ_STRING_LENGTH);
	strcpy(strString, FAUCET_PLAYERLANDSOUNDINDEX);
	fwrite(strString, LEVELOBJ_STRING_LENGTH, 1, pFile);
	free(strString);
	strString = (char*)malloc(LEVELOBJ_STRING_LENGTH);
	memset(strString, 0, LEVELOBJ_STRING_LENGTH);
	strcpy(strString, FAUCET_EMITPOWERUPSOUNDINDEX);
	fwrite(strString, LEVELOBJ_STRING_LENGTH, 1, pFile);
	free(strString);
	iInt = FAUCET_MODELINDEX;
	fwrite(&iInt, sizeof(int), 1, pFile);
	{
        int iIntArray[2] = FAUCET_ATTACHEDOBJECTS;
		fwrite(iIntArray, sizeof(int), 2, pFile);
		Vec4DT AttachedPoints[] = FAUCET_ATTACHEDOBJECTPOSITIONS;
		fwrite(AttachedPoints, sizeof(Vec4DT), 2, pFile);
	}
	bBool = FAUCET_HASEMITTER;
	fwrite(&bBool, sizeof(bool), 1, pFile);
	{
		Vec4DT Emitter = FAUCET_EMITTERPOS;
		fwrite(&Emitter, sizeof(Vec4DT), 1, pFile);
	}
	iInt = FAUCET_PARTICLE;
	fwrite(&iInt, sizeof(int), 1, pFile);
	
	Vec4DT FaucetLeft = FAUCET_LEFT_POINT;
	Vec4DT FaucetRight = FAUCET_RIGHT_POINT;

	// Output the left and right-most points on the faucet.
	fwrite(&FaucetLeft, sizeof(Vec4DT), 1, pFile);
	fwrite(&FaucetRight, sizeof(Vec4DT), 1, pFile);
	fclose(pFile);

	//write HANDLE info out to a file
	pFile = fopen("..\\..\\Handle.data", "wb");
	iInt = HANDLE_NUMLINES;
	fwrite(&iInt, sizeof(int), 1, pFile);
	{
		Vec4DT ColLines[] = HANDLE_LINES;
		fwrite(ColLines, (sizeof(Vec4DT)), HANDLE_NUMLINES, pFile);
	}
	fFloat = HANDLE_MAGNITUDE;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	fFloat = HANDLE_WAVEPERIOD;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	fFloat = HANDLE_MAXBOB;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	fFloat = HANDLE_MINBOB;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	fFloat = HANDLE_BOBPERIOD;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	strString = (char*)malloc(LEVELOBJ_STRING_LENGTH);
	memset(strString, 0, LEVELOBJ_STRING_LENGTH);
	strcpy(strString, HANDLE_PLAYERLANDSOUNDINDEX);
	fwrite(strString, LEVELOBJ_STRING_LENGTH, 1, pFile);
	free(strString);
	strString = (char*)malloc(LEVELOBJ_STRING_LENGTH);
	memset(strString, 0, LEVELOBJ_STRING_LENGTH);
	strcpy(strString, HANDLE_EMITPOWERUPSOUNDINDEX);
	fwrite(strString, LEVELOBJ_STRING_LENGTH, 1, pFile);
	free(strString);
	iInt = HANDLE_MODELINDEX;
	fwrite(&iInt, sizeof(int), 1, pFile);
	{
	// Write the number of attachments.
	int fBoatNumAttach =	HANDLE_NUM_ATTACH_OBJECTS;
	fwrite(&fBoatNumAttach, sizeof(int), 1, pFile);

#if(HANDLE_NUM_ATTACH_OBJECTS)
	// Write their ids.
	int iAttachIDs[HANDLE_NUM_ATTACH_OBJECTS] = HANDLE_ATTACHEDOBJECTS;
	fwrite(iAttachIDs, sizeof(int), HANDLE_NUM_ATTACH_OBJECTS, pFile);

	// Write their positions.
	Vec4DT vPositions[HANDLE_NUM_ATTACH_OBJECTS] = HANDLE_ATTACHEDOBJECTPOSITIONS;
	fwrite(vPositions, sizeof(Vec4DT), HANDLE_NUM_ATTACH_OBJECTS, pFile);
#endif
	}
	//{
 //       int iIntArray[2] = HANDLE_ATTACHEDOBJECTS;
	//	fwrite(iIntArray, sizeof(int), 2, pFile);
	//	Vec4DT AttachedPoints[] = HANDLE_ATTACHEDOBJECTPOSITIONS;
	//	fwrite(AttachedPoints, sizeof(Vec4DT), 2, pFile);
	//}
	bBool = HANDLE_HASEMITTER;
	fwrite(&bBool, sizeof(bool), 1, pFile);
	{
		Vec4DT Emitter = HANDLE_EMITTERPOS;
		fwrite(&Emitter, sizeof(Vec4DT), 1, pFile);
	}
	iInt = HANDLE_PARTICLE;
	fwrite(&iInt, sizeof(int), 1, pFile);
	Vec4DT HandleLeft = HANDLE_LEFT_POINT;
	Vec4DT HandleRight = HANDLE_RIGHT_POINT;

	// Output the left and right-most points on the handle.
	fwrite(&HandleLeft, sizeof(Vec4DT), 1, pFile);
	fwrite(&HandleRight, sizeof(Vec4DT), 1, pFile);
	fclose(pFile);

	//write WATERGUN info out to a file
	pFile = fopen("..\\..\\WaterGun.data", "wb");
	iInt = WATERGUN_NUMLINES;
	fwrite(&iInt, sizeof(int), 1, pFile);
	{
		Vec4DT ColLines[] = WATERGUN_LINES;
		fwrite(ColLines, (sizeof(Vec4DT)), WATERGUN_NUMLINES, pFile);
	}
	fFloat = WATERGUN_MAGNITUDE;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	fFloat = WATERGUN_WAVEPERIOD;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	fFloat = WATERGUN_MAXBOB;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	fFloat = WATERGUN_MINBOB;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	fFloat = WATERGUN_BOBPERIOD;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	strString = (char*)malloc(LEVELOBJ_STRING_LENGTH);
	memset(strString, 0, LEVELOBJ_STRING_LENGTH);
	strcpy(strString, WATERGUN_PLAYERLANDSOUNDINDEX);
	fwrite(strString, LEVELOBJ_STRING_LENGTH, 1, pFile);
	free(strString);
	strString = (char*)malloc(LEVELOBJ_STRING_LENGTH);
	memset(strString, 0, LEVELOBJ_STRING_LENGTH);
	strcpy(strString, WATERGUN_EMITPOWERUPSOUNDINDEX);
	fwrite(strString, LEVELOBJ_STRING_LENGTH, 1, pFile);
	free(strString);
	iInt = WATERGUN_MODELINDEX;
	{

	// Write the number of attachments.
	int fBoatNumAttach =	WATERGUN_NUM_ATTACH_OBJECTS;
	fwrite(&fBoatNumAttach, sizeof(int), 1, pFile);

#if(WATERGUN_NUM_ATTACH_OBJECTS)

	// Write their ids.
	int iAttachIDs[WATERGUN_NUM_ATTACH_OBJECTS] = WATERGUN_ATTACHEDOBJECTS;
	fwrite(iAttachIDs, sizeof(int), WATERGUN_NUM_ATTACH_OBJECTS, pFile);

	// Write their positions.
	Vec4DT vPositions[WATERGUN_NUM_ATTACH_OBJECTS] = WATERGUN_ATTACHEDOBJECTPOSITIONS;
	fwrite(vPositions, sizeof(Vec4DT), WATERGUN_NUM_ATTACH_OBJECTS, pFile);
#endif
	}
	//fwrite(&iInt, sizeof(int), 1, pFile);
	//{
 //       int iIntArray[2] = WATERGUN_ATTACHEDOBJECTS;
	//	fwrite(iIntArray, sizeof(int), 2, pFile);
	//	Vec4DT AttachedPoints[] = WATERGUN_ATTACHEDOBJECTPOSITIONS;
	//	fwrite(AttachedPoints, sizeof(Vec4DT), 2, pFile);
	//}
	bBool = WATERGUN_HASEMITTER;
	fwrite(&bBool, sizeof(bool), 1, pFile);
	{
		Vec4DT Emitter = WATERGUN_EMITTERPOS;
		fwrite(&Emitter, sizeof(Vec4DT), 1, pFile);
	}
	iInt = WATERGUN_PARTICLE;
	fwrite(&iInt, sizeof(int), 1, pFile);

	Vec4DT WaterGunLeft = WATERGUN_LEFT_POINT;
	Vec4DT WaterGunRight = WATERGUN_RIGHT_POINT;

	// Output the left and right-most points on the gun.
	fwrite(&WaterGunLeft, sizeof(Vec4DT), 1, pFile);
	fwrite(&WaterGunRight, sizeof(Vec4DT), 1, pFile);
	fclose(pFile);




























	//*********************



	//write ferris wheel RUBBERDUCK info out to a file
	pFile = fopen("..\\..\\RubberDuckWheel.data", "wb");
	iInt = RUBBERDUCK_NUMLINES;
	fwrite(&iInt, sizeof(int), 1, pFile);
	{
		Vec4DT ColLines[] = RUBBERDUCK_LINES;
		fwrite(ColLines, (sizeof(Vec4DT)), (2 * RUBBERDUCK_NUMLINES), pFile);
	}
	fFloat = RUBBERDUCKWHEEL_MAGNITUDE;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	fFloat = RUBBERDUCKWHEEL_WAVEPERIOD;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	fFloat = RUBBERDUCK_MAXBOB;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	fFloat = RUBBERDUCK_MINBOB;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	fFloat = RUBBERDUCK_BOBPERIOD;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	strString = (char*)malloc(LEVELOBJ_STRING_LENGTH);
	memset(strString, 0, LEVELOBJ_STRING_LENGTH);
	strcpy(strString, RUBBERDUCK_PLAYERLANDSOUNDINDEX);
	fwrite(strString, LEVELOBJ_STRING_LENGTH, 1, pFile);
	free(strString);
	strString = (char*)malloc(LEVELOBJ_STRING_LENGTH);
	memset(strString, 0, LEVELOBJ_STRING_LENGTH);
	strcpy(strString, RUBBERDUCK_EMITPOWERUPSOUNDINDEX);
	fwrite(strString, LEVELOBJ_STRING_LENGTH, 1, pFile);
	free(strString);
	iInt = RUBBERDUCK_MODELINDEX;
	fwrite(&iInt, sizeof(int), 1, pFile);
	{
	// Write the number of attachments.
	int fBoatNumAttach =	RUBBERDUCK_NUM_ATTACH_OBJECTS;
	fwrite(&fBoatNumAttach, sizeof(int), 1, pFile);

#if(RUBBERDUCK_NUM_ATTACH_OBJECTS)
	// Write their ids.
	int iAttachIDs[RUBBERDUCK_NUM_ATTACH_OBJECTS] = RUBBERDUCK_ATTACHEDOBJECTS;
	fwrite(iAttachIDs, sizeof(int), RUBBERDUCK_NUM_ATTACH_OBJECTS, pFile);

	// Write their positions.
	Vec4DT vPositions[RUBBERDUCK_NUM_ATTACH_OBJECTS] = RUBBERDUCK_ATTACHEDOBJECTPOSITIONS;
	fwrite(vPositions, sizeof(Vec4DT), RUBBERDUCK_NUM_ATTACH_OBJECTS, pFile);
#endif
	}
	//{
 //       int iIntArray[2] = RUBBERDUCK_ATTACHEDOBJECTS;
	//	fwrite(iIntArray, sizeof(int), 2, pFile);
	//	Vec4DT AttachedPoints[] = RUBBERDUCK_ATTACHEDOBJECTPOSITIONS;
	//	fwrite(AttachedPoints, sizeof(Vec4DT), 2, pFile);
	//}
	bBool = RUBBERDUCK_HASEMITTER;
	fwrite(&bBool, sizeof(bool), 1, pFile);
	{
		Vec4DT Emitter = RUBBERDUCK_EMITTERPOS;
		fwrite(&Emitter, sizeof(Vec4DT), 1, pFile);
	}
	iInt = RUBBERDUCK_PARTICLE;
	fwrite(&iInt, sizeof(int), 1, pFile);

	// Output the left and right-most points on the duck.
	fwrite(&RubberDuckLeft, sizeof(Vec4DT), 1, pFile);
	fwrite(&RubberDuckRight, sizeof(Vec4DT), 1, pFile);

	fclose(pFile);





	//write ferris wheel RUBBERDUCK info out to a file
	pFile = fopen("..\\..\\ferrisWheel.data", "wb");
	iInt = WHEEL_NUMLINES;
	fwrite(&iInt, sizeof(int), 1, pFile);
	{
		Vec4DT ColLines[] = WHEEL_LINES;
		fwrite(ColLines, (sizeof(Vec4DT)), (2 * WHEEL_NUMLINES), pFile);
	}
	fFloat = WHEEL_MAGNITUDE;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	fFloat = WHEEL_WAVEPERIOD;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	fFloat = WHEEL_MAXBOB;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	fFloat = WHEEL_MINBOB;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	fFloat = WHEEL_BOBPERIOD;
	fwrite(&fFloat, sizeof(float), 1, pFile);
	strString = (char*)malloc(LEVELOBJ_STRING_LENGTH);
	memset(strString, 0, LEVELOBJ_STRING_LENGTH);
	strcpy(strString, WHEEL_PLAYERLANDSOUNDINDEX);
	fwrite(strString, LEVELOBJ_STRING_LENGTH, 1, pFile);
	free(strString);
	strString = (char*)malloc(LEVELOBJ_STRING_LENGTH);
	memset(strString, 0, LEVELOBJ_STRING_LENGTH);
	strcpy(strString, WHEEL_EMITPOWERUPSOUNDINDEX);
	fwrite(strString, LEVELOBJ_STRING_LENGTH, 1, pFile);
	free(strString);
	iInt = WHEEL_MODELINDEX;
	fwrite(&iInt, sizeof(int), 1, pFile);
	{
	// Write the number of attachments.
	int fBoatNumAttach = WHEEL_NUM_ATTACH_OBJECTS;
	fwrite(&fBoatNumAttach, sizeof(int), 1, pFile);

#if(WHEEL_NUM_ATTACH_OBJECTS)
	// Write their ids.
	int iAttachIDs[WHEEL_NUM_ATTACH_OBJECTS] = WHEEL_ATTACHEDOBJECTS;
	fwrite(iAttachIDs, sizeof(int), WHEEL_NUM_ATTACH_OBJECTS, pFile);

	// Write their positions.
	Vec4DT vPositions[WHEEL_NUM_ATTACH_OBJECTS] = WHEEL_ATTACHEDOBJECTPOSITIONS;
	fwrite(vPositions, sizeof(Vec4DT), WHEEL_NUM_ATTACH_OBJECTS, pFile);
#endif
	}
	//{
 //       int iIntArray[2] = WHEEL_ATTACHEDOBJECTS;
	//	fwrite(iIntArray, sizeof(int), 2, pFile);
	//	Vec4DT AttachedPoints[] = WHEEL_ATTACHEDOBJECTPOSITIONS;
	//	fwrite(AttachedPoints, sizeof(Vec4DT), 2, pFile);
	//}
	bBool = WHEEL_HASEMITTER;
	fwrite(&bBool, sizeof(bool), 1, pFile);
	{
		Vec4DT Emitter = WHEEL_EMITTERPOS;
		fwrite(&Emitter, sizeof(Vec4DT), 1, pFile);
	}
	iInt = WHEEL_PARTICLE;
	fwrite(&iInt, sizeof(int), 1, pFile);

	// Output the left and right-most points on the duck.
	fwrite(&RubberDuckLeft, sizeof(Vec4DT), 1, pFile);
	fwrite(&RubberDuckRight, sizeof(Vec4DT), 1, pFile);

	fclose(pFile);
};