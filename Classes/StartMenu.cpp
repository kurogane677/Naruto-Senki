#include "StartMenu.h"
using namespace CocosDenshion;
USING_NS_CC_EXT;


int Cheats=0;
std::string CValue="999999";
bool isFirst=false;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)   
#include <jni.h>   
#include "platform/android/jni/JniHelper.h"   

extern "C"  
{  
	void Java_net_zakume_game_NarutoSenki_onBannerInit( JNIEnv *env,jobject thiz,jint points)
	{
		adResult=points;
		
	}
}  
#endif  


/*----------------------
init MenuButton ;
----------------------*/
MenuButton::MenuButton(void)
{
	_isTop=false;
	_isBottom=false;
}


MenuButton::~MenuButton(void)
{
	
}


bool MenuButton::init(const char *szImage){
	bool bRet = false;
	do{
		CC_BREAK_IF(!CCSprite::init());
		this->initWithSpriteFrameName(szImage);
		this->setAnchorPoint(ccp(0.5,0));

		bRet=true;

	}while(0);

	return bRet;

}

void MenuButton::onEnter(){
	CCSprite::onEnter();
	CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this,10,true);
	
#if CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID

	JniMethodInfo minfo;

	bool isHave = JniHelper::getStaticMethodInfo(minfo,"net/zakume/game/NarutoSenki","showBanner", "()V"); 
	if (isHave) { 	
		minfo.env -> CallStaticVoidMethod(minfo.classID,minfo.methodID);
	} 

#endif  


}
void MenuButton::onExit(){
	CCSprite::onExit();
	CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
}

CCRect MenuButton::getRect(){
	CCSize size=this->getContentSize();
	return CCRect(0,0, size.width, size.height);

}
bool MenuButton::containsTouchLocation(CCTouch *touch){

	return getRect().containsPoint(convertTouchToNodeSpace(touch));
}


void MenuButton::setBtnType(btnType type){
	_btnType=type;
}

btnType	MenuButton::getBtnType(){
	return _btnType;
}

bool MenuButton::ccTouchBegan(CCTouch* touch,CCEvent* event){
	// touch area 
	if (!this->containsTouchLocation(touch))
		return false;

	//click();
 
	prePosY=0;

	return true;

}

void MenuButton::ccTouchMoved(CCTouch* touch,CCEvent* event){
	// touch area 
	if(prePosY==0){
		prePosY=touch->getLocation().y;

	}else{
		if(this->getBtnType()!=HardCore){

			if(abs(touch->getLocation().y-prePosY)>16){
				if(touch->getLocation().y<prePosY){
					_delegate->isClockwise=true;
				}else{
					_delegate->isClockwise=false;
				}
				_delegate->isDrag=true;
			}


		}
		
	}
	


}

void MenuButton::ccTouchEnded(CCTouch* touch,CCEvent* event){
	if(_isTop && !_delegate->isDrag){
	
		switch(_btnType){
		case Training:
				SimpleAudioEngine::sharedEngine()->playEffect("Audio/Menu/confirm.ogg");
			_delegate->onTrainingCallBack();
			break;
		case Credits:
				SimpleAudioEngine::sharedEngine()->playEffect("Audio/Menu/confirm.ogg");
			_delegate->onCreditsCallBack();
			break;
		case Exit:
				SimpleAudioEngine::sharedEngine()->playEffect("Audio/Menu/confirm.ogg");
			_delegate->onExitCallBack();
			break;
		case Network:
			SimpleAudioEngine::sharedEngine()->playEffect("Audio/Menu/confirm.ogg");
			_delegate->onHardLayerCallBack();
			break;
		}
	}else{
		SimpleAudioEngine::sharedEngine()->playEffect(SELECT_SOUND);
		prePosY=0;
		_delegate->scrollMenu(this->getPositionY());
		_delegate->isDrag=false;
	}

};



void MenuButton::playSound(){

	SimpleAudioEngine::sharedEngine()->stopAllEffects();

	switch(_btnType){
	case Training:
		SimpleAudioEngine::sharedEngine()->playEffect(TRAINING_SOUND);
		/*if(this->getBtnType()==Training){
		_delegate->hardCore_btn->setVisible(true);
		}*/
		break;
	case Network:
		SimpleAudioEngine::sharedEngine()->playEffect(NETWORK_SOUND);
		break;
	case Credits:
		SimpleAudioEngine::sharedEngine()->playEffect(CREDITS_SOUND);
		break;
	case Exit:
		SimpleAudioEngine::sharedEngine()->playEffect(EXIT_SOUND);
		break;
	}
	
}

MenuButton* MenuButton::create( const char *szImage ){
	MenuButton* mb=new MenuButton();
	if (mb && mb->init(szImage))
	{
		mb->autorelease();
		return mb;
	}
	else
	{
		delete mb;
		return NULL;
	}
}



/*----------------------
init StartMenu layer;
----------------------*/

StartMenu::StartMenu(void)
{
	_menu_array=NULL;
	isClockwise=false;
	isDrag=false;
	hardCore_btn=NULL;
	hardCoreLayer=NULL;
	notice_layer=NULL;
	noticeLabel=NULL;
}


StartMenu::~StartMenu(void)
{
	
}

bool StartMenu::init()
{
	bool bRet = false;
	do{
		CC_BREAK_IF(!CCLayer::init());
		
		

		SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.5);

		//CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();


		//CCSprite* bgSprite = CCSprite::create("red_bg.png");
		////pSprite->setPosition(ccp(winSize.width/2 + origin.x, winSize.height/2 + origin.y));
		//bgSprite->setAnchorPoint(ccp(0,0));
		//bgSprite->setPosition(ccp(0,0));
		//this->addChild(bgSprite, -5);


		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Menu.plist");
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Result.plist");	
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("NamePlate.plist");	
		//produce groud 

		CCSprite* gold_left=CCSprite::createWithSpriteFrameName("gold_left.png");
		gold_left->setAnchorPoint(ccp(0,0));
		gold_left->setPosition(ccp(0,20));
		this->addChild(gold_left,1);


		CCSprite* gold_right=CCSprite::createWithSpriteFrameName("gold_right.png");
		gold_right->setAnchorPoint(ccp(0,1));
		gold_right->setPosition(ccp(winSize.width-gold_right->getContentSize().width-20,winSize.height-20));
		this->addChild(gold_right,1);

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

	
		

		//produce the menu_bar
		CCSprite* menu_bar_b = CCSprite::create("menu_bar2.png");
		menu_bar_b->setAnchorPoint(ccp(0,0));
		this->addChild(menu_bar_b,2);


		CCSprite* menu_bar_t = CCSprite::create("menu_bar3.png");
		menu_bar_t->setAnchorPoint(ccp(0,0));
		menu_bar_t->setPosition(ccp(0,winSize.height-menu_bar_t->getContentSize().height));
		this->addChild(menu_bar_t,2);

		CCSprite* startmenu_title=CCSprite::createWithSpriteFrameName("startmenu_title.png");
		startmenu_title->setAnchorPoint(ccp(0,0));
		startmenu_title->setPosition(ccp(2,winSize.height-startmenu_title->getContentSize().height-2));
		this->addChild(startmenu_title,3);

		//produce the menu button

		this->setMenus(CCArray::createWithCapacity(3));

		MenuButton* network_btn=MenuButton::create("menu01.png");
		network_btn->setDelegate(this);
		network_btn->setBtnType(Network);
		network_btn->setScale(0.5f);
		network_btn->setPositionY(_pos03);
		_menu_array->addObject(network_btn);

		MenuButton* credits_btn=MenuButton::create("menu04.png");
		credits_btn->setDelegate(this);
		credits_btn->setBtnType(Credits);
		credits_btn->setScale(0.5f);
		credits_btn->setVisible(false);
		credits_btn->_isBottom=true;
		credits_btn->setPositionY(_pos02);
		_menu_array->addObject(credits_btn);


		MenuButton* training_btn=MenuButton::create("menu02.png");
		training_btn->setDelegate(this);
		training_btn->setBtnType(Training);
		training_btn->_isTop=true;
		training_btn->setPositionY(_pos02);
		_menu_array->addObject(training_btn);

		/*if(!CCUserDefault::sharedUserDefault()->getBoolForKey("isHardCore")){
			CCUserDefault::sharedUserDefault()->setBoolForKey("isHardCore", false);
			CCUserDefault::sharedUserDefault()->flush();
		}
		

		if(CCUserDefault::sharedUserDefault()->getBoolForKey("isHardCore")==true){
			hardCore_btn=MenuButton::create("menu02_1.png");
		}else{
			hardCore_btn=MenuButton::create("menu02_2.png");
		}

		hardCore_btn->setDelegate(this);
		hardCore_btn->setBtnType(HardCore);
		hardCore_btn->setPosition(ccp(146,_pos02-18));
		hardCore_btn->_isTop=true;
		this->addChild(hardCore_btn,10);*/




		MenuButton* exit_btn=MenuButton::create("menu03.png");
		exit_btn->setDelegate(this);
		exit_btn->setBtnType(Exit);
		exit_btn->setScale(0.5f);
		exit_btn->setPositionY(_pos01);
		_menu_array->addObject(exit_btn);
		
		
		

		menuText=CCSprite::createWithSpriteFrameName("menu02_text.png");
		menuText->setAnchorPoint(ccp(0,0));
		menuText->setPosition(ccp(10,2));
		this->addChild(menuText,5);

		CCObject* pObject=NULL;
		CCARRAY_FOREACH(_menu_array,pObject){
			MenuButton* menu=(MenuButton *)pObject;
			menu->setPositionX(105);
			this->addChild(menu,2);
		};


		//版本号
		CCLabelBMFont* versionLabel=CCLabelBMFont::create("1.23 EX Version","Fonts/1.fnt");
		versionLabel->setScale(0.3f);
		versionLabel->setPosition(winSize.width-60,10);
		this->addChild(versionLabel,5);

		CCSprite* avator=CCSprite::createWithSpriteFrameName("avator1.png");
		avator->setAnchorPoint(ccp(0,0));
		avator->setOpacity(0);
		avator->setPosition(ccp(winSize.width-avator->getContentSize().width,19));
		this->addChild(avator,1);


		CCArray* tempArray= CCArray::create();

		int i=1;
		
		CCArray* list=CCArray::create();
		while (i<5)
		{
			CCString* path=CCString::createWithFormat("avator%d.png",i);
			CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->getSpriteFrameByName(path->getCString());
			tempArray->addObject(frame);
			CCAnimation* tempAnimation= CCAnimation::createWithSpriteFrames(tempArray->toVector<CCSpriteFrame*>(), 0.1f);
			CCAction* tempAction=CCAnimate::create(tempAnimation);
			list->addObject(tempAction);
			CCActionInterval* fadeIn=CCFadeIn::create(0.8f);	
			CCDelayTime* delay=CCDelayTime::create(1.0f);
			CCActionInterval* fadeOut=CCFadeOut::create(0.5f);	
			list->addObject(fadeIn);
			list->addObject(delay);
			list->addObject(fadeOut);
			i+=1;
		}


		avator->runAction(CCRepeatForever::create(CCSequence::create(list)));

		this->setNotice();
	


		if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID){
			this->setKeypadEnabled(true);
		}

		this->scheduleUpdate();
		
		bRet=true;

	}while(0);

	return bRet;

}
void StartMenu::onEnter(){
	CCLayer::onEnter();
	SimpleAudioEngine::sharedEngine()->end();

	if(CCUserDefault::sharedUserDefault()->getBoolForKey("isBGM")!=false){
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Audio/Music/menu_music.mp3",true);
	}


}
void StartMenu::onExit(){
	CCLayer::onExit();
	
}

void StartMenu::update(float dt){

	if(!noticeLabel){
		return;
	}

	float currentX = noticeLabel->getPositionX();
	//float contentX = getContentSize().width;
	float lableX = noticeLabel->getContentSize().width;


	if(noticeLabel->getPositionX()>=-lableX){
		noticeLabel->setPositionX(noticeLabel->getPositionX()-0.6f);
	}	
	else{
		noticeLabel->setPositionX(190);
	}



}

void StartMenu::setNotice(){

	if(!notice_layer){
		notice_layer=CCLayer::create();
		CCSprite* notice_bg=CCSprite::createWithSpriteFrameName("notice_bg.png");
		notice_bg->setAnchorPoint(ccp(0,0));
		notice_bg->setPosition(ccp(15,228));
		notice_layer->addChild(notice_bg);


		CCClippingNode* clipper = CCClippingNode::create();
		CCNode *stencil = CCSprite::createWithSpriteFrameName("notice_mask.png");
		stencil->setAnchorPoint(ccp(0,0));
		clipper->setStencil(stencil);

		noticeLabel=CCLabelTTF::create("",FONT_TYPE,12);
		noticeLabel->setAnchorPoint(ccp(0,0));
		clipper->addChild(noticeLabel);
		noticeLabel->retain();
		clipper->setPosition(ccp(35,228));

		notice_layer->addChild(clipper);

		this->addChild(notice_layer);
		CCDictionary *strings = CCDictionary::createWithContentsOfFile("Element/strings.xml");  
		const char *reply = ((CCString*)strings->objectForKey("Notice"))->m_sString.c_str(); 
		
		noticeLabel->setString(reply);
		
	}
	

}
void StartMenu::onHardCoreOn(CCObject* sender){
	CCUserDefault::sharedUserDefault()->setBoolForKey("isHardCore",true);
	if(hardCore_btn){
		CCSpriteFrame *frame=CCSpriteFrameCache::sharedSpriteFrameCache()->getSpriteFrameByName("menu02_1.png"); 
		hardCore_btn->setSpriteFrame(frame);
	}

	SimpleAudioEngine::sharedEngine()->playEffect("Audio/Menu/confirm.ogg");
	if(hardCoreLayer){
		hardCoreLayer->removeAllChildren();
		hardCoreLayer->removeFromParent();
		hardCoreLayer=NULL;
	}

	this->onHardCoreCallBack();
}

void StartMenu::onHardCoreOff(CCObject* sender){

	SimpleAudioEngine::sharedEngine()->playEffect("Audio/Menu/cancel.ogg");
	if(hardCoreLayer){
		hardCoreLayer->removeAllChildren();
		hardCoreLayer->removeFromParent();
		hardCoreLayer=NULL;
	}

}

// Network button: the HardCore warning is shown until the player accepts it once, then it goes straight to hero select.
void StartMenu::onHardLayerCallBack(){

	if( CCUserDefault::sharedUserDefault()->getBoolForKey("isHardCore")==true){
		this->onHardCoreCallBack();
		return;
	}

	if( CCUserDefault::sharedUserDefault()->getBoolForKey("isHardCore")==false){
		if(!hardCoreLayer){
	
		hardCoreLayer=CCLayer::create();

		CCSprite* confirm_bg=CCSprite::createWithSpriteFrameName("confirm_bg.png");
		confirm_bg->setPosition(ccp(winSize.width/2,winSize.height/2));

		// hardcore_title.png / hardcore_text.png were never shipped in Menu.plist; use the generic confirm title and a text label instead
		CCSprite* hardcore_title=CCSprite::createWithSpriteFrameName("confirm_title.png");
		hardcore_title->setPosition(ccp(winSize.width/2,winSize.height/2+38));

		CCDictionary *strings = CCDictionary::createWithContentsOfFile("Element/strings.xml");
		const char *tips = ((CCString*)strings->objectForKey("HardCoreTips"))->getCString();
		CCLabelTTF* hardcore_text=CCLabelTTF::create(tips,FONT_TYPE,12,CCSizeMake(confirm_bg->getContentSize().width-24,0),ax::TextHAlignment::CENTER);
		hardcore_text->setPosition(ccp(winSize.width/2,winSize.height/2+8));

		CCMenuItem* yes_btn=CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("yes_btn1.png"),CCSprite::createWithSpriteFrameName("yes_btn2.png"),this,menu_selector(StartMenu::onHardCoreOn));
		CCMenuItem* no_btn=CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("no_btn1.png"),CCSprite::createWithSpriteFrameName("no_btn2.png"),this,menu_selector(StartMenu::onHardCoreOff));

		CCMenu* confirm_menu = CCMenu::create(yes_btn,no_btn,NULL);
		confirm_menu->alignItemsHorizontallyWithPadding(24);
		confirm_menu->setPosition(ccp(winSize.width/2,winSize.height/2-30));

		hardCoreLayer->addChild(confirm_bg,600);
		hardCoreLayer->addChild(confirm_menu,650);
		hardCoreLayer->addChild(hardcore_title,650);
		hardCoreLayer->addChild(hardcore_text,650);
		this->addChild(hardCoreLayer,700);

		}
	}
	
}

void StartMenu::onHardCoreCallBack(){

	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	this->openHeroSelect(true);
}

void StartMenu::onNormalCallBack(CCObject* sender){

	this->openHeroSelect(false);
}

void StartMenu::openHeroSelect(bool isHardCore){

	//加载序列集
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Select.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("UI.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Report.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Ougis.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Ougis2.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Map.plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Gears.plist");

	if (isHardCore==true)
        {
		CCScene* selectScene=CCScene::create();
		NetworkLayer* networkLayer=NetworkLayer::create(isHardCore);
		selectScene->addChild(networkLayer);
		CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.5f,selectScene));
	}
	else {
		CCScene* selectScene      = CCScene::create();
        SelectLayer* selectLayer = SelectLayer::create();
        selectScene->addChild(selectLayer);
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.5f, selectScene));
	}
}

void StartMenu::onTrainingCallBack(){

        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Select.plist");
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("UI.plist");
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Report.plist");
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Ougis.plist");
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Ougis2.plist");
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Map.plist");
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Gears.plist");


	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
        CCScene* selectScene     = CCScene::create();
        SelectLayer* selectLayer = SelectLayer::create();
        selectScene->addChild(selectLayer);
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.5f, selectScene));
}

void StartMenu::onCreditsCallBack(){


	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	CCScene* creditsScene=CCScene::create();
	CreditsLayer* creditsLayer=CreditsLayer::create();
	creditsScene->addChild(creditsLayer);
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.5f,creditsScene));

}

void StartMenu::scrollMenu(int posY){
		CCObject* pObject=NULL;
		if(posY>_pos02 || (isDrag && isClockwise)){ //顺时针
			CCARRAY_FOREACH(_menu_array,pObject){
				MenuButton* menu=(MenuButton *)pObject;
				if(menu->getPositionY()==_pos01){
					CCActionInterval* spn=CCMoveTo::create(0.5,ccp(105,_pos02));
					this->reorderChild(menu,1);
					menu->_isBottom=true;
					CCFadeOut* fo=CCFadeOut::create(0.3f);
					ax::Spawn* seq=CCSpawn::createWithTwoActions(spn,fo);
					menu->runAction(seq);

				}else if(menu->getPositionY()==_pos03){
					CCAction* spn=CCSpawn::create(
						CCMoveTo::create(0.5,ccp(105,_pos02)),
						CCScaleTo::create(0.5,1),
						NULL
						);
					this->reorderChild(menu,3);
					menu->_isTop=true;
				
					/*if(menu->getBtnType()!=Training){
						hardCore_btn->setVisible(false);
					}*/

					CCFiniteTimeAction *call = CCCallFunc::create(menu,callfunc_selector(MenuButton::playSound));
					CCArray* seqArray=CCArray::create();
					seqArray->addObject(spn);
					seqArray->addObject(call);
					CCActionInterval* seq=CCSequence::create(seqArray);
					menu->runAction(seq);

				}else if(menu->getPositionY()==_pos02 && menu->_isBottom){
				
					CCActionInterval* spn=CCMoveTo::create(0.5,ccp(105,_pos03));
					this->reorderChild(menu,1);
					menu->setVisible(true);
					CCFadeIn* fi=CCFadeIn::create(0.3f);
					ax::Spawn* seq=CCSpawn::createWithTwoActions(spn,fi);
					menu->_isBottom=false;
					menu->runAction(seq);
				}else if(menu->getPositionY()==_pos02 && !menu->_isBottom){

					CCAction* spn=CCSpawn::create(
						CCMoveTo::create(0.5,ccp(105,_pos01)),
						CCScaleTo::create(0.5,0.5),
						NULL
						);
					this->reorderChild(menu,1);
					menu->_isTop=false;
					menu->runAction(spn);
				}

			};

		}else{ //逆时针
			CCARRAY_FOREACH(_menu_array,pObject){
				MenuButton* menu=(MenuButton *)pObject;
				if(menu->getPositionY()==_pos01){
					CCAction* spn=CCSpawn::create(
						CCMoveTo::create(0.5,ccp(105,_pos02)),
						CCScaleTo::create(0.5,1),
						NULL
						);
					this->reorderChild(menu,3);
					menu->_isTop=true;
				/*	if(menu->getBtnType()!=Training){
						hardCore_btn->setVisible(false);
					}*/
				
					CCFiniteTimeAction *call = CCCallFunc::create(menu,callfunc_selector(MenuButton::playSound));
					CCArray* seqArray=CCArray::create();
					seqArray->addObject(spn);
					seqArray->addObject(call);
					CCActionInterval* seq=CCSequence::create(seqArray);
					menu->runAction(seq);
				}else if(menu->getPositionY()==_pos03){
					CCActionInterval* spn=CCMoveTo::create(0.5,ccp(105,_pos02));
					this->reorderChild(menu,1);
					menu->_isBottom=true;
					CCFadeOut* fo=CCFadeOut::create(0.3f);
					ax::Spawn* seq=CCSpawn::createWithTwoActions(spn,fo);
					menu->runAction(seq);

				}else if(menu->getPositionY()==_pos02 && menu->_isBottom){

					CCActionInterval* spn=CCMoveTo::create(0.5,ccp(105,_pos01));
					this->reorderChild(menu,2);
					menu->setVisible(true);
					CCFadeIn* fi=CCFadeIn::create(0.3f);
					ax::Spawn* seq=CCSpawn::createWithTwoActions(spn,fi);
					menu->_isBottom=false;
					menu->runAction(seq);
				}
				
				else if(menu->getPositionY()==_pos02  && !menu->_isBottom){
					CCAction* spn=CCSpawn::create(
						CCMoveTo::create(0.5,ccp(105,_pos03)),
						CCScaleTo::create(0.5,0.5),
						NULL
						);
					this->reorderChild(menu,2);
					menu->_isTop=false;
					menu->runAction(spn);
				}
				
			};

		};
		std::string src="";
		CCARRAY_FOREACH(_menu_array,pObject){
			MenuButton* menu=(MenuButton *)pObject;
			
			if(menu->_isTop){
				switch(menu->getBtnType()){
				case Training:
					 src="menu02_text.png";
					break;
				case Network:
					 src="menu01_text.png";
					break;
				case Credits:
					 src="menu04_text.png";
					break;
				case Exit:
					 src="menu03_text.png";
					break;
				}

			}
		}

		CCSpriteFrame *frame=CCSpriteFrameCache::sharedSpriteFrameCache()->getSpriteFrameByName(src.c_str()); 
		menuText->setSpriteFrame(frame);
	
}


void StartMenu::keyBackClicked(){


	#if CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID

	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo,"net/zakume/game/DialogUtils","showTipDialog", "(Ljava/lang/String;Ljava/lang/String;)V");  

	if (isHave)   
	{  

		jstring jTitle = minfo.env->NewStringUTF("Exit Game");
		jstring jMsg = minfo.env->NewStringUTF("Do you really want to exit?");
		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, jTitle, jMsg);
		minfo.env->DeleteLocalRef(jTitle);
		minfo.env->DeleteLocalRef(jMsg);
	}  

	#endif  

	
};

void StartMenu::onExitCallBack(){
	
	this->keyBackClicked();
}