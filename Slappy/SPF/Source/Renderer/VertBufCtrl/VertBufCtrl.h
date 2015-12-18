//////////////////////////////////////////////////////////////////////////////////////////
// File Name:      VertBufCtrl.h
//
// Author:         Kevin Cecelski
//
// Date Created:   Monday, August 30, 2004
//
// Purpose:        This declares the vertex buffer control class.
//
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef _VERTBUFCTRL_H_
#define _VERTBUFCTRL_H_
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "../DonkeyExt/DonkeyExt.h"
#include "../../Logger/Logger.h"
#include "../../Memmanager/Memmanager.h"
#include "../GeometryTypes.h"

#define MAX_VERT_BUFS	64		// Maximum number of vertex buffers this will control.

class CVertBufCtrl
{
private:
	// The only instance of this class.
	static CVertBufCtrl *m_pInstance;

	// Tracks which buffers are available.
	bool m_bUsed[MAX_VERT_BUFS];

	// How many buffers are we controlling now.
	int m_iNumBuffers;

	// IDs for each buffer given to us by GenBuffersARB.
	GLuint m_uBufferIds[MAX_VERT_BUFS];

	// Used for logging.
    CLogger	m_outLog;

	// Has this module been intiailized?
	bool m_bInitialized;

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
	int FindUnusedBuffer();

	// ** Overide functions that singletons should not expose. **
		// Copy constructor.
		CVertBufCtrl(const CVertBufCtrl &cbm);	
		// Assignment operator.
		CVertBufCtrl &operator=(const CVertBufCtrl &cbm) {}
		// Default constructor.
		CVertBufCtrl(void);
		~CVertBufCtrl(void);
	// **********************************************************
public:
	enum VERTTYPES {STATIC_GEOMETRY, DYNAMIC_GEOMETRY};

	//////////////////////////////////////////
	// Function: GetInstance()
	//
	// Purpose:	 Returns a pointer to the only instance of this class.
	//
	// Last Modified: Monday, August 30, 2004
	//
	// Input: None.
	//
	// Output: None. 
	//
	// Returns: A pointer to the only instance of this class.
	//
	//////////////////////////////////////////
	static CVertBufCtrl *GetInstance()
	{
		if(!m_pInstance)
		{
			m_pInstance = (CVertBufCtrl *)MemMalloc(sizeof(CVertBufCtrl));	// Make a new instance.
			m_pInstance->CVertBufCtrl::CVertBufCtrl();
		}

		// Give 'em that pointer.
        return m_pInstance;
	}

	//////////////////////////////////////////
	// Function: DeleteInstance()
	//
	// Purpose:	 Deletes the only instance of this class.
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
	static void DeleteInstance()
	{
		if(m_pInstance)
		{
			// Get rid of this instance.
			m_pInstance->CVertBufCtrl::~CVertBufCtrl();
			MemFree(m_pInstance);
			m_pInstance = 0;
		}
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
	void Initialize();

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
	void Shutdown();

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
	int CreateNewVertBuf(VertexT *pData, int iNumVerts, int iType = STATIC_GEOMETRY);

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
	void BindBuffer(int iId);

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
	void Cleanup();

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
	void DeleteBuffer(int iId);

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
	void ReleaseBufferUsage();
};

#endif