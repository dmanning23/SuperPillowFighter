#include "levelobject.h"
#include "..\MemManager\MemManager.h"
#include "..\MathLibrary\spfmath.h"
#include "..\ContentLoader\Geometryimp.h"
#include "..\ObjectManager\ObjectFactory.h"
//#include "..\Sound\Sound.h"

/*////////////////////////////////////////////////////////////////////////
//Function:	Constructor
//
//Input:	
//iObjType:	the type of the object to create as enumerated in levelobject.h
//
//Purpose:	Construct a level object of the correct type
//
/////////////////////////////////////////////////////////////////////////*/
CLevelObject::CLevelObject(int iObjType)
{
	m_pOptionalAttachedObjects = 0;
	m_pOptionalAttachmentPoints = 0;
	m_pOwner = 0;

	//open the correct file
	FILE *pFile;
	switch (iObjType)
	{
		case Boat_levelobj:
			{
				pFile = fopen("ResourceFiles\\Boat.data", "rb");
				break;
			}
		case Whale_levelobj:
			{
				pFile = fopen("ResourceFiles\\Whale.data", "rb");
				break;
			}
		case Cannon_levelobj:
			{
				pFile = fopen("ResourceFiles\\Cannon.data", "rb");
				break;
			}
		case RubberDuck_levelobj:
			{
				pFile = fopen("ResourceFiles\\RubberDuck.data", "rb");
				break;
			}
		case Soap_levelobj:
			{
				pFile = fopen("ResourceFiles\\Soap.data", "rb");
				break;
			}
		case Faucet_levelobj:
			{
				pFile = fopen("ResourceFiles\\Faucet.data", "rb");
				break;
			}
		case Handle_levelobj:
			{
				pFile = fopen("ResourceFiles\\Handle.data", "rb");
				break;
			}
		case WaterGun_levelobj:
			{
				pFile = fopen("ResourceFiles\\WaterGun.data", "rb");
				break;
			}
		case FerrisWheel_levelobj:
			{
				pFile = fopen("ResourceFiles\\ferrisWheel.data", "rb");
				break;
			}
		case RubberDuckWheel_levelobj:
			{
				pFile = fopen("ResourceFiles\\RubberDuckWheel.data", "rb");
				break;
			}
	}
	
	//set all collision data
	fread(&m_iNumLines, sizeof(int), 1, pFile);
	CollisionData = MemMalloc((m_iNumLines * (sizeof(Vec4DT) * 2)) + sizeof(int));
	memcpy(CollisionData, &m_iNumLines, sizeof(int));
	fread(((char*)CollisionData + sizeof(int)), (m_iNumLines * (sizeof(Vec4DT) * 2)), 1, pFile);

	//read in floats used for object movement
	fread(&m_fMagnitude, sizeof(float), 1, pFile);
	fread(&m_fWavePeriod, sizeof(float), 1, pFile);
	fread(&m_fMaxBob, sizeof(float), 1, pFile);
	fread(&m_fMinBob, sizeof(float), 1, pFile);
	fread(&m_fBobPeriod, sizeof(float), 1, pFile);

	//read in files for sound
	m_strPlayerLandFileName = (char*)MemMalloc(LEVELOBJ_STRING_LENGTH);
	memset(m_strPlayerLandFileName, 0, LEVELOBJ_STRING_LENGTH);
	fread(m_strPlayerLandFileName, 1, LEVELOBJ_STRING_LENGTH, pFile);
	m_strEmitPowerupFileName = (char*)MemMalloc(LEVELOBJ_STRING_LENGTH);
	memset(m_strEmitPowerupFileName, 0, LEVELOBJ_STRING_LENGTH);
	fread(m_strEmitPowerupFileName, 1, LEVELOBJ_STRING_LENGTH, pFile);

	//load model geometry index
	fread(&this->m_iModelIndex, sizeof(int), 1, pFile);

	//get the attached objects and create them.

	// Get the number attached object.
	fread(&m_numAttachedObjects, sizeof(int), 1, pFile);

	if(m_numAttachedObjects > 0)
	{
		// Get space for this info.
		m_pOptionalAttachedObjects = (int *)MemMalloc(sizeof(int)*m_numAttachedObjects);
		// Get the attached objects.
		fread(m_pOptionalAttachedObjects, sizeof(int), m_numAttachedObjects, pFile);

		// Get the objects that are attached.
		for(int i = 0; i < m_numAttachedObjects; i++)
		{
			CLevelObject *pTemp = CObjectFactory::GetInstance()->CreateLevelObject(m_pOptionalAttachedObjects[i]);
			pTemp->m_pOwner = this;
			pTemp->m_iAttachmentPointIndex = i;
		}

		// Get space for this info.
		m_pOptionalAttachmentPoints = (Vec4DT *)MemMalloc(sizeof(Vec4DT)*m_numAttachedObjects);

		//load attachment points.
		fread(m_pOptionalAttachmentPoints, sizeof(Vec4DT), m_numAttachedObjects, pFile);
	}
	
	//load emitter information
	fread(&m_bHasEmitter, sizeof(bool), 1, pFile);
	fread(&m_EmitterPos, sizeof(Vec4DT), 1, pFile);

	//load particle information
	fread(&m_iParticleIndex, sizeof(int), 1, pFile);

	// Load the extents of the boat.
	fread(&m_LeftMostPoint, sizeof(Vec4DT), 1, pFile);
	fread(&m_RightMostPoint, sizeof(Vec4DT), 1, pFile);

	//close up file
	fclose(pFile);
}

/*////////////////////////////////////////////////////////////////////////
//Function:	Destructor
//	
//Purpose:	Clean up all memory allocated to the level object
//
/////////////////////////////////////////////////////////////////////////*/
CLevelObject::~CLevelObject(void)
{
	MemFree(m_strEmitPowerupFileName);
	MemFree(m_strPlayerLandFileName);
	MemFree(m_pOptionalAttachedObjects);
	MemFree(m_pOptionalAttachmentPoints);
}