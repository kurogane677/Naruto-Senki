#include "GameLayer.h"
#include "Characters.h"
#include "BGLayer.h"
#include "HudLayer.h"
#include "StartMenu.h"


using namespace CocosDenshion;

GameLayer::GameLayer(void)
{
	_isAttackButtonRelease=true;
	_isSkillFinish=true;

	_KonohaFlogArray=NULL;
	_AkatsukiFlogArray=NULL;
	_CharacterArray=NULL;

	_second=0;
	_minute=0;
	_playNum=2;

	kEXPBound=25;
	aEXPBound=25;

	totalKills=NULL;
	totalTM=NULL;
	_isShacking=false;
	_isSurrender=false;
	_isGuardian=false;
	_isExiting=false;
	ougisChar=NULL;
	controlChar=NULL;
	_isOugis2Game=false;
	_isHardCoreGame=false;
	_isRandomChar=false;
	zhenying=1;
	currentPlayer=NULL;
	postTime=0;

}


GameLayer::~GameLayer(void)
{
	//CC_SAFE_RELEASE(totalKills);
	CC_SAFE_RELEASE(totalTM);
	
}

bool GameLayer::init(){
	bool bRet=false;
	do{
		CC_BREAK_IF(!CCLayer::init());
		//CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_RGBA4444);  

		this->setTouchEnabled(true);

		bRet=true;

	}while(0);


	return bRet;



}
void GameLayer::onEnter(){
	if(_isExiting){
		this->onLeft();	
		return;
	}

	if(currentPlayer && !ougisChar){
		if(currentPlayer->getActionState()==ACTION_STATE_WALK){
			currentPlayer->idle();
		}
	}

	CCLayer::onEnter();

	if(_isSurrender){
		this->onGameOver(false);
	}

};
void GameLayer::onExit(){
	CCLayer::onExit();
	if(_isExiting){
		_isExiting=false;
		CC_SAFE_RELEASE(_TowerArray);
		CC_SAFE_RELEASE(_KonohaFlogArray);
		CC_SAFE_RELEASE(_AkatsukiFlogArray);
		CC_SAFE_RELEASE(_CharacterArray);
		
	}
	

};



void GameLayer::initTileMap(){
	//随机地图
	

	if(_isHardCoreGame){
		randomMap=randInt(4)+1;
	}else {
		randomMap=0;
	}
	
	const char* filePath;
	if (randomMap==0){
		filePath=S_MAP01;
	}else if(randomMap==1){
		filePath=S_MAP02;
	}else if(randomMap==2){
		filePath=S_MAP03;
	}else if(randomMap==3){
		filePath=S_MAP04;
	}else if(randomMap==4){
		filePath=S_MAP05;
	}


	currentMap=CCTMXTiledMap::create(filePath);

	this->addChild(currentMap,currentMapTag);

}

void GameLayer::initGard(){



	int index=0;
	index=randInt(2);
	Hero* Guardian=Hero::create();
	Guardian->setDelegate(this);

	const char* groupName;

	if(zhenying>0){
		groupName="Akatsuki";
	}else{
		groupName="Konoha";
	}
	if(index==0){
		Guardian->setID(CCString::create("Roshi"),CCString::create("Com"),CCString::create(groupName));
	}else if(index==1){
		Guardian->setID(CCString::create("Han"),CCString::create("Com"),CCString::create(groupName));
	}


	if(zhenying>0){
	Guardian->setPosition(ccp(2800,80));
	Guardian->setSpawnPoint(ccp(2800,80));
	}else{
		Guardian->setPosition(ccp(272,80));
		Guardian->setSpawnPoint(ccp(272,80));
	}



	this->addChild(Guardian,-Guardian->getPositionY());
	Guardian->setLV(6);
	Guardian->setHPbar();
	Guardian->setShadows();
	Guardian->setCharNO(7);

	Guardian->idle();
	CCDictionary* callValue = CCDictionary::create(); 
	callValue->setObject(CCString::create("smk"), 1);
	Guardian->setSkillEffect(Guardian,callValue);

	Guardian->doAI();


	

	_CharacterArray->addObject(Guardian);
	_hudLayer->addMapIcon();
	_isGuardian=true;

}

void GameLayer::initHeros(){
	
	this->initTileMap();
	this->initEffects();

	_CharacterArray=CCArray::create();
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Element/hpBar/hpBar.plist");	
	CCObject* pObject=NULL;
	int i=0;


	CCTMXObjectGroup* group = currentMap->getObjectGroup("object");
	CCArray* objectArray = ccArrayFromValueVector(group->getObjects());

	_isOugis2Game=true;
	//4v4

	if(Cheats>10){

		CCARRAY_FOREACH(Heros,pObject){

			CCDictionary* tempdict=(CCDictionary *) pObject;

			CCString* player=CCString::create(tempdict->valueForKey("character")->getCString());
			CCString* role=CCString::create(tempdict->valueForKey("role")->getCString());
			CCString* group=CCString::create(tempdict->valueForKey("group")->getCString());

			if(i==0){
				spawnPoint=ccp(432,80);

			}else if(i==7){
				spawnPoint=ccp(2608,80);
			}else{
				CCObject* mapObject = objectArray->objectAtIndex(i-1);
				CCDictionary* mapdict=(CCDictionary *) mapObject;
				int x=((CCString*)mapdict->objectForKey("x"))->intValue();
				int y=((CCString*)mapdict->objectForKey("y"))->intValue();
				spawnPoint=ccp(x,y);
			}




			if(i==0){			

			
			
				currentPlayer=Hero::create();
				currentPlayer->setDelegate(this);
				currentPlayer->setID(player,role,group);
				currentPlayer->setPosition(spawnPoint);
				currentPlayer->setSpawnPoint(spawnPoint);
				this->addChild(currentPlayer,-currentPlayer->getPositionY());
				currentPlayer->setHPbar();
				currentPlayer->setShadows();
				currentPlayer->idle();
				currentPlayer->setCharNO(i+1);
				currentPlayer->schedule(schedule_selector(ActionManager::setRestore2),1.0f);
				_CharacterArray->addObject(currentPlayer);

			}else{

				Hero* Com=Hero::create();
				Com->setDelegate(this);
				Com->setID(player,role,group);
				Com->setPosition(spawnPoint);
				Com->setSpawnPoint(spawnPoint);
				this->addChild(Com,-Com->getPositionY());
				Com->setHPbar();
				Com->setShadows();
				Com->idle();
				Com->setCharNO(i+1);
				Com->schedule(schedule_selector(ActionManager::setRestore2),1.0f);
				_CharacterArray->addObject(Com);
			}
			i++;
		};

	}else{
		CCARRAY_FOREACH(Heros,pObject){

			CCDictionary* tempdict=(CCDictionary *) pObject;

			CCString* player=CCString::create(tempdict->valueForKey("character")->getCString());
			CCString* role=CCString::create(tempdict->valueForKey("role")->getCString());
			CCString* group=CCString::create(tempdict->valueForKey("group")->getCString());

			int mapPos=i;
			if(strcmp(group->getCString(),"Akatsuki")==0){
				if(mapPos<=2){
					mapPos+=3;
				}
				
			}else{
				if(mapPos>2){
					mapPos-=3;
				}
			}


			CCObject* mapObject = objectArray->objectAtIndex(mapPos);
			CCDictionary* mapdict=(CCDictionary *) mapObject;
			int x=((CCString*)mapdict->objectForKey("x"))->intValue();
			int y=((CCString*)mapdict->objectForKey("y"))->intValue();
			spawnPoint=ccp(x,y);

			

			if(i==0){			

				if(strcmp(group->getCString(),"Akatsuki")==0){
					zhenying=0;
				}
				
				currentPlayer=Hero::create();
				currentPlayer->setDelegate(this);
				currentPlayer->setID(player,role,group);
				currentPlayer->setPosition(spawnPoint);
				currentPlayer->setSpawnPoint(spawnPoint);
				this->addChild(currentPlayer,-currentPlayer->getPositionY());
				currentPlayer->setHPbar();
				currentPlayer->setShadows();
				currentPlayer->idle();
				if(zhenying<=0){
					currentPlayer->_isFlipped=true;
					currentPlayer->setFlippedX(true);
				}
				currentPlayer->setCharNO(i+1);
				currentPlayer->schedule(schedule_selector(ActionManager::setRestore2),1.0f);
				_CharacterArray->addObject(currentPlayer);

			}else{

				Hero* Com=Hero::create();
				Com->setDelegate(this);
				Com->setID(player,role,group);
				Com->setPosition(spawnPoint);
				Com->setSpawnPoint(spawnPoint);
				this->addChild(Com,-Com->getPositionY());
				Com->setHPbar();
				Com->setShadows();
				Com->idle();
				if(Com->group()==Group::Akatsuki){
					Com->_isFlipped=true;
					Com->setFlippedX(true);
				}

				Com->setCharNO(i+1);
				Com->schedule(schedule_selector(ActionManager::setRestore2),1.0f);
				_CharacterArray->addObject(Com);
			}
			i++;
		};


	}


	_CharacterArray->retain();
	
	this->initTower();



	this->schedule(schedule_selector(GameLayer::updateViewPoint),0.0f);


	

	/*if(currentPlayer->isCharacter("Asuma")){
		this->schedule(schedule_selector(GameLayer::checkPost),10.0f);
	}*/
	

	
	
	this->scheduleOnce(schedule_selector(GameLayer::onKaichang),0.5f);

	
};

void GameLayer::onKaichang(float dt){
	this->getHudLayer()->onKaichang();

	if(_isHardCoreGame){
		SimpleAudioEngine::sharedEngine()->playEffect("Audio/Menu/battle_start1.ogg");
	}else{
		SimpleAudioEngine::sharedEngine()->playEffect("Audio/Menu/battle_start.ogg");
	}

	
	this->scheduleOnce(schedule_selector(GameLayer::onGameStart),0.75f);

}

void GameLayer::onGameStart(float dt){
	this->getHudLayer()->Kaichang->removeFromParent();
	this->getHudLayer()->Kaichang=NULL;
	this->getHudLayer()->initHeroInterface();
	this->schedule(schedule_selector(GameLayer::updateGameTime),1.0f);
	this->schedule(schedule_selector(GameLayer::checkBackgroundMusic),2.0f);
	this->schedule(schedule_selector(GameLayer::addFlog),15.0f);
	this->initFlogs();
	
	CCObject* pObject;
	CCARRAY_FOREACH(this->_CharacterArray,pObject){
		ActionManager* tempChar=(ActionManager*) pObject;
		// guardians (also "Com") already started their AI in initGard
		if(tempChar->role()==Role::Com && !tempChar->_isAI){
			tempChar->doAI();
		}
		
	}

}

void GameLayer::initFlogs(){

	CCString* KonohaFlogName=CCString::create("KotetsuFlog");
	CCString* AkatsukiFlogName=CCString::create("FemalePainFlog");

	kName="KotetsuFlog";
	aName="FemalePainFlog";

	_KonohaFlogArray=CCArray::create();
	_AkatsukiFlogArray=CCArray::create();
	
	
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Element/hpBar/flogBar.plist");

	int i;
	Flog* aiFlog;
	float mainPosY;
	for(i= 0;i<6;i++){
		aiFlog=Flog::create();
		aiFlog->setDelegate(this);
		aiFlog->setID(KonohaFlogName,CCString::create("Flog"),CCString::create("Konoha"));
		if(i<3){
			mainPosY=(5.5-i/1.5)*32;

		}else{
			mainPosY=(3.5-i/1.5)*32;
		}
		aiFlog->_mainPosY= mainPosY;
		aiFlog->setPosition(ccp(13*32,aiFlog->_mainPosY));
		aiFlog->setHPbar();
		this->addChild(aiFlog,-int(aiFlog->getPositionY()));
		aiFlog->idle();
		aiFlog->doAI();
		_KonohaFlogArray->addObject(aiFlog);

	}

	_KonohaFlogArray->retain();

	for(i= 0;i<6;i++){
		aiFlog=Flog::create();
		aiFlog->setDelegate(this);
		aiFlog->setID(AkatsukiFlogName,CCString::create("Flog"),CCString::create("Akatsuki"));
		if(i<3){
			mainPosY=(5.5-i/1.5)*32 ;

		}else{
			mainPosY=(3.5-i/1.5)*32;
		}
		aiFlog->_mainPosY= mainPosY;
		aiFlog->setPosition(ccp(83*32,aiFlog->_mainPosY));
		aiFlog->setHPbar();
		aiFlog->idle();
		aiFlog->doAI();
		_AkatsukiFlogArray->addObject(aiFlog);
		this->addChild(aiFlog,-aiFlog->getPositionY());

	}

	_AkatsukiFlogArray->retain();

};
void GameLayer::addFlog(float dt){

	CCString* KonohaFlogName=CCString::create(kName);
	CCString* AkatsukiFlogName=CCString::create(aName);


	int i;
	Flog* aiFlog;
	float mainPosY;
	for(i= 0;i<6;i++){
		aiFlog=Flog::create();
		aiFlog->setDelegate(this);
		aiFlog->setID(KonohaFlogName,CCString::create("Flog"),CCString::create("Konoha"));
		if(i<3){
			mainPosY=(5.5-i/1.5)*32;

		}else{
			mainPosY=(3.5-i/1.5)*32;
		}
		aiFlog->_mainPosY= mainPosY;
		aiFlog->setPosition(ccp(13*32,aiFlog->_mainPosY));
		aiFlog->setHPbar();	
		aiFlog->idle();
		aiFlog->doAI();
		_KonohaFlogArray->addObject(aiFlog);
		this->addChild(aiFlog,-int(aiFlog->getPositionY()));
	}

	for(i= 0;i<6;i++){
		aiFlog=Flog::create();
		aiFlog->setDelegate(this);
		aiFlog->setID(AkatsukiFlogName,CCString::create("Flog"),CCString::create("Akatsuki"));
		if(i<3){
			mainPosY=(5.5-i/1.5)*32 ;

		}else{
			mainPosY=(3.5-i/1.5)*32;
		}
		aiFlog->_mainPosY= mainPosY;
		aiFlog->setPosition(ccp(83*32,aiFlog->_mainPosY));
		aiFlog->setHPbar();
		aiFlog->idle();
		aiFlog->doAI();
		_AkatsukiFlogArray->addObject(aiFlog);
		this->addChild(aiFlog,-aiFlog->getPositionY());

	}

}



void GameLayer::initTower(){


	if (randomMap==0) {
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Element/Tower/Tower.plist");
	}else if(randomMap==1){
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Element/Tower/Tower2.plist");
	}else if(randomMap==2){
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Element/Tower/Tower3.plist");
	}else if(randomMap==3){
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Element/Tower/Tower4.plist");
	}else if(randomMap==4){
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Element/Tower/Tower5.plist");
	}



	CCTMXObjectGroup* metaGroup = currentMap->getObjectGroup("meta");
	CCArray* metaArray= ccArrayFromValueVector(metaGroup->getObjects());

	_TowerArray=CCArray::create();

	CCObject *pObject;
	int i=0;

	CCARRAY_FOREACH(metaArray,pObject){

		CCDictionary* dict=(CCDictionary *)pObject;

		int metaX=((CCString*)dict->objectForKey("x"))->intValue();
		int metaY=((CCString*)dict->objectForKey("y"))->intValue();

		int metaWidth=((CCString*)dict->objectForKey("width"))->intValue();
		int metaHeight=((CCString*)dict->objectForKey("height"))->intValue();

		CCString* name=(CCString*)dict->objectForKey("name");


		Tower* tower=Tower::create();
		tower->setDelegate(this);
		char towerName[7]="abcdef";
		strncpy(towerName,name->getCString(),6);
		if(strcmp(towerName,"Konoha")==0){
			tower->setID(name,CCString::create("Tower"),CCString::create("Konoha"));
		}else{
			tower->setID(name,CCString::create("Tower"),CCString::create("Akatsuki"));
			tower->setFlippedX(true);
			tower->_isFlipped=true;
		}
		float posX=metaX+metaWidth/2;
		float posY=metaY+metaHeight/2;
		tower->setPosition(ccp(posX,posY));
		tower->setSpawnPoint(ccp(posX,posY));
		tower->setCharNO(i+1);

		if(i==1 || i==4 ){
			//4v4
			if(Cheats>10){
				tower->setMaxHP(80000);
			}else{
				tower->setMaxHP(50000);
			}	
			tower->setHP(tower->getMaxHP());
		}
		tower->setHPbar();
		tower->_hpBar->setVisible(false);
		tower->idle();
		this->addChild(tower,-tower->getPositionY());

		_TowerArray->addObject(tower);
		_TowerArray->retain();

		i++;
	}

};

void GameLayer::initEffects(){
	

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Effects/SkillEffect.plist");
	skillEffectBatch=CCSpriteBatchNode::create("Effects/SkillEffect.png");
	this->addChild(skillEffectBatch,currentSkillTag);

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Effects/DamageEffect.plist");
	damageEffectBatch=CCSpriteBatchNode::create("Effects/DamageEffect.png");
	this->addChild(damageEffectBatch,currentDamageTag);

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Effects/Shadows.plist");
	shadowBatch=CCSpriteBatchNode::create("Effects/Shadows.png");
	this->addChild(shadowBatch,currentShadowTag);

};

void GameLayer::updateGameTime(float dt)
{
	_second+=1;
	if(_second==60){
		_minute+=1;
		_second=0;
	}
	CCString* tempTime=CCString::createWithFormat("%02d:%02d",_minute,_second);
	this->_hudLayer->gameClock->setString(tempTime->getCString());

	int newValue=atoi(this->getTotalTM()->getCString())+1;
	this->setTotalTM(CCString::createWithFormat("%d",newValue));
}


void GameLayer::updateViewPoint(float dt){



	if(!currentPlayer) return;
	CCPoint playerPoint;
	if(ougisChar){
		playerPoint=ougisChar->getPosition();
	}else if(controlChar){
		playerPoint=controlChar->getPosition();
	}else{
		playerPoint=currentPlayer->getPosition();
	}

	int x=MAX(playerPoint.x,winSize.width/2);
	int y=MAX(playerPoint.y,winSize.height/2);
	x=MIN(x,(currentMap->getMapSize().width*currentMap->getTileSize().width)-winSize.width/2);
	y=MIN(y,(currentMap->getMapSize().height*currentMap->getTileSize().height)-winSize.height/2);

	CCPoint actualPoint=ccp(x,y);
	CCPoint centerPoint=ccp(winSize.width/2,y);
	CCPoint viewPoint=ccpSub(centerPoint,actualPoint);

	this->setPosition(viewPoint);
	//CCDirector::sharedDirector()->getScheduler()->setTimeScale(1.0f);  

};

void GameLayer::setHPLose(float percent){
	this->_hudLayer->setHPLose(percent);
}
void GameLayer::setCKRLose(bool isCRK2){
	this->_hudLayer->setCKRLose(isCRK2);
}



void GameLayer::setReport(const char* name1, const char* name2,int killNum){
	this->_hudLayer->setReport(name1,name2,killNum);
}

void GameLayer::resetStatusBar(){
	this->_hudLayer->status_hpbar->setRotation(0);

}

void GameLayer::setCoin(const char* value){
	this->_hudLayer->setCoin(value);

}
void GameLayer::removeOugisMark(int type){
	if(type==1){
		if(this->_hudLayer->skill4Button){
			if(this->_hudLayer->skill4Button->lockLabel1){
				this->_hudLayer->skill4Button->lockLabel1->removeFromParent();
				this->_hudLayer->skill4Button->lockLabel1=NULL;
			}
		}

	}else{

		if(this->_hudLayer->skill5Button){
			if(this->_hudLayer->skill5Button->lockLabel1){
				this->_hudLayer->skill5Button->lockLabel1->removeFromParent();
				this->_hudLayer->skill5Button->lockLabel1=NULL;
			}
		}
	}
}

void GameLayer::checkTower(){

	int konohaTowerCount=0;
	int akatsukiTowerCount=0;
	CCObject* pObject;

	CCARRAY_FOREACH(_TowerArray,pObject){
		Tower* tower=(Tower*) pObject;
		if(tower->group()==Group::Konoha){
			konohaTowerCount++;
		}else{
			akatsukiTowerCount++;
		}
	}

	if(konohaTowerCount==2){
		aName="PainFlog";	
		kEXPBound=50;			
	}else if(konohaTowerCount==1){
		aName="ObitoFlog";
		kEXPBound=100;
	}

	if(akatsukiTowerCount==2){
		kName="IzumoFlog";
		aEXPBound=50;	
	}else if(akatsukiTowerCount==1){
		kName="KakashiFlog";
		aEXPBound=100;	
	}

	


	//通知战斗局势
	CCARRAY_FOREACH(_CharacterArray,pObject){
		ActionManager* tmpHero=(ActionManager*) pObject;

		if(tmpHero->role()!=Role::Com){
			continue;
		}

		if(tmpHero->group()==Group::Konoha){
			tmpHero->battleCondiction=konohaTowerCount-akatsukiTowerCount;
			tmpHero->isBaseDanger=konohaTowerCount==1;
	

		}else{
			
			tmpHero->battleCondiction=akatsukiTowerCount-konohaTowerCount;
			tmpHero->isBaseDanger=akatsukiTowerCount==1;
		}


	}



	if(konohaTowerCount==0 || akatsukiTowerCount==0){
		if(zhenying>0){
			if(konohaTowerCount==0){
				this->onGameOver(false);
			}else{
				this->onGameOver(true);
			}
		}else{
			if(akatsukiTowerCount==0){
				this->onGameOver(false);
			}else{
				this->onGameOver(true);
			}
		}
		

	}
}


void GameLayer::clearDoubleClick(){
	if(_hudLayer->skill1Button->getDoubleSkill() &&
		_hudLayer->skill1Button->_clickNum>=1 
		){
			_hudLayer->skill1Button->setFreezeAction(NULL);
			_hudLayer->skill1Button->beganAnimation();
	}

}

void GameLayer::JoyStickRelease(){

	if (currentPlayer->getActionState()==ACTION_STATE_WALK){
		currentPlayer->idle();
	}
}

void GameLayer::JoyStickUpdate(CCPoint direction){
	if(!ougisChar){
		//CCLOG("x:%f,y:%f",direction.x,direction.y);
		currentPlayer->walk(direction);
	}
}

void GameLayer::attackButtonClick(abType type){
	if(type==NAttack){
		_isAttackButtonRelease=false;
	}

	if(type==Item1){
		currentPlayer->setItem(type);
	}else{
		currentPlayer->attack(type);
	};
}
void GameLayer::gearButtonClick(gearType type){

	currentPlayer->useGear(type);

}


void GameLayer::attackButtonRelease(){
	_isAttackButtonRelease=true;
}

void GameLayer::onPause(){

	CCRenderTexture* snapshoot=CCRenderTexture::create(winSize.width,winSize.height);
	CCScene* f = CCDirector::sharedDirector()->getRunningScene();
	CCObject* pObject= f->getChildren().at(0);
	BGLayer* bg=(BGLayer*) pObject;
	snapshoot->begin();
	ccVisitForSnapshot(bg);

	ccVisitForSnapshot(this);
	ccEndSnapshot(snapshoot);


	CCScene* pscene =CCScene::create();
	PauseLayer* layer=PauseLayer::create(snapshoot);

	layer->setDelegate(this);
	pscene->addChild(layer);
	CCDirector::sharedDirector()->pushScene(pscene);

}

void GameLayer::onGear(){

	if(this->_isHardCoreGame){

		CCRenderTexture* snapshoot=CCRenderTexture::create(winSize.width,winSize.height);
		CCScene* f = CCDirector::sharedDirector()->getRunningScene();
		CCObject* pObject= f->getChildren().at(0);
		BGLayer* bg=(BGLayer*) pObject;
		snapshoot->begin();
		ccVisitForSnapshot(bg);

		ccVisitForSnapshot(this);
		ccEndSnapshot(snapshoot);

		CCScene* pscene =CCScene::create();
		GearLayer* layer=GearLayer::create(snapshoot);

		layer->setDelegate(this);
		layer->updatePlayerGear();
		pscene->addChild(layer);
		CCDirector::sharedDirector()->pushScene(pscene);

	}


}

void GameLayer::onGameOver(bool isWin){


	CCRenderTexture* snapshoot=CCRenderTexture::create(winSize.width,winSize.height);
	CCScene* f = CCDirector::sharedDirector()->getRunningScene();
	CCObject* pObject= f->getChildren().at(0);
	BGLayer* bg=(BGLayer*) pObject;
	snapshoot->begin();
	ccVisitForSnapshot(bg);
	ccVisitForSnapshot(this);
	ccEndSnapshot(snapshoot);

	CCScene* pscene =CCScene::create();
	GameOver* layer=GameOver::create(snapshoot);
	layer->setWin(isWin);
	layer->setDelegate(this);	
	pscene->addChild(layer);
	CCDirector::sharedDirector()->pushScene(pscene);


};

void GameLayer::onLeft(){

	CCArray* childArray=CCArray::createWithVector(this->getChildren());
	CCObject* pObject;

	CCARRAY_FOREACH(childArray,pObject){		
		//清除侦听
		ActionManager* ac=(ActionManager*) pObject;
		CCNotificationCenter::sharedNotificationCenter()->removeObserver(ac,"acceptAttack");
	}

	//清除地图缓存

	if(randomMap==0) {
		CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("Element/Tower/Tower.plist");	
	}else if(randomMap==1){
		CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("Element/Tower/Tower2.plist");
	}else if (randomMap==2){
		CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("Element/Tower/Tower3.plist");
	}else if (randomMap==3){
		CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("Element/Tower/Tower4.plist");
	}else if (randomMap==4){
		CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("Element/Tower/Tower5.plist");
	}

	KTools* tool=KTools::create();
	

	const char* path;
	CCARRAY_FOREACH(_CharacterArray,pObject){
		ActionManager* player=(ActionManager*) pObject;

		if(player->role()==Role::Clone ||
			player->role()==Role::Summon){
				continue;
		}

		//删除人物缓存
		path=CCString::createWithFormat("Element/%s/%s.plist",player->getCharacter()->getCString(),player->getCharacter()->getCString())->getCString();
		CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(path);

		//删除预加载音效
		if(player->role()==Role::Com ||
			player->role()==Role::Player
			){
			tool->prepareFileOGG(player->getCharacter()->getCString(),1);
		}
		

		//删除额外角色
		if(player->isCharacter("Jiraiya")){
			CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("Element/SageJiraiya/SageJiraiya.plist");
			tool->prepareFileOGG("SageJiraiya",1);
		}else if(player->isCharacter("Kankuro")){
			CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("Element/Karasu/Karasu.plist");
			CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("Element/Sanshouuo/Sanshouuo.plist");
		}else if(player->isCharacter("Kakuzu")){
			CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("Element/MaskFudon/MaskFudon.plist");
			CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("Element/MaskRaidon/MaskRaidon.plist");
			CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("Element/MaskKadon/MaskKadon.plist");
		}else if(player->isCharacter("Naruto")){
			CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("Element/SageNaruto/SageNaruto.plist");
			CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("Element/RikudoNaruto/RikudoNaruto.plist");
			tool->prepareFileOGG("SageNaruto",1);
			tool->prepareFileOGG("RikudoNaruto",1);
		}else if(player->isCharacter("RockLee")){
			CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("Element/Lee/Lee.plist");
		}else if(player->isCharacter("Lee")){
			CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("Element/RockLee/RockLee.plist");
		}else if(player->isCharacter("Sasuke")){
			tool->prepareFileOGG("ImmortalSasuke",1);
			CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("Element/ImmortalSasuke/ImmortalSasuke.plist");
		}
		player->removeFromParentAndCleanup(true);

	}

	//加载守护忍
	if(this->_isHardCoreGame){
		CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("Element/Roshi/Roshi.plist");
		CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("Element/Han/Han.plist");
		tool->prepareFileOGG("Roshi",1);
		tool->prepareFileOGG("Han",1);
	}

	tool->prepareFileOGG("Effect",1);
	tool->prepareFileOGG("Ougis",1);


	_CharacterArray->removeAllObjects();
	_CharacterArray=NULL;


	_TowerArray->removeAllObjects();
	_TowerArray=NULL;
	_KonohaFlogArray->removeAllObjects();
	_KonohaFlogArray=NULL;
	_AkatsukiFlogArray->removeAllObjects();
	_AkatsukiFlogArray=NULL;

	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("UI.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("Map.plist");

	CCScene* menuScene =CCScene::create();
	StartMenu* menuLayer=StartMenu::create();

	menuScene->addChild(menuLayer);
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(2.0f,menuScene));



}

void GameLayer::checkBackgroundMusic(float dt){
	if(CCUserDefault::sharedUserDefault()->getBoolForKey("isBGM")!=false){

		if(!SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying()){
			if(!_isHardCoreGame){
				SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Audio/Music/Battle1.mp3");
			}else{
				if(_playNum==0){
					SimpleAudioEngine::sharedEngine()->playBackgroundMusic(CCString::createWithFormat("Audio/Music/Battle%d.mp3",int(2+randomMap*3))->getCString(),false);
					_playNum++;
				}else if(_playNum==1){
					SimpleAudioEngine::sharedEngine()->playBackgroundMusic(CCString::createWithFormat("Audio/Music/Battle%d.mp3",int(3+randomMap*3))->getCString(),false);
					_playNum++;
				}else if(_playNum==2){
					SimpleAudioEngine::sharedEngine()->playBackgroundMusic(CCString::createWithFormat("Audio/Music/Battle%d.mp3",int(1+randomMap*3))->getCString(),false);
					_playNum=0;
				}
			}

			
		}
	}
}


void GameLayer::setOugis(CCNode* sender){

	if(!this->_hudLayer->ougisLayer){
		CCArray* childArray=CCArray::createWithVector(this->getChildren());
		ougisChar=sender;
		ActionManager* Sender=(ActionManager*) sender;
		CCObject* pObject;
		CCARRAY_FOREACH(childArray,pObject){

			CCNode* object=(CCNode*) pObject;

			object->pause();

		}
		this->pause();

		this->updateViewPoint(0.0);

		blend=CCLayerColor::create(ccc4(0, 0, 0, 200),winSize.width,winSize.height);
		blend->setPosition(ccp(-this->getPositionX(),0));
		this->addChild(blend,1000);
		sender->setLocalZOrder(2000);

		if(CCUserDefault::sharedUserDefault()->getBoolForKey("isVoice")!=false){
			SimpleAudioEngine::sharedEngine()->playEffect(CCString::createWithFormat("Audio/Ougis/%s_ougis.mp3",Sender->getCharacter()->getCString())->getCString());
		}

		_hudLayer->setOugis(Sender->getCharacter(),Sender->getGroup());
	}



}

void GameLayer::removeOugis(){
	ougisChar->setLocalZOrder(-ougisChar->getPositionY());
	CCArray* childArray=CCArray::createWithVector(this->getChildren());
	CCObject* pObject;
	CCARRAY_FOREACH(childArray,pObject){
		CCNode* object=(CCNode*) pObject;
		object->resume();
	}
	this->resume();

	blend->removeFromParent();
	ougisChar=NULL;
}
