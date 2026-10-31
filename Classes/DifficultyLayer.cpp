#include "DifficultyLayer.h"
#include "LoadLayer.h"
#include "NetworkLayer.h"
#include "SelectLayer.h"
using namespace CocosDenshion;

namespace
{
const char* kNames[] = {"EASY", "NORMAL", "HARD"};
const char* kDescriptions[] = {
    "Bots react slowly, hesitate and rarely use their skills.",
    "Bots fight as a team, focus targets and use combos.",
    "Bots react fast, dodge ougis, combo every opening\nand copy your best gear builds.",
};
}  // namespace

DifficultyLayer::DifficultyLayer()
    : _heroes(nullptr), _hardCore(false), _randomChar(false), _started(false), _level(1), _marker(nullptr),
      _desc(nullptr)
{
    _items[0] = _items[1] = _items[2] = nullptr;
}

DifficultyLayer::~DifficultyLayer()
{
    CC_SAFE_RELEASE(_heroes);
}

void DifficultyLayer::show(CCArray* heroes, bool hardCore, bool randomChar)
{
    CCScene* scene = CCScene::create();
    DifficultyLayer* layer = DifficultyLayer::create();
    heroes->retain();
    layer->_heroes = heroes;
    layer->_hardCore = hardCore;
    layer->_randomChar = randomChar;
    scene->addChild(layer);
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.8f, scene));
}

void DifficultyLayer::startLoad(CCArray* heroes, bool hardCore, bool randomChar)
{
    CCScene* loadScene = CCScene::create();
    LoadLayer* loadLayer = LoadLayer::create();
    loadLayer->tempHeros = heroes;
    loadScene->addChild(loadLayer);
    loadLayer->_isHardCoreMode = hardCore;
    loadLayer->_isRandomChar = randomChar;
    loadLayer->preloadAudio();
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.5f, loadScene));
}

bool DifficultyLayer::init()
{
    if (!CCLayer::init())
        return false;

    // same backdrop as the hero select screens
    CCSprite* bg = CCSprite::create("red_bg.png");
    bg->setAnchorPoint(ccp(0, 0));
    this->addChild(bg, -5);

    CCSprite* goldLeft = CCSprite::createWithSpriteFrameName("gold_left.png");
    goldLeft->setAnchorPoint(ccp(0, 0));
    goldLeft->setPosition(ccp(0, 20));
    this->addChild(goldLeft, 1);

    CCSprite* goldRight = CCSprite::createWithSpriteFrameName("gold_right.png");
    goldRight->setAnchorPoint(ccp(0, 1));
    goldRight->setPosition(ccp(winSize.width - goldRight->getContentSize().width - 20, winSize.height - 20));
    this->addChild(goldRight, 1);

    CCSprite* barBottom = CCSprite::create("menu_bar2.png");
    barBottom->setAnchorPoint(ccp(0, 0));
    this->addChild(barBottom, 2);

    CCSprite* barTop = CCSprite::create("menu_bar3.png");
    barTop->setAnchorPoint(ccp(0, 0));
    barTop->setPosition(ccp(0, winSize.height - barTop->getContentSize().height));
    this->addChild(barTop, 2);

    CCLabelBMFont* title = CCLabelBMFont::create("AI DIFFICULTY", "Fonts/1.fnt");
    title->setScale(0.6f);
    title->setPosition(ccp(winSize.width / 2, winSize.height - 70));
    this->addChild(title, 5);

    CCMenu* levels = CCMenu::create();
    for (int i = 0; i < 3; i++)
    {
        CCLabelBMFont* label = CCLabelBMFont::create(kNames[i], "Fonts/1.fnt");
        CCMenuItemLabel* item = CCMenuItemLabel::create(label, this, menu_selector(DifficultyLayer::onLevel));
        item->setTag(i);
        item->setPosition(ccp((i - 1) * 150, 0));
        levels->addChild(item);
        _items[i] = item;
    }
    levels->setPosition(ccp(winSize.width / 2, winSize.height / 2 + 30));
    this->addChild(levels, 5);

    _marker = CCSprite::createWithSpriteFrameName("1p.png");
    _marker->runAction(CCRepeatForever::create(CCBlink::create(0.6f, 1)));
    this->addChild(_marker, 5);

    _desc = CCLabelTTF::create("", FONT_TYPE, 12);
    _desc->setPosition(ccp(winSize.width / 2, winSize.height / 2 - 30));
    this->addChild(_desc, 5);

    CCMenuItem* start = CCMenuItemSprite::create(CCSprite::createWithSpriteFrameName("start_btn.png"), NULL, NULL, this,
                                                 menu_selector(DifficultyLayer::onStart));
    CCMenu* startMenu = CCMenu::create(start, NULL);
    startMenu->setPosition(winSize.width - 40, 36);
    this->addChild(startMenu, 500);

    CCLabelBMFont* backLabel = CCLabelBMFont::create("BACK", "Fonts/1.fnt");
    backLabel->setScale(0.5f);
    CCMenuItemLabel* back = CCMenuItemLabel::create(backLabel, this, menu_selector(DifficultyLayer::onBack));
    CCMenu* backMenu = CCMenu::create(back, NULL);
    backMenu->setPosition(48, 36);
    this->addChild(backMenu, 500);

    this->setKeypadEnabled(true);
    select(CCUserDefault::sharedUserDefault()->getIntegerForKey("aiDifficulty", 1));
    return true;
}

void DifficultyLayer::select(int level)
{
    _level = MAX(0, MIN(2, level));
    for (int i = 0; i < 3; i++)
    {
        _items[i]->setScale(i == _level ? 0.75f : 0.55f);
        _items[i]->setColor(i == _level ? ccc3(255, 214, 64) : ccc3(200, 200, 200));
    }
    CCPoint p = _items[_level]->getParent()->convertToWorldSpace(_items[_level]->getPosition());
    _marker->setPosition(ccp(p.x, p.y - 26));
    _desc->setString(kDescriptions[_level]);
}

void DifficultyLayer::onLevel(CCObject* sender)
{
    SimpleAudioEngine::sharedEngine()->playEffect("Audio/Menu/select.mp3");
    select(((CCNode*)sender)->getTag());
}

void DifficultyLayer::onStart(CCObject* sender)
{
    if (_started)
        return;
    _started = true;
    SimpleAudioEngine::sharedEngine()->playEffect("Audio/Menu/confirm.ogg");
    CCUserDefault::sharedUserDefault()->setIntegerForKey("aiDifficulty", _level);
    CCUserDefault::sharedUserDefault()->flush();
    startLoad(_heroes, _hardCore, _randomChar);
}

void DifficultyLayer::onBack(CCObject* sender)
{
    keyBackClicked();
}

void DifficultyLayer::keyBackClicked()
{
    if (_started)
        return;
    _started = true;
    SimpleAudioEngine::sharedEngine()->playEffect("Audio/Menu/cancel.ogg");
    CCScene* scene = CCScene::create();
    if (_hardCore)
        scene->addChild(NetworkLayer::create(true));
    else
        scene->addChild(SelectLayer::create());
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.8f, scene));
}
