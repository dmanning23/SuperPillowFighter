//All variable game stats for players and powerups

#include "..\..\..\ContentLoader\GeometryImp.h"
#include "..\..\..\ContentLoader\AnimImp.h"

//WINDOW DATA
//All the data for the starting window mode
#define WINDOWED_MODE			true
#define WINDOW_WIDTH			1024
#define WINDOW_HEIGHT			768
#define ARCADE_MODE				false

//COLLISION DATA
//Player Collision Data
#define CENTER_POINT			1.0f
#define CENTER_RADIUS			0.5f
#define ENTIRE_RADIUS			1.0f
#define TOP_CENTER_POINT		1.5f
#define TOP_RADIUS				0.5f
#define BOTTOM_CENTER_POINT		0.5f
#define BOTTOM_RADIUS			0.5f

//Item Collision Data
#define ITEM_RADIUS				0.5f

//Pillow Collision Data
#define PILLOW_RADIUS			0.75f

//PLAYER STATS
//Josie stats
#define JOSIE_ATTACKSPEED		0.45f
#define JOSIE_WALKSPEED			15.0f
#define JOSIE_JUMPHEIGHT		9.0f
#define JOSIE_MASS				100
#define JOSIE_ATTACKPOWER		5
#define JOSIE_GEOMETRY_DATA		JOSIE_MODEL
#define JOSIE_ANIMATION_DATA	JOSIE_ANIMS
#define JOSIE_PILLOW_DATA		JOSIESPILLOW_MODEL
//sound data
#define JOSIE_PICKUPSOUND		"ResourceFiles\\Sounds\\Josie_Banana.wav"
#define JOSIE_VICTORYSOUND		"ResourceFiles\\Sounds\\Josie_Bump.wav"
#define JOSIE_TAUNTSOUND		"ResourceFiles\\Sounds\\Josie_Puddin.wav"
#define JOSIE_HITSOUND			"ResourceFiles\\Sounds\\Josie_hehe.wav"
#define JOSIE_GOTHIT			"ResourceFiles\\Sounds\\Josie_Eek_Laugh.wav"

//CaptainSally stats
#define CAPTAINSALLY_ATTACKSPEED	0.45f
#define CAPTAINSALLY_WALKSPEED		15.0f
#define CAPTAINSALLY_JUMPHEIGHT		8.5f
#define CAPTAINSALLY_MASS			110
#define CAPTAINSALLY_ATTACKPOWER	6
#define CAPTAINSALLY_GEOMETRY_DATA	CAPTAINSALLY_MODEL
#define CAPTAINSALLY_ANIMATION_DATA	SALLY_ANIMS
#define CAPTAINSALLY_PILLOW_DATA	SALLYPILLOW_MODE
//sound data
#define CAPTAINSALLY_PICKUPSOUND		"ResourceFiles\\Sounds\\Sally_Treasure2.wav"
#define CAPTAINSALLY_VICTORYSOUND		"ResourceFiles\\Sounds\\Sally_Booty2.wav"
#define CAPTAINSALLY_TAUNTSOUND			"ResourceFiles\\Sounds\\Sally_Matey2.wav"
#define CAPTAINSALLY_HITSOUND			"ResourceFiles\\Sounds\\Sally_Aye2.wav"
#define CAPTAINSALLY_GOTHIT				"ResourceFiles\\Sounds\\Sally_YarHar2.wav"

//MISSELIZABETH stats
#define MISSELIZABETH_ATTACKSPEED		0.45f
#define MISSELIZABETH_WALKSPEED			12.0f
#define MISSELIZABETH_JUMPHEIGHT		9.0f
#define MISSELIZABETH_MASS				105
#define MISSELIZABETH_ATTACKPOWER		7
#define MISSELIZABETH_GEOMETRY_DATA		JOSIE_MODEL
#define MISSELIZABETH_ANIMATION_DATA	ELIZ_ANIMS
#define MISSELIZABETH_PILLOW_DATA		JOSIESPILLOW_MODEL
//sound data
#define MISSELIZABETH_PICKUPSOUND		"ResourceFiles\\Sounds\\Elizabeth_Quiet.wav"
#define MISSELIZABETH_VICTORYSOUND		"ResourceFiles\\Sounds\\Elizabeth_Conan.wav"
#define MISSELIZABETH_TAUNTSOUND		"ResourceFiles\\Sounds\\Elizabeth_Decimal.wav"
#define MISSELIZABETH_HITSOUND			"ResourceFiles\\Sounds\\Elizabeth_HaHa.wav"
#define MISSELIZABETH_GOTHIT			"ResourceFiles\\Sounds\\Elizabeth_Eek.wav"

//SUMI stats
#define SUMI_ATTACKSPEED		0.4f
#define SUMI_WALKSPEED			18.0f
#define SUMI_JUMPHEIGHT			10.0f
#define SUMI_MASS				90
#define SUMI_ATTACKPOWER		3
#define SUMI_GEOMETRY_DATA		JOSIE_MODEL
#define SUMI_ANIMATION_DATA		SUMI_ANIMS
#define SUMI_PILLOW_DATA		JOSIESPILLOW_MODEL

//sound data
#define SUMI_PICKUPSOUND		"ResourceFiles\\Sounds\\Sumi_VeryGood3.wav"
#define SUMI_VICTORYSOUND		"ResourceFiles\\Sounds\\Sumi_Pillows3.wav"
#define SUMI_TAUNTSOUND			"ResourceFiles\\Sounds\\Sumi_Grasshopper3.wav"
#define SUMI_HITSOUND			"ResourceFiles\\Sounds\\Sumi_VeryGood3.wav"
#define SUMI_GOTHIT				"ResourceFiles\\Sounds\\Sumi_Aaiiee3.wav"

//POWERUP STATS
//pear stats
#define PEAR_HEALTHITEM		true
#define PEAR_EFFECT			10
#define PEAR_TIMETOLIVE		15.0f
#define PEAR_GEOMETRY_DATA	PEAR_MODEL

//GRAPES stats
#define GRAPES_HEALTHITEM		true
#define GRAPES_EFFECT			30
#define GRAPES_TIMETOLIVE		13.0f
#define GRAPES_GEOMETRY_DATA	PINEAPPLE_MODEL

//PINEAPPLE stats
#define PINEAPPLE_HEALTHITEM	false
#define PINEAPPLE_EFFECT		30
#define PINEAPPLE_TIMETOLIVE	13.0f
#define PINEAPPLE_GEOMETRY_DATA	POWERUP_BALL

//WHOLETURKEY stats
#define WHOLETURKEY_HEALTHITEM		false
#define WHOLETURKEY_EFFECT			40
#define WHOLETURKEY_TIMETOLIVE		12.0f
#define WHOLETURKEY_GEOMETRY_DATA	TURKEY_MODEL

//ICECREAMCONE stats
#define ICECREAMCONE_HEALTHITEM		false
#define ICECREAMCONE_EFFECT			20
#define ICECREAMCONE_TIMETOLIVE		15.0f
#define ICECREAMCONE_GEOMETRY_DATA	CONE_MODEL

//BLUEBERRYPIE stats
#define BLUEBERRYPIE_HEALTHITEM		false
#define BLUEBERRYPIE_EFFECT			10
#define BLUEBERRYPIE_TIMETOLIVE		17.0f
#define BLUEBERRYPIE_GEOMETRY_DATA	PIE_MODEL