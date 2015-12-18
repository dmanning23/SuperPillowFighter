////////////////////////////////////////////////////////
//
// Name: "InputInterface.cpp"
//
// Author: Daron Keeler (DK)
//
// Creation Date: August 8, 2004
//
// Purpose: The input interface will use the input manager to control the game.
//
////////////////////////////////////////////////////////

#include "InputInterface.h"
#include "..\\MemManager\\MemManager.h"
#include "..\\GameManager\\GameManager.h"
#include "..\\Entities\\Player.h"


CInputInterface *CInputInterface::pInstance = 0;

CInputInterface::CInputInterface()
{

}

void CInputInterface::InitInputInterface(HWND hWnd, HINSTANCE hInst)
{
    //for(int i = 0; i < 4; i++)
    //   m_bPlayerValidity[i] = false;

	//m_iMenuControllers = 0;
    //m_iActivePlayers = 0;
    m_iTypeFlag = INPUT_MENUMODE;

    //CInput * Input = CInput::GetInstance();

    CInput::GetInstance()->InitInput(hWnd, hInst);
    CInput::GetInstance()->InitKeyBoard();
    CInput::GetInstance()->InitJoy();

    // Go through and set validity active once for each available joystick.
	//for(int i = 0; i < CInput::GetInstance()->m_iNumJoysticks; i++)
	//{
    //    m_bPlayerValidity[i] = true;
	//	m_iMenuControllers++;
	//}
	//if(!(CInput::GetInstance()->m_iNumJoysticks))
    //    m_iActivePlayers = 1;
}

void CInputInterface::InitGameInput()
{
    // Loop through each PLAYER and initialize the right amount of joysticks.
    /*for(int i = 0; i < CInput::GetInstance()->m_iNumJoysticks; i++)
    {    
		//Make sure its a valid player!
		if (i < CObjectManager::GetInstance()->m_iPlayerArraySize)
		{
			// check to see if the player is type input or not.
			if(CObjectManager::GetInstance()->m_PlayerArray[i]->m_pPlayerData->iInputType == 1 && m_bPlayerValidity[i] == true)
			{
				m_iActivePlayers++;
				m_iTypeFlag = INPUT_GAMEMODE;
			}
		}
    }

	if(!(CInput::GetInstance()->m_iNumJoysticks))
        m_iTypeFlag = INPUT_KEYBOARDMODE;	*/
}

void CInputInterface::UpdateInput()
{
    // Always update the basic input types
    CInput::GetInstance()->UpdateKeyBoard();
    CInput::GetInstance()->UpdateBuffKeyBoard();
    CInput::GetInstance()->UpdateJoystick();

#ifdef _DEBUG
    if(CInput::GetInstance()->GetBuffKey(DIK_F1))
    {
        //change collision
        CGameManager::GetInstance()->GameDebugInfo.bCollisionData = !CGameManager::GetInstance()->GameDebugInfo.bCollisionData;

    }
    if(CInput::GetInstance()->GetBuffKey(DIK_F2))
    {
        // change FPS
        CGameManager::GetInstance()->GameDebugInfo.FPS = !CGameManager::GetInstance()->GameDebugInfo.FPS;
    }
    if(CInput::GetInstance()->GetBuffKey(DIK_F3))
    {
        // change AI
        CGameManager::GetInstance()->GameDebugInfo.AIStates = !CGameManager::GetInstance()->GameDebugInfo.AIStates;
    }
    if(CInput::GetInstance()->GetBuffKey(DIK_F4))
    {
        // change player states
        CGameManager::GetInstance()->GameDebugInfo.PlayerStates = !CGameManager::GetInstance()->GameDebugInfo.PlayerStates;
    }
#endif



    switch(m_iTypeFlag)
    {

    case INPUT_MENUMODE:
        {
            // Actions are not required for menu's. 
            // The menu will make specific key press calls to handle menus.
        }
        break;

    /*case INPUT_KEYBOARDMODE:
        {
			if (CGameManager::GetInstance()->m_bArcadeMode)
			{
				//TODO
				// Add in an Esc key for pause

				// Handle directions for keyboard
				if(CInput::GetInstance()->GetKey(DIK_NUMPAD4))
				{

					CObjectManager::GetInstance()->m_PlayerArray[0]->m_iMovement = 1;    
					CObjectManager::GetInstance()->m_PlayerArray[0]->SendStateMessage(Left_action);
				}
				else if(CInput::GetInstance()->GetKey(DIK_NUMPAD6))
				{
					CObjectManager::GetInstance()->m_PlayerArray[0]->m_iMovement = 2;
					CObjectManager::GetInstance()->m_PlayerArray[0]->SendStateMessage(Right_action);
				}
	            
				else
				{
					CObjectManager::GetInstance()->m_PlayerArray[0]->m_iMovement = 0;
					CObjectManager::GetInstance()->m_PlayerArray[0]->SendStateMessage(Neutral_action);
				}

				//fuck blocking
				//// Block
				//if(CInput::GetInstance()->GetBuffKey(DIK_D))
				//{
					
				//   //CObjectManager::GetInstance()->m_PlayerArray[0]->SendStateMessage(Block_action);  
				//   break;
				//}

				// Attacks
				if(CInput::GetInstance()->GetBuffKey(DIK_Z))
				{
					// if not 0, player did a Forward Attack
					if(CObjectManager::GetInstance()->m_PlayerArray[0]->m_iMovement)
					{
						CObjectManager::GetInstance()->m_PlayerArray[0]->SendStateMessage(ForwardAttack_action);
						break;
					}

					// if players pressed up, send UpAttack action
					if(CInput::GetInstance()->GetKey(DIK_NUMPAD8))
					{
						CObjectManager::GetInstance()->m_PlayerArray[0]->SendStateMessage(UpAttack_action);
						break;
					}

					// if players pressed down, send DownAttack action
					if(CInput::GetInstance()->GetKey(DIK_NUMPAD2))
					{
						CObjectManager::GetInstance()->m_PlayerArray[0]->SendStateMessage(DownAttack_action);
						break;
					}

					// Apparently they arent attacking in a direction, so just do the basic ones.
					CObjectManager::GetInstance()->m_PlayerArray[0]->SendStateMessage(Attack_action);
					break;
				}

				// Jump
				if(CInput::GetInstance()->GetBuffKey(DIK_LSHIFT))
				{
					CObjectManager::GetInstance()->m_PlayerArray[0]->SendStateMessage(Jump_action);    
					break;
				}
			}
			else
			{
				//TODO
				// Add in an Esc key for pause

				// Handle directions for keyboard
				if(CInput::GetInstance()->GetKey(DIK_LEFT))
				{

					CObjectManager::GetInstance()->m_PlayerArray[0]->m_iMovement = 1;    
					CObjectManager::GetInstance()->m_PlayerArray[0]->SendStateMessage(Left_action);
				}
				else if(CInput::GetInstance()->GetKey(DIK_RIGHT))
				{
					CObjectManager::GetInstance()->m_PlayerArray[0]->m_iMovement = 2;
					CObjectManager::GetInstance()->m_PlayerArray[0]->SendStateMessage(Right_action);
				}
	            
				else
				{
					CObjectManager::GetInstance()->m_PlayerArray[0]->m_iMovement = 0;
					CObjectManager::GetInstance()->m_PlayerArray[0]->SendStateMessage(Neutral_action);
				}

				//fuck blocking
				//// Block
				//if(CInput::GetInstance()->GetBuffKey(DIK_D))
				//{
					
				//   //CObjectManager::GetInstance()->m_PlayerArray[0]->SendStateMessage(Block_action);  
				//   break;
				//}

				// Attacks
				if(CInput::GetInstance()->GetBuffKey(DIK_A))
				{
					// if not 0, player did a Forward Attack
					if(CObjectManager::GetInstance()->m_PlayerArray[0]->m_iMovement)
					{
						CObjectManager::GetInstance()->m_PlayerArray[0]->SendStateMessage(ForwardAttack_action);
						break;
					}

					// if players pressed up, send UpAttack action
					if(CInput::GetInstance()->GetKey(DIK_UP))
					{
						CObjectManager::GetInstance()->m_PlayerArray[0]->SendStateMessage(UpAttack_action);
						break;
					}

					// if players pressed down, send DownAttack action
					if(CInput::GetInstance()->GetKey(DIK_DOWN))
					{
						CObjectManager::GetInstance()->m_PlayerArray[0]->SendStateMessage(DownAttack_action);
						break;
					}

					// Apparently they arent attacking in a direction, so just do the basic ones.
					CObjectManager::GetInstance()->m_PlayerArray[0]->SendStateMessage(Attack_action);
					break;
				}

				// Jump
				if(CInput::GetInstance()->GetBuffKey(DIK_W))
				{
					CObjectManager::GetInstance()->m_PlayerArray[0]->SendStateMessage(Jump_action);    
					break;
				}
			}
        }
        break;   */

    case INPUT_GAMEMODE:
        {
			int iTempDirections = 0;

			// change the game to pause mode.
			if(CInput::GetInstance()->GetBuffKey(DIK_ESCAPE))
			{

			}

			
			for(int j = 0; j < CObjectManager::GetInstance()->m_iPlayerArraySize; j++)
			{

				if(CObjectManager::GetInstance()->m_PlayerArray[j] == 0)
					continue;

				switch(CObjectManager::GetInstance()->m_PlayerArray[j]->m_pPlayerData->iInputType)
				{

				// None - Don't do anything.
				case None_input:
					break;
				// Default keyboard - input for one player.
				case Keyboard_input:
					{
						// Handle directions for keyboard
						if(CInput::GetInstance()->GetKey(DIK_LEFT))
						{

							CObjectManager::GetInstance()->m_PlayerArray[j]->m_iMovement = 1;    
							CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(Left_action);
						}
						else if(CInput::GetInstance()->GetKey(DIK_RIGHT))
						{
							CObjectManager::GetInstance()->m_PlayerArray[j]->m_iMovement = 2;
							CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(Right_action);
						}
		            
						else
						{
							CObjectManager::GetInstance()->m_PlayerArray[j]->m_iMovement = 0;
							CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(Neutral_action);
						}
		
						
						// Attacks
						if(CInput::GetInstance()->GetBuffKey(DIK_A))
						{
							// if not 0, player did a Forward Attack
							if(CObjectManager::GetInstance()->m_PlayerArray[j]->m_iMovement)
							{
								CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(ForwardAttack_action);
								break;
							}

							// if players pressed up, send UpAttack action
							if(CInput::GetInstance()->GetKey(DIK_UP))
							{
								CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(UpAttack_action);
								break;
							}

							// if players pressed down, send DownAttack action
							if(CInput::GetInstance()->GetKey(DIK_DOWN))
							{
								CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(DownAttack_action);
								break;
							}

							// Apparently they arent attacking in a direction, so just do the basic ones.
							CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(Attack_action);
							break;
						}

						// Jump
						if(CInput::GetInstance()->GetBuffKey(DIK_W))
						{
							CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(Jump_action);    
							break;
						}
					}
					break;
				// AI bot - handled by the AI module, don't do anything.
				case AI_input:
					break;
				// 1st Joystick - Player 1's input
				case Joystick1_input:
					{
						// figure out whether or not the player pressed left or right. 
						// If they did a variable must be changed accordingly in the Player
						iTempDirections = CInput::GetInstance()->GetJoyDir(INPUT_JOY1);
						// Left        
						if(iTempDirections == JOYSTICK_LEFT || iTempDirections == JOYSTICK_UPLEFT)
						{	
							CObjectManager::GetInstance()->m_PlayerArray[j]->m_iMovement = 1;
							CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(Left_action);
						}
						// Right
						else if(iTempDirections == JOYSTICK_RIGHT || iTempDirections == JOYSTICK_UPRIGHT)
						{
							CObjectManager::GetInstance()->m_PlayerArray[j]->m_iMovement = 2;
							CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(Right_action);

						}
						// Neutral 
						else
						{
							CObjectManager::GetInstance()->m_PlayerArray[j]->m_iMovement = 0;
							CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(Neutral_action);
						}

						
						//////////////////////////////////////////////////
						// Start checkin which action the player attempted
						//////////////////////////////////////////////////

						// Attacks
						if(CInput::GetInstance()->JoyBuffPush(INPUT_ATTACK, INPUT_JOY1))
						{
							// if not 0, player did a Forward Attack
							if(CObjectManager::GetInstance()->m_PlayerArray[j]->m_iMovement)
							{
								CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(ForwardAttack_action);
								break;
							}

							// if players pressed up, send UpAttack action
							if(iTempDirections == JOYSTICK_UP)
							{
								CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(UpAttack_action);
								break;;
							}

							// if players pressed down, send DownAttack action
							if(iTempDirections == JOYSTICK_DOWN)
							{
								CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(DownAttack_action);
								break;
							}

							// Apparently they arent attacking in a direction, so just do the basic ones.
							CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(Attack_action);
							break;
						}

						// Jump
						if(CInput::GetInstance()->JoyBuffPush(INPUT_JUMP, INPUT_JOY1))
						{
							CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(Jump_action);
							break;
						}

						// No actions have been taken by the player this frame, so return neutral
						//CObjectManager::GetInstance()->m_PlayerArray[i]->SendStateMessage(Neutral_action);
		            
					}
					break;
				// 2nd Joystick - Player 2's input
				case Joystick2_input:
					{
						// figure out whether or not the player pressed left or right. 
						// If they did a variable must be changed accordingly in the Player
						iTempDirections = CInput::GetInstance()->GetJoyDir(INPUT_JOY2);
						// Left        
						if(iTempDirections == JOYSTICK_LEFT || iTempDirections == JOYSTICK_UPLEFT)
						{	
							CObjectManager::GetInstance()->m_PlayerArray[j]->m_iMovement = 1;
							CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(Left_action);
						}
						// Right
						else if(iTempDirections == JOYSTICK_RIGHT || iTempDirections == JOYSTICK_UPRIGHT)
						{
							CObjectManager::GetInstance()->m_PlayerArray[j]->m_iMovement = 2;
							CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(Right_action);

						}
						// Neutral 
						else
						{
							CObjectManager::GetInstance()->m_PlayerArray[j]->m_iMovement = 0;
							CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(Neutral_action);
						}

						
						//////////////////////////////////////////////////
						// Start checkin which action the player attempted
						//////////////////////////////////////////////////

						// Attacks
						if(CInput::GetInstance()->JoyBuffPush(INPUT_ATTACK, INPUT_JOY2))
						{
							// if not 0, player did a Forward Attack
							if(CObjectManager::GetInstance()->m_PlayerArray[j]->m_iMovement)
							{
								CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(ForwardAttack_action);
								break;
							}

							// if players pressed up, send UpAttack action
							if(iTempDirections == JOYSTICK_UP)
							{
								CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(UpAttack_action);
								break;;
							}

							// if players pressed down, send DownAttack action
							if(iTempDirections == JOYSTICK_DOWN)
							{
								CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(DownAttack_action);
								break;
							}

							// Apparently they arent attacking in a direction, so just do the basic ones.
							CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(Attack_action);
							break;
						}

						// Jump
						if(CInput::GetInstance()->JoyBuffPush(INPUT_JUMP, INPUT_JOY2))
						{
							CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(Jump_action);
							break;
						}

						// No actions have been taken by the player this frame, so return neutral
						//CObjectManager::GetInstance()->m_PlayerArray[i]->SendStateMessage(Neutral_action);
		            
					}
					break;
				// 3rd Joystick - Player 3's input
				case Joystick3_input:
					{
						// figure out whether or not the player pressed left or right. 
						// If they did a variable must be changed accordingly in the Player
						iTempDirections = CInput::GetInstance()->GetJoyDir(INPUT_JOY3);
						// Left        
						if(iTempDirections == JOYSTICK_LEFT || iTempDirections == JOYSTICK_UPLEFT)
						{	
							CObjectManager::GetInstance()->m_PlayerArray[j]->m_iMovement = 1;
							CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(Left_action);
						}
						// Right
						else if(iTempDirections == JOYSTICK_RIGHT || iTempDirections == JOYSTICK_UPRIGHT)
						{
							CObjectManager::GetInstance()->m_PlayerArray[j]->m_iMovement = 2;
							CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(Right_action);

						}
						// Neutral 
						else
						{
							CObjectManager::GetInstance()->m_PlayerArray[j]->m_iMovement = 0;
							CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(Neutral_action);
						}

						
						//////////////////////////////////////////////////
						// Start checkin which action the player attempted
						//////////////////////////////////////////////////

						// Attacks
						if(CInput::GetInstance()->JoyBuffPush(INPUT_ATTACK, INPUT_JOY3))
						{
							// if not 0, player did a Forward Attack
							if(CObjectManager::GetInstance()->m_PlayerArray[j]->m_iMovement)
							{
								CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(ForwardAttack_action);
								break;
							}

							// if players pressed up, send UpAttack action
							if(iTempDirections == JOYSTICK_UP)
							{
								CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(UpAttack_action);
								break;;
							}

							// if players pressed down, send DownAttack action
							if(iTempDirections == JOYSTICK_DOWN)
							{
								CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(DownAttack_action);
								break;
							}

							// Apparently they arent attacking in a direction, so just do the basic ones.
							CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(Attack_action);
							break;
						}

						// Jump
						if(CInput::GetInstance()->JoyBuffPush(INPUT_JUMP, INPUT_JOY3))
						{
							CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(Jump_action);
							break;
						}

						// No actions have been taken by the player this frame, so return neutral
						//CObjectManager::GetInstance()->m_PlayerArray[i]->SendStateMessage(Neutral_action);
		            
					}
					break;
				// 4th Joystick - Player 4's input
				case Joystick4_input:
					{
						// figure out whether or not the player pressed left or right. 
						// If they did a variable must be changed accordingly in the Player
						iTempDirections = CInput::GetInstance()->GetJoyDir(INPUT_JOY4);
						// Left        
						if(iTempDirections == JOYSTICK_LEFT || iTempDirections == JOYSTICK_UPLEFT)
						{	
							CObjectManager::GetInstance()->m_PlayerArray[j]->m_iMovement = 1;
							CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(Left_action);
						}
						// Right
						else if(iTempDirections == JOYSTICK_RIGHT || iTempDirections == JOYSTICK_UPRIGHT)
						{
							CObjectManager::GetInstance()->m_PlayerArray[j]->m_iMovement = 2;
							CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(Right_action);

						}
						// Neutral 
						else
						{
							CObjectManager::GetInstance()->m_PlayerArray[j]->m_iMovement = 0;
							CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(Neutral_action);
						}

						
						//////////////////////////////////////////////////
						// Start checkin which action the player attempted
						//////////////////////////////////////////////////

						// Attacks
						if(CInput::GetInstance()->JoyBuffPush(INPUT_ATTACK, INPUT_JOY4))
						{
							// if not 0, player did a Forward Attack
							if(CObjectManager::GetInstance()->m_PlayerArray[j]->m_iMovement)
							{
								CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(ForwardAttack_action);
								break;
							}

							// if players pressed up, send UpAttack action
							if(iTempDirections == JOYSTICK_UP)
							{
								CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(UpAttack_action);
								break;;
							}

							// if players pressed down, send DownAttack action
							if(iTempDirections == JOYSTICK_DOWN)
							{
								CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(DownAttack_action);
								break;
							}

							// Apparently they arent attacking in a direction, so just do the basic ones.
							CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(Attack_action);
							break;
						}

						// Jump
						if(CInput::GetInstance()->JoyBuffPush(INPUT_JUMP, INPUT_JOY4))
						{
							CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(Jump_action);
							break;
						}

						// No actions have been taken by the player this frame, so return neutral
						//CObjectManager::GetInstance()->m_PlayerArray[i]->SendStateMessage(Neutral_action);
		            
					}
					break;
				// Arcade Map One - Player 1's input
				case ArcadeKeyMap1_input:
					{
						// Handle directions for keyboard
						if(CInput::GetInstance()->GetKey(DIK_NUMPAD4))
						{

							CObjectManager::GetInstance()->m_PlayerArray[j]->m_iMovement = 1;    
							CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(Left_action);
						}
						else if(CInput::GetInstance()->GetKey(DIK_NUMPAD6))
						{
							CObjectManager::GetInstance()->m_PlayerArray[j]->m_iMovement = 2;
							CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(Right_action);
						}
		            
						else
						{
							CObjectManager::GetInstance()->m_PlayerArray[j]->m_iMovement = 0;
							CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(Neutral_action);
						}
		
						
						// Attacks
						if(CInput::GetInstance()->GetBuffKey(DIK_LSHIFT))
						{
							// if not 0, player did a Forward Attack
							if(CObjectManager::GetInstance()->m_PlayerArray[j]->m_iMovement)
							{
								CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(ForwardAttack_action);
								break;
							}

							// if players pressed up, send UpAttack action
							if(CInput::GetInstance()->GetKey(DIK_NUMPAD8))
							{
								CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(UpAttack_action);
								break;
							}

							// if players pressed down, send DownAttack action
							if(CInput::GetInstance()->GetKey(DIK_NUMPAD2))
							{
								CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(DownAttack_action);
								break;
							}

							// Apparently they arent attacking in a direction, so just do the basic ones.
							CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(Attack_action);
							break;
						}

						// Jump
						if(CInput::GetInstance()->GetBuffKey(DIK_Z))
						{
							CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(Jump_action);    
							break;
						}

					}
					break;
				// Arcade Map Two - Player 2's input
				case ArcadeKeyMap2_input:
					{
						// Handle directions for keyboard
						if(CInput::GetInstance()->GetKey(DIK_D))
						{

							CObjectManager::GetInstance()->m_PlayerArray[j]->m_iMovement = 1;    
							CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(Left_action);
						}
						else if(CInput::GetInstance()->GetKey(DIK_G))
						{
							CObjectManager::GetInstance()->m_PlayerArray[j]->m_iMovement = 2;
							CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(Right_action);
						}
		            
						else
						{
							CObjectManager::GetInstance()->m_PlayerArray[j]->m_iMovement = 0;
							CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(Neutral_action);
						}
		
						
						// Attacks
						if(CInput::GetInstance()->GetBuffKey(DIK_W))
						{
							// if not 0, player did a Forward Attack
							if(CObjectManager::GetInstance()->m_PlayerArray[j]->m_iMovement)
							{
								CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(ForwardAttack_action);
								break;
							}

							// if players pressed up, send UpAttack action
							if(CInput::GetInstance()->GetKey(DIK_R))
							{
								CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(UpAttack_action);
								break;
							}

							// if players pressed down, send DownAttack action
							if(CInput::GetInstance()->GetKey(DIK_F))
							{
								CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(DownAttack_action);
								break;
							}

							// Apparently they arent attacking in a direction, so just do the basic ones.
							CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(Attack_action);
							break;
						}

						// Jump
						if(CInput::GetInstance()->GetBuffKey(DIK_E))
						{
							CObjectManager::GetInstance()->m_PlayerArray[j]->SendStateMessage(Jump_action);    
							break;
						}
					}
					break;


				};
			}
        }
        break;
    };
}

CInputInterface *CInputInterface::GetInstance()
{
    // Make one if it hasnt been done.
    if(!pInstance)
    //    pInstance = new CInputInterface;
	{
     pInstance = (CInputInterface*)MemMalloc(sizeof(CInputInterface));
     pInstance->CInputInterface::CInputInterface();
	}

    // Return the address of the instance.
    return pInstance;
}

void CInputInterface::DeleteInstance()
{
    // delete the instance
    if(pInstance)
    {
        // delete pInstance;
        pInstance->CInputInterface::~CInputInterface();
         MemFree(pInstance);

        pInstance = NULL;
    }
}

void CInputInterface::ShutDownInputInterface()
{
    CInput::GetInstance()->ShutDownInput();
    CInput::DeleteInstance();
}


void CInputInterface::ResetGameInput()
{
    //m_iActivePlayers = 0;
    m_iTypeFlag = INPUT_MENUMODE;

}