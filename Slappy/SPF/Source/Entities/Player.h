#ifndef _CPLAYER_H_
#define _CPLAYER_H_
/*////////////////////////////////////////////////////////////////////////
//File:	Player.h
//
//Author:	Dan Manning
//
//Creation Date:	8-9-4
//
//Purpose:	Class definition of the CPlayer object
//
/////////////////////////////////////////////////////////////////////////*/
#include "BaseObject.h"
#include "Pillow.h"
#include "Powerup.h"
#include "..\\StateMachine\\State_Manager.h"
#include "..\\GameTimer\\StopWatch.h"
#include "..\\ContentLoader\\animImp.h"

#define MAX_FILENAME_LENGTH 64

//The amount of time the player stays in the "falling" state
#define FALLING_TIME		0.7f

#define PICKUP_SOUND 0
#define VICTORY_SOUND 1
#define TAUNT_SOUND 2
#define HIT_SOUND 3
#define GOTHIT_SOUND 4

//This enumeration is all the possible player characters that can be chosen
enum PlayerCharacters
{
	Josie_Player,
	CaptainSally_Player,
	MissElizabeth_Player,
	Sumi_Player,
	Num_Characters
};

//Enumeration of possible input types for a player
enum {
	None_input,
	Keyboard_input,
	AI_input,
	Joystick1_input,
	Joystick2_input,
	Joystick3_input,
	Joystick4_input,
	ArcadeKeyMap1_input,
	ArcadeKeyMap2_input,
	Num_InputTypes
};

//This structure holds game specific data for a player
struct PlayerDataT
{
	//this variable holds the id of the character
	int iCharacter;

	//This holds the color for the palette swap
	float PaletteColor[3];

	//int to tell what type of input this player takes, as enumerated above
	int iInputType;

	//bool to tell if there is a player or not
	bool bIsActive;
};

//These are all the possible states a player can be in
//You'd better read these comments and the tech doc before you ask me questions about this shit.
enum PlayerStates
{
	Standing_state,	//player is standing on the ground
	Walking_state,	//player is walking on the ground
	Jumping_state,	//player has jumped in the air
	DoubleJumping_state,	//player has jumped twice in the air
	Attack_state,	//player is doing the ground attack (neutral stick)
	ForwardAttack_state,	//player is doing ground forward attack (left/right stick)
	SpecialAttack_state,	//player is doing ground special attack
	UpAttack_state,	//player is doing ground up attack
	AirSpecialAttack_state,	//player is doing air special attack (neutral stick)
	/*NOTES: This state needs knowledge of the previous state.  After this state is done, the player will
	go to the previous state, either Jumping_state or DoubleJumping_state*/
	AirDownAttack_state,	//player is doing air downward attack
	/*NOTES: This state needs knowledge of the previous state.  After this state is done, the player will
	go to the previous state, either Jumping_state or DoubleJumping_state*/
	AirUpAttack_state,	//player is doing air up attack
	/*NOTES: This state needs knowledge of the previous state.  After this state is done, the player will
	go to the previous state, either Jumping_state or DoubleJumping_state*/
	AirForwardAttack_state,	//player is doing air forward attack (left/right stick)
	/*NOTES: This state needs knowledge of the previous state.  After this state is done, the player will
	go to the previous state, either Jumping_state or DoubleJumping_state*/
	Throwing_state,	//player is in the throwing state
	/*NOTES: This state needs knowledge of the previous state.  After this state is done, the player will
	go to the previous state, either Standing_state, Jumping_state or DoubleJumping_state*/
    Blocking_state,	//player is in the blocking state (on ground, holding block button)
	Falling_state,	//player has been hit and is falling through the air (no control)
	Dead_state,	//TRANSITION STATE - player is fucking dead!!!
	PhysicsUp_state,	//TRANSITION STATE - player needs to have physics applied in +y direction
	PhysicsLeft_state,	//TRANSITION STATE - player needs to have physics applied in -x direction
	/*NOTES: This state needs knowledge of the previous state.  After this state is done, the player will
	go to the previous state, either Walking_state, Jumping_state or DoubleJumping_state*/
	PhysicsRight_state,	//TRANSITION STATE - player needs to have physics applied in +x direction
	/*NOTES: This state needs knowledge of the previous state.  After this state is done, the player will
	go to the previous state, either Walking_state, Jumping_state or DoubleJumping_state*/
	AttackTransition_state,	//TRANSITION STATE - player has tried to go to attack state
	/*NOTES: The physics needs to check if the player is colliding with an emitted powerup. If she is, 
	attach the powerup to the player.  Send the EndTransition_action after processing this state.  After
	this state, the player will go to either standin, jumping, or doublejumping*/
	Num_Player_States
};

//These are all the possible actions that can be sent to a player
enum PlayerActions
{
	Left_action, //This action is sent by the input and ai
	Right_action,	//this action is sent by the input and ai
	Block_action,	//this action is sent by the input and ai
	Hit_action,	//This action is sent by the event manager
	Jump_action,	//this action is sent by the input and ai
	Death_action,	//this action is sent by the event manager
	Attack_action,	//This action is sent by the input and ai
	/*NOTES:	This action will go to the attack_state or airSpecialAttack_state if the player is
	not holding a powerup.  Otherwise, it will go to either standin, jumping, or doublejumoing, 
	depending on the previous state*/	
	DownAttack_action,	//This action will be sent by the input and ai
	UpAttack_action,	//this action will be sent by the input and ai
	ForwardAttack_action,	//this action will be sent by the input and ai
	Neutral_action,	//this action will be sent by the input and ai
	Timer_action,	//this action will be sent internally by the player
	GroundCollision_action,	//this action will be sent by the event manager
	EndTransitionState_action,	//this action will be sent internally by the player
	Num_Player_Actions
};

class CPlayer : public CBaseObject
{
	private:

		//The player state machine
		CStateMachine m_PlayerStateMachine;

		/*This timer will be used to time certain aspects of the player object, 
		including timing  the attacks and how long the player loses control 
		after being hit.*/
		CStopWatch m_PlayerTimer;

		/*////////////////////////////////////////////////////////////////////////
		//Function:	LoadPlayer
		//
		//Purpose:	
		//This function will load all the player stats from the correct file,
		//based on the player id in the m_pPlayerData.
		//
		/////////////////////////////////////////////////////////////////////////*/
		void LoadPlayer();

	public:

		//This variable holds the id of the previous state
		int m_iPreviousState;

		//The index of the model and animation to load for this player
		int m_iModelIndex;
		int m_iAnimationIndex;

		//This variable holds the velocity of the player.  This is used by the physics engine
		//when the player gets hit.
		Vec4DT m_Velocity;

		//This holds the indexes of the unique sounds for this player.  These are character specific.
		int m_iUniqueSounds[5];

		//Filenames of unique sounds for this player
		char m_strPickupFileName[MAX_FILENAME_LENGTH];
		char m_strVictoryFileName[MAX_FILENAME_LENGTH];
		char m_strTauntFileName[MAX_FILENAME_LENGTH];
		char m_strHitFileName[MAX_FILENAME_LENGTH];
		char m_strGotHitFileName[MAX_FILENAME_LENGTH];

		//This variable will point to all the animation data for this player
		AnimationDataT *m_pAnimationData;

		//This holds game specific player information
		PlayerDataT *m_pPlayerData;

		//These are the player stats
		//Depending on which character is loaded, 
		//these are loaded at run time from the players.txt file
		float m_fAttackSpeed;	//how long it takes a player to execute a move in seconds
		float m_fWalkSpeed;	//how many meters/second a player walks at
		float m_fJumpHeight;	//how many meters high a player can jump
		int m_iMass;		//how many kilograms a player wieghs
		int m_iAttackPower;	//how many damage points a player inflicts when they hit another player

		//This variable is used by the input and AI to move the player.  
		//This variable will hold either neutral, left, or right. 
		int m_iMovement;

		//This points to the current powerup the player is holding
		CPowerup *m_pPowerup;

		//This points to the player's pillow
		CPillow *m_pPillow;
		//The pillow geometry for this player
		int m_iPillowGeometry;

		/*This pointer points to the memory address of the last player to hit this player.  
		This pointer points to null until the player gets hit, at which time this pointer 
		will be set to point to the attacking player.  When this player collides with the 
		ground again, this variable will be set to null.  This provides a way to tell who 
		was the last person to hit this player and increment their score in Time Attack 
		and Knockout modes.*/
		CPlayer *m_pLastPlayerToHit;
	
		/*In Knockout and Time Attack mode, this variable holds the number of kills the player 
		has.  In Survival mode, this variable holds the number of lives remaining.  */
		int m_iScore;

		//This holds the amount of damage that has been dealt to this player
		int m_iDamage;
        
	public:

		/*////////////////////////////////////////////////////////////////////////
		//Function:	Constructor
		//
		//Purpose:	This sets up a player and all member variables, including instances
		//			of other classes.
		//
		/////////////////////////////////////////////////////////////////////////*/
		CPlayer(PlayerDataT *PlayerData);

		/*////////////////////////////////////////////////////////////////////////
		//Function:	Destructor
		//
		//Purpose:	This shuts down a player, including instances of other classes.
		//		
		/////////////////////////////////////////////////////////////////////////*/
		virtual ~CPlayer(void);

		/*////////////////////////////////////////////////////////////////////////
		//Function:	Update
		//
		//Purpose:	This function updates the player.  It will be called every frame.
		//	This function will do such things as update the state machine and check the timer.
		//
		/////////////////////////////////////////////////////////////////////////*/
		void Update();

		/*////////////////////////////////////////////////////////////////////////
		//Function:	SendStateMessage
		//
		//Input:	
		//ActionMessage:
		//The action to send to the player
		//
		//Purpose:	This function provides the functionality for other modules
		//to send state actions to the players.
		//
		/////////////////////////////////////////////////////////////////////////*/
		void SendStateMessage(int iActionMessage);

		/*////////////////////////////////////////////////////////////////////////
		//Function:	GetState
		//
		//Return:
		//int:
		//The current state. 
		//
		//Purpose:	this function provides a way for other modules to access the 
		//	player's state
		//
		/////////////////////////////////////////////////////////////////////////*/
		int GetState() { return m_PlayerStateMachine.GetCurrentState(); }

		/*////////////////////////////////////////////////////////////////////////
		//Function:	PickUpPowerup
		//
		//Input:	pPowerup:	the powerup to pick up
		//
		//Return:	true:	player was able to pick up the powerup
		//			false:	player is already holding a powerup
		//
		//Purpose:	provide a streamlined way for other modules to get a player to 
		//			hold a powerup
		//
		/////////////////////////////////////////////////////////////////////////*/
		bool PickUpPowerup(CPowerup *pPowerup);

		/*////////////////////////////////////////////////////////////////////////
		//Function:	ThrowPowerup
		//
		//Purpose:	Provide a streamlined interface for other modules to tell the
		//			player to throw his powerup
		//
		/////////////////////////////////////////////////////////////////////////*/
		void ThrowPowerup();
};
#endif