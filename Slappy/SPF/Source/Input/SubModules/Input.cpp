////////////////////////////////////////////////////////
//
// Name: "Input.cpp"
//
// Author: Daron Keeler (DK)
//
// Creation Date: August 4, 2004
//
// Purpose: The input manager will control all input from 
//          the keyboard or controllers.
//
////////////////////////////////////////////////////////
#include "Input.h"
#include "..\\..\\MemManager\\MemManager.h"

CInput *CInput::pInstance = 0;

CInput::CInput()
{
    // set everything to null
    m_diObj = NULL;
    m_diKeyboard = NULL;

    m_iNumJoysticks = 0;

    m_JoyDat[InputP1].Joystick = NULL;
    m_JoyDat[InputP2].Joystick = NULL;
    m_JoyDat[InputP3].Joystick = NULL;
    m_JoyDat[InputP4].Joystick = NULL;

    
}

CInput *CInput::GetInstance()
{
    // Make one if it hasnt been done.
    if(!pInstance)
    {
        //pInstance = new CInput;
        pInstance = (CInput*)MemMalloc(sizeof(CInput));
        pInstance->CInput::CInput();
    }
    // Return the address of the instance.
    return pInstance;
}

void CInput::DeleteInstance()
{
    // delete the instance
    if(pInstance)
    {
        //delete pInstance;
        pInstance->CInput::~CInput();
        MemFree(pInstance);

       pInstance = NULL;
    }
}

void CInput::InitInput(HWND hWin, HINSTANCE hInst)
{
    // init the handles
    m_hWin = hWin;
    m_hInst = hInst;

    // init the logger
    m_LogMe.Init("InputLog.txt");

    m_hResult = 0;

    // create main device, check error
    if(FAILED(m_hResult = DirectInput8Create(m_hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&m_diObj, NULL)))
    {
        // m_LogMe << "Input Init Failed - " << DXGetErrorString9(m_hResult) << " - " << DXGetErrorDescription9(m_hResult) << "\n";
    }
}

void CInput::ShutDownInput()
{
    for(int i = 0; i < 4; i++)
    {
        if(m_JoyDat[i].Joystick)
        {
            m_JoyDat[i].Joystick->Unacquire();
            SAFE_RELEASE(m_JoyDat[i].Joystick);
            m_JoyDat[i].Joystick = NULL;
        }
    }

    // safely releases keyboard, get error
    if(m_diKeyboard)
    {
        if(FAILED(m_diKeyboard->Unacquire()))
        {
            //m_LogMe << "ShutdownInput Unacquire Failed - " << DXGetErrorString9(m_hResult) << " - " << DXGetErrorDescription9(m_hResult) << "\n";
        }

        SAFE_RELEASE(m_diKeyboard);
        m_diKeyboard = NULL;
    }  
    // release main object
    SAFE_RELEASE(m_diObj);
    m_diObj = NULL;


    m_LogMe << "Input shutdown";
    m_LogMe.Shutdown();

}

void CInput::InitKeyBoard()
{
    // create keyboard device, check error
    if(FAILED(m_diObj->CreateDevice(GUID_SysKeyboard, &m_diKeyboard, NULL)))
    {
        //m_LogMe << "InitKeyboard CreateDevice Failed - " << DXGetErrorString9(m_hResult) << " - " << DXGetErrorDescription9(m_hResult) << "\n";
    }

    // set coop, check error
    if(FAILED(m_diKeyboard->SetCooperativeLevel(m_hWin, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
    {
        //m_LogMe << "InitKeyboard SetCoop Failed - " << DXGetErrorString9(m_hResult) << " - " << DXGetErrorDescription9(m_hResult) << "\n";
    }

    // load data format, check error
    if(FAILED(m_diKeyboard->SetDataFormat(&c_dfDIKeyboard)))
    {
        //m_LogMe << "InitKeyboard SetDataFormat Failed - " << DXGetErrorString9(m_hResult) << " - " << DXGetErrorDescription9(m_hResult) << "\n";
    }

    // fill keyboard info struct
    DIPROPDWORD temp;
    memset(&temp, 0, sizeof(temp));
    temp.diph.dwSize = sizeof(DIPROPDWORD);
    temp.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    temp.diph.dwObj = 0;
    temp.diph.dwHow = DIPH_DEVICE;
    temp.dwData = INPUT_BUFF_SIZE;

    // set keyboard properties, get error
    if(FAILED(m_diKeyboard->SetProperty(DIPROP_BUFFERSIZE, &temp.diph)))
    {
        //m_LogMe << "InitKeyboard SetProperty Failed - " << DXGetErrorString9(m_hResult) << " - "<< DXGetErrorDescription9(m_hResult) << "\n";
    }

    // attempt to acquire keyboard, get error
    if(FAILED(m_diKeyboard->Acquire()))
    {
        //m_LogMe << "InitKeyboard Acquire Failed - " << DXGetErrorString9(m_hResult) << " - " << DXGetErrorDescription9(m_hResult) << "\n";
    }

    // find and save keyboard layout
    m_kblayout = GetKeyboardLayout(0);
}

bool CInput::GetKey(const DWORD dwKey)
{
	//	Check to see if the key was pressed.
	if (m_szKeyboard[dwKey] & 0x80)
		return true;
    else
	    return false;
}

bool CInput::GetBuffKey(const DWORD dwKey)
{
    // Check all possible elements.
    for(DWORD i = 0; i < m_Elements; i++)
    {
        // return success if key was pressed.
        if((m_ObjData[i].dwOfs == dwKey) && (m_ObjData[i].dwData & 0x80))
            return true;
    }

    return false;
}

bool CInput::UpdateKeyBoard()
{
    memset(m_szKeyboard, 0, sizeof(m_szKeyboard));

    if(FAILED(m_hResult = m_diKeyboard->GetDeviceState(sizeof(m_szKeyboard), (LPVOID)&m_szKeyboard)))
    {
        //m_LogMe << "Keyboard Update Failed. Attempting to Reacquire - " << DXGetErrorString9(m_hResult);
        //m_LogMe << " - " << DXGetErrorDescription9(m_hResult) << "\n";

        if(m_hResult == DIERR_INPUTLOST)
            if(FAILED(m_hResult = m_diKeyboard->Acquire()))
            {  
               //m_LogMe << "Keyboard Update Failed. FAILED TO REACQUIRE - " << DXGetErrorString9(m_hResult);
               //m_LogMe << " - " << DXGetErrorDescription9(m_hResult) << "\n";
               return false;
            }
    }

    return true;
}

bool CInput::UpdateBuffKeyBoard()
{
    
    m_Elements = INPUT_BUFF_SIZE;
    memset(&m_ObjData, 0, sizeof(m_ObjData));

    if(FAILED(m_hResult = m_diKeyboard->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), m_ObjData, &m_Elements, 0)))
    {
        //m_LogMe << "Keyboard Buff Update Failed. Attempting to Reacquire - " << DXGetErrorString9(m_hResult);
        //m_LogMe << " - " << DXGetErrorDescription9(m_hResult) << "\n";

        if(m_hResult = DIERR_INPUTLOST)
            if(FAILED(m_diKeyboard->Acquire()))
            {
                //m_LogMe << "Keyboard Update Failed. FAILED TO REACQUIRE - " << DXGetErrorString9(m_hResult);
                //m_LogMe << " - " << DXGetErrorDescription9(m_hResult) << "\n";
                return false;
            }
    }
    return true;
}

char CInput::CheckBuffKey()
{
    // check key state
    if(!(GetKeyboardState(m_Keys)))
    {
        MessageBox(m_hWin, "Cant get key state", "Fail", MB_OK);
        return 0;
    }

    // temporary variables
    unsigned short num = 0;
    int vk = 0;

    // check all possible elements
    for(DWORD i = 0; i < m_Elements; i++)
    {
        for (int j = 0; j < 256; j++)
        {
            if((m_ObjData[i].dwOfs == j) && (m_ObjData[i].dwData & 0x80))
            {
                // saved pressed key
                vk = MapVirtualKeyEx(m_ObjData[i].dwOfs, 1, m_kblayout);           // vk = j;
                ToAsciiEx(vk, m_ObjData[i].dwOfs, m_Keys, &num, 0, m_kblayout);
            }
        }
    }

    // return the pressed key
    return (char)num;
}

void CInput::InitJoy()
{
    // Set some to null
    for(int i = 0; i < NUM_JOYSTICKS; i++)
    {
        m_JoyDat[i].Joystick = NULL;
        m_JoyDat[i].JoyFound = false;
    }

    // get each joystick.
    m_diObj->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoystickCallback, NULL, DIEDFL_ATTACHEDONLY);

    for(int i = 0; i < 4; i++)
    {
        // only run if they exist
        if(m_JoyDat[i].JoyFound)
        {

            // run basic setups
            m_diObj->CreateDevice(m_JoyDat[i].JoyGUID, &m_JoyDat[i].Joystick, NULL);
            m_JoyDat[i].Joystick->SetCooperativeLevel(m_hWin, DISCL_EXCLUSIVE | DISCL_FOREGROUND);  //  FOREGROUND
            m_JoyDat[i].Joystick->SetDataFormat(&c_dfDIJoystick2);

            // Used to set joystick stats
            DIPROPRANGE diPropRange;
            memset(&diPropRange, 0, sizeof(diPropRange));

            // set the min and max size for the axis.
            diPropRange.lMin = -1000;
            diPropRange.lMax = 1000;

            // Set the Sizes of the structures.
            diPropRange.diph.dwSize = sizeof(DIPROPRANGE);
            diPropRange.diph.dwHeaderSize = sizeof(DIPROPHEADER);

            // Set the property by offset.
            diPropRange.diph.dwHow = DIPH_BYOFFSET;

            // Set the Axis Range.
            diPropRange.diph.dwObj = DIJOFS_X;

            // Set the Range property for that Axis.
            m_JoyDat[i].Joystick->SetProperty(DIPROP_RANGE, &diPropRange.diph);

            // Set the Axis Range for the Y-Axis.
            diPropRange.diph.dwObj = DIJOFS_Y;

            m_JoyDat[i].Joystick->SetProperty(DIPROP_RANGE, &diPropRange.diph);
            m_JoyDat[i].Joystick->Acquire();
        }
    }
}

bool CInput::UpdateJoystick()
{
    for(int i = 0; i < NUM_JOYSTICKS; i++)
    {
        // make sure it was found
        if(m_JoyDat[i].JoyFound)
        {
            memset(&m_JoyDat[i].JoyState, 0, sizeof(m_JoyDat[i].JoyState));
            
            if(FAILED(m_hResult = m_JoyDat[i].Joystick->Poll()))
            {
                m_JoyDat[i].Joystick->Acquire();
                
                // is it plugged in?
                if(!m_JoyDat[i].JoyUnplug)
                {
                    if(m_hResult == DIERR_UNPLUGGED)
                    {
                        m_JoyDat[i].JoyUnplug = true;
                    }
                }
                
                // try and find the stick
                if(m_JoyDat[i].JoyUnplug)
                    //try to reacquire it
                    if(m_JoyDat[i].Joystick->Acquire() == DI_OK)
                        m_JoyDat[i].JoyUnplug = false;
            
                 if(m_hResult == DIERR_INPUTLOST)
                 {
                     //m_LogMe << "Joystick Input Lost Reacquiring Now JoyStick# - " << i+1 << ' ' << DXGetErrorString9(m_hResult); 
                     //m_LogMe << DXGetErrorDescription9(m_hResult) << "\n";
                     m_hResult = m_JoyDat[i].Joystick->Acquire();
                     
                 }
                 return true; // worked.

            }

            if(FAILED(m_hResult = m_JoyDat[i].Joystick->GetDeviceState(sizeof(DIJOYSTATE2),
                &m_JoyDat[i].JoyState)))
            {
                //m_LogMe << "Joystick GetDeviceState Failed# - " << i+1 << " - " << DXGetErrorString9(m_hResult); 
                //m_LogMe << " - " << DXGetErrorDescription9(m_hResult) << "\n";
                return false; // Device not acquired.
            }
        }
    }
    return true;
}

bool CInput::JoyPush(int iButton, int iID)   
{
    // Caps the game to four controllers
    if(iID < 0 || iID > 3)
        return false;

    if(m_JoyDat[iID].JoyFound)
    {
        if(m_JoyDat[iID].JoyState.rgbButtons[iButton] & 0x80)
            return true;
    }

    return false;
}

bool CInput::JoyBuffPush(int iButton, int iID)
{
    // Caps the game to four controllers
    if(iID < 0 || iID > 3)
        return false;

    if(m_JoyDat[iID].JoyFound)
    {
        if(JoyPush(iButton, iID))
        {
            if(!m_JoyDat[iID].JoyButtons[iButton])
            {
                // succeeded, it was hit
                m_JoyDat[iID].JoyButtons[iButton] = true;
                return true;
            }
        }
        // failed.
        else// if(!JoyPush(iButton, iID)) 
           m_JoyDat[iID].JoyButtons[iButton] = false;      
    }
    return false;
}

int CInput::GetJoyDir(int iID)
{
    // caps the game to four controllers.
    if(iID < 0 || iID > 3)
        return false;

    // Check Through th directions
    if(m_JoyDat[iID].JoyFound)
    {
        // If its at rest, it should be neutral
        if(m_JoyDat[iID].JoyState.lY > -800 && m_JoyDat[iID].JoyState.lY < 800 &&
           m_JoyDat[iID].JoyState.lX > -800 && m_JoyDat[iID].JoyState.lX < 800)
           return JOYSTICK_NEUTRAL;

        // Test Up and Right
        if(m_JoyDat[iID].JoyState.lY < -800 && m_JoyDat[iID].JoyState.lX > 800)
           return JOYSTICK_UPRIGHT;
        // Test Down and Right
        //if(m_JoyDat[iID].JoyState.lY > 800 && m_JoyDat[iID].JoyState.lX > 800)
        //   return JOYSTICK_DOWNRIGHT;
        // Test Down and Left
        //if(m_JoyDat[iID].JoyState.lY > 800 && m_JoyDat[iID].JoyState.lX < -800)
        //   return JOYSTICK_DOWNLEFT;
        // Test Up and Left
        if(m_JoyDat[iID].JoyState.lY < -800 && m_JoyDat[iID].JoyState.lX < -800)
           return JOYSTICK_UPLEFT;
        // Test Left
        if(m_JoyDat[iID].JoyState.lX < -800)
           return JOYSTICK_LEFT;
        // Test Right
        if(m_JoyDat[iID].JoyState.lX > 800)              
           return JOYSTICK_RIGHT;  
        // Test Up
        if(m_JoyDat[iID].JoyState.lY < -800)
           return JOYSTICK_UP;
        // Test Down
        if(m_JoyDat[iID].JoyState.lY > 800)
           return JOYSTICK_DOWN;
          
    }
    return false;
}

BOOL CALLBACK EnumJoystickCallback(const DIDEVICEINSTANCE * pDid, void *pV)
{
    // int to keep track of the amount of joysticks
    static int nCount= 0;

    
    CInput * pInput = CInput::GetInstance();
    pInput->m_JoyDat[nCount].JoyFound = true;
    pInput->m_JoyDat[nCount].JoyGUID = pDid->guidInstance;
    pInput->m_JoyDat[nCount].JoyUnplug = false;
    pInput->m_iNumJoysticks++;

    pInput->m_JoyDat[nCount].JoyButtons[0] = 0;

    nCount++;
    return DIENUM_CONTINUE;
}