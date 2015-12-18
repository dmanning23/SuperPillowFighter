#include "vertbufctrl.h"

// Initialize the only pointer to this class.
CVertBufCtrl *CVertBufCtrl::m_pInstance = 0;

CVertBufCtrl::CVertBufCtrl(void)
{
	m_outLog.Init("VertexBufferControl.txt");
	m_outLog << "Constructing the vertex buffer control system.\n";
	
	memset(m_bUsed, 0, sizeof(bool) * MAX_VERT_BUFS);
	memset(m_uBufferIds, 0, sizeof(GLuint) * MAX_VERT_BUFS);
	m_iNumBuffers = 0;
	m_bInitialized = false;
}

CVertBufCtrl::~CVertBufCtrl(void)
{
	m_outLog << "Destroying the vertex buffer control system.\n";
	m_outLog.Shutdown();	
}

int CVertBufCtrl::FindUnusedBuffer()
{
	// Ready... 
	int ii = -1;

	// Try to find an empty slot.
	while(++ii < MAX_VERT_BUFS)
		if(!m_bUsed[ii])
			return ii;

	// None found.
	return -1;
}

//////////////////////////////////////////
// Function: initialize()
//
// Purpose:	Initializes the bitmap manager.
//
// Last Modified: Monday, August 30, 2004
//
// Input: None.
//
// Output: None. 
//
// Returns: None.
//
//////////////////////////////////////////
void CVertBufCtrl::Initialize()
{
	if(m_bInitialized)
		return;

	m_outLog << "Initializing the vertex buffer control system.\n";
    m_bInitialized = true;
}

//////////////////////////////////////////
// Function: shutdown()
//
// Purpose:	Shutdown the bitmap manager.
//
// Last Modified: Monday, August 30, 2004
//
// Input: None.
//
// Output: None. 
//
// Returns: None.
//
//////////////////////////////////////////
void CVertBufCtrl::Shutdown()
{
	if(!m_bInitialized)
		return;
	m_outLog << "Shut down of the vertex buffer control system.\n";
	m_outLog << "Vertex buffers left in the verbufctrl: " << m_iNumBuffers << "\n";
	Cleanup();
}

//////////////////////////////////////////////////////////////////////////////////////////
// Function:       
//
// Purpose:        
//
// Last Modified:  Monday, August 30, 2004
//
// Input:          
//
// Output:         
//
// Returns:        
//
//////////////////////////////////////////////////////////////////////////////////////////
int CVertBufCtrl::CreateNewVertBuf(VertexT *pData, int iNumVerts, int iType)
{
	// Find an empty slot to track a new vertex buffer.
	int iEmptySlot = FindUnusedBuffer();

	if(iEmptySlot == -1)
		return -1;	// No room...

	// Make sure we are given a valid size.
	if(iNumVerts < 0)
		return -1;

    // Make a new buffer.
	GenBuffersARB(1, &m_uBufferIds[iEmptySlot]);

	// Start tracking this new buffer.
    m_bUsed[iEmptySlot] = true;
	m_iNumBuffers++;

	// Static geometry
	if(iType == STATIC_GEOMETRY)
	{
		BindBufferARB(ARRAY_BUFFER_ARB, m_uBufferIds[iEmptySlot]);
		BufferDataARB(ARRAY_BUFFER_ARB, sizeof(VertexT)*iNumVerts, 
			pData, STATIC_DRAW_ARB);
	}

	m_outLog << "\tCreated a vertex buffer containing " << iNumVerts << " vertices.\n";

	// No more vertex buffers.
	ReleaseBufferUsage();
    
	return iEmptySlot;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Function:       
//
// Purpose:        
//
// Last Modified:  Monday, August 30, 2004
//
// Input:          
//
// Output:         
//
// Returns:        
//
//////////////////////////////////////////////////////////////////////////////////////////
void CVertBufCtrl::BindBuffer(int iId)
{
	// No invalid ids.
	if(iId < 0 || iId >= m_iNumBuffers)
		return;

	// Bbbind it!
	BindBufferARB(ARRAY_BUFFER_ARB, m_uBufferIds[iId]);
    
}

//////////////////////////////////////////////////////////////////////////////////////////
// Function:       
//
// Purpose:        
//
// Last Modified:  Monday, August 30, 2004
//
// Input:          
//
// Output:         
//
// Returns:        
//
//////////////////////////////////////////////////////////////////////////////////////////
void CVertBufCtrl::Cleanup()
{
	int ii;
	for(ii = 0; ii < MAX_VERT_BUFS; ii++)
	{
		// Is there a vertex buffer here?
		if(m_bUsed[ii])
		{
			// Get rid of this vertex buffer.
			m_outLog << "\tReleasing a vertex buffer.\n";
			DeleteBuffersARB(1, &(m_uBufferIds[ii]));
			m_bUsed[ii] = false;
			m_uBufferIds[ii] = 0;
			m_iNumBuffers--;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
// Function:       
//
// Purpose:        
//
// Last Modified:  Monday, August 30, 2004
//
// Input:          
//	
// Output:         
//
// Returns:        
//
//////////////////////////////////////////////////////////////////////////////////////////
void CVertBufCtrl::DeleteBuffer(int iId)
{
	// No invalid ids.
	if(iId < 0 || iId >= m_iNumBuffers)
		return;

	DeleteBuffersARB(1, &(m_uBufferIds[iId]));
	m_bUsed[iId] = false;
	m_uBufferIds[iId] = 0;
	m_iNumBuffers--;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Function:       
//
// Purpose:        
//
// Last Modified:  Monday, August 30, 2004
//
// Input:          
//
// Output:         
//
// Returns:        
//
//////////////////////////////////////////////////////////////////////////////////////////
void CVertBufCtrl::ReleaseBufferUsage()
{
  	BindBufferARB(ARRAY_BUFFER_ARB, 0);
}
