#pragma once
#include "Compat/Compat2x.h"

USING_NS_CC;

class ActionManager;

class Effect : public CCSprite{
public:
	Effect(void);
	~Effect(void);

	virtual bool			init(const char *szImage,CCObject* Attacker);
	static Effect*			create(const char *szImage,CCObject* Attacker);

	ActionManager* at;

private:
	
	void					removeEffect();
	void					removeFontEffect();
	CCAction*				createEffectAnimation(const char* file,int frameCount,float fps,bool isRepeat);
	CCAction*				createFontAnimation();
};

