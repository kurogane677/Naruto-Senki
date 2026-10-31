#pragma once
#include "Compat/Compat2x.h"
#include "Defines.h"
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID) 
#include "platform/android/jni/JniHelper.h"  
#endif  
USING_NS_CC;


class GameLayer;
class Hero;
class GameOver : public CCLayer{

public:
	GameOver(void);
	~GameOver(void);

	bool	init(CCRenderTexture* snapshoot);
	CC_SYNTHESIZE(bool,_isWin,Win);

	static GameOver*	create(CCRenderTexture* snapshoot);
	
	CCLayer* exitLayer;
	CCLayer* cheatLayer;
	CCSprite* result_bg;


	CCMenuItem* upload_btn;

	//CC_SYNTHESIZE_RETAIN(CCString*,detailRecord,DetialRecord);
	 std::string detailRecord;
	float finnalScore;
	CC_SYNTHESIZE_RETAIN(CCString*,resultChar,ResultChar);

private:
	void	onBackToMenu(CCObject* sender);
	void	listResult();
	void	onCancel(CCObject* sender);
	void	onLeft(CCObject* sender);
	CC_SYNTHESIZE(GameLayer*,_delegate,Delegate);
};

