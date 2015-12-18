//////////////////////////////////////////////////////////////////////////////////////////
//File Name:		geometryImp.cpp
// 
//Author:			Kevin Cecelski
//
//Date Created:		8/15/2004
//
//Purpose:			Implements the geometry importer interface.
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "geometryimp.h"

//////////////////////////////////////////
// Function: LoadGeometry()
//
// Purpose:	Loads geometry from a file.
//
// Last Modified: Monday, August 30, 2004
//
// Input: *pBaseObject		- The object that will store the geometry.
//		  iModelID			- The id of the model to load.
//
// Output: None.
//
// Returns: true if successful, false otherwise.
//
//////////////////////////////////////////
bool LoadGeometry(CBaseObject *pBaseObject, int iModelID)
{
	// New logger.
	CLogger logOut;
	logOut.Init("geometryImporter.txt");

	// Get the full path for the model.
    int iModelPathLen = (int)strlen(strModelPath) + (int)strlen(strIdToFileName[iModelID]);
    char *modelFilePathName = (char *)MemMalloc(sizeof(char) * (iModelPathLen + 1));

	// Copy the full path for the model file into the appropriate 
	// array.
	strcpy(modelFilePathName, strModelPath);
	strcat(modelFilePathName, strIdToFileName[iModelID]);

	logOut << "Attempting to load model: \"" << modelFilePathName << "\"\n";

    // Open the model.
	FILE *pFile;
	pFile = fopen(modelFilePathName, "rb");

	if(!pFile)
	{
		// There was an error opening the file... exit
		logOut << "There was an error loading model: \"" << modelFilePathName << "\"\n";
		MemFree(modelFilePathName);
		return false;
	}

	// Read in the model data.
	// Get the number of vertices.
	int vertCount;
	fread(&vertCount, sizeof(int), 1, pFile);

	logOut << "\tNumber of vertices in this model: " << vertCount << "\n";

	// Make some room.
	pBaseObject->m_pGeometryData = (GeometryT *)MemMalloc(sizeof(GeometryT));

	// Store the vertex count.
	pBaseObject->m_pGeometryData->tuNumVerts = vertCount;

	// Get the index count.
	int indexCount;
	fread(&indexCount, sizeof(int), 1, pFile);
	pBaseObject->m_pGeometryData->tiNumIndices = indexCount;

	logOut << "\tNumber of indices for this model: " << indexCount << "\n";
    
	// Read in the texture file name.
	char strTexFileName[MAX_STRING];
	fread(strTexFileName, sizeof(char), MAX_STRING, pFile);
	logOut << "\tName of the texture file for this model: " << strTexFileName << "\n";

	char *strTexFilePathName = (char *)MemMalloc(sizeof(char) * (MAX_STRING + (int)strlen(strTexImgPath) + 1));
    strcpy(strTexFilePathName, strTexImgPath);
	strcat(strTexFilePathName, strTexFileName);

	// Get the geometry data.
    pBaseObject->m_pGeometryData->tpVertices = (VertexT *)MemMalloc( (sizeof(VertexT) * vertCount) );
	fread(pBaseObject->m_pGeometryData->tpVertices, sizeof(VertexT), vertCount, pFile);

	// Load the texture from the bitmap manager and get an id.
	pBaseObject->m_pGeometryData->tuTexID = CBitmapManager::GetInstance()->LoadImage(strTexFilePathName, false);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Read in the indices.
	pBaseObject->m_pGeometryData->tpIndices = (int *)MemMalloc(sizeof(int) * indexCount);
	fread(pBaseObject->m_pGeometryData->tpIndices, sizeof(int), indexCount, pFile);

	//// If this is to be a static model, use a vertex buffer instead.
	//if(pBaseObject->m_iType == Powerup_obj)
	//{
	//	logOut << "\tAttempting to use vertex buffers for this object.\n";
	//	pBaseObject->m_pGeometryData->tiVertID = CVertBufCtrl::GetInstance()->CreateNewVertBuf(pBaseObject->m_pGeometryData->tpVertices,
	//		pBaseObject->m_pGeometryData->tuNumVerts, CVertBufCtrl::STATIC_GEOMETRY);

	//	if(pBaseObject->m_pGeometryData->tiVertID == -1)
	//	{
	//		// Could not get a vertex buffer.
	//		logOut << "\tCould not use vertex buffers.\n";
	//	}
	//	else
	//	{
	//		// Free the client side vertex memory.
	//		MemFree(pBaseObject->m_pGeometryData->tpVertices);
	//		pBaseObject->m_pGeometryData->tpVertices = 0;
	//		logOut << "\tSuccess using vertex buffers.\n";
	//	}
	//}

	logOut << "Success loading model: " << modelFilePathName << "\n";

	fclose(pFile);

	// Free up the temporary data.
	MemFree(strTexFilePathName);
	MemFree(modelFilePathName);

	logOut.Shutdown();
        
	return true;
}