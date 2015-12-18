#include <stdio.h>
#include <memory.h>
#include <malloc.h>

#include "GameStats.h"

#define MAX_STRING_LENGTH 64

void main()
{
	FILE *pFile;

	//generic pointers for writing to file
	int *pInt = (int*)malloc(sizeof(int));
	float *pFloat = (float*)malloc(sizeof(float));
	bool *pBool = (bool*)malloc(sizeof(bool));
	char strString[MAX_STRING_LENGTH];

	//write window data out to a file
	pFile = fopen("..\\..\\Window.data", "wb");
	*pBool = WINDOWED_MODE;
	fwrite(pBool, sizeof(bool), 1, pFile);
	*pInt = WINDOW_WIDTH;
	fwrite(pInt, sizeof(int), 1, pFile);
	*pInt = WINDOW_HEIGHT;
	fwrite(pInt, sizeof(int), 1, pFile);
	*pBool = ARCADE_MODE;
	fwrite(pBool, sizeof(bool), 1, pFile);
	fclose(pFile);

	//write collision data information out to a file
	pFile = fopen("..\\..\\Collision.data", "wb");
	*pFloat = CENTER_POINT;
	fwrite(pFloat, sizeof(float), 1, pFile);
	*pFloat = CENTER_RADIUS;
	fwrite(pFloat, sizeof(float), 1, pFile);
	*pFloat = ENTIRE_RADIUS;
	fwrite(pFloat, sizeof(float), 1, pFile);
	*pFloat = TOP_CENTER_POINT;
	fwrite(pFloat, sizeof(float), 1, pFile);
	*pFloat = TOP_RADIUS;
	fwrite(pFloat, sizeof(float), 1, pFile);
	*pFloat = BOTTOM_CENTER_POINT;
	fwrite(pFloat, sizeof(float), 1, pFile);
	*pFloat = BOTTOM_RADIUS;
	fwrite(pFloat, sizeof(float), 1, pFile);
	*pFloat = ITEM_RADIUS;
	fwrite(pFloat, sizeof(float), 1, pFile);
	*pFloat = PILLOW_RADIUS;
	fwrite(pFloat, sizeof(float), 1, pFile);
	fclose(pFile);

	//write josie information out to a file
	pFile = fopen("..\\..\\Josie.data", "wb");
	*pFloat = JOSIE_ATTACKSPEED;
	fwrite(pFloat, sizeof(float), 1, pFile);
	*pFloat = JOSIE_WALKSPEED;
	fwrite(pFloat, sizeof(float), 1, pFile);
	*pFloat = JOSIE_JUMPHEIGHT;
	fwrite(pFloat, sizeof(float), 1, pFile);
	*pInt = JOSIE_MASS;
	fwrite(pInt, sizeof(int), 1, pFile);
	*pInt = JOSIE_ATTACKPOWER;
	fwrite(pInt, sizeof(int), 1, pFile);
	*pInt = JOSIE_GEOMETRY_DATA;
	fwrite(pInt, sizeof(int), 1, pFile);
	*pInt = JOSIE_ANIMATION_DATA;
	fwrite(pInt, sizeof(int), 1, pFile);
	*pInt = JOSIE_PILLOW_DATA;
	fwrite(pInt, sizeof(int), 1, pFile);
	memset(strString, 0, MAX_STRING_LENGTH);
	strcpy(strString, JOSIE_PICKUPSOUND);
	fwrite(strString, MAX_STRING_LENGTH, 1, pFile);
	memset(strString, 0, MAX_STRING_LENGTH);
	strcpy(strString, JOSIE_VICTORYSOUND);
	fwrite(strString, MAX_STRING_LENGTH, 1, pFile);
	memset(strString, 0, MAX_STRING_LENGTH);
	strcpy(strString, JOSIE_TAUNTSOUND);
	fwrite(strString, MAX_STRING_LENGTH, 1, pFile);
	memset(strString, 0, MAX_STRING_LENGTH);
	strcpy(strString, JOSIE_HITSOUND);
	fwrite(strString, MAX_STRING_LENGTH, 1, pFile);
	memset(strString, 0, MAX_STRING_LENGTH);
	strcpy(strString, JOSIE_GOTHIT);
	fwrite(strString, MAX_STRING_LENGTH, 1, pFile);
	fclose(pFile);

	//write CAPTAINSALLY information out to a file
	pFile = fopen("..\\..\\CaptainSally.data", "wb");
	*pFloat = CAPTAINSALLY_ATTACKSPEED;
	fwrite(pFloat, sizeof(float), 1, pFile);
	*pFloat = CAPTAINSALLY_WALKSPEED;
	fwrite(pFloat, sizeof(float), 1, pFile);
	*pFloat = CAPTAINSALLY_JUMPHEIGHT;
	fwrite(pFloat, sizeof(float), 1, pFile);
	*pInt = CAPTAINSALLY_MASS;
	fwrite(pInt, sizeof(int), 1, pFile);
	*pInt = CAPTAINSALLY_ATTACKPOWER;
	fwrite(pInt, sizeof(int), 1, pFile);
	*pInt = CAPTAINSALLY_GEOMETRY_DATA;
	fwrite(pInt, sizeof(int), 1, pFile);
	*pInt = CAPTAINSALLY_ANIMATION_DATA;
	fwrite(pInt, sizeof(int), 1, pFile);
	*pInt = CAPTAINSALLY_PILLOW_DATA;
	fwrite(pInt, sizeof(int), 1, pFile);
	memset(strString, 0, MAX_STRING_LENGTH);
	strcpy(strString, CAPTAINSALLY_PICKUPSOUND);
	fwrite(strString, MAX_STRING_LENGTH, 1, pFile);
	memset(strString, 0, MAX_STRING_LENGTH);
	strcpy(strString, CAPTAINSALLY_VICTORYSOUND);
	fwrite(strString, MAX_STRING_LENGTH, 1, pFile);
	memset(strString, 0, MAX_STRING_LENGTH);
	strcpy(strString, CAPTAINSALLY_TAUNTSOUND);
	fwrite(strString, MAX_STRING_LENGTH, 1, pFile);
	memset(strString, 0, MAX_STRING_LENGTH);
	strcpy(strString, CAPTAINSALLY_HITSOUND);
	fwrite(strString, MAX_STRING_LENGTH, 1, pFile);
	memset(strString, 0, MAX_STRING_LENGTH);
	strcpy(strString, CAPTAINSALLY_GOTHIT);
	fwrite(strString, MAX_STRING_LENGTH, 1, pFile);
	fclose(pFile);

	//write MISSELIZABETH information out to a file
	pFile = fopen("..\\..\\MissElizabeth.data", "wb");
	*pFloat = MISSELIZABETH_ATTACKSPEED;
	fwrite(pFloat, sizeof(float), 1, pFile);
	*pFloat = MISSELIZABETH_WALKSPEED;
	fwrite(pFloat, sizeof(float), 1, pFile);
	*pFloat = MISSELIZABETH_JUMPHEIGHT;
	fwrite(pFloat, sizeof(float), 1, pFile);
	*pInt = MISSELIZABETH_MASS;
	fwrite(pInt, sizeof(int), 1, pFile);
	*pInt = MISSELIZABETH_ATTACKPOWER;
	fwrite(pInt, sizeof(int), 1, pFile);
	*pInt = MISSELIZABETH_GEOMETRY_DATA;
	fwrite(pInt, sizeof(int), 1, pFile);
	*pInt = MISSELIZABETH_ANIMATION_DATA;
	fwrite(pInt, sizeof(int), 1, pFile);
	*pInt = MISSELIZABETH_PILLOW_DATA;
	fwrite(pInt, sizeof(int), 1, pFile);
	memset(strString, 0, MAX_STRING_LENGTH);
	strcpy(strString, MISSELIZABETH_PICKUPSOUND);
	fwrite(strString, MAX_STRING_LENGTH, 1, pFile);
	memset(strString, 0, MAX_STRING_LENGTH);
	strcpy(strString, MISSELIZABETH_VICTORYSOUND);
	fwrite(strString, MAX_STRING_LENGTH, 1, pFile);
	memset(strString, 0, MAX_STRING_LENGTH);
	strcpy(strString, MISSELIZABETH_TAUNTSOUND);
	fwrite(strString, MAX_STRING_LENGTH, 1, pFile);
	memset(strString, 0, MAX_STRING_LENGTH);
	strcpy(strString, MISSELIZABETH_HITSOUND);
	fwrite(strString, MAX_STRING_LENGTH, 1, pFile);
	memset(strString, 0, MAX_STRING_LENGTH);
	strcpy(strString, MISSELIZABETH_GOTHIT);
	fwrite(strString, MAX_STRING_LENGTH, 1, pFile);
	fclose(pFile);

	//write SUMI information out to a file
	pFile = fopen("..\\..\\Sumi.data", "wb");
	*pFloat = SUMI_ATTACKSPEED;
	fwrite(pFloat, sizeof(float), 1, pFile);
	*pFloat = SUMI_WALKSPEED;
	fwrite(pFloat, sizeof(float), 1, pFile);
	*pFloat = SUMI_JUMPHEIGHT;
	fwrite(pFloat, sizeof(float), 1, pFile);
	*pInt = SUMI_MASS;
	fwrite(pInt, sizeof(int), 1, pFile);
	*pInt = SUMI_ATTACKPOWER;
	fwrite(pInt, sizeof(int), 1, pFile);
	*pInt = SUMI_GEOMETRY_DATA;
	fwrite(pInt, sizeof(int), 1, pFile);
	*pInt = SUMI_ANIMATION_DATA;
	fwrite(pInt, sizeof(int), 1, pFile);
	*pInt = SUMI_PILLOW_DATA;
	fwrite(pInt, sizeof(int), 1, pFile);
	memset(strString, 0, MAX_STRING_LENGTH);
	strcpy(strString, SUMI_PICKUPSOUND);
	fwrite(strString, MAX_STRING_LENGTH, 1, pFile);
	memset(strString, 0, MAX_STRING_LENGTH);
	strcpy(strString, SUMI_VICTORYSOUND);
	fwrite(strString, MAX_STRING_LENGTH, 1, pFile);
	memset(strString, 0, MAX_STRING_LENGTH);
	strcpy(strString, SUMI_TAUNTSOUND);
	fwrite(strString, MAX_STRING_LENGTH, 1, pFile);
	memset(strString, 0, MAX_STRING_LENGTH);
	strcpy(strString, SUMI_HITSOUND);
	fwrite(strString, MAX_STRING_LENGTH, 1, pFile);
	memset(strString, 0, MAX_STRING_LENGTH);
	strcpy(strString, SUMI_GOTHIT);
	fwrite(strString, MAX_STRING_LENGTH, 1, pFile);
	fclose(pFile);

	//write pear information out to a file
	pFile = fopen("..\\..\\Pear.data", "wb");
	*pBool = PEAR_HEALTHITEM;
	fwrite(pBool, sizeof(bool), 1, pFile);
	*pInt = PEAR_EFFECT;
	fwrite(pInt, sizeof(int), 1, pFile);
	*pFloat = PEAR_TIMETOLIVE;
	fwrite(pFloat, sizeof(float), 1, pFile);
	*pInt = PEAR_GEOMETRY_DATA;
	fwrite(pInt, sizeof(int), 1, pFile);
	fclose(pFile);

	//write GRAPES information out to a file
	pFile = fopen("..\\..\\Grapes.data", "wb");
	*pBool = GRAPES_HEALTHITEM;
	fwrite(pBool, sizeof(bool), 1, pFile);
	*pInt = GRAPES_EFFECT;
	fwrite(pInt, sizeof(int), 1, pFile);
	*pFloat = GRAPES_TIMETOLIVE;
	fwrite(pFloat, sizeof(float), 1, pFile);
	*pInt = GRAPES_GEOMETRY_DATA;
	fwrite(pInt, sizeof(int), 1, pFile);
	fclose(pFile);

	//write PINEAPPLE information out to a file
	pFile = fopen("..\\..\\Pineapple.data", "wb");
	*pBool = PINEAPPLE_HEALTHITEM;
	fwrite(pBool, sizeof(bool), 1, pFile);
	*pInt = PINEAPPLE_EFFECT;
	fwrite(pInt, sizeof(int), 1, pFile);
	*pFloat = PINEAPPLE_TIMETOLIVE;
	fwrite(pFloat, sizeof(float), 1, pFile);
	*pInt = PINEAPPLE_GEOMETRY_DATA;
	fwrite(pInt, sizeof(int), 1, pFile);
	fclose(pFile);

	//write WHOLETURKEY information out to a file
	pFile = fopen("..\\..\\WholeTurkey.data", "wb");
	*pBool = WHOLETURKEY_HEALTHITEM;
	fwrite(pBool, sizeof(bool), 1, pFile);
	*pInt = WHOLETURKEY_EFFECT;
	fwrite(pInt, sizeof(int), 1, pFile);
	*pFloat = WHOLETURKEY_TIMETOLIVE;
	fwrite(pFloat, sizeof(float), 1, pFile);
	*pInt = WHOLETURKEY_GEOMETRY_DATA;
	fwrite(pInt, sizeof(int), 1, pFile);
	fclose(pFile);

	//write ICECREAMCONE information out to a file
	pFile = fopen("..\\..\\IceCreamCone.data", "wb");
	*pBool = ICECREAMCONE_HEALTHITEM;
	fwrite(pBool, sizeof(bool), 1, pFile);
	*pInt = ICECREAMCONE_EFFECT;
	fwrite(pInt, sizeof(int), 1, pFile);
	*pFloat = ICECREAMCONE_TIMETOLIVE;
	fwrite(pFloat, sizeof(float), 1, pFile);
	*pInt = ICECREAMCONE_GEOMETRY_DATA;
	fwrite(pInt, sizeof(int), 1, pFile);
	fclose(pFile);

	//write BLUEBERRYPIE information out to a file
	pFile = fopen("..\\..\\BlueberryPie.data", "wb");
	*pBool = BLUEBERRYPIE_HEALTHITEM;
	fwrite(pBool, sizeof(bool), 1, pFile);
	*pInt = BLUEBERRYPIE_EFFECT;
	fwrite(pInt, sizeof(int), 1, pFile);
	*pFloat = BLUEBERRYPIE_TIMETOLIVE;
	fwrite(pFloat, sizeof(float), 1, pFile);
	*pInt = BLUEBERRYPIE_GEOMETRY_DATA;
	fwrite(pInt, sizeof(int), 1, pFile);
	fclose(pFile);

	//free temp pointers
	free(pInt);
	free(pFloat);
	free(pBool);
}