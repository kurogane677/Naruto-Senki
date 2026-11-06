#include "CreditsLayer.h"
#include "MyUtils/KTools.h"
using namespace CocosDenshion;
#include "StartMenu.h"

// Credits text (#70) comes from Element/credits.xml: "# Title" is a heading, an empty
// line is a gap. Text is rasterized at kCreditsTextScale times its size to stay sharp.
static const float kCreditsTextScale=4;
static const float kCreditsTextWidth=420;
static const float kCreditsScrollSpeed=18;
static const float kCreditsLineGap=3;
static const float kCreditsBlankHeight=10;

static ax::Label* createCreditsLine(const std::string& line){
	bool isHeading=line.compare(0,2,"# ")==0;
	std::string text=isHeading ? line.substr(2) : line;
	const char* font=isHeading ? "Fonts/Arimo-SemiBold.ttf" : "Fonts/Arimo-Medium.ttf";
	float fontSize=isHeading ? 13 : 11;
	ax::Label* label=ax::Label::createWithTTF(text,font,fontSize*kCreditsTextScale,
		ax::Size(kCreditsTextWidth*kCreditsTextScale,0),ax::TextHAlignment::CENTER);
	label->enableOutline(ax::Color4B(40,40,40,255),kCreditsTextScale);
	if(isHeading){
		label->setTextColor(ax::Color4B(255,204,64,255));
	}
	label->setScale(1/kCreditsTextScale);
	label->setAnchorPoint(ccp(0.5f,1));
	return label;
}




CreditsLayer::CreditsLayer(void)
{

}


CreditsLayer::~CreditsLayer(void)
{

}

bool CreditsLayer::init(){

	bool bRet=false;
	do{
		CC_BREAK_IF(!CCLayer::init());

		CCSprite* bgSprite = CCSprite::create("blue_bg.png");
		KTools::fitScreenWidth(bgSprite);
		bgSprite->setAnchorPoint(ccp(0,0));
		bgSprite->setPosition(ccp(0,0));
		this->addChild(bgSprite, -5);


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
		KTools::extendBarToScreen(menu_bar_b);
		menu_bar_b->setAnchorPoint(ccp(0,0));
		this->addChild(menu_bar_b,2);


		CCSprite* menu_bar_t = CCSprite::create("menu_bar3.png");
		KTools::extendBarToScreen(menu_bar_t);
		menu_bar_t->setAnchorPoint(ccp(0,0));
		menu_bar_t->setPosition(ccp(0,winSize.height-menu_bar_t->getContentSize().height));
		this->addChild(menu_bar_t,2);

		ax::Label* credits_title=KTools::createBarTitle("TitleCredits");
		credits_title->setPosition(ccp(4,winSize.height-credits_title->getBoundingBox().size.height-2));
		this->addChild(credits_title,3);

		// the text scrolls up between the two menu bars, then starts again from the bottom
		float viewBottom=menu_bar_b->getContentSize().height;
		float viewTop=winSize.height-menu_bar_t->getContentSize().height;
		ax::ClippingRectangleNode* creditsView=ax::ClippingRectangleNode::create(
			ax::Rect(0,viewBottom,winSize.width,viewTop-viewBottom));
		this->addChild(creditsView,1);

		ax::Node* creditsText=ax::Node::create();
		float lineY=0;
		for(const ax::Value& line : ax::FileUtils::getInstance()->getValueVectorFromFile("Element/credits.xml")){
			std::string text=line.asString();
			if(text.empty()){
				lineY-=kCreditsBlankHeight;
				continue;
			}
			ax::Label* label=createCreditsLine(text);
			label->setPosition(ccp(0,lineY));
			creditsText->addChild(label);
			lineY-=label->getBoundingBox().size.height+kCreditsLineGap;
		}
		float textHeight=-lineY;
		creditsText->setPosition(ccp(winSize.width/2,viewBottom));
		creditsView->addChild(creditsText);

		creditsText->schedule([creditsText,textHeight,viewBottom,viewTop](float dt){
			float textY=creditsText->getPositionY()+kCreditsScrollSpeed*dt;
			if(textY-textHeight>viewTop){
				textY=viewBottom;
			}
			creditsText->setPositionY(textY);
		},"creditsScroll");
		if(CCUserDefault::sharedUserDefault()->getBoolForKey("isBGM")!=false){
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Audio/Music/credits_music.mp3",true);
		}
		
		if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID){
		this->setKeypadEnabled(true);
		}


		bRet=true;
	}while(0);

	return bRet;

}

void CreditsLayer::keyBackClicked(){

	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	SimpleAudioEngine::sharedEngine()->playEffect("Audio/Menu/cancel.ogg");
	if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID){
		this->setKeypadEnabled(false);
	}
	CCScene* menuScene =CCScene::create();
	CCLayer* menuLayer=StartMenu::create();
	menuScene->addChild(menuLayer);
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.5f,menuScene));

}
