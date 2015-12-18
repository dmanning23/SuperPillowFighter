//////////////////////////////////////////////////////////////////////////////////////////
// File Name:      levelGenerate.cpp
//
// Author:         Kevin Cecelski
//
// Date Created:   Thursday, August 20, 2004
//
// Purpose:		   Usede to manually create a level.	
// 
//////////////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <malloc.h>
#include "spfmath.h"
#include "..\..\Entities\LevelObject.h"

// Level file header information.
struct LevelHeaderT {
	unsigned int	tuNumLevObjs;	// Number of level objects.
	unsigned int	tuNumSpPoints;	// Number of spawn points.
	unsigned int	tuNumLights;	// Number of lights.
	unsigned int	tuNumBgs;		// Number of background images.
};

#define MAX_STRING_LENGTH 64

int main()
{
	//Temporary variable used to write to the file
	int iInt;

    // Write a header to the file.
	LevelHeaderT fileHeader;

	fileHeader.tuNumBgs = 1;		// Number of backgrounds.
	fileHeader.tuNumLevObjs = 1;	// Number of level objects.
	fileHeader.tuNumLights = 1;		// Number of lights.
	fileHeader.tuNumSpPoints = 4;	// Number of spawn points.

	FILE *pFile;	// New files.

	//TODO
	//add a background image to the level file info

	// open the file.
	pFile = fopen("..\\levels\\pirateBoat.spl", "wb");

	// Write the header information.
	fwrite(&fileHeader, sizeof(LevelHeaderT), 1, pFile);

	// Write the board boundaries.
	float bounds[4] = { -15.0f, 40.0f, 35.0f, -10.0f }; 
	fwrite(bounds, sizeof(float), 4, pFile);

	// Write all level objects.
	//Info is in this order:
	//int:			Type of level object
	//Matrix4x4T:	Position of object
	//repeat this pattern for each level object

	iInt = Boat_levelobj;
    fwrite(&iInt, sizeof(int), 1, pFile);
	Matrix4x4T temp = identityMatrix;
	temp.rowVecs[3].tfX = 5.0f;
	temp.rowVecs[3].tfY = -1.0f;
	temp.rowVecs[3].tfZ = 0.5f;
	fwrite(&temp, sizeof(Matrix4x4T), 1, pFile);

	//iInt = RubberDuck_levelobj;
	//fwrite(&iInt, sizeof(int), 1, pFile);
	//temp = identityMatrix;
	//temp.rowVecs[3].tfX = 40.0f;
	//temp.rowVecs[3].tfY = -1.0f;
	//temp.rowVecs[3].tfZ = 0.5f;
	//fwrite(&temp, sizeof(Matrix4x4T), 1, pFile);

	// Write spawn points.
	Vec4DT spPoints[4] = { {-4.0f,16.0f,0.0f}, {4.0f,16.0f,0.0f}, {9.0f,16.0f,0.0f}, {15.0f,16.0f,0.0f} };
	fwrite(spPoints, sizeof(Vec4DT), 4, pFile);

	// Write lights.
	Vec4DT lpPoints[1] = { {15.0f,11.0f,0.0f} };
	fwrite(lpPoints, sizeof(Vec4DT), 1, pFile);

	// Write background information.
	int iLayer = 0;
	fwrite(&iLayer, sizeof(int), 1, pFile);
	char fName[256] = {"ResourceFiles\\textures\\galaxy2.tga"};
	fwrite(fName, sizeof(char), 256, pFile);
	float wh[2] = { 2.7f, 8.9f };
	fwrite(wh, sizeof(float), 2, pFile);

	//Write the powerup time delta
	//This is how often powerup will be emitted in this level
	float myFloat = 2.1f;
	fwrite(&myFloat, 1, sizeof(float), pFile);

	//Number of background sound effects
	int iMyInt = 3;
	fwrite(&iMyInt, sizeof(int), 1, pFile);

	char strString[MAX_STRING_LENGTH];
	memset(strString, 0, MAX_STRING_LENGTH);
	strcpy(strString, "ResourceFiles\\Sounds\\Boat Creak Long.wav");
	fwrite((void*)strString, sizeof(char), MAX_STRING_LENGTH, pFile);
	memset(strString, 0, MAX_STRING_LENGTH);
	strcpy(strString, "ResourceFiles\\Sounds\\Flag 2.wav");
	fwrite((void*)strString, sizeof(char), MAX_STRING_LENGTH, pFile);
	memset(strString, 0, MAX_STRING_LENGTH);
	strcpy(strString, "ResourceFiles\\Sounds\\Wave Long.wav");
	fwrite((void*)strString, sizeof(char), MAX_STRING_LENGTH, pFile);

	//write the music file
	memset(strString, 0, MAX_STRING_LENGTH);
	strcpy(strString, "ResourceFiles\\Music\\pirate.ogg");
	fwrite((void*)strString, sizeof(char), MAX_STRING_LENGTH, pFile);

	// Write the limitations on the camera.
	float fCameraAng = SPF_PI/12.0f;
	fwrite(&fCameraAng, sizeof(float), 1, pFile);

	float fMaxBotHeight = 4.0f;
	fwrite(&fMaxBotHeight, sizeof(float), 1, pFile);

	float fZoomCap = -2.0f;
	fwrite(&fZoomCap, sizeof(float), 1, pFile);

	float fZoomOutCap = -38.0f;
	fwrite(&fZoomOutCap, sizeof(float), 1, pFile);

	// Level of the water.
	float fWaterLevel = 2.0f;;
	fwrite(&fWaterLevel, sizeof(float), 1, pFile);

	fclose(pFile);



	/// **********************
	//Temporary variable used to write to the file
	int iIntFW;

    // Write a header to the file.
	LevelHeaderT fileHeaderFW;

	fileHeaderFW.tuNumBgs = 1;		// Number of backgrounds.
	fileHeaderFW.tuNumLevObjs = 1;	// Number of level objects.
	fileHeaderFW.tuNumLights = 1;		// Number of lights.
	fileHeaderFW.tuNumSpPoints = 4;	// Number of spawn points.

	//TODO
	//add a background image to the level file info

	// open the file.
	pFile = fopen("..\\levels\\ferrisWheel.spl", "wb");

	// Write the header information.
	fwrite(&fileHeaderFW, sizeof(LevelHeaderT), 1, pFile);

	// Write the board boundaries.
	float boundsFW[4] = { -30.0f, 30.0f, 30.0f, -30.0f }; 
	fwrite(boundsFW, sizeof(float), 4, pFile);

	// Write all level objects.
	//Info is in this order:
	//int:			Type of level object
	//Matrix4x4T:	Position of object
	//repeat this pattern for each level object

	iIntFW = FerrisWheel_levelobj;
    fwrite(&iIntFW, sizeof(int), 1, pFile);
	Matrix4x4T tempFW = identityMatrix;
	tempFW.rowVecs[3].tfX = 0.0f;
	tempFW.rowVecs[3].tfY = 0.0f;
	tempFW.rowVecs[3].tfZ = -5.5f;
	fwrite(&tempFW, sizeof(Matrix4x4T), 1, pFile);

	//iInt = RubberDuck_levelobj;
	//fwrite(&iInt, sizeof(int), 1, pFile);
	//temp = identityMatrix;
	//temp.rowVecs[3].tfX = 40.0f;
	//temp.rowVecs[3].tfY = -1.0f;
	//temp.rowVecs[3].tfZ = 0.5f;
	//fwrite(&temp, sizeof(Matrix4x4T), 1, pFile);

	// Write spawn points.
	Vec4DT spPointsFW[4] = { {-4.0f,16.0f,0.0f}, {4.0f,16.0f,0.0f}, {9.0f,16.0f,0.0f}, {15.0f,16.0f,0.0f} };
	fwrite(spPointsFW, sizeof(Vec4DT), 4, pFile);

	// Write lights.
	Vec4DT lpPointsFW[1] = { {15.0f,11.0f,0.0f} };
	fwrite(lpPointsFW, sizeof(Vec4DT), 1, pFile);

	// Write background information.
	int iLayerFW = 0;
	fwrite(&iLayerFW, sizeof(int), 1, pFile);
	char fNameFW[256] = {"ResourceFiles\\textures\\galaxy1.tga"};
	fwrite(fNameFW, sizeof(char), 256, pFile);
	float whFW[2] = { 2.7f, 8.9f };
	fwrite(whFW, sizeof(float), 2, pFile);

	//Write the powerup time delta
	//This is how often powerup will be emitted in this level
	float myFloatFW = 0.0f;
	fwrite(&myFloatFW, 1, sizeof(float), pFile);

	//Number of background sound effects
	int iMyIntFW = 1;
	fwrite(&iMyIntFW, sizeof(int), 1, pFile);

	char strStringFW[MAX_STRING_LENGTH];
	strcpy(strStringFW, "ResourceFiles\\Sounds\\Boat Creak Long.wav");
	fwrite((void*)strStringFW, sizeof(char), MAX_STRING_LENGTH, pFile);

	//write the music file
	memset(strStringFW, 0, MAX_STRING_LENGTH);
	strcpy(strStringFW, "ResourceFiles\\Music\\bathboard.ogg");
	fwrite((void*)strStringFW, sizeof(char), MAX_STRING_LENGTH, pFile);

	// Write the limitations on the camera.
	float fCameraAngFW = SPF_PI/18.0f;
	fwrite(&fCameraAngFW, sizeof(float), 1, pFile);

	// Make the lower bottom bound not matter on this board.
	float fMaxBotHeightFW = boundsFW[2];
	fwrite(&fMaxBotHeightFW, sizeof(float), 1, pFile);

	float fZoomCapFW = -25.0f;
	fwrite(&fZoomCapFW, sizeof(float), 1, pFile);

	float fZoomOutCapFW = -55.0f;
	fwrite(&fZoomOutCapFW, sizeof(float), 1, pFile);

	// Level of the water.
	float fWaterLevelFW = NO_WATER;
	fwrite(&fWaterLevelFW, sizeof(float), 1, pFile);

	fclose(pFile);    

	return 0;
}