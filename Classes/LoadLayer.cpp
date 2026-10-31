#include "LoadLayer.h"


#include<time.h>
#include <fstream>

#ifdef _WIN32
#include <direct.h>
#endif


USING_NS_CC_EXT;
using namespace std;
using namespace CocosDenshion;

LoadLayer::LoadLayer(void)
{
	tempHeros=NULL;
	loadNum=0;
	_hudLayer=NULL;
	_bgLayer=NULL;
	_hudLayer=NULL;
	_isHardCoreMode=false;
	_isRandomChar=false;
}


LoadLayer::~LoadLayer(void)
{

	CC_SAFE_RELEASE(tempHeros);

}

bool LoadLayer::init(){

	bool bRet=false;
	do{
		CC_BREAK_IF(!CCLayer::init());


	
		//produce the menu_bar
		CCSprite* menu_bar_b = CCSprite::create("menu_bar2.png");
		menu_bar_b->setAnchorPoint(ccp(0,0));
		this->addChild(menu_bar_b,2);


		CCSprite* menu_bar_t = CCSprite::create("menu_bar3.png");
		menu_bar_t->setAnchorPoint(ccp(0,0));
		menu_bar_t->setPosition(ccp(0,winSize.height-menu_bar_t->getContentSize().height));
		this->addChild(menu_bar_t,2);

		CCSprite* loading_title=CCSprite::createWithSpriteFrameName("loading_title.png");
		loading_title->setAnchorPoint(ccp(0,0));
		loading_title->setPosition(ccp(2,winSize.height-loading_title->getContentSize().height-2));
		this->addChild(loading_title,3);


		//produce the cloud
		CCSprite* cloud_left=CCSprite::createWithSpriteFrameName("cloud.png");	
		cloud_left->setPosition(ccp(0,15));
		cloud_left->setFlippedX(true);
		cloud_left->setFlippedY(true);
		cloud_left->setAnchorPoint(ccp(0,0));
		this->addChild(cloud_left,1);

		CCActionInterval* cmv1=CCMoveBy::create(1,ccp(-15,0));
		CCAction* cseq1=CCRepeatForever::create(CCSequence::create(cmv1,cmv1->reverse(),NULL));
		cloud_left->runAction(cseq1);

		CCSprite* cloud_right=CCSprite::createWithSpriteFrameName("cloud.png");	
		cloud_right->setPosition(ccp(winSize.width-cloud_right->getContentSize().width,
			winSize.height-(cloud_right->getContentSize().height+15)));
		cloud_right->setAnchorPoint(ccp(0,0));
		this->addChild(cloud_right,1);

		CCActionInterval* cmv2=CCMoveBy::create(1,ccp(15,0));
		CCAction* cseq2=CCRepeatForever::create(CCSequence::create(cmv2,cmv2->reverse(),NULL));
		cloud_right->runAction(cseq2);


		bRet=true;
	}while(0);

	return bRet;

};


void LoadLayer::preloadIMG(){

	CCObject* pObject=NULL;
	int i=0;
	const char* path;
	KTools* tool=KTools::create();

	CCARRAY_FOREACH(tempHeros,pObject){

		CCDictionary* tempdict=(CCDictionary *) pObject;

		CCString* player=CCString::create(tempdict->valueForKey("character")->getCString());

		
		//加载技能图包
		// PVRImagesHavePremultipliedAlpha(true): not needed on Axmol (premultiplied alpha is read from the PVR header) 

		if (strcmp(player->getCString(),"Kiba")!=0 &&
			strcmp(player->getCString(),"Kakuzu")!=0 
			){

				path=CCString::createWithFormat("Element/Skills/%s_Skill.plist",player->getCString())->getCString();
				CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(path);
		}

		//预加载音效
		tool->prepareFileOGG(player->getCString());

	


		path=CCString::createWithFormat("Element/%s/%s.plist",player->getCString(),player->getCString())->getCString();
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(path);



		//加载额外角色
		if(strcmp(player->getCString(),"Jiraiya")==0){
			CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Element/SageJiraiya/SageJiraiya.plist");
			tool->prepareFileOGG("SageJiraiya");
		}else if(strcmp(player->getCString(),"Kankuro")==0){
			CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Element/Karasu/Karasu.plist");
			CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Element/Sanshouuo/Sanshouuo.plist");
			CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Element/Saso/Saso.plist");
		}else if(strcmp(player->getCString(),"Chiyo")==0){
			CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Element/Parents/Parents.plist");	
		}else if(strcmp(player->getCString(),"Kakuzu")==0){
			CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Element/MaskRaidon/MaskRaidon.plist");
			CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Element/MaskFudon/MaskFudon.plist");
			CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Element/MaskKadon/MaskKadon.plist");
		}else if(strcmp(player->getCString(),"Naruto")==0){
			CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Element/RikudoNaruto/RikudoNaruto.plist");
			CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Element/SageNaruto/SageNaruto.plist");
			CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Element/Kurama/Kurama.plist");
			tool->prepareFileOGG("SageNaruto");
			tool->prepareFileOGG("RikudoNaruto");
		}else if(strcmp(player->getCString(),"Lee")==0){
			CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Element/RockLee/RockLee.plist");
		}else if(strcmp(player->getCString(),"Tsunade")==0){
			CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Element/Slug/Slug.plist");
		}else if(strcmp(player->getCString(),"Kakashi")==0){
			CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Element/DogWall/DogWall.plist");
		}else if(strcmp(player->getCString(),"Deidara")==0){
			CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Element/Centipede/Centipede.plist");
		}else if(strcmp(player->getCString(),"Pain")==0){
			CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Element/DevaPath/DevaPath.plist");
			CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Element/AsuraPath/AsuraPath.plist");
			CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Element/AnimalPath/AnimalPath.plist");
		}else if(strcmp(player->getCString(),"Sasuke")==0){
			CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Element/ImmortalSasuke/ImmortalSasuke.plist");
			tool->prepareFileOGG("ImmortalSasuke");
		}else if(strcmp(player->getCString(),"Kiba")==0){
			CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Element/Akamaru/Akamaru.plist");
		}

		
		if(i==0 ||( (i==1 || i==2 ) && _isHardCoreMode) ){
			
			CCSprite* loadingAvator=CCSprite::createWithSpriteFrameName(CCString::createWithFormat("%s_Walk_01.png",player->getCString())->getCString());
			loadingAvator->setFlippedX(true);
			loadingAvator->setPosition(ccp(winSize.width-100+i*16,30));
			loadingAvator->setAnchorPoint(ccp(0,0));

			CCArray* animeFrames = CCArray::create();
			CCString* str;
			
			const char* file=CCString::createWithFormat("%s_Walk_",player->getCString())->getCString();
			int frameCount;
			if(strcmp(player->getCString(),"Konan")==0){
				frameCount=1;
			}else {
				frameCount=7;
			}

			for(int i = 1; i < frameCount; i++){	
				str=CCString::createWithFormat("%s%02d.png",file,i);
				CCSpriteFrame *frame =  CCSpriteFrameCache::sharedSpriteFrameCache()->getSpriteFrameByName(str->getCString());
				animeFrames->addObject(frame);
			}

			CCAnimation* tempAnimation=CCAnimation::createWithSpriteFrames(animeFrames->toVector<CCSpriteFrame*>(), float(1.0 / 10.0));
			CCAction* tempAction=CCAnimate::create(tempAnimation);
			CCArray* seqArray=CCArray::createWithObject(tempAction);
			CCAction* seq;
            seq=CCRepeatForever::create(CCSequence::create(seqArray));

			this->addChild(loadingAvator);
			loadingAvator->runAction(seq);


			
		}

		i++;

	};

	tempHeros->retain();


	//加载守护忍
	if(_isHardCoreMode){
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Element/Roshi/Roshi.plist");
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Element/Han/Han.plist");
		tool->prepareFileOGG("Roshi");
		tool->prepareFileOGG("Han");
	}
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Element/Flog/KotetsuFlog.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Element/Flog/FemalePainFlog.plist");


	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Element/Flog/IzumoFlog.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Element/Flog/KakashiFlog.plist");

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Element/Flog/PainFlog.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Element/Flog/ObitoFlog.plist");
	
	tool->prepareFileOGG("Effect");
	tool->prepareFileOGG("Ougis");

	
	try{
		int num=rand()%3+1;	
		CCSprite* tips=CCSprite::createWithSpriteFrameName(CCString::createWithFormat("tip%d.png",num)->getCString());	
		tips->setPosition(ccp(winSize.width/2,winSize.height/2));
		this->addChild(tips);

	}catch(...){
		CCLOG("error");
		return;
	}


	CCSprite* loading=CCSprite::createWithSpriteFrameName("loading_font.png");

	loading->setPosition(ccp(winSize.width-120,45));
	CCActionInterval* fade=CCFadeOut::create(1.0f);
	CCAction* fadeseq=CCRepeatForever::create(CCSequence::create(fade,fade->reverse(),NULL));
	this->addChild(loading);
	loading->runAction(fadeseq);

	this->scheduleOnce(schedule_selector(LoadLayer::playBGM),1.0f);



	this->scheduleOnce(schedule_selector(LoadLayer::onLoadFinish),3.0f);
	


}

void LoadLayer::playBGM(float dt){
   SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Audio/Music/loading_music.mp3",false);
}


void LoadLayer::preloadAudio(){
	
	char* bg_src;
	if(_isHardCoreMode){
		bg_src="blue_bg.png";
	}else{
		bg_src="red_bg.png";
	}
	CCSprite* bgSprite = CCSprite::create(bg_src);
	
	bgSprite->setAnchorPoint(ccp(0,0));
	bgSprite->setPosition(ccp(0,0));
	this->addChild(bgSprite, -5);


	this->preloadIMG();
}

void LoadLayer::onLoadFinish(float dt){

	

	CCScene* gameScene=CCScene::create();

	_gameLayer=GameLayer::create();
	_gameLayer->Heros=tempHeros;
	_gameLayer->_isHardCoreGame=_isHardCoreMode;
	_gameLayer->_isRandomChar=_isRandomChar;
	_gameLayer->initHeros();

	_bgLayer=BGLayer::create();
	_bgLayer->setDelegate(_gameLayer);

	_hudLayer=HudLayer::create();
	_hudLayer->setDelegate(_gameLayer);
	//_hudLayer->initHeroInterface();

	_gameLayer->setHudLayer(_hudLayer);
	_gameLayer->setTotalKills(CCString::create("0"));
	_gameLayer->setTotalTM(CCString::create("0"));

	gameScene->addChild(_gameLayer,GlTag);
	gameScene->addChild(_bgLayer,BgTag);
	gameScene->addChild(_hudLayer,HudTag);

	KTools* tool= KTools::create();
	
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f,gameScene));

}

void LoadLayer::preloadPlist(CCObject* obj){

	CCTexture2D* texture=(CCTexture2D*)obj; //传入的obj即是异步生成的纹理  
	
	CCObject* pObject=tempHeros->objectAtIndex(loadNum);
	CCDictionary* tempdict=(CCDictionary *) pObject;
	CCString* player=CCString::create(tempdict->valueForKey("character")->getCString());
	const char* name=player->getCString();

    const char* path=CCString::createWithFormat("Element/%s/%s.plist",name,name)->getCString();
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(path,texture);//通过纹理和.plist文件加入CCSpriteFrameCache  

	loadNum++;

	if(loadNum<6){
	this->preloadIMG();
	}else{
		this->onLoadFinish(0.2f);
	}


}

void LoadLayer::onEnter(){
	CCLayer::onEnter();

}

void LoadLayer::onExit(){
	CCLayer::onExit();
}