///////////////////////////////////////////////////////////////
// File Name:     animImp.h
// 
// Author:        Matt Strand
//
// Date Created:  8/24/2004
//
// Purpose:       This file declares all exporter functions
//                and defines.  Purpose is to load and unload
//                animations for players.
///////////////////////////////////////////////////////////////
#ifndef _ANIM_IMP_H_
#define _ANIM_IMP_H_

#include "..\MathLibrary\spfMath.h"
class CPlayer;

// Ids for all the animation types in the game.
enum ANIM_TYPES{
	JOSIE_ANIMS = 0,
	SALLY_ANIMS,
	ELIZ_ANIMS,
	SUMI_ANIMS,
	NUM_TYPE_ANIMS,
};

// Ids for all the animations in the game.
enum ANIMATIONS{
	STANDING = 0,
	WALKING,
	JUMPING,
	FALLING,
	ATTACK,
	ATTACK_FORWARD,
	ATTACK_SPECIAL,
	ATTACK_UP,
	AIR_ATTACK_SPECIAL,
	AIR_ATTACK_FORWARD,
	AIR_ATTACK_DOWN,
	AIR_ATTACK_UP,
	THROWING,
	BIND,
	NUM_ANIMS,
};

// Associates each animation ID with a filename.
static const char *pcIdToFileName[NUM_TYPE_ANIMS][NUM_ANIMS] = {
	
	//	JOSIE_ANIMS
	{	"josieIdle.pfa",			//  Standing
		"josieRun.pfa",				//  Walking
		"josieJump.pfa",			//  Jumping
		"josieFall.pfa",			//  Falling
		"josieAttack.pfa",			//  Attack
		"josieAttackForward.pfa",	//  Attack Forward
		"josieAttackSpecial.pfa",	//  Attack Special
		"josieAttackUp.pfa",		//  Attack Up
		"josieAirAttackSpecial.pfa",//  Air Attack Special
		"josieAirAttackForward.pfa",//  Air Attack Forward
		"josieAirAttackDown.pfa",	//  Air Attack Down
		"josieAirAttackUp.pfa",		//  Air Attack Up
		"josieThrow.pfa",			//	Throwing
		"josie.pfb"},				//  Bind

	//	SALLY_ANIMS
	{	"CaptainSallyIdle.pfa",			//  Standing
		"CaptainSallyRun.pfa",		//  Walking
		"CaptainSallyJump.pfa",			//  Jumping
		"CaptainSallyFall.pfa",			//  Falling
		"CaptainSallyAttack.pfa",			//  Attack
		"CaptainSallyAttackForward.pfa",	//  Attack Forward
		"CaptainSallyAttackSpecial.pfa",	//  Attack Special
		"CaptainSallyAttackUp.pfa",		//  Attack Up
		"CaptainSallyAirAttackSpecial.pfa",//  Air Attack Special
		"CaptainSallyAirAttackForward.pfa",//  Air Attack Forward
		"CaptainSallyAirAttackDown.pfa",	//  Air Attack Down
		"CaptainSallyAirAttackUp.pfa",		//  Air Attack Up
		"CaptainSallyThrow.pfa",			//	Throwing
		"captainSally.pfb"},				//  Bind

	//	ELIZ_ANIMS
	{	"josieIdle.pfa",			//  Standing
		"josieRun.pfa",				//  Walking
		"josieJump.pfa",			//  Jumping
		"josieFall.pfa",			//  Falling
		"josieAttack.pfa",			//  Attack
		"josieAttackForward.pfa",	//  Attack Forward
		"josieAttackSpecial.pfa",	//  Attack Special
		"josieAttackUp.pfa",		//  Attack Up
		"josieAirAttackSpecial.pfa",//  Air Attack Special
		"josieAirAttackForward.pfa",//  Air Attack Forward
		"josieAirAttackDown.pfa",	//  Air Attack Down
		"josieAirAttackUp.pfa",		//  Air Attack Up
		"josieThrow.pfa",			//	Throwing
		"josie.pfb"},				//  Bind

	//	SUMI_ANIMS
	{	"josieIdle.pfa",			//  Standing
		"josieRun.pfa",				//  Walking
		"josieJump.pfa",			//  Jumping
		"josieFall.pfa",			//  Falling
		"josieAttack.pfa",			//  Attack
		"josieAttackForward.pfa",	//  Attack Forward
		"josieAttackSpecial.pfa",	//  Attack Special
		"josieAttackUp.pfa",		//  Attack Up
		"josieAirAttackSpecial.pfa",//  Air Attack Special
		"josieAirAttackForward.pfa",//  Air Attack Forward
		"josieAirAttackDown.pfa",	//  Air Attack Down
		"josieAirAttackUp.pfa",		//  Air Attack Up
		"josieThrow.pfa",			//	Throwing
		"josie.pfb"},				//  Bind
};

struct QuaternionT
{
	float tfX, tfY, tfZ, tfW;
};

struct AJointT
{
	int tiId;
	int tiNumParent, tiNumChild;
	/*Vec4DT tWTranslation, tWRotation, tWScale;	*/			//  World
	/*QuaternionT tLOrientation, tLSOrientation, tLRotation;*/	//  Local
	int *tpiParents, *tpiChildren;
	Matrix4x4T tLocalMat, tConcMat/*, tInvMat, tInvConcMat*/;
};

struct KeyframeT
{
	int tiFrame;
	float tfTime;
	AJointT *tpJointInfo;
};

struct AnimT
{
	int tiNumFrames;
	int tiTotalTime;	//  Milleseconds
	int tiJointCount;
	KeyframeT *tpKeyInfo;
};

struct BindT
{
	int tiJointCount;
	AJointT *tpJointInfo;
};

struct AnimationDataT
{
	int tiPrevState, tiPrevFrame;
	float tfPrevTime, tfStartTime, tfElapsedTime;
	AnimT *tpAnimations;
	BindT tBind;
};

// Path to the animation directory.
static const char *pcAnimPath = "ResourceFiles//animations//";

///////////////////////////////////////////////////////////////
// Function: loadAnim()
//
// Purpose: Opens multiple files to load all animations for
//          for a player.
//
// Last Modified: 8/24/2004
//
// Input: *pPlayer    - The player to load the animations to.
//        iAnimID     - The type of animations to load.
//
// Output: pPlayer::m_pAnimationData - Filled out.
//
// Returns: True - If success.
//
// Notes: Animation type IDs can be found in ANIM_TYPES enum.
///////////////////////////////////////////////////////////////
bool loadAnim(CPlayer *pPlayer, int iAnimID);

///////////////////////////////////////////////////////////////
// Function: unloadHeritage()
//
// Purpose: Releases all memory allocated for joint parents and
//          chidlren.
//
// Last Modified:9/18/2004
//
// Input: *pJoint    - Pointer to the joint array.
//        iNumJoints - Number of joints in the array.
//
// Output: pPlayer::m_pAnimationData - Partial Release.
//
// Returns: None.
//
// Notes: This is release memory that was allocated but is not
//        used later in the game other than for debugging.
//        Calling this makes debugging joints impossible.
///////////////////////////////////////////////////////////////
void unloadHeritage(AJointT *pJoint, const int iNumJoints);

///////////////////////////////////////////////////////////////
// Function: unloadAnim()
//
// Purpose: Releases all memory allocated for animations.
//
// Last Modified: 8/24/2004
//
// Input: *pPlayer    - The player whos animations are being
//                      unloaded
//
// Output: pPlayer::m_pAnimationData - Released.
//
// Returns: None.
//
// Notes: Pass in player, and have all their animations relased
///////////////////////////////////////////////////////////////
void unloadAnim(CPlayer *pPlayer);

///////////////////////////////////////////////////////////////
// Function: makeMatrix()
//
// Purpose: Concatenates the matrices from parents to chidlren.
//
// Last Modified: 9/18/2004
//
// Input: *pJoint    - Pointer to the joint to change.
//        &parentMat - The parent's final matrix.
//        &frame     - What keyframe we're operating in.
//
// Output: pPlayer::m_pAnimationData - Joint matrices modified.
//
// Returns: None.
//
// Notes: Called only once after a frame is loaded.
//        Recursive.
///////////////////////////////////////////////////////////////
static void makeMatrix(AJointT *pJoint, const Matrix4x4T &parentMat, const KeyframeT &frame);

///////////////////////////////////////////////////////////////
// Function: makeInvMatrix()
//
// Purpose: Concatenates the inverted matrices from parents to
//          chidlren.
//
// Last Modified: 9/18/2004
//
// Input: *pJoint       - Pointer to the joint to change.
//        &parentInvMat - The parent's final inverted matrix.
//        &frame        - What keyframe we're operating in.
//
// Output: pPlayer::m_pAnimationData->tBind - Joint matrices
//                                            modified.
//
// Returns: None.
//
// Notes: Called only once after bind is loaded.
//        Recursive.
///////////////////////////////////////////////////////////////
static void makeInvMatrix(AJointT *pJoint, const Matrix4x4T &parentInvMat, AJointT *joints);

//void Quat2Eul(QuaternionT *resultQuat, const QuaternionT &input);

#endif