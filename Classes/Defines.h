#pragma once
#include "Compat/Compat2x.h"
#include <map>
#include "MyUtils/Util.h"

// lane x where each side's base area ends (the old 11*32 / 85*32)
constexpr float BASE_X_KONOHA = 11 * 32;
constexpr float BASE_X_AKATSUKI = 85 * 32;

// chakra needed for one ougis, and the bar caps (CKR: 3 bars, CKR2: 2 bars)
constexpr int CKR_OUGIS = 15000;
constexpr int CKR_MAX = 45000;
constexpr int CKR2_OUGIS = 25000;
constexpr int CKR2_MAX = 50000;


//global parameter

#define winSize  CCDirector::sharedDirector()->getWinSize()



#define  FONT_TYPE "微软雅黑"

// layer's tag
#define BgTag	1
#define GlTag	2
#define HudTag	3


extern int Cheats;



//menu sound

#define SELECT_SOUND	"Audio/Menu/select.mp3"
#define TRAINING_SOUND	"Audio/Menu/training.ogg"
#define NETWORK_SOUND	"Audio/Menu/arcade.ogg"
#define CREDITS_SOUND	"Audio/Menu/credits.ogg"
#define EXIT_SOUND		"Audio/Menu/exit.ogg"


//tile map


// game layer tag
#define currentMapTag		-5000
#define currentShadowTag    -4000
#define currentCutTag		1000
#define currentcharacterTag	2
#define currentBulletTag	3
#define currentDamageTag	100
#define currentNumberTag	200
#define currentSkillTag		300
#define currentFontTag		400

enum abType{
	NAttack,
	Item1,
	GearItem,
	GearBtn,
	SKILL1,
	SKILL2,
	SKILL3,
	OUGIS1,
	OUGIS2
};


// declare GearButton 
enum gearType{
	gear00,
	gear01,
	gear02,
	gear03,
	gear04,
	gear05,
	gear06,
	gear07,
	gear08,
	gear09,
	gear10,
	None  // also the number of gears
};

