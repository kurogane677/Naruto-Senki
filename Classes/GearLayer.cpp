#include "GearLayer.h"
#include "GameLayer.h"
#include "HudLayer.h"
#include "Characters.h"
using namespace CocosDenshion;

// The shop text is rasterized at kTextScale times its size and scaled back down,
// so it stays sharp on screens much larger than the 568x320 design size.
static const float kTextScale=4;
static const float kDetailWidth=70;
static const int kMaxGearLevel=3;

static ax::Label* shopLabel(const char* font,float size,float width,ax::TextHAlignment align){
	ax::Label* label=ax::Label::createWithTTF("",font,size*kTextScale,ax::Size(width*kTextScale,0),align);
	label->setScale(1/kTextScale);
	return label;
}

static ax::Label* detailLabel(const char* font,float size,ax::TextHAlignment align){
	ax::Label* label=shopLabel(font,size,kDetailWidth,align);
	label->setAnchorPoint(ccp(0.5f,1));
	return label;
}

// Placeholder text button until there is art for Upgrade/Sell
static ax::MenuItemLabel* textButton(const ax::Color4B& color,const ax::ccMenuCallback& callback){
	const float w=66,h=15;
	ax::Label* label=ax::Label::createWithTTF("","Fonts/PixelifySans-Bold.ttf",7*kTextScale,
		ax::Size(w*kTextScale,h*kTextScale),ax::TextHAlignment::CENTER,ax::TextVAlignment::CENTER);
	ax::LayerColor* bg=ax::LayerColor::create(color,w*kTextScale,h*kTextScale);
	bg->setName("bg");
	label->addChild(bg,-1);
	// scale the item, not the label: the item takes its size from the unscaled label
	ax::MenuItemLabel* item=ax::MenuItemLabel::create(label,callback);
	item->setScale(1/kTextScale);
	return item;
}

/*----------------------
init GearButton ;
----------------------*/
GearButton::GearButton(void)
{
	_isBuyed=false;
}


GearButton::~GearButton(void)
{

}


bool GearButton::init(const char *szImage){
	bool bRet = false;
	do{
		CC_BREAK_IF(!CCSprite::init());
		if(strcmp(szImage,"")!=0)
		this->initWithSpriteFrameName(szImage);


		this->setAnchorPoint(ccp(0,0));

		bRet=true;

	}while(0);

	return bRet;

}

// Not swallowing: a drag that starts on a gear still scrolls the list
void GearButton::onEnter(){
	CCSprite::onEnter();
	CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this,-1,false);
}
void GearButton::onExit(){
	CCSprite::onExit();
	CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
}

CCRect GearButton::getRect(){
	CCSize size=this->getContentSize();
	return CCRect(0,0, size.width, size.height);

}
bool GearButton::containsTouchLocation(CCTouch *touch){

	return getRect().containsPoint(convertTouchToNodeSpace(touch));
}


void GearButton::setBtnType(gearType type,gearbtnType type2,int level){

	_gearType=type;
	_btnType=type2;
	_isBuyed=level>0;

	if(_btnType==0){
		CCSprite* gearIcon=CCSprite::createWithSpriteFrameName(CCString::createWithFormat("gear_%02d.png",gearType(_gearType))->getCString());
		gearIcon->setAnchorPoint(ccp(0.5f,0.5f));
		gearIcon->setPosition(ccp(20,30));
		this->addChild(gearIcon);

		if(_isBuyed){
			ax::Label* levelLabel=shopLabel("Fonts/PixelifySans-Bold.ttf",7,0,ax::TextHAlignment::RIGHT);
			levelLabel->setString(ax::StringUtils::format("Lv%d",level));
			levelLabel->setTextColor(ax::Color4B(255,214,96,255));
			levelLabel->enableOutline(ax::Color4B::BLACK,kTextScale);
			levelLabel->setAnchorPoint(ccp(1,1));
			levelLabel->setPosition(ccp(this->getContentSize().width-2,this->getContentSize().height-2));
			this->addChild(levelLabel);
		}

	}else{
		CCSprite* gearIcon=CCSprite::createWithSpriteFrameName(CCString::createWithFormat("gear_%02d.png",gearType(_gearType))->getCString());
		// owned slot: no frame image, so give it a size to be tapped
		this->setContentSize(CCSize(30,30));
		gearIcon->setAnchorPoint(ccp(0.5f,0.5f));
		gearIcon->setScale(0.75f);
		gearIcon->setPosition(ccp(15,15));
		this->addChild(gearIcon);
	}


}

gearType	GearButton::getBtnType(){
	return _gearType;
}

bool GearButton::ccTouchBegan(CCTouch* touch,CCEvent* event){
	// touch area
	if (!this->containsTouchLocation(touch) || (_btnType==0 && !_delegate->isInGearList(touch))){

		return false;
	}
	_touchStart=touch->getLocation();
	return true;


}

void GearButton::ccTouchMoved(CCTouch* touch,CCEvent* event){
	// touch area


}

void GearButton::ccTouchEnded(CCTouch* touch,CCEvent* event){
	// a scroll, not a tap
	if(ccpDistance(_touchStart,touch->getLocation())>8){
		return;
	}
	if(CCUserDefault::sharedUserDefault()->getBoolForKey("isVoice")!=false){
	SimpleAudioEngine::sharedEngine()->playEffect("Audio/Menu/select.mp3");
	}
	_delegate->currentGear=this->_gearType;
	_delegate->showGearDetail(_gearType);
};



void GearButton::playSound(){


}

GearButton* GearButton::create( const char *szImage ){
	GearButton* mb=new GearButton();
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




ScrewLayer::ScrewLayer(void)
{
	_delegate=NULL;
	_gearArray=NULL;

}


ScrewLayer::~ScrewLayer(void)
{
}


bool ScrewLayer::init(){
	bool bRet = false;
	do{
		CC_BREAK_IF(!CCLayer::init());

		bRet=true;

	}while(0);

	return bRet;

}

void ScrewLayer::onEnter(){
	CCLayer::onEnter();
	CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this,0,true);
};

void ScrewLayer::onExit(){
	CCLayer::onExit();
	CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
};

// The list starts at y 76 with rows 60 apart and two rows on screen
static const float kListTop=76;
static const float kRowHeight=60;

float ScrewLayer::maxScroll(){
	return kListTop+std::max(0,totalRow-2)*kRowHeight;
}

// The bar runs from 122 (top of the list) down to 90 (bottom)
void ScrewLayer::updateScrollBar(){
	float range=maxScroll()-kListTop;
	float t=range>0 ? (this->getPositionY()-kListTop)/range : 0;
	t=std::min(std::max(t,0.0f),1.0f);
	this->screwBar->setPositionY(122-t*32);
}

bool ScrewLayer::ccTouchBegan(CCTouch* touch,CCEvent* event){

	prePosY=0;
	return true;

};

void ScrewLayer::ccTouchMoved(CCTouch* touch,CCEvent* event){
	// touch area
	CCPoint curPoint = touch->getLocation();
	if(prePosY==0){
		prePosY=curPoint.y;
	}else{
		float distanceY=curPoint.y-prePosY;
		float y=this->getPositionY()+distanceY;
		this->setPositionY(std::min(std::max(y,kListTop-20),maxScroll()+20));
		updateScrollBar();
		prePosY=curPoint.y;

	}

};


void ScrewLayer::ccTouchEnded(CCTouch* touch,CCEvent* event){
	prePosY=0;
	this->setPositionY(std::min(std::max(this->getPositionY(),kListTop),maxScroll()));
	updateScrollBar();
};





GearLayer::GearLayer(void)
{
	currentGear_layer=NULL;
	currentGear=None;
	_gearName=NULL;
	_gearDesc=NULL;
	_gearDescEn=NULL;
	_gearInfo=NULL;
	_buyMenu=NULL;
	_ownedMenu=NULL;
	_upgradeItem=NULL;
	_sellItem=NULL;
	_listClip=NULL;
}


GearLayer::~GearLayer(void)
{
}


bool GearLayer::init(CCRenderTexture* snapshoot){

	bool bRet=false;
	do{
		CC_BREAK_IF(!CCLayer::init());

		SimpleAudioEngine::sharedEngine()->stopAllEffects();


		CCTexture2D* bgTexture=snapshoot->getSprite()->getTexture();
		CCSprite* bg=CCSprite::createWithTexture(bgTexture);
		bg->setAnchorPoint(ccp(0,0));
		bg->setFlippedY(true);
		this->addChild(bg,0);

		CCLayerColor* blend=CCLayerColor::create(ccc4(0, 0, 0, 150),winSize.width,winSize.height);
		this->addChild(blend,1);


		//忍具界面




		CCLayer* gears_layer=CCLayer::create();

		gears_bg=CCSprite::createWithSpriteFrameName("gears_bg.png");
		gears_bg->setAnchorPoint(ccp(0.5f,0.5f));
		gears_bg->setPosition(ccp(winSize.width/2,winSize.height/2-12));
		gears_layer->addChild(gears_bg,1);

		coinLabel=CCLabelBMFont::create("0","Fonts/arial.fnt");
		coinLabel->setAnchorPoint(ccp(0,0));
		coinLabel->setPosition(ccp(gears_bg->getPositionX()+2,58));
		gears_layer->addChild(coinLabel,2);


		this->addChild(gears_layer,10);



		CCClippingNode* clipper = CCClippingNode::create();
		CCNode *stencil = CCSprite::createWithSpriteFrameName("screwMask.png");
		stencil->setAnchorPoint(ccp(0,0));
		clipper->setStencil(stencil);
		clipper->setContentSize(stencil->getContentSize());
		_listClip=clipper;

		_screwLayer=ScrewLayer::create();
		_screwLayer->setAnchorPoint(ccp(0,0));
		_screwLayer->setPositionY(kListTop);
		_screwLayer->gearNum=None;
		_screwLayer->totalRow=(None+2)/3;


		_screwLayer->screwBar=CCSprite::createWithSpriteFrameName("screwBar.png");
		_screwLayer->screwBar->setAnchorPoint(ccp(0.5f,0));
		_screwLayer->screwBar->setPosition(ccp(gears_bg->getPositionX()+25,122));
		this->addChild(_screwLayer->screwBar,600);


		_gearTexts=ax::FileUtils::getInstance()->getValueMapFromFile("Element/gears.xml");
		float detailX=gears_bg->getPositionX()+gears_bg->getContentSize().width/2-54;
		_gearName=detailLabel("Fonts/PixelifySans-Bold.ttf",8,ax::TextHAlignment::CENTER);
		_gearName->setTextColor(ax::Color4B(255,214,96,255));
		_gearName->setPosition(ccp(detailX,210));
		this->addChild(_gearName,600);
		_gearDesc=detailLabel("Fonts/PixelifySans-Regular.ttf",7,ax::TextHAlignment::LEFT);
		_gearDesc->setPositionX(detailX);
		this->addChild(_gearDesc,600);
		_gearDescEn=detailLabel("Fonts/PixelifySans-Regular.ttf",6,ax::TextHAlignment::LEFT);
		_gearDescEn->setTextColor(ax::Color4B(170,170,180,255));
		_gearDescEn->setPositionX(detailX);
		this->addChild(_gearDescEn,600);
		_gearInfo=detailLabel("Fonts/PixelifySans-Regular.ttf",7,ax::TextHAlignment::CENTER);
		_gearInfo->setTextColor(ax::Color4B(255,214,96,255));
		_gearInfo->setPosition(ccp(detailX,88));
		this->addChild(_gearInfo,600);




		CCMenuItem* buy_btn=CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("gearBuy_btn.png"),
			CCSprite::createWithSpriteFrameName("gearBuy_btn2.png"),this,menu_selector(GearLayer::onGearBuy));
		_buyMenu=CCMenu::create(buy_btn,NULL);
		_buyMenu->setPosition(ccp(gears_bg->getPositionX()+78,65));
		this->addChild(_buyMenu,600);

		_upgradeItem=textButton(ax::Color4B(36,110,60,255),[this](ax::Object* sender){ onGearUpgrade(sender); });
		_upgradeItem->setPosition(ccp(0,10));
		_sellItem=textButton(ax::Color4B(130,40,40,255),[this](ax::Object* sender){ onGearSell(sender); });
		_sellItem->setPosition(ccp(0,-9));
		_ownedMenu=CCMenu::create(_upgradeItem,_sellItem,NULL);
		_ownedMenu->setPosition(ccp(detailX,68));
		_ownedMenu->setVisible(false);
		this->addChild(_ownedMenu,600);

		clipper->setPosition(ccp(gears_bg->getPositionX()-gears_bg->getContentSize().width/2+4,85));
		clipper->addChild(_screwLayer);
		this->addChild(clipper,600);



		CCMenuItem* btm_btn=CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("close_btn1.png"),
			CCSprite::createWithSpriteFrameName("close_btn2.png"),this,menu_selector(GearLayer::onResume));
		CCMenu* overMenu=CCMenu::create(btm_btn,NULL);
		overMenu->setPosition(ccp(winSize.width/2+gears_bg->getContentSize().width/2-12,winSize.height/2+gears_bg->getContentSize().height/2-20));
		this->addChild(overMenu,600);





		bRet=true;
	}while(0);

	return bRet;

}



void GearLayer::onResume(CCObject* sender){

	this->getDelegate()->getHudLayer()->updateGears();
	CCDirector::sharedDirector()->popScene();

};


static void playConfirm(){
	if(CCUserDefault::sharedUserDefault()->getBoolForKey("isVoice")!=false){
		SimpleAudioEngine::sharedEngine()->playEffect("Audio/Menu/confirm.ogg");
	}
}

void GearLayer::onGearBuy(CCObject* sender){

	playConfirm();
	if( _delegate->currentPlayer->setGear(currentGear)){
		this->updatePlayerGear();
	}

};

void GearLayer::onGearUpgrade(CCObject* sender){

	playConfirm();
	if(_delegate->currentPlayer->upgradeGear(currentGear)){
		this->updatePlayerGear();
	}
}

void GearLayer::onGearSell(CCObject* sender){

	playConfirm();
	if(_delegate->currentPlayer->sellGear(currentGear)){
		this->updatePlayerGear();
	}
}

void GearLayer::updatePlayerGear(){

	if(currentGear_layer!=NULL){
		currentGear_layer->removeFromParentAndCleanup(true);
		currentGear_layer=NULL;
	}

	if(_delegate->currentPlayer->getGearArray() && _delegate->currentPlayer->getGearArray()->count()>0){

		currentGear_layer=CCLayer::create();
		currentGear_layer->setAnchorPoint(ccp(0,0));
		CCObject* pObject;
		int i=0;
		CCARRAY_FOREACH(_delegate->currentPlayer->getGearArray(),pObject){
			CCString* tmpGear=(CCString*) pObject;
			GearButton* btn=GearButton::create("");
			btn->setBtnType(gearType(atoi(tmpGear->getCString())),gearbtnType(1),0);
			btn->setPosition(ccp(13+i*34-15,-15));
			btn->setDelegate(this);
			currentGear_layer->addChild(btn);
			i++;
		}
		currentGear_layer->setPosition(ccp(gears_bg->getPositionX()-gears_bg->getContentSize().width/2+8,64));
		this->addChild(currentGear_layer,800);
	}


	coinLabel->setString(CCString::createWithFormat("%d",_delegate->currentPlayer->getCoin())->getCString());
	this->updateGearList();

};

bool GearLayer::isInGearList(CCTouch* touch){
	CCSize size=_listClip->getContentSize();
	return CCRect(0,0,size.width,size.height).containsPoint(_listClip->convertTouchToNodeSpace(touch));
}

// {v} and {w} in the description become the values for Lv1/Lv2/Lv3, e.g. "160/240/320"
static std::string fillGearValues(std::string desc,gearType type){
	const char* keys[]={"{v}","{w}"};
	for(int which=0;which<2;which++){
		size_t pos=desc.find(keys[which]);
		if(pos==std::string::npos){
			continue;
		}
		std::string values;
		for(int lv=1;lv<=kMaxGearLevel;lv++){
			values+=(lv>1 ? "/" : "")+std::to_string(ActionManager::gearValue(type,lv,which));
		}
		desc.replace(pos,3,values);
	}
	return desc;
}

static void setButton(ax::MenuItemLabel* item,const std::string& text,bool enabled){
	item->setString(text);
	item->setEnabled(enabled);
	item->getLabel()->getChildByName("bg")->setOpacity(enabled ? 255 : 90);
}

void GearLayer::showGearDetail(gearType type){

	if(type>=None){
		return;
	}
	Hero* player=_delegate->currentPlayer;
	int level=player->gearLevel(type);

	std::string id=ax::StringUtils::format("gear%02d",int(type));
	std::string name=id,desc,descEn;
	auto it=_gearTexts.find(id);
	if(it!=_gearTexts.end() && it->second.getType()==ax::Value::Type::MAP){
		ax::ValueMap& entry=it->second.asValueMap();
		if(entry.count("name")) name=entry["name"].asString();
		if(entry.count("desc")) desc=fillGearValues(entry["desc"].asString(),type);
		if(entry.count("desc_en")) descEn=fillGearValues(entry["desc_en"].asString(),type);
	}
	if(level){
		name+=ax::StringUtils::format(" Lv%d",level);
	}
	_gearName->setString(name);
	_gearDesc->setString(desc);
	_gearDesc->setPositionY(_gearName->getPositionY()-_gearName->getContentSize().height/kTextScale-4);
	_gearDescEn->setString(descEn);
	_gearDescEn->setPositionY(_gearDesc->getPositionY()-_gearDesc->getContentSize().height/kTextScale-4);

	_buyMenu->setVisible(!level);
	_ownedMenu->setVisible(level>0);
	if(!level){
		bool full=player->getGearArray()->count()>=3;
		_gearInfo->setString(full ? "Slot penuh (maks 3)" : ax::StringUtils::format("Harga %d",ActionManager::gearCost(type)));
		_gearInfo->setPositionY(88);
	}else{
		_gearInfo->setString("");
		if(level<kMaxGearLevel){
			int cost=ActionManager::gearUpgradeCost(type,level);
			setButton(_upgradeItem,ax::StringUtils::format("UPGRADE %d",cost),player->getCoin()>=cost);
		}else{
			setButton(_upgradeItem,"LEVEL MAKS",false);
		}
		setButton(_sellItem,ax::StringUtils::format("JUAL +%d",ActionManager::gearSellValue(type,level)),true);
	}
}

void GearLayer::updateGearList(){

		if(!_screwLayer->getGearArray()){
			_screwLayer->setGearArray(CCArray::create());
		}else{

			CCObject* pObject;
			CCARRAY_FOREACH(_screwLayer->getGearArray(),pObject){
				GearButton* btn=(GearButton*) pObject;
				btn->removeFromParentAndCleanup(true);
			}
			_screwLayer->getGearArray()->removeAllObjects();
		}
		Hero* player=_delegate->currentPlayer;
		for(int i=0;i<_screwLayer->gearNum;i++){
			int row=i/3;
			int column=i%3;

			GearButton* btn;
			if(ActionManager::gearCost(gearType(i))>500){
				btn=GearButton::create("value_1000.png");
			}else{
				btn=GearButton::create("value_500.png");
			}
			int level=player->gearLevel(gearType(i));

			// first opening: show the first gear the player does not own yet
			if(currentGear==None && !level){
				currentGear=gearType(i);
			}

			btn->setPosition(ccp(6+column*46,-row*kRowHeight));
			btn->setAnchorPoint(ccp(0,0));
			btn->setBtnType(gearType(i),gearbtnType(0),level);
			btn->setDelegate(this);
			_screwLayer->getGearArray()->addObject(btn);
			_screwLayer->addChild(btn);

		}
		showGearDetail(currentGear);


}


GearLayer*  GearLayer::create(CCRenderTexture* snapshoot){
	GearLayer* grl=new  GearLayer();
	if (grl && grl->init(snapshoot))
	{
		grl->autorelease();
		return grl;
	}
	else
	{
		delete grl;
		return NULL;
	}
}
