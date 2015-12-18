///////////////////////////////////////////////////////////////
// File Name:     animMain.cpp
// 
// Author:        Matt Strand
//
// Date Created:  8/17/2004
//
// Purpose:       This file creates a command line interface
//                to the animation exporter.
///////////////////////////////////////////////////////////////
#include <windows.h>
#include "animExp.h"

int main(int argc, char **argv)
{
	int	iError;	//  Used to catch error codes.

	// Make sure they're giving us the correct operation parameters
	if(argc != 4)
	{
		printf("Usage: AnimExp <maya binary file (*.mb)> <export filename> <-a (Animation) -b (Bind)>\n");
		return 0;
	}

	//  Load Maya API.
	printf("\nLoading Maya API...\n");
	loadMaya();

	//  Load animations from a file and save them.
	iError = exportAnimations(argv[1], argv[2], ((argv[3])[1] == 'a'));

	switch(iError)
	{
		case 0:
			printf("Operation completed successfully!\n");
			break;
		case 1:
			printf("Error Code: %i\t(MFileIO::open()) - File failed to load.\n", iError);
			break;
		case 2:
			printf("Error Code: %i\t(MGlobal::executeCommand(\"delete -ch\")) - Failed to delete history.\n", iError);
			break;
		case 51:
			printf("Error Code: %i\t(MItDag()) - Failed to create DAG Iterator.\n", iError);
			break;
		case 52:
			printf("Error Code: %i\t(MItDag::getPath()) - Failed to get DAG path.\n", iError);
			break;
		case 53:
			printf("Error Code: %i\t(MFnDagNode()) - Failed to create DAG Node.\n", iError);
			break;
		case 54:
			printf("Error Code: %i\t(MObject()) - Failed to create Object.\n", iError);
			break;
		case 55:
			printf("Error Code: %i\t(MFnIkJoint()) - Failed to create IkJoint.\n", iError);
			break;
		case 56:
			printf("Error Code: %i\t - Invalid rotation order.\n", iError);
			break;
		case 101:
			printf("Error Code: %i\t(MAnimControl::setPlaybackMode()) - Failed to set playback mode.\n", iError);
			break;
		case 102:
			printf("Error Code: %i\t(MTime::setUnit()) - Failed to set units of time.\n", iError);
			break;
		case 151:
			printf("Error Code: %i\t(MItDag()) - Failed to create DAG Iterator.\n", iError);
			break;
		case 201:
			printf("Error Code: %i\t(MItDag()) - Failed to create DAG Iterator.\n", iError);
			break;
		case 202:
			printf("Error Code: %i\t(MItDag::getPath()) - Failed to get DAG path.\n", iError);
			break;
		case 203:
			printf("Error Code: %i\t(MFnIkJoint()) - Failed to create IkJoint.\n", iError);
			break;
		case 204:
			printf("Error Code: %i\t(MFnIkJoint::parent()) - Failed to get parent.\n", iError);
			break;
		case 205:
			printf("Error Code: %i\t(MFnIkJoint::child()) - Failed to get child.\n", iError);
			break;
		case 206:
			printf("Error Code: %i\t(MObject::object()) - Failed to get object.\n", iError);
			break;
		case 251:
			printf("Error Code: %i\t(FILE::fopen()) - Failed to open file.\n", iError);
			break;
		case 252:
			printf("Error Code: %i\t(Command Line Arguments) - 3rd argument is balls.\n", iError);
			break;
		default:
			printf("Unknown Error Code: %i\n", iError);
			break;
	}

	//  Shutdown Maya API.
	printf("Unloading Maya API...\n\n");
	unloadMaya();
}