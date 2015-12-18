//All stats used for level objects in the game

#include "..\..\..\Entities\LevelObject.h"
#include "..\..\..\ContentLoader\GeometryImp.h"

//Boat object
#define BOAT_NUMLINES					17
#define BOAT_LINES						{	{-9.958f, 11.353f, 0.0f},	{-9.665f, 11.353f, 0.0f},	\
											{-9.665f, 11.353f, 0.0f},	{-9.665f, 10.014f, 0.0f},	\
											{-9.665f, 10.014f, 0.0f},	{-2.307f, 10.014f, 0.0f},	\
											{-2.307f, 10.014f, 0.0f},	{-2.638f, 7.794f, 0.0f},	\
											{-2.638f, 7.794f, 0.0f},	{-1.586f, 5.446f, 0.0f},	\
											{-2.986f, 5.446f, 0.0f},	{5.826f, 5.346f, 0.0f},		\
											{5.226f, 5.446f, 0.0f},		{5.904f, 6.048f, 0.0f},		\
											{5.904f, 6.048f, 0.0f},		{6.806f, 7.549f, 0.0f},		\
											{6.806f, 7.549f, 0.0f},		{7.054f, 7.564f, 0.0f},		\
											{7.054f, 7.564f, 0.0f},		{7.054f, 7.084f, 0.0f},		\
											{7.054f, 7.084f, 0.0f},		{12.081f, 7.776f, 0.0f},	\
											{12.081f, 7.776f, 0.0f},	{16.416f, 8.437f, 0.0f},	\
											{15.416f, 9.102f, 0.0f},	{15.416f, 8.437f, 0.0f},	\
											{15.416f, 9.102f, 0.0f},	{25.914f, 12.487f, 0.0f},	\
											{-9.351f, 17.497f, 0.0f},	{-1.139f, 16.959f, 0.0f},	\
											{6.509f, 17.732f, 0.0f},	{12.762f, 15.478f, 0.0f},	\
											{0.722f, 22.201f, 0.0f},	{3.503f, 22.201f, 0.0f}		\
										}
#define BOAT_MAGNITUDE					0.15f
#define BOAT_WAVEPERIOD					1.0f
#define BOAT_MAXBOB						1.415f
#define BOAT_MINBOB						0.73f
#define BOAT_BOBPERIOD					0.02f
#define BOAT_PLAYERLANDSOUNDINDEX		"ResourceFiles\\Sounds\\Wood Clunk.wav"
#define BOAT_EMITPOWERUPSOUNDINDEX		""
#define BOAT_MODELINDEX					PIRATESHIP_MODEL
#define BOAT_NUM_ATTACH_OBJECTS			2
#define BOAT_ATTACHEDOBJECTS			{ Cannon_levelobj, Cannon_levelobj } 
#define BOAT_ATTACHEDOBJECTPOSITIONS	{ {-2.85f, 3.624f, 5.0f}, {5.85f, 3.624f, 5.0f} }
#define BOAT_HASEMITTER					false
#define BOAT_EMITTERPOS					{0.0f,0.0f,0.0f}
#define BOAT_PARTICLE					-1
#define BOAT_LEFT_POINT					{-9.958f, 11.353f, 0.0f}
#define BOAT_RIGHT_POINT				{25.914f, 12.487f, 0.0f}

//whale object
#define WHALE_NUMLINES					1
#define WHALE_LINES						{ {-5.5f, 0.0f, 0.0f}, {5.5f, 0.0f, 0.0f} }
#define WHALE_MAGNITUDE					1.15f
#define WHALE_WAVEPERIOD				1.75f
#define WHALE_MAXBOB					1.415f
#define WHALE_MINBOB					0.73f
#define WHALE_BOBPERIOD					0.01f
#define WHALE_PLAYERLANDSOUNDINDEX		"ResourceFiles\\Sounds\\Thud.wav"
#define WHALE_EMITPOWERUPSOUNDINDEX		"ResourceFiles\\Sounds\\Blow Hole.wav"
#define WHALE_MODELINDEX				DUCK_MODEL
//#define WHALE_ATTACHEDOBJECTS			{ -1, -1 }
#define WHALE_NUM_ATTACH_OBJECTS			0
#define WHALE_ATTACHEDOBJECTPOSITIONS	{ {0.0f,0.0f,0.0f}, {0.0f,0.0f,0.0f} }
#define WHALE_HASEMITTER				false
#define WHALE_EMITTERPOS				{0.0f,0.0f,0.0f}
#define WHALE_PARTICLE					-1
#define WHALE_LEFT_POINT				{0.0f, 0.0f, 0.0f}
#define WHALE_RIGHT_POINT				{0.0f, 0.0f, 0.0f}

//cannon object
#define CANNON_NUMLINES					0
#define CANNON_LINES					{ {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} }
#define CANNON_MAGNITUDE				0.85f
#define CANNON_WAVEPERIOD				1.15f
#define CANNON_MAXBOB					0.0f
#define CANNON_MINBOB					0.0f
#define CANNON_BOBPERIOD				0.0f
#define CANNON_PLAYERLANDSOUNDINDEX		""
#define CANNON_EMITPOWERUPSOUNDINDEX	"ResourceFiles\\Sounds\\Cannon.wav"
#define CANNON_MODELINDEX				CANNON_MODEL
#define CANNON_NUM_ATTACH_OBJECTS			0
#define CANNON_ATTACHEDOBJECTS			{ -1, -1 }
#define CANNON_ATTACHEDOBJECTPOSITIONS	{ {0.0f,0.0f,0.0f}, {0.0f,0.0f,0.0f} }
#define CANNON_HASEMITTER				true
#define CANNON_EMITTERPOS				{0.0f, 2.69f, 1.84f}
#define CANNON_PARTICLE					CannonShoot_ParticleEffect
#define CANNON_LEFT_POINT				{0.0f, 0.0f, 0.0f}
#define CANNON_RIGHT_POINT				{0.0f, 0.0f, 0.0f}

// The ferris wheel.
#define WHEEL_NUMLINES					0
#define WHEEL_LINES						{ {0.0f,0.0f,0.0f}, {0.0f,0.0f,0.0f} }
#define WHEEL_MAGNITUDE					4.5f
#define WHEEL_WAVEPERIOD				0.2f
#define WHEEL_MAXBOB					0.0f
#define WHEEL_MINBOB					0.0f
#define WHEEL_BOBPERIOD					0.0f
#define WHEEL_PLAYERLANDSOUNDINDEX		"ResourceFiles\\Sounds\\Wood Clunk.wav"
#define WHEEL_EMITPOWERUPSOUNDINDEX		""
#define WHEEL_MODELINDEX				FERRIS_WHEEL_MODEL
#define WHEEL_NUM_ATTACH_OBJECTS		8
#define WHEEL_ATTACHEDOBJECTS			{ RubberDuckWheel_levelobj, RubberDuckWheel_levelobj, RubberDuckWheel_levelobj, RubberDuckWheel_levelobj, RubberDuckWheel_levelobj, RubberDuckWheel_levelobj, RubberDuckWheel_levelobj, RubberDuckWheel_levelobj }
#define WHEEL_ATTACHEDOBJECTPOSITIONS	{ {-15.0f, 0.0f, 1.5f}, {15.0f, 0.0f, 1.5f}, {0.0f, 15.0f, 1.5f}, {0.0f, -15.0f, 1.5f},  {-10.60f, 10.6f, 1.5f}, {10.6f, 10.6f, 1.5f}, {-10.6f, -10.6f, 1.5f}, {10.6f, -10.6f, 1.5f} }
#define WHEEL_HASEMITTER				false
#define WHEEL_EMITTERPOS				{0.0f,0.0f,0.0f}
#define WHEEL_PARTICLE					-1
#define WHEEL_LEFT_POINT				{0.0f,0.0f,0.0f}
#define WHEEL_RIGHT_POINT				{0.0f,0.0f,0.0f}

//RubberDuck object
#define RUBBERDUCK_NUMLINES					10
#define RUBBERDUCK_LINES					{	{-4.61f, 8.62f, 4.0f},	{-3.14f, 8.34f, 4.0f},	\
												{-3.14f, 8.84f, 4.0f},	{-2.16f, 10.2f, 4.0f},	\
												{-2.16f, 10.2f, 4.0f},	{-.63f, 9.98f, 4.0f},	\
												{-.63f, 9.98f, 4.0f},	{.32f, 9.01f, 4.0f},	\
												{.32f, 9.01f, 4.0f},	{.39f, 7.64f, 4.0f},	\
												{.39f, 7.64f, 4.0f},	{.3f, 6.26f, 4.0f},		\
												{0.0f, 6.26f, 4.0f},	{3.17f, 6.13f, 4.0f},	\
												{2.17f, 6.13f, 4.0f},	{3.35f, 6.92f, 4.0f},	\
												{3.35f, 6.92f, 4.0f},	{3.86f, 6.89f, 4.0f},	\
												{3.86f, 6.89f, 4.0f},	{4.68f, 5.72f, 4.0f}	\
											}
//{	{-4.61f, 8.62f, 0.0f},	{-3.14f, 8.34f, 0.0f},	\
//												{-3.14f, 8.84f, 0.0f},	{-2.16f, 10.2f, 0.0f},	\
//												{-2.16f, 10.2f, 0.0f},	{-.63f, 9.98f, 0.0f},	\
//												{-.63f, 9.98f, 0.0f},	{.32f, 9.01f, 0.0f},	\
//												{.32f, 9.01f, 0.0f},	{.39f, 7.64f, 0.0f},	\
//												{.39f, 7.64f, 0.0f},	{.3f, 6.26f, 0.0f},		\
//												{0.0f, 6.26f, 0.0f},	{3.17f, 6.13f, 0.0f},	\
//												{2.17f, 6.13f, 0.0f},	{3.35f, 6.92f, 0.0f},	\
//												{3.35f, 6.92f, 0.0f},	{3.86f, 6.89f, 0.0f},	\
//												{3.86f, 6.89f, 0.0f},	{4.68f, 5.72f, 0.0f}	\
//											}

// For the ferris wheel board.
#define RUBBERDUCKWHEEL_MAGNITUDE			0.0f
//-WHEEL_MAGNITUDE/SPF_PI
#define RUBBERDUCKWHEEL_WAVEPERIOD			0.0f

// For the pirate board.
#define RUBBERDUCK_MAGNITUDE				0.15f
#define RUBBERDUCK_WAVEPERIOD				1.75f

// Generic duck attributes.
#define RUBBERDUCK_MAXBOB					1.04f
#define RUBBERDUCK_MINBOB					0.53f
#define RUBBERDUCK_BOBPERIOD				0.01f
#define RUBBERDUCK_PLAYERLANDSOUNDINDEX		"ResourceFiles\\Sounds\\Duck Squeak.wav"
#define RUBBERDUCK_EMITPOWERUPSOUNDINDEX	"ResourceFiles\\Sounds\\Quack 1.wav"
#define RUBBERDUCK_MODELINDEX				DUCK_MODEL
#define RUBBERDUCK_NUM_ATTACH_OBJECTS			0
#define RUBBERDUCK_ATTACHEDOBJECTS			{ -1, -1 }
#define RUBBERDUCK_ATTACHEDOBJECTPOSITIONS	{ {0.0f,0.0f,0.0f}, {0.0f,0.0f,0.0f} }
#define RUBBERDUCK_HASEMITTER				true
#define RUBBERDUCK_EMITTERPOS				{-4.2f,8.401f,0.0f}
#define RUBBERDUCK_PARTICLE					-1
#define RUBBERDUCK_LEFT_POINT				{-4.61f, 8.62f, 0.0f}
#define RUBBERDUCK_RIGHT_POINT				{4.68f, 5.72f, 0.0f}

//Soap object
#define SOAP_NUMLINES					1
#define SOAP_LINES						{ {-5.5f, 0.0f, 0.0f}, {5.5f, 0.0f, 0.0f} }
#define SOAP_MAGNITUDE					1.15f
#define SOAP_WAVEPERIOD					1.75f
#define SOAP_MAXBOB						1.12f
#define SOAP_MINBOB						0.20f
#define SOAP_BOBPERIOD					0.01f
#define SOAP_PLAYERLANDSOUNDINDEX		"ResourceFiles\\Sounds\\Thud.wav"
#define SOAP_EMITPOWERUPSOUNDINDEX		""
#define SOAP_MODELINDEX					DUCK_MODEL
#define SOAP_NUM_ATTACH_OBJECTS		0
#define SOAP_ATTACHEDOBJECTS			{ -1, -1 }
#define SOAP_ATTACHEDOBJECTPOSITIONS	{ {0.0f,0.0f,0.0f}, {0.0f,0.0f,0.0f} }
#define SOAP_HASEMITTER					false
#define SOAP_EMITTERPOS					{0.0f,0.0f,0.0f}
#define SOAP_PARTICLE					-1
#define SOAP_LEFT_POINT					{0.0f, 0.0f, 0.0f}
#define SOAP_RIGHT_POINT				{0.0f, 0.0f, 0.0f}

//faucet object
#define FAUCET_NUMLINES					1
#define FAUCET_LINES					{ {-5.5f, 0.0f, 0.0f}, {5.5f, 0.0f, 0.0f} }
#define FAUCET_MAGNITUDE				1.15f
#define FAUCET_WAVEPERIOD				1.75f
#define FAUCET_MAXBOB					3.0f
#define FAUCET_MINBOB					-1.0f
#define FAUCET_BOBPERIOD				0.01f
#define FAUCET_PLAYERLANDSOUNDINDEX		"ResourceFiles\\Sounds\\Thud.wav"
#define FAUCET_EMITPOWERUPSOUNDINDEX	""
#define FAUCET_MODELINDEX				DUCK_MODEL
#define FAUCET_NUM_ATTACH_OBJECTS		0
#define FAUCET_ATTACHEDOBJECTS			{ -1, -1 }
#define FAUCET_ATTACHEDOBJECTPOSITIONS	{ {0.0f,0.0f,0.0f}, {0.0f,0.0f,0.0f} }
#define FAUCET_HASEMITTER				false
#define FAUCET_EMITTERPOS				{0.0f,0.0f,0.0f}
#define FAUCET_PARTICLE					-1
#define FAUCET_LEFT_POINT				{0.0f, 0.0f, 0.0f}
#define FAUCET_RIGHT_POINT				{0.0f, 0.0f, 0.0f}

//handle object
#define HANDLE_NUMLINES					1
#define HANDLE_LINES					{ {-5.5f, 0.0f, 0.0f}, {5.5f, 0.0f, 0.0f} }
#define HANDLE_MAGNITUDE				1.15f
#define HANDLE_WAVEPERIOD				1.75f
#define HANDLE_MAXBOB					3.0f
#define HANDLE_MINBOB					-1.0f
#define HANDLE_BOBPERIOD				0.01f
#define HANDLE_PLAYERLANDSOUNDINDEX		"ResourceFiles\\Sounds\\Thud.wav"
#define HANDLE_EMITPOWERUPSOUNDINDEX	""
#define HANDLE_MODELINDEX				DUCK_MODEL
#define HANDLE_NUM_ATTACH_OBJECTS			0
#define HANDLE_ATTACHEDOBJECTS			{ -1, -1 }
#define HANDLE_ATTACHEDOBJECTPOSITIONS	{ {0.0f,0.0f,0.0f}, {0.0f,0.0f,0.0f} }
#define HANDLE_HASEMITTER				false
#define HANDLE_EMITTERPOS				{0.0f,0.0f,0.0f}
#define HANDLE_PARTICLE					-1
#define HANDLE_LEFT_POINT				{0.0f, 0.0f, 0.0f}
#define HANDLE_RIGHT_POINT				{0.0f, 0.0f, 0.0f}

//Watergun object
#define WATERGUN_NUMLINES					0
#define WATERGUN_LINES						{ {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} }
#define WATERGUN_MAGNITUDE					1.15f
#define WATERGUN_WAVEPERIOD					1.75f
#define WATERGUN_MAXBOB						3.0f
#define WATERGUN_MINBOB						-1.0f
#define WATERGUN_BOBPERIOD					0.01f
#define WATERGUN_PLAYERLANDSOUNDINDEX		""
#define WATERGUN_EMITPOWERUPSOUNDINDEX		"ResourceFiles\\Sounds\\Squirt gun.wav"
#define WATERGUN_MODELINDEX					DUCK_MODEL
#define WATERGUN_NUM_ATTACH_OBJECTS			0
#define WATERGUN_ATTACHEDOBJECTS			{ -1, -1 }
#define WATERGUN_ATTACHEDOBJECTPOSITIONS	{ {0.0f,0.0f,0.0f}, {0.0f,0.0f,0.0f} }
#define WATERGUN_HASEMITTER					true
#define WATERGUN_EMITTERPOS					{0.0f,0.0f,0.0f}
#define WATERGUN_PARTICLE					-1
#define WATERGUN_LEFT_POINT					{0.0f, 0.0f, 0.0f}
#define WATERGUN_RIGHT_POINT				{0.0f, 0.0f, 0.0f}
