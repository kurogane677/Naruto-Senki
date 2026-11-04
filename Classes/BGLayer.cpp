#include "BGLayer.h"
#include "Maps.h"


BGLayer::BGLayer(void)
{
	texMap=NULL;
}



BGLayer::~BGLayer(void)
{
}


bool BGLayer::init(){
	bool bRet=false;
	do{

		CC_BREAK_IF(!CCLayer::init());

		bRet=true;

	}while(0);

	return bRet;

}

void BGLayer::initBg(){

	bgMap=CCSprite::create(maps::background(_delegate->randomMap+1).c_str());

	

	bgMap->setAnchorPoint(ccp(0,0));
	bgMap->setPosition(ccp(0,192));

	this->addChild(bgMap);

}

void BGLayer::setDelegate(GameLayer* layer){
	_delegate=layer;
	this->initBg();
}
