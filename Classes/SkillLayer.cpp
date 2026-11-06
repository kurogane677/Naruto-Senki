#include "SkillLayer.h"
#include "Heroes/HeroRegistry.h"
#include "MyUtils/KTools.h"
#include "SelectLayer.h"
#include "NetworkLayer.h"
USING_NS_CC_EXT;
using namespace CocosDenshion;


/*----------------------
init MenuButton ;
----------------------*/
SelectButton::SelectButton(void)
{

	_btnType=Menu;
	_isCanBuy=false;
	_isCanBuy2=false;
	_isUnlock=false;
	_clickTime=0;
	_isAviable=true;
	_delegate1=NULL;
	_delegate2=NULL;
	_delegate3=NULL;
	_charName=NULL;
}


SelectButton::~SelectButton(void)
{

}

bool SelectButton::init(const char *szImage){
	bool bRet = false;
	do{
		CC_BREAK_IF(!CCSprite::init());
		const char* path;

		path=CCString::createWithFormat("%s",szImage)->getCString();
		this->initWithSpriteFrameName(path);
		this->setAnchorPoint(ccp(0,0));

		bRet=true;

	}while(0);

	return bRet;

}

void SelectButton::onEnter(){
	CCSprite::onEnter();
	CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this,0,true);
}
void SelectButton::onExit(){
	CCSprite::onExit();
	CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
}

CCRect SelectButton::getRect(){
	CCSize size=this->getContentSize();
	return CCRect(0,0, size.width, size.height);

}

bool SelectButton::containsTouchLocation(CCTouch *touch){

	return getRect().containsPoint(convertTouchToNodeSpace(touch));
}

bool SelectButton::ccTouchBegan(CCTouch* touch,CCEvent* event){
	// touch area 
	if (!this->containsTouchLocation(touch))
		return false;
	click();
	return true;

}
void SelectButton::ccTouchEnded(CCTouch* touch,CCEvent* event){

};

void SelectButton::click(){

	if(this->getBtnType()==Menu){
			if(strcmp(_charName->getCString(),"None2")==0 ){
				CCTips *tip=CCTips::create("LimitedChar");
				this->_delegate2->addChild(tip,5000);
			}else if( strcmp(_charName->getCString(),"None")!=0 && _isAviable){
			SimpleAudioEngine::sharedEngine()->stopAllEffects();

			if(_delegate2){
				// HardCore picks COM1, COM2, then the player; the player pick is always the last one
				if(_delegate2->_playerSelect){

					return;
				}
				{

					CCObject* pObject;
					CCARRAY_FOREACH(_delegate2->selectArray,pObject){
						SelectButton* selectBtn=(SelectButton*) pObject;
						if (strcmp(selectBtn->getCharName()->getCString(),this->getCharName()->getCString())!=0){
							selectBtn->_clickTime=0;
						}
					}
					if(this->_clickTime==0){
						if(CCUserDefault::sharedUserDefault()->getBoolForKey("isVoice")!=false){
							SimpleAudioEngine::sharedEngine()->stopAllEffects();
							CCString* introSRC=CCString::createWithFormat("Audio/Intro/%s.mp3",this->getCharName()->getCString());
							SimpleAudioEngine::sharedEngine()->playEffect(introSRC->getCString());
						}
						SimpleAudioEngine::sharedEngine()->playEffect(SELECT_SOUND);
					}else if(this->_clickTime==1){
						SimpleAudioEngine::sharedEngine()->playEffect("Audio/Menu/confirm2.ogg");
						_isAviable=false;
						this->spriteToGrey();
						this->getParent()->reorderChild(this,500);
					}

					this->_clickTime++;

					_delegate2->setSelected(this);
				}
				

			}else if(_delegate1){
				SimpleAudioEngine::sharedEngine()->playEffect(SELECT_SOUND);
				_delegate1->setSelected(this);
			}

			
		}

	}else{
		
		
		_delegate3->setSkillExplain(_btnType);
			
	}

}


void SelectButton::spriteToGrey(){
	CCSprite* black=CCSprite::createWithSpriteFrameName("Black_select.png");
	black->setAnchorPoint(ccp(0.5,0.5));
	black->setPosition(ccp(17,32));
	this->addChild(black);

}

SelectButton* SelectButton::create( const char *szImage ){

	SelectButton* mb=new SelectButton();
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


//SkillLayer


SkillLayer::SkillLayer(void)
{
	_skillExplain=NULL;
	buyLayer=NULL;
	_buyType=Menu;
	fengSprite=NULL;
	fengSprite2=NULL;
	_autoMove=true;

	_delegate=NULL;
	_delegate2=NULL;

}


SkillLayer::~SkillLayer(void)
{

}

bool SkillLayer::init(){

	bool bRet=false;
	do{
		CC_BREAK_IF(!CCLayer::init());
		
	
		
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
		this->addChild(cloud_left,2);

		CCActionInterval* cmv1=CCMoveBy::create(1,ccp(-15,0));
		CCAction* cseq1=CCRepeatForever::create(CCSequence::create(cmv1,cmv1->reverse(),NULL));
		cloud_left->runAction(cseq1);

		CCSprite* cloud_right=CCSprite::createWithSpriteFrameName("cloud.png");	
		cloud_right->setPosition(ccp(winSize.width-cloud_right->getContentSize().width,15));
		cloud_right->setFlippedY(true);
		cloud_right->setAnchorPoint(ccp(0,0));
		this->addChild(cloud_right,2);

		CCActionInterval* cmv2=CCMoveBy::create(1,ccp(15,0));
		CCAction* cseq2=CCRepeatForever::create(CCSequence::create(cmv2,cmv2->reverse(),NULL));
		cloud_right->runAction(cseq2);

		CCSprite* cloud_top=CCSprite::createWithSpriteFrameName("cloud_top.png");
		cloud_top->setPosition(ccp(winSize.width/2,winSize.height-(cloud_top->getContentSize().height+12)));
		cloud_top->setAnchorPoint(ccp(0.5f,0));
		this->addChild(cloud_top,1);


	

		//produce the menu_bar
		CCSprite* menu_bar_b = CCSprite::create("menu_bar2.png");
		KTools::extendBarToScreen(menu_bar_b);
		menu_bar_b->setAnchorPoint(ccp(0,0));
		this->addChild(menu_bar_b,2);


		CCSprite* menu_bar_t = CCSprite::create("menu_bar3.png");
		KTools::extendBarToScreen(menu_bar_t);
		menu_bar_t->setAnchorPoint(ccp(0,0));
		menu_bar_t->setPosition(ccp(0,winSize.height-menu_bar_t->getContentSize().height));
		this->addChild(menu_bar_t,2);

		CCSprite* record_title=CCSprite::createWithSpriteFrameName("record_title.png");
		record_title->setAnchorPoint(ccp(0,0));
		record_title->setPosition(ccp(2,winSize.height-record_title->getContentSize().height-2));
		this->addChild(record_title,3);



		CCMenuItem* start_bt=CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("return_btn.png"),NULL,NULL,this,menu_selector(SkillLayer::onCancel));
		CCMenu* menu =CCMenu::create(start_bt,NULL);
		menu->setPosition(winSize.width-38,86);
		this->addChild(menu,5);

		bRet=true;

	}while(0);

	return bRet;

}

void SkillLayer::initInterface(){

	char* bg_src;
	if(_delegate){
		bg_src="red_bg.png";
		selectHero=_delegate->_selectHero;
	}else if(_delegate2){
		bg_src="blue_bg.png";
		selectHero=_delegate2->_selectHero;
	}

	CCSprite* bgSprite = CCSprite::create(bg_src);
	KTools::fitScreenWidth(bgSprite);
	bgSprite->setAnchorPoint(ccp(0,0));
	bgSprite->setPosition(ccp(0,0));
	this->addChild(bgSprite, -5);

	KTools* tool= KTools::create();

	

		char* skillbg_src="skill_bg1.png";
		

		skillSprite = CCSprite::createWithSpriteFrameName(skillbg_src);
		skillSprite->setPosition(ccp(winSize.width/2,winSize.height/2));
		this->addChild(skillSprite,2);


		

	
		CCMenuItem* change_bt=NULL;
		if(HeroRegistry::nextAwakening(selectHero)){

				change_bt=CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("change_btn.png"),NULL,NULL,this,menu_selector(SkillLayer::onChangeBtn));

		}

		if(change_bt){
			changemenu =CCMenu::create(change_bt,NULL);
			changemenu->setPosition(ccp(winSize.width/2,winSize.height/2-skillSprite->getContentSize().height/2+120));
			CCActionInterval*  fd=CCFadeOut::create(1.0f);
			CCAction* seq=CCRepeatForever::create(CCSequence::create(fd,fd->reverse(),NULL));
			change_bt->runAction(seq);
			this->addChild(changemenu,15);
		}

		CCSprite* goldBG=CCSprite::createWithSpriteFrameName("gold_bg.png");
		goldBG->setAnchorPoint(ccp(0.5,0));
		goldBG->setPosition(ccp(winSize.width/2+skillSprite->getContentSize().width/4+25,winSize.height/2+skillSprite->getContentSize().width/2-74));
		this->addChild(goldBG,9);

		CCString* cl=tool->readFromSQLite();
		coinLabel=CCLabelBMFont::create(cl->getCString(),"Fonts/1.fnt");
		coinLabel->setScale(0.3f);
		coinLabel->setAnchorPoint(ccp(0.5,0));
		coinLabel->setPosition(ccp(goldBG->getPositionX()-goldBG->getContentSize().width/2+40,goldBG->getPositionY()+4));
		this->addChild(coinLabel,15);





		CCSprite* bondBG=CCSprite::createWithSpriteFrameName("golds_1.png");
		bondBG->setAnchorPoint(ccp(0.5,0));
		bondBG->setPosition(ccp(goldBG->getPositionX()-goldBG->getContentSize().width/2+15,goldBG->getPositionY()+4));
		this->addChild(bondBG,15);

		CCSprite* bondBG2=CCSprite::createWithSpriteFrameName("golds_2.png");
		bonds=CCProgressTimer::create(bondBG2);
		bonds->setType(kCCProgressTimerTypeBar);
		bonds->setAnchorPoint(ccp(0.5,0));
		bonds->setMidpoint(ccp(0,0));
		bonds->setBarChangeRate(ccp(0, 1));



		if(atoi(cl->getCString())>=99999){
		bonds->setPercentage(100);
		}else if(atoi(cl->getCString())>=80000){
		bonds->setPercentage(80);
		}else if(atoi(cl->getCString())>=60000){
		bonds->setPercentage(60);
		}else if(atoi(cl->getCString())>=40000){
		bonds->setPercentage(40);
		}else if(atoi(cl->getCString())>=20000){
		bonds->setPercentage(20);
		}

		bonds->setPosition(ccp(goldBG->getPositionX()-goldBG->getContentSize().width/2+15,goldBG->getPositionY()+4));
		this->addChild(bonds,20);


		
		
		

		CCString* winNum=tool->readSQLite("CharRecord","name",selectHero,"column1");
		std::string bondString=winNum->getCString();
		CCString* totalNum=tool->readSQLite("CharRecord","name",selectHero,"column2"); 
		

		
		char* rank_src;
		bool isBlink=false;
		if(atoi(bondString.c_str())>=300){
			skillbg_src="skill_bg6.png";
			rank_src="rank_sss.png";
			isBlink=true;
		}else if(atoi(bondString.c_str())>=200){
			skillbg_src="skill_bg5.png";
			rank_src="rank_ss.png";
			isBlink=true;
		}else if(atoi(bondString.c_str())>=100){
			skillbg_src="skill_bg4.png";
			rank_src="rank_s.png";
			isBlink=true;
		}else if(atoi(bondString.c_str())>=50){
			skillbg_src="skill_bg3.png";
			rank_src="rank_a.png";
		}else if(atoi(bondString.c_str())>=25){
			skillbg_src="skill_bg2.png";
			rank_src="rank_b.png";
		}else{
			skillbg_src="skill_bg1.png";
			rank_src="rank_c.png";
		}

		skillSprite = CCSprite::createWithSpriteFrameName(skillbg_src);
		skillSprite->setPosition(ccp(winSize.width/2,winSize.height/2));
		this->addChild(skillSprite,2);

		CCString* imgPath=NULL;

		
		if((strcmp(selectHero,"Hinata")==0 || 
			strcmp(selectHero,"Sakura")==0 || 
			strcmp(selectHero,"Ino")==0 ||
			strcmp(selectHero,"Tsunade")==0 ||
			strcmp(selectHero,"Tenten")==0 ||
			strcmp(selectHero,"Konan")==0 ||
			strcmp(selectHero,"Karin")==0 
			) && atoi(bondString.c_str())>=100){
				imgPath=CCString::createWithFormat("%s_full2.png",selectHero);
		}else{
			imgPath=CCString::createWithFormat("%s_full.png",selectHero);
		}
		_heroFull=CCSprite::createWithSpriteFrameName(imgPath->getCString());
		_heroFull->setAnchorPoint(ccp(0.5,0));
		_heroFull->setPosition(ccp(winSize.width/2,
			winSize.height/2-skillSprite->getContentSize().height/2+23));
		this->addChild(_heroFull,5);



		if(isBlink){

			CCSprite* blink=CCSprite::createWithSpriteFrameName("skill_blink6.png");
			blink->setPosition(ccp(winSize.width/2,winSize.height/2));

			this->addChild(blink,9);


			CCArray* tempArray= CCArray::create();

			int i=1;

			CCArray* list=CCArray::create();
			while (i<7)
			{
				CCString* path=CCString::createWithFormat("skill_blink%d.png",i);
				CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->getSpriteFrameByName(path->getCString());
				tempArray->addObject(frame);


				i+=1;
			}

			CCAnimation* tempAnimation= CCAnimation::createWithSpriteFrames(tempArray->toVector<CCSpriteFrame*>(), 0.1f);
			CCAction* tempAction=CCAnimate::create(tempAnimation);
			list->addObject(tempAction);
			CCDelayTime* delay=CCDelayTime::create(2.0f);
			list->addObject(delay);

			blink->runAction(CCRepeatForever::create(CCSequence::create(list)));
		}


		CCSprite* frameSprite=CCSprite::createWithSpriteFrameName("skill_frame.png");
		frameSprite->setPosition(ccp(winSize.width/2,winSize.height/2));
		this->addChild( frameSprite,10);

		CCSprite* rankSprite=CCSprite::createWithSpriteFrameName(rank_src);
		rankSprite->setAnchorPoint(ccp(0,0));
		rankSprite->setPosition(ccp(skillSprite->getPositionX()-skillSprite->getContentSize().width/2+5,skillSprite->getPositionY()+skillSprite->getContentSize().height/2-20));
		this->addChild(rankSprite,11);


		CCSprite* detailBG1=CCSprite::createWithSpriteFrameName("detail_bg.png");
		detailBG1->setAnchorPoint(ccp(0,0));
		detailBG1->setPosition(ccp(skillSprite->getPositionX()-skillSprite->getContentSize().width/2+8,skillSprite->getPositionY()-skillSprite->getContentSize().height/2+74));
		this->addChild(detailBG1,5);

		CCString* recordTime=tool->readSQLite("CharRecord","name",selectHero,"column3");
		CCLabelBMFont* bestLabel;
		if(strcmp(recordTime->getCString(),"")==0){
			bestLabel=CCLabelBMFont::create("00:00:00","Fonts/1.fnt");
		}else{
			bestLabel=CCLabelBMFont::create(recordTime->getCString(),"Fonts/1.fnt");
		}
		
		bestLabel->setScale(0.3f);
		bestLabel->setAnchorPoint(ccp(0,0));
		bestLabel->setPosition(ccp(detailBG1->getPositionX()+8,detailBG1->getPositionY()+7));
		this->addChild(bestLabel,6);



		CCSprite* detailBG2=CCSprite::createWithSpriteFrameName("detail_bg.png");
		detailBG2->setAnchorPoint(ccp(0,0));
		detailBG2->setPosition(ccp(skillSprite->getPositionX()+skillSprite->getContentSize().width/2-detailBG2->getContentSize().width-8,skillSprite->getPositionY()-skillSprite->getContentSize().height/2+74));
		this->addChild(detailBG2,5);



		CCLabelBMFont* recordLabel=CCLabelBMFont::create(CCString::createWithFormat("%s bonds",bondString.c_str())->getCString(),"Fonts/1.fnt");
		recordLabel->setScale(0.3f);
		recordLabel->setAnchorPoint(ccp(0,0));

		recordLabel->setPosition(ccp(detailBG2->getPositionX()+12,detailBG2->getPositionY()+7));
		this->addChild(recordLabel,6);




		//更新技能表
		this->updateSkillGroup();
	


	





	this->scheduleUpdate();

}


void SkillLayer::onChangeBtn(CCObject* sender){


	SimpleAudioEngine::sharedEngine()->playEffect("Audio/Menu/chang_btn.ogg");
	skillGroup->removeFromParent();
	changemenu->removeFromParent();

	



	//觉醒必备
	if(const char* awakened=HeroRegistry::nextAwakening(selectHero)){
		selectHero=awakened;
	}

	CCSpriteFrame *frame=CCSpriteFrameCache::sharedSpriteFrameCache()->getSpriteFrameByName(CCString::createWithFormat("%s_full.png",selectHero)->getCString()); 
	_heroFull->setSpriteFrame(frame);


	//更新技能表
	this->updateSkillGroup();

	if(HeroRegistry::nextAwakening(selectHero)){

		CCMenuItem* change_bt=CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("change_btn2.png"),NULL,NULL,this,menu_selector(SkillLayer::onChangeBtn));

		if(change_bt){

			changemenu =CCMenu::create(change_bt,NULL);
			changemenu->setPosition(ccp(winSize.width/2,winSize.height/2-skillSprite->getContentSize().height/2+120));
			CCActionInterval*  fd=CCFadeOut::create(1.0f);
			CCAction* seq=CCRepeatForever::create(CCSequence::create(fd,fd->reverse(),NULL));
			change_bt->runAction(seq);
			this->addChild(changemenu,15);
		}

	};


}

void SkillLayer::updateSkillGroup(){

	skillGroup=CCSprite::create();

	for(int i=1;i<=5;i++){

		CCString* path=CCString::createWithFormat("%s_skill%d.png",selectHero,i);
		SelectButton* skill_btn=SelectButton::create(path->getCString());

		if(i==4){		
			skill_btn->_isUnlock=true;	
		}

		if(i==5){	
			skill_btn->_isCanBuy2=false;
			skill_btn->_isUnlock=true;		
		}

		
		skill_btn->setPosition(ccp((i-1)*46,0));


		skill_btn->setBtnType((sbtnType)(i+2));
		if(this->_delegate){
			skill_btn->setDelegate1(this->_delegate);
		}else{
			skill_btn->setDelegate2(this->_delegate2);
		}
		
		
		skill_btn->setDelegate3(this);
		skillGroup->addChild(skill_btn);

	}


	skillGroup->setPosition(ccp(skillSprite->getPositionX()-skillSprite->getContentSize().width/2+2,60));
	this->addChild(skillGroup,8);
}

// Skill descriptions come from Element/skills.xml (English) and are drawn with
// Fonts/1.fnt. A hero without an entry there, e.g. a new mod hero, keeps its old
// <hero>_label<n>.png image.
static std::string skillText(const char* hero,sbtnType type){
	static ax::ValueMap texts=ax::FileUtils::getInstance()->getValueMapFromFile("Element/skills.xml");
	if(type==Unlock1 || type==Unlock2){
		auto it=texts.find(type==Unlock1 ? "unlock1" : "unlock2");
		return it!=texts.end() ? it->second.asString() : "";
	}
	auto it=texts.find(hero);
	if(it==texts.end() || it->second.getType()!=ax::Value::Type::MAP){
		return "";
	}
	ax::ValueMap& skills=it->second.asValueMap();
	auto skill=skills.find(std::to_string((int)type-2));
	return skill!=skills.end() ? skill->second.asString() : "";
}

void SkillLayer::setSkillExplain(sbtnType type){


	SimpleAudioEngine::sharedEngine()->playEffect("Audio/Menu/select.mp3");
	if(_skillExplain) _skillExplain->getParent()->removeFromParent();
	CCString* imgPath=CCString::create("");
	if(type==Unlock1){
		imgPath=CCString::createWithFormat("fenglabel.png");
		
	}else if(type==Unlock2){
		imgPath=CCString::createWithFormat("fenglabel2.png");
		
	}else{
		imgPath=CCString::createWithFormat("%s_label%d.png",selectHero,(int)type-2);
		
	}
	
	CCClippingNode* clipper = CCClippingNode::create();
	CCNode *stencil = CCSprite::createWithSpriteFrameName("clipperMask.png");
	stencil->setAnchorPoint(ccp(0,0));
	clipper->setStencil(stencil);
	
	std::string text=skillText(selectHero,type);
	if(text.empty() && CCSpriteFrameCache::sharedSpriteFrameCache()->getSpriteFrameByName(imgPath->getCString())){
		_skillExplain=CCSprite::createWithSpriteFrameName(imgPath->getCString());
	}else{
		CCLabelBMFont* label=CCLabelBMFont::create(text.c_str(),"Fonts/1.fnt");
		label->setScale(0.36f);
		label->setPositionY(1);
		_skillExplain=label;
	}
	_skillExplain->setAnchorPoint(ccp(0,0));
	_skillExplain->setPositionX(10);
	
	clipper->setPosition(ccp(winSize.width/2-skillSprite->getContentSize().width/2+2,
		winSize.height/2-skillSprite->getContentSize().height/2+4));

	clipper->addChild(_skillExplain);
	
	this->addChild(clipper,600);


};


void SkillLayer::update(float dt){

	if(!_skillExplain || !_autoMove){
		return;
	}

	float currentX = _skillExplain->getPositionX();
	float contentX = getContentSize().width;
	float lableX = _skillExplain->getBoundingBox().size.width;

	if(_skillExplain->getPositionX()>=-lableX){
		_skillExplain->setPositionX(_skillExplain->getPositionX()-0.6f);
	}else{
		_skillExplain->setPositionX(320);
	}

}



void SkillLayer::onCancel(CCObject* sender){

	_skillExplain=NULL;
	buyLayer=NULL;
	fengSprite=NULL;
	fengSprite2=NULL;


	this->removeFromParent();
	SimpleAudioEngine::sharedEngine()->playEffect("Audio/Menu/cancel.ogg");

	CCDirector::sharedDirector()->popScene();
}






