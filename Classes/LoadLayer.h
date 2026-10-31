#pragma once
#include "Compat/Compat2x.h"
#include "Defines.h"
#include "GameLayer.h"
#include "BGLayer.h"
#include "HudLayer.h"


#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID) 
#include "platform/android/jni/JniHelper.h"  
#endif  



#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#endif

USING_NS_CC;
using namespace rapidjson; 

class LoadLayer : public CCLayer{

public:
	LoadLayer(void);
	~LoadLayer(void);


	CCArray* tempHeros;
	bool init();

	//void dfsFolder(std::string folderPath, int depth);
	int loadNum;

	CREATE_FUNC(LoadLayer);

	CC_SYNTHESIZE_RETAIN(GameLayer*,_gameLayer,GameLayer);
	CC_SYNTHESIZE_RETAIN(BGLayer*,_bgLayer,BGLayer);
	CC_SYNTHESIZE_RETAIN(HudLayer*,_hudLayer,HudLayer);
	bool _isHardCoreMode;
	bool _isRandomChar;

	void preloadAudio();
	void preloadIMG();
	void playBGM(float dt);
	void onLoadFinish(float dt);


private:
	
	void preloadPlist(CCObject* obj);

protected:
	void onEnter();
	void onExit();


};

