//////////////////////////////////////////////////////////////////////////////////////////
//File Name:		main.cpp
// 
//Author:			Kevin Cecelski
//
//Date Created:		8/11/2004
//
//Purpose:			This file creates a command line interface
//					to the geometry exporter.
//
//////////////////////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include "geometryExp.h"
#include "../MemManager/memmanager.h"

//// a/s Mayafilename texture-image outputFileName.
//struct exportFmt {
//	bool tbAnimated;
//	char *tstrMayaName;
//	char *tstrTexName;
//	char *tstrExportName;
//};	
//
//// a/s Mayafilename texture-image outputFileName.
//int getExpsFromFile(char *strExportListFile)
//{
//	FILE pFile;
//
//	pFile = fopen(strExportListFile
//
//	// Get the size of the file.
//	fseek(pOpenFile, 0, SEEK_END);
//	isize = ftell(pOpenFile);
//}

int main(int argc, char **argv)
{
	// Get some memory to work with.
	MemInit(10000000);

	// Make sure we are given the parameters that we need.
	if(argc != 5)
	{
		printf("Usage: mayaExporter (-a|-s) <maya file> <texture file name> <export file>\n Notes: -a animated model -s static model\n");
		return 0;
	}

	// Start maya.
	printf("Starting maya...\n");
	startMaya();

	// Read in the geometry.
	readGeometry(argv[2], ((argv[1])[1] == 'a'));

	// Save the vertex information to a file.
	saveGeometry(argv[4], argv[3]);

	// Cleanup the geometry exporter.
	cleanGeometryExp();

	// MemManager shutdown.
	MemCheck();
	MemShutDown();
    
	// Maya, go away.
	exitMaya();
}

