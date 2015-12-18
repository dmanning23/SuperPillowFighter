//////////////////////////////////////////////////////////////////////////////////////////
//File Name:		geometryExp.cpp
// 
//Author:			Kevin Cecelski
//
//Date Created:		8/11/2004
//
//Purpose:			This file implements the interface and functionality
//					of the maya exporter.
//////////////////////////////////////////////////////////////////////////////////////////
#include "geometryExp.h"
#include <stdio.h>
#include "vertht.h"

// Used to track the joint list for a vertex.
struct vertexJointListT
{
	JointT  jList[3];	// Two joints per vertex.
};

// Points to the vertex data we read in.
static VertexT	*pVertices = 0;	

// The condensed list of vertices for the model(the index list references this list).
static int iPackedListSize;
static VertexT *pPackedVertList;

// The indices into the condensed vert list that define the model's triangles.
static int *pIndices;

// Points to the joint weights for each UNIQUE vertex.
static vertexJointListT *pJoints = 0;

// Does this file define texture coordinates?
static bool bUvs = false;

// *** Leave this here in case the modelers decide to	***
// *** add vertex colors to our models					***
//static bool bColors = false;

// Number of vertices to read.
static unsigned int uVertCount = 0;

//////////////////////////////////////////////////////////////////////////////////////////
// Function:
//
// Purpose:
//
// Last Modified:  Tuesday, September 07, 2004
//
// Input:
//
// Output:
//
// Returns:
//
//////////////////////////////////////////////////////////////////////////////////////////
void condenseVertexList()
{
	unsigned int ii;
	// Make no assumptions about the data.  There should be enough room in the
	// condensed array to hold every vertex in the uncondensed vertex array.
	pPackedVertList = (VertexT *)MemMalloc(sizeof(VertexT) * uVertCount);
	memset(pPackedVertList, 0, sizeof(VertexT) * uVertCount);

	iPackedListSize = createVertHT(uVertCount, pVertices, pPackedVertList);

	// Make room for the index list.
	pIndices = (int *)MemMalloc(uVertCount * sizeof(int));

	// Go through each vertex and build an index list for the new condensed 
	// vertex list.
	for(ii = 0; ii < uVertCount; ii++)
        pIndices[ii] = getCndVertIndex(pVertices[ii], pPackedVertList);

	// Check for errors.
	for(ii = 0; ii < uVertCount; ii++)
		if(pIndices[ii] < 0)
			printf("Error %i\n ", ii);

	// Print out a report of the conversion statistics.
	printf("Size of the original vertex list: %i\n", uVertCount);
	printf("Size of the condensed vertex list: %i\n", iPackedListSize);
	printf("Reduction in vertex count: %.2f%%\n", (float)(uVertCount-iPackedListSize)/(float)(uVertCount)*100);

	ii = sizeof(int)*uVertCount + (sizeof(VertexT) * iPackedListSize);
	printf("Memsize for indexed model: %i bytes\n", ii);
	printf("Memsize for raw model: %i bytes\n", uVertCount * sizeof(VertexT) );
	printf("Reduction in memory footprint: %.2f%%\n", (float)(uVertCount * sizeof(VertexT) - ii)/(float)(uVertCount * sizeof(VertexT) )*100 );

	// Cleanup the hash table.
	cleanupVertHT();
}

//////////////////////////////////////////
// Function: storeVertexInfo()
//
// Purpose: Reads the geometry data of the current polygon 
//			in the given mesh and stores it in the temporary 
//			array.
//
// Last Modified: 8/15/2004
//
// Input: verts		- the array of vertices for this mesh.
//		  indices	- the indices into the vertex array for this
//					  polygon.
//		  polyiter	- the current polygon we are looking at in the 
//					  given mesh.
//		  mesh		- the mesh currently being considered.
//		  uvName	- the name of the texture set to use.
//		  index		- the exact vertex index we want to read.
//		  pVertData - where the data read will be stored.
//
// Output: pVertData	- Will recieve the vertex data read.
//
// Returns: None.
//
//////////////////////////////////////////
void storeVertexInfo(MFloatPointArray &verts, MIntArray &indices, MItMeshPolygon &polyiter,
					 MFnMesh &mesh, MString &uvName, int index, VertexT *pVertData)
{
	MStatus status;
	MFloatPoint temp =  verts[indices[index]].cartesianize();
	static int count = 0;

	// Store the position of the vertex.
	pVertData->tPosition.tfX = temp.x;
	pVertData->tPosition.tfY = temp.y;
	pVertData->tPosition.tfZ = temp.z;


	// Store the uv coordinates.
	float2 f2uvs;
	if(polyiter.hasUVs())
	{
		status = polyiter.getUV(index, f2uvs, &uvName);

		if(status != MS::kSuccess)
			cout << count++ << ") " << status.errorString().asChar() << endl;
		memcpy((pVertData->tfTex), f2uvs, sizeof(float) * 2);
		bUvs = true;
	}
	else
		memset(pVertData->tfTex, 0, sizeof(float) * 2);

	// Store the vertex normal.
	MVector norm;
	mesh.getVertexNormal(indices[index], norm, MSpace::kWorld);

	pVertData->tNormal.tfX = (float)norm.x;
	pVertData->tNormal.tfY = (float)norm.y;
	pVertData->tNormal.tfZ = (float)norm.z;

	// *** Leave this here in case the modelers decide to	***
	// *** add vertex colors to our models					***
	//// Get the color for this vertex.
	//if(polyiter.hasColor())
	//{
	//	MColor thisCol;
	//	polyiter.getColor(thisCol, index);
	//	pVertData->tfColor[0] = thisCol.r;
	//	pVertData->tfColor[1] = thisCol.g;
	//	pVertData->tfColor[2] = thisCol.b;
	//	pVertData->tfColor[3] = thisCol.a;
	//	bColors = true;
	//}
	//else
	//{
	//	pVertData->tfColor[0] = 1.0f;
	//	pVertData->tfColor[1] = 1.0f;
	//	pVertData->tfColor[2] = 1.0f;
	//	pVertData->tfColor[3] = 1.0f;
	//}
}

//////////////////////////////////////////
// Function: createJointList()
//
// Purpose: Creates a list of joint weights for each unique vertex in 
//			the geometry at the given dag node.  The geometry given should
//			be the geometry that the specified skin cluster affects.
//			The list will also have the same ordering as the 
//			unique vertex list for this geometry.
//
// Last Modified: 8/18/2004
//
// Input: geoPath		- path to the dag node that contains the geometry 
//						  we will inspect.
//		  skinClust		- the skincluster that affects the aforementioned 
//						  geometry.
//
// Output: pJoints		- will contain the joint weight list for this
//						  geometry/skincluster combination.
//
// Returns: None.
//
//////////////////////////////////////////
static void createJointList(MDagPath &geoPath, MFnSkinCluster &skinClust)
{
	// Stores the status of maya calls.
	MStatus status;

	// Make room for the joint list.
	pJoints = (vertexJointListT *)MemMalloc(sizeof(vertexJointListT) * uVertCount);
	memset(pJoints, 0, sizeof(vertexJointListT) * uVertCount);

	// Get the list of influence objects for this skincluster.
	MDagPathArray influenceObjects;
	skinClust.influenceObjects(influenceObjects, &status);

	// Stores the list of components for each influence object.
	MSelectionList *list = new MSelectionList[influenceObjects.length()];

	// For each influence object get the list of components it affects.
	MFloatArray ftemp;	// Dummy variable used to catch the weight list returned from the influence calls.
    for(unsigned int ii = 0; ii < influenceObjects.length(); ii++)
		skinClust.getPointsAffectedByInfluence(influenceObjects[ii], list[ii], ftemp);

	// Counts how many errors were found the joint-weight list.
	int iErrorCount = 0;

	// Make an iterator to go through all the components of the geometry affected by this
	// skincluster.
	MItGeometry   geo(geoPath);

	// Stores the number of joints that affect the given component.
    unsigned int weightcount; 

	// Stores the weight of influence that each joint has on the given component.
	MFloatArray  weightarr;

	// Track which geometry component we are analyzing.
	int numgeos = 0;

	// Go through each component of this geometry.
	for(;!geo.isDone(); geo.next())
	{
		// Get the weight that each influence object has on this component.
		skinClust.getWeights(geoPath, geo.component(), weightarr, weightcount);

		// Find the total of all the weights on this vertex.(should add up to one)
		float totWeight = 0.0f;		

		// How many of the weights returned actually have any affect on this vertex.
		// (All but two of them should 0)
		int validWeightCount = 0;

		// Find which influence objects actually affect this component.
		for(unsigned int ii = 0; ii < weightcount; ii++)
		{
			if(list[ii].hasItem(geoPath, geo.component()))	// Let maya filter out any invalid weights.
			{
				// This influence object affects this component. Use it.
				if(validWeightCount < 4)	// Make sure that we define no more than two weights per vertex.
				{ 
	                totWeight += weightarr[ii];
					pJoints[numgeos].jList[validWeightCount].tiJointId = ii+1;	// Store the joint id.
					pJoints[numgeos].jList[validWeightCount].tfWeight = weightarr[ii];	// Store the weight of influence.
				}
				validWeightCount++;
			}
		}

		// Report when there are more than two influence objects per vertex.
		if(validWeightCount > 3)
		{
			// This vertex has more than two joints affecting it.
			cout << "error" << ++iErrorCount << " number of valid weights for this component: " << validWeightCount << endl;
			cout << "The sum of the weights on this vertex is: " << totWeight << endl;
			cout << "The weight from each influence object on this vertex is: ";

			// Print all the weights.
			for(unsigned int ij = 0; ij < weightcount; ij++)
				cout << weightarr[ij] << " ";
			cout << endl;
			
			// Try to fix it.
			validWeightCount = 0;
			// Find which influence objects actually affect this component.
			for(unsigned int ik = 0; ik < weightcount; ik++)
			{
				if(list[ik].hasItem(geoPath, geo.component()))	// Let maya filter out any invalid weights.
				{
					// This influence object affects this component. Use it.
					if(validWeightCount < 4)	// Make sure that we define no more than two weights per vertex.
					{
						if(weightarr[ik] >  0.01f)	// If the weight of influence is close enough to zero, ignore it.
						{
							pJoints[numgeos].jList[validWeightCount].tiJointId = ik+1;	// Store the joint id.
							pJoints[numgeos].jList[validWeightCount++].tfWeight = weightarr[ik]; // Store the weight of influence.
						}
					}
					else
						break;
				}
			}					                    					
		}
		numgeos++;	// Venture on to the next component...
	}
}

//////////////////////////////////////////
// Function: getAnimatedVertices()
//
// Purpose:	Reads all the vertex information(including joint weights
//			from the model's DG.
//
// Last Modified: 8/18/2004
//
// Output: pVertices		- Will contain the list of vertices for this 
//							  model.
//
// Returns: None.
//
//////////////////////////////////////////
static void getAnimatedVertices()
{
	// Make an iterator to go through all the skincluster dependency nodes.
	MItDependencyNodes iterDG(MFn::kSkinClusterFilter);

	int count = 0;
	int numgeos = 0;

	// Stores the status of Maya functions.
	MStatus status;

	int uCount = 0;	// Stores where we are in the vertex data.

	// Make room for the data.
	pVertices = (VertexT *)MemMalloc(sizeof(VertexT) * uVertCount);
	memset(pVertices, 0, sizeof(VertexT) * uVertCount);

	// Go through each skincluster in the dependency graph.
	for(;!iterDG.isDone(); iterDG.next())
	{
		MFnDependencyNode  dg(iterDG.item());

		// Do not trust Maya, make sure this is a skincluster node.
		if(dg.hasObj(MFn::kSkinClusterFilter))
		{
			cout << "skincluster found\n";

			// Operate on this skin cluster using the skincluster function class.
			MFnSkinCluster j(iterDG.item());

			// Assume only one output connection for this skin cluster and get the id of that connection.
			unsigned int index = j.indexForOutputConnection(0);

			// Get the path to the geometry that this skincluster affects.
			MDagPath m;
			j.getPathAtIndex(index, m);
			cout << "number of output connections for this skin " << j.numOutputConnections() << endl;

			// Create the joint list for this skincluster.
			createJointList(m, j);

			// Make an iterator to go through all the components of the geometry affected by this
			// skincluster.
			MItGeometry   geo(m);

			// This is a mesh node.
			// Get functions to operate on a mesh node.
			MFnMesh thisMesh(m);

			// Storage for this mesh's points.
			MFloatPointArray meshPoints;

			// Get the points for this mesh.
			thisMesh.getPoints(meshPoints, MSpace::kWorld);

			// Get the texture set this mesh uses.
			MStringArray uvNames;
			thisMesh.getUVSetNames(uvNames);

			// Get access to the polygon list.
			MItMeshPolygon polyiter(m);

			// Go through each polygon.
			for(; !polyiter.isDone(); polyiter.next())
			{
				// Get the indices.
				MIntArray indexList;
				polyiter.getVertices(indexList);

				// Store the vertex info for this polygon.
				for(int i = 0; i < polyiter.polygonVertexCount(); i++)
				{
					// Store aesthetic vertex information.
					storeVertexInfo(meshPoints, indexList, polyiter, thisMesh, uvNames[0],
						i, (&pVertices[uCount]));

					// Store the animation data.
					pVertices[uCount].tuJoints[0] = pJoints[indexList[i]].jList[0];	
					pVertices[uCount].tuJoints[1] = pJoints[indexList[i]].jList[1];
					pVertices[uCount].tuJoints[2] = pJoints[indexList[i]].jList[2];
					uCount++;	// Onto the next vertex.
				}
			}

			// Free the temporary joint information.
			MemFree(pJoints);
			pJoints = 0;
		}
	}
}

//////////////////////////////////////////
// Function: getVertices()
//
// Purpose:	Reads all the vertex information from the model's
//			DAG.
//
// Last Modified: 8/15/2004
//
// Output: pVertices		- Will contain the list of vertices for this 
//							  model.
//
// Returns: None.
//
//////////////////////////////////////////
void getVertices()
{
	// Stores the status of Maya functions.
	MStatus status;

	int uCount = 0;	// Stores where we are in the vertex data.

	// Make room for the data.
	pVertices = (VertexT *)MemMalloc(sizeof(VertexT) * uVertCount);
	memset(pVertices, 0, sizeof(VertexT) * uVertCount);

	// Get an iterator to go through all the nodes.
	MItDag iterMesh( MItDag::kDepthFirst, MFn::kInvalid, &status);

	// Search every node in the dag.
	for(; !iterMesh.isDone(); iterMesh.next())
	{
		// Get the path to the current node.
		MDagPath pathTo;
		status = iterMesh.getPath(pathTo);

		// Were we able to get a path?
		if(status != MStatus::kSuccess)
			continue;

		// Yes, get the node.
		MFnDagNode NodeFunc(pathTo, &status);

        // Is this a mesh node?
		if(NodeFunc.isIntermediateObject())
			continue;
		if(!pathTo.hasFn(MFn::kMesh))
			continue;
		if(pathTo.hasFn(MFn::kTransform))
			continue;

		// This is a mesh node.
		// Get functions to operate on a mesh node.
		MFnMesh thisMesh(pathTo);

		// Storage for this mesh's points.
		MFloatPointArray meshPoints;

		// Get the points for this mesh.
		thisMesh.getPoints(meshPoints, MSpace::kWorld);

		MStringArray uvNames;
		thisMesh.getUVSetNames(uvNames);

		// Get access to the polygon list.
		MItMeshPolygon polyiter(pathTo);

		// Go through each polygon.
		for(; !polyiter.isDone(); polyiter.next())
		{
            // Get the indices.
			MIntArray indexList;
			polyiter.getVertices(indexList);

			// Store the vertex info for this polygon.
			for(int i = 0; i < polyiter.polygonVertexCount(); i++)
			{
				storeVertexInfo(meshPoints, indexList, polyiter, thisMesh, uvNames[0],
					i, (&pVertices[uCount]));
				uCount++;
			}

		}
	}	
}

//////////////////////////////////////////
// Function: getVertexCount()
//
// Purpose:	Counts how many vertices are in the model.
//
// Last Modified: 8/15/2004
//
// Input: None.
//
// Output: None.
//
// Returns: The number of vertices in the model.
//
//////////////////////////////////////////
int getVertexCount()
{
	MStatus status;
	int iVertCount = 0;

	// Get an iterator to go through all the nodes.
	MItDag iterMesh(MItDag::kDepthFirst, MFn::kInvalid, &status);

	// Search every node in the dag.
	for(; !iterMesh.isDone(); iterMesh.next())
	{
		// Get the path to the current node.
		MDagPath pathTo;
		status = iterMesh.getPath(pathTo);

		// Were we able to get a path?
		if(status != MStatus::kSuccess)
			continue;

		// Yes, get the node.
		MFnDagNode NodeFunc(pathTo, &status);

        // Is this a mesh node?
		if(NodeFunc.isIntermediateObject())
			continue;
		if(!pathTo.hasFn(MFn::kMesh))
			continue;
		if(pathTo.hasFn(MFn::kTransform ) )
			continue;

		// This is a mesh node.
		// Get functions to operate on a mesh node.
		MFnMesh thisMesh(pathTo);

		// Find out how many face vertices.
		iVertCount += thisMesh.numFaceVertices();	
	}

	return iVertCount;
}

//////////////////////////////////////////
// Function: readGeometry()
//
// Purpose:	Reads geometry information from a 
//			Maya file.
//
// Last Modified: 8/15/2004
//
// Input: *strMayaFile		The name of the maya file.
//
// Output: None.
//
// Returns: true if successful, false otherwise.
//
//////////////////////////////////////////
bool readGeometry(char *strMayaFile, bool animated)
{
	// Start a new file.
	MFileIO::newFile(true);

	// Try to open the maya file.
	MStatus stat = MFileIO::open(strMayaFile);
	if(!stat)
		return false;	// There was a problem, get out.

	// Get rid of any unecessary data.
	stat = MGlobal::executeCommand("delete -ch");
	if(!stat)
		return false;

	// Get the number of vertices in this file.
	uVertCount = getVertexCount();
	printf("Vertex Count: %i\n", uVertCount);
	printf("Triangle count: %i\n", uVertCount/3);

	// Read in the vertices.
	if(animated)
		getAnimatedVertices();	// Animated model.
	else	
		getVertices();			// Non-animated model.

	// Are uvs defined?
	if(bUvs)
		printf("Texture coordinates are defined for this model.\n");
	else
		printf("Texture coordinates are NOT defined for this model.\n");

	// Convert the model to an indexed vertex model.
	condenseVertexList();

	// *** Leave this here in case the modelers decide to	***
	// *** add vertex colors to our models					***
	//// Are vertex colors defined?
	//if(bColors)
	//	printf("Vertex colors are defined for this model.\n");
	//else
	//	printf("Vertex colors are NOT defined for this model.\n");
    
    return false;
}

//////////////////////////////////////////
// Function: saveGeometry()
//
// Purpose:	Saves the geometry information
//			and texture filename to a file.
//
// Last Modified: 8/15/2004
//
// Input: *strSaveFile		The name of the file where the 
//							data shall be saved.
//		  *strTexFileName	The name of the texture file
//							that this model uses.
//
// Output: None.
//
// Returns: true if successful, false otherwise.
//
//////////////////////////////////////////
bool saveGeometry(char *strSaveFile, char *strTexFileName)
{
	// Write the data to a file.
	FILE *pFile;
	pFile = fopen(strSaveFile, "wb");
	unsigned int ii;

	if(!pFile)
		return false;

	// Write the number of vertices.
	fwrite(&iPackedListSize, sizeof(int), 1, pFile);

	// Write the number of indices.
	fwrite(&uVertCount, sizeof(int), 1, pFile);

	// This assumes a file name no greater than MAX_STRING chasracters
	// and that the string is null terminated.
    fwrite(strTexFileName, sizeof(char), MAX_STRING, pFile);

	// Write the vertices.
	fwrite(pPackedVertList, sizeof(VertexT), iPackedListSize, pFile);

	// Write the indices.
	fwrite(pIndices, sizeof(int), uVertCount, pFile);

	fclose(pFile);

	// ** Leave this here in case we want to check the validity of  **
	// ** the data this exporter creates.							**
	FILE *pTextOut = fopen("verts.txt", "wt");
	for(ii = 0; ii < iPackedListSize; ii++)
	{
		fprintf(pTextOut, "vert %i x:%.6f y:%.6f z:%.6f nx:%.6f ny:%.6f nz:%.6f j1w:%.6f JOINTID:%i j2w:%.6f JOINTID:%i j3w:%.6f JOINTID:%i weight total:%.6f\n",
			ii, pPackedVertList[ii].tPosition.tfX, pPackedVertList[ii].tPosition.tfY, pPackedVertList[ii].tPosition.tfZ,
			pPackedVertList[ii].tNormal.tfX, pPackedVertList[ii].tNormal.tfY, pPackedVertList[ii].tNormal.tfZ,
			pPackedVertList[ii].tuJoints[0].tfWeight, pPackedVertList[ii].tuJoints[0].tiJointId, 
			pPackedVertList[ii].tuJoints[1].tfWeight, pPackedVertList[ii].tuJoints[1].tiJointId,
			pPackedVertList[ii].tuJoints[2].tfWeight, pPackedVertList[ii].tuJoints[2].tiJointId,
			(pPackedVertList[ii].tuJoints[0].tfWeight + pPackedVertList[ii].tuJoints[1].tfWeight + pPackedVertList[ii].tuJoints[2].tfWeight));
	}
	fclose(pTextOut);

    return true;
}

//////////////////////////////////////////
// Function: cleanGeometryExp()
//
// Purpose: Cleans up all the temporary data used
//			by the exporter.
//
// Last Modified: 8/15/2004
//
// Input: None.
//
// Output: None.
//
// Returns: None.
//
//////////////////////////////////////////
void cleanGeometryExp()
{
	if(pVertices)
	    MemFree(pVertices);

	pVertices = 0;
	uVertCount = 0;
}