/****************************************************************************
 Compat2x.h - temporary cocos2d-x 2.2.2 -> Axmol 2.x compatibility layer.

 Lets the legacy game code (written against cocos2d-x 2.2.2) compile on Axmol
 with as few call-site edits as possible. Everything here is a migration
 crutch: remove it piece by piece in roadmap step M6.5 ("hapus shim").

 Rules:
   - Never `using namespace ax;` here: the game has its own global class
     named ActionManager, which would clash with ax::ActionManager.
   - Types that only changed name are plain aliases.
   - Types that Axmol removed (CCString, CCArray, CCDictionary,
     CCNotificationCenter, SimpleAudioEngine) are re-implemented with the
     2.x semantics the game relies on.
   - Factories that took (target, selector) are small subclasses that wrap
     the selector in a lambda.
 ****************************************************************************/
#pragma once

#include "axmol.h"
#include "audio/AudioEngine.h"
#include "network/HttpClient.h"
#include "ui/UIEditBox/UIEditBox.h"
#include "ui/UIScale9Sprite.h"
#include "rapidjson/document.h"

#include <cstdarg>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

/* ---------------------------------------------------------------------------
 * NULL as nullptr. MSVC defines NULL as int 0; the game ends every variadic
 * list (CCSequence::create(a, b, NULL), CCMenu::create(...), CCArray::create)
 * with NULL, and on x64 an int vararg is not guaranteed to fill the 8-byte
 * pointer slot read by va_arg. Must come after all system/engine headers.
 * ------------------------------------------------------------------------- */
#undef NULL
#define NULL nullptr

/* ---------------------------------------------------------------------------
 * Namespaces / platform
 * ------------------------------------------------------------------------- */
#define USING_NS_CC
#define USING_NS_CC_EXT
#define NS_CC_BEGIN
#define NS_CC_END
#define NS_CC_EXT_BEGIN
#define NS_CC_EXT_END

#define CC_TARGET_PLATFORM  AX_TARGET_PLATFORM
#define CC_PLATFORM_ANDROID AX_PLATFORM_ANDROID
#define CC_PLATFORM_WIN32   AX_PLATFORM_WIN32
#define CC_PLATFORM_IOS     AX_PLATFORM_IOS
#define CC_PLATFORM_MAC     AX_PLATFORM_MAC
#define CC_PLATFORM_LINUX   AX_PLATFORM_LINUX

/* ---------------------------------------------------------------------------
 * Property / memory macros (2.x semantics: members are *protected*)
 * ------------------------------------------------------------------------- */
#define CC_SYNTHESIZE(varType, varName, funName)          \
protected:                                                \
    varType varName;                                      \
public:                                                   \
    virtual varType get##funName(void) const { return varName; } \
public:                                                   \
    virtual void set##funName(varType var) { varName = var; }

#define CC_SYNTHESIZE_READONLY(varType, varName, funName) \
protected:                                                \
    varType varName;                                      \
public:                                                   \
    virtual varType get##funName(void) const { return varName; }

#define CC_SYNTHESIZE_RETAIN(varType, varName, funName)   \
protected:                                                \
    varType varName;                                      \
public:                                                   \
    virtual varType get##funName(void) const { return varName; } \
public:                                                   \
    virtual void set##funName(varType var)                \
    {                                                     \
        if (varName != var)                               \
        {                                                 \
            CC_SAFE_RETAIN(var);                          \
            CC_SAFE_RELEASE(varName);                     \
            varName = var;                                \
        }                                                 \
    }

#define CC_PROPERTY                 AX_PROPERTY
#define CC_SAFE_RETAIN              AX_SAFE_RETAIN
#define CC_SAFE_RELEASE             AX_SAFE_RELEASE
#define CC_SAFE_RELEASE_NULL        AX_SAFE_RELEASE_NULL
#define CC_SAFE_DELETE              AX_SAFE_DELETE
#define CC_SAFE_DELETE_ARRAY        AX_SAFE_DELETE_ARRAY
#define CC_BREAK_IF                 AX_BREAK_IF
#define CC_DEGREES_TO_RADIANS       AX_DEGREES_TO_RADIANS
#define CC_RADIANS_TO_DEGREES       AX_RADIANS_TO_DEGREES
#define CCRANDOM_0_1                AXRANDOM_0_1
#define CCAssert(cond, msg)         AXASSERT(cond, msg)
#define CC_ASSERT(cond)             AX_ASSERT(cond)
#define CC_CALLBACK_0               AX_CALLBACK_0
#define CC_CALLBACK_1               AX_CALLBACK_1
#define CC_CALLBACK_2               AX_CALLBACK_2
#define CC_INVALID_INDEX            0xffffffff

/* ---------------------------------------------------------------------------
 * Selectors (SEL_* typedefs still exist in Axmol)
 * ------------------------------------------------------------------------- */
#define schedule_selector(_SEL)    AX_SCHEDULE_SELECTOR(_SEL)
#define callfunc_selector(_SEL)    AX_CALLFUNC_SELECTOR(_SEL)
#define callfuncN_selector(_SEL)   AX_CALLFUNCN_SELECTOR(_SEL)
#define callfuncND_selector(_SEL)  AX_CALLFUNCND_SELECTOR(_SEL)
#define callfuncO_selector(_SEL)   AX_CALLFUNCO_SELECTOR(_SEL)
#define menu_selector(_SEL)        AX_MENU_SELECTOR(_SEL)

/* ---------------------------------------------------------------------------
 * Singleton accessors: Xxx::sharedXxx() -> Xxx::getInstance()
 * (token macros; only valid because the aliased class names keep working)
 * ------------------------------------------------------------------------- */
#define sharedDirector          getInstance
#define sharedSpriteFrameCache  getInstance
#define sharedUserDefault       getInstance
#define sharedFileUtils         getInstance
#define sharedAnimationCache    getInstance
#define sharedApplication       getInstance
#define purgeSharedAnimationCache   destroyInstance
#define purgeSharedSpriteFrameCache destroyInstance

/* Renamed member functions (token macros) */
#define unscheduleAllSelectors          unscheduleAllCallbacks
#define numberOfRunningActionsInTarget  getNumberOfRunningActionsInTarget

/* ---------------------------------------------------------------------------
 * Plain type aliases
 * ------------------------------------------------------------------------- */
using CCObject            = ax::Object;
using CCNode              = ax::Node;
using CCSprite            = ax::Sprite;
using CCScene             = ax::Scene;
using CCMenu              = ax::Menu;
using CCMenuItem          = ax::MenuItem;
using CCAction            = ax::Action;
using CCActionInterval    = ax::ActionInterval;
using CCActionInstant     = ax::ActionInstant;
using CCFiniteTimeAction  = ax::FiniteTimeAction;
using CCRepeat            = ax::Repeat;
using CCRepeatForever     = ax::RepeatForever;
using CCDelayTime         = ax::DelayTime;
using CCFadeIn            = ax::FadeIn;
using CCFadeOut           = ax::FadeOut;
using CCFadeTo            = ax::FadeTo;
using CCTintTo            = ax::TintTo;
using CCMoveBy            = ax::MoveBy;
using CCMoveTo            = ax::MoveTo;
using CCRotateBy          = ax::RotateBy;
using CCRotateTo          = ax::RotateTo;
using CCScaleBy           = ax::ScaleBy;
using CCScaleTo           = ax::ScaleTo;
using CCJumpBy            = ax::JumpBy;
using CCJumpTo            = ax::JumpTo;
using CCBlink             = ax::Blink;
using CCEaseIn            = ax::EaseIn;
using CCEaseOut           = ax::EaseOut;
using CCAnimate           = ax::Animate;
using CCAnimation         = ax::Animation;
using CCAnimationCache    = ax::AnimationCache;
using CCSpriteFrame       = ax::SpriteFrame;
using CCSpriteFrameCache  = ax::SpriteFrameCache;
using CCSpriteBatchNode   = ax::SpriteBatchNode;
using CCTexture2D         = ax::Texture2D;
using CCTextureCache      = ax::TextureCache;
using CCDirector          = ax::Director;
using CCScheduler         = ax::Scheduler;
using CCApplication       = ax::Application;
using CCTransitionFade    = ax::TransitionFade;
using CCRenderTexture     = ax::RenderTexture;
using CCClippingNode      = ax::ClippingNode;
using CCProgressTimer     = ax::ProgressTimer;
using CCProgressTo        = ax::ProgressTo;
using CCLayerColor        = ax::LayerColor;
using CCTMXTiledMap       = ax::FastTMXTiledMap;
using CCTMXObjectGroup    = ax::TMXObjectGroup;
using CCUserDefault       = ax::UserDefault;
using CCFileUtils         = ax::FileUtils;
using CCTouch             = ax::Touch;
using CCEvent             = ax::Event;
using CCPoint             = ax::Vec2;
using CCSize              = ax::Size;
using CCRect              = ax::Rect;
using ccColor3B           = ax::Color3B;
using ccColor4B           = ax::Color4B;
using ccBlendFunc         = ax::BlendFunc;
using CCScale9Sprite      = ax::ui::Scale9Sprite;
using CCEditBox           = ax::ui::EditBox;
using CCEditBoxDelegate   = ax::ui::EditBoxDelegate;
using CCHttpClient        = ax::network::HttpClient;
using CCHttpResponse      = ax::network::HttpResponse;

/* ---------------------------------------------------------------------------
 * Geometry / colour helpers
 * ------------------------------------------------------------------------- */
inline ax::Vec2 ccp(float x, float y) { return ax::Vec2(x, y); }
inline ax::Vec2 ccpAdd(const ax::Vec2& a, const ax::Vec2& b) { return a + b; }
inline ax::Vec2 ccpSub(const ax::Vec2& a, const ax::Vec2& b) { return a - b; }
inline ax::Vec2 ccpMult(const ax::Vec2& a, float s) { return a * s; }
inline ax::Vec2 ccpNormalize(const ax::Vec2& a) { return a.getNormalized(); }
inline float ccpDistance(const ax::Vec2& a, const ax::Vec2& b) { return a.distance(b); }
inline float ccpLength(const ax::Vec2& a) { return a.length(); }
inline float ccpToAngle(const ax::Vec2& a) { return a.getAngle(); }
#define CCPointMake(x, y)       ax::Vec2((float)(x), (float)(y))
#define CCPointZero             ax::Vec2::ZERO
#define CCSizeMake(w, h)        ax::Size((float)(w), (float)(h))
#define CCSizeZero              ax::Size::ZERO
#define CCRectMake(x, y, w, h)  ax::Rect((float)(x), (float)(y), (float)(w), (float)(h))
#define CCRectZero              ax::Rect::ZERO

inline ax::Color3B ccc3(uint8_t r, uint8_t g, uint8_t b) { return ax::Color3B(r, g, b); }
inline ax::Color4B ccc4(uint8_t r, uint8_t g, uint8_t b, uint8_t a) { return ax::Color4B(r, g, b, a); }
#define ccWHITE   ax::Color3B::WHITE
#define ccBLACK   ax::Color3B::BLACK
#define ccRED     ax::Color3B::RED
#define ccGREEN   ax::Color3B::GREEN
#define ccBLUE    ax::Color3B::BLUE
#define ccYELLOW  ax::Color3B::YELLOW
#define ccGRAY    ax::Color3B::GRAY
#define ccORANGE  ax::Color3B::ORANGE

#define kCCProgressTimerTypeRadial ax::ProgressTimer::Type::RADIAL
#define kCCProgressTimerTypeBar    ax::ProgressTimer::Type::BAR

#define kEditBoxInputModeAny          ax::ui::EditBox::InputMode::ANY
#define kEditBoxInputModeNumeric      ax::ui::EditBox::InputMode::NUMERIC
#define kEditBoxInputModeSingleLine   ax::ui::EditBox::InputMode::SINGLE_LINE
#define kEditBoxInputModeEmailAddr    ax::ui::EditBox::InputMode::EMAIL_ADDRESS
#define kEditBoxInputFlagPassword     ax::ui::EditBox::InputFlag::PASSWORD
#define kEditBoxInputFlagSensitive    ax::ui::EditBox::InputFlag::SENSITIVE
#define kKeyboardReturnTypeDefault    ax::ui::EditBox::KeyboardReturnType::DEFAULT
#define kKeyboardReturnTypeDone       ax::ui::EditBox::KeyboardReturnType::DONE

/* ---------------------------------------------------------------------------
 * Logging (printf-style, like 2.x; Axmol's AXLOG is fmt-style)
 * ------------------------------------------------------------------------- */
void CCLog(const char* format, ...);
#define CCLOG(...)      CCLog(__VA_ARGS__)
#define CCLOGINFO(...)  CCLog(__VA_ARGS__)
#define CCLOGERROR(...) CCLog(__VA_ARGS__)
#define CCLOGWARN(...)  CCLog(__VA_ARGS__)

/* ---------------------------------------------------------------------------
 * Time
 * ------------------------------------------------------------------------- */
struct cc_timeval
{
    long tv_sec;
    long tv_usec;
};

class CCTime
{
public:
    static int gettimeofdayCocos2d(cc_timeval* tp, void* tzp);
    static double timersubCocos2d(cc_timeval* start, cc_timeval* end);
};

/* ---------------------------------------------------------------------------
 * Snapshot helper: 2.x code rendered nodes into a CCRenderTexture by calling
 * node->visit() between begin()/end() at arbitrary times (touch handlers,
 * onEnter). Axmol nodes such as FastTMXLayer need Camera::getVisitingCamera(),
 * which is only set while a scene renders. This visits `node` with the running
 * scene's default camera as the visiting camera.
 * ------------------------------------------------------------------------- */
void ccVisitForSnapshot(ax::Node* node);

/* ---------------------------------------------------------------------------
 * File helper: replaces CCFileUtils::getFileData(path, mode, &size).
 * Returns a new[]-allocated buffer with a trailing '\0' (not counted in size),
 * or nullptr. Caller frees with delete[].
 * ------------------------------------------------------------------------- */
unsigned char* ccGetFileData(const char* path, const char* mode, unsigned long* size);

/* ---------------------------------------------------------------------------
 * CCString
 * ------------------------------------------------------------------------- */
class CCString : public ax::Object
{
public:
    CCString() = default;
    explicit CCString(std::string_view s) : m_sString(s) {}

    static CCString* create(const char* str);
    static CCString* create(const std::string& str);
    static CCString* createWithFormat(const char* format, ...);
    static CCString* createWithData(const unsigned char* data, unsigned long length);

    const char* getCString() const { return m_sString.c_str(); }
    unsigned int length() const { return (unsigned int)m_sString.length(); }
    int intValue() const;
    unsigned int uintValue() const;
    float floatValue() const;
    double doubleValue() const;
    bool boolValue() const;
    int compare(const char* str) const { return strcmp(getCString(), str); }
    bool isEqual(const ax::Object* obj) const;

    std::string m_sString;
};

/* ---------------------------------------------------------------------------
 * CCArray (retains its elements, like 2.x)
 * ------------------------------------------------------------------------- */
class CCArray : public ax::Object
{
public:
    ~CCArray() override;

    static CCArray* create();
    static CCArray* create(ax::Object* first, ...);
    static CCArray* createWithObject(ax::Object* obj);
    static CCArray* createWithCapacity(unsigned int capacity);
    static CCArray* createWithArray(CCArray* other);
    template <class T>
    static CCArray* createWithVector(const ax::Vector<T>& vec)
    {
        auto arr = create();
        for (auto* obj : vec)
            arr->addObject(obj);
        return arr;
    }

    unsigned int count() const { return (unsigned int)_data.size(); }
    unsigned int capacity() const { return (unsigned int)_data.capacity(); }
    ax::Object* objectAtIndex(unsigned int index) const;
    ax::Object* lastObject() const;
    ax::Object* randomObject() const;
    unsigned int indexOfObject(ax::Object* obj) const;
    bool containsObject(ax::Object* obj) const;

    void addObject(ax::Object* obj);
    void addObjectsFromArray(CCArray* other);
    void insertObject(ax::Object* obj, unsigned int index);
    void removeObject(ax::Object* obj, bool releaseObj = true);
    void removeObjectAtIndex(unsigned int index, bool releaseObj = true);
    void removeLastObject(bool releaseObj = true);
    void removeAllObjects();
    void fastRemoveObject(ax::Object* obj);
    void fastRemoveObjectAtIndex(unsigned int index);
    void exchangeObjectAtIndex(unsigned int a, unsigned int b);
    void reverseObjects();

    template <class T>
    ax::Vector<T> toVector() const
    {
        ax::Vector<T> vec;
        for (auto* obj : _data)
            if (auto t = dynamic_cast<T>(obj))
                vec.pushBack(t);
        return vec;
    }

private:
    std::vector<ax::Object*> _data;
};

// Index-based so that removing elements inside the loop never reads freed memory.
#define CCARRAY_FOREACH(__array__, __object__)                                      \
    for (unsigned int __ccarr_i = 0;                                                \
         (__array__) && __ccarr_i < (__array__)->count() &&                         \
         (((__object__) = (__array__)->objectAtIndex(__ccarr_i)), true);            \
         ++__ccarr_i)

#define CCARRAY_FOREACH_REVERSE(__array__, __object__)                              \
    for (int __ccarr_i = (__array__) ? (int)(__array__)->count() - 1 : -1;          \
         __ccarr_i >= 0 && (((__object__) = (__array__)->objectAtIndex(__ccarr_i)), true); \
         --__ccarr_i)

/* ---------------------------------------------------------------------------
 * CCDictionary (string or int keys, insertion-ordered like 2.x uthash)
 * ------------------------------------------------------------------------- */
class CCDictElement
{
public:
    const char* getStrKey() const { return _strKey.c_str(); }
    intptr_t getIntKey() const { return _intKey; }
    ax::Object* getObject() const { return _object; }

    std::string _strKey;
    intptr_t _intKey = 0;
    bool _isIntKey   = false;
    ax::Object* _object = nullptr;
};

class CCDictionary : public ax::Object
{
public:
    ~CCDictionary() override;

    static CCDictionary* create();
    static CCDictionary* createWithDictionary(CCDictionary* src);
    static CCDictionary* createWithContentsOfFile(const char* path);

    unsigned int count() const { return (unsigned int)_elements.size(); }
    CCArray* allKeys() const;

    void setObject(ax::Object* obj, const std::string& key);
    void setObject(ax::Object* obj, intptr_t key);
    ax::Object* objectForKey(const std::string& key) const;
    ax::Object* objectForKey(intptr_t key) const;
    const CCString* valueForKey(const std::string& key) const;
    const CCString* valueForKey(intptr_t key) const;
    void removeObjectForKey(const std::string& key);
    void removeObjectForKey(intptr_t key);
    void removeAllObjects();

    std::vector<CCDictElement> _elements;

private:
    CCDictElement* find(const std::string& key);
    CCDictElement* find(intptr_t key);
    const CCDictElement* find(const std::string& key) const;
    const CCDictElement* find(intptr_t key) const;
};

// ValueVector/ValueMap (e.g. TMXObjectGroup::getObjects()) -> CCArray of CCDictionary/CCString
CCArray* ccArrayFromValueVector(const ax::ValueVector& values);

#define CCDICT_FOREACH(__dict__, __el__)                                           \
    for (size_t __ccdict_i = 0;                                                     \
         (__dict__) && __ccdict_i < (__dict__)->_elements.size() &&                 \
         (((__el__) = &(__dict__)->_elements[__ccdict_i]), true);                   \
         ++__ccdict_i)

/* ---------------------------------------------------------------------------
 * CCNotificationCenter (2.x semantics, does not retain targets)
 * ------------------------------------------------------------------------- */
class CCNotificationCenter
{
public:
    static CCNotificationCenter* sharedNotificationCenter();

    void addObserver(ax::Object* target, ax::SEL_CallFuncO selector, const char* name, ax::Object* obj);
    void removeObserver(ax::Object* target, const char* name);
    int removeAllObservers(ax::Object* target);
    void postNotification(const char* name);
    void postNotification(const char* name, ax::Object* obj);

private:
    struct Observer
    {
        ax::Object* target;
        ax::SEL_CallFuncO selector;
        std::string name;
        ax::Object* object;
    };
    std::vector<Observer> _observers;
};

/* ---------------------------------------------------------------------------
 * Touch: CCTouchDelegate + targeted dispatcher on top of EventDispatcher
 * ------------------------------------------------------------------------- */
class CCTouchDelegate
{
public:
    virtual ~CCTouchDelegate() = default;
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event) { return false; }
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event) {}
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event) {}
    virtual void ccTouchCancelled(CCTouch* touch, CCEvent* event) {}
};

class CCTouchDispatcher
{
public:
    static CCTouchDispatcher* sharedDispatcher();
    // priority uses 2.x numbering (lower = earlier; menus were -128)
    void addTargetedDelegate(CCTouchDelegate* delegate, int priority, bool swallowsTouches);
    void removeDelegate(CCTouchDelegate* delegate);

private:
    std::unordered_map<CCTouchDelegate*, ax::EventListener*> _listeners;
};

/* ---------------------------------------------------------------------------
 * CCLayer: 2.x layers were touch + keypad delegates
 * ------------------------------------------------------------------------- */
class CCLayer : public ax::Layer, public CCTouchDelegate
{
public:
    CREATE_FUNC(CCLayer);

    void setTouchEnabled(bool enabled) {}  // only targeted delegates are used by the game
    void setKeypadEnabled(bool enabled);
    virtual void keyBackClicked() {}
    virtual void keyMenuClicked() {}

    void onExit() override;

private:
    ax::EventListenerKeyboard* _keypadListener = nullptr;
};

/* ---------------------------------------------------------------------------
 * Sequence / Spawn: 2.x also accepted a CCArray of actions
 * ------------------------------------------------------------------------- */
class CCSequence : public ax::Sequence
{
public:
    static ax::Sequence* create(CCArray* actions);
    static ax::Sequence* create(ax::FiniteTimeAction* action1, ...);
    static ax::Sequence* createWithTwoActions(ax::FiniteTimeAction* one, ax::FiniteTimeAction* two)
    {
        return ax::Sequence::createWithTwoActions(one, two);
    }
};

class CCSpawn : public ax::Spawn
{
public:
    static ax::Spawn* create(CCArray* actions);
    static ax::Spawn* create(ax::FiniteTimeAction* action1, ...);
};

/* ---------------------------------------------------------------------------
 * Actions that used (target, selector)
 * ------------------------------------------------------------------------- */
class CCCallFunc : public ax::CallFunc
{
public:
    static CCCallFunc* create(ax::Object* target, ax::SEL_CallFunc selector);
};

class CCCallFuncN : public ax::CallFuncN
{
public:
    static CCCallFuncN* create(ax::Object* target, ax::SEL_CallFuncN selector);
};

class CCCallFuncND : public ax::CallFuncN
{
public:
    static CCCallFuncND* create(ax::Object* target, ax::SEL_CallFuncND selector, void* data);
};

/* ---------------------------------------------------------------------------
 * Menu items that used (target, selector)
 * ------------------------------------------------------------------------- */
class CCMenuItemSprite : public ax::MenuItemSprite
{
public:
    static CCMenuItemSprite* create(ax::Node* normal, ax::Node* selected, ax::Node* disabled = nullptr);
    static CCMenuItemSprite* create(ax::Node* normal, ax::Node* selected, ax::Object* target, ax::SEL_MenuHandler selector);
    static CCMenuItemSprite* create(ax::Node* normal, ax::Node* selected, ax::Node* disabled,
                                    ax::Object* target, ax::SEL_MenuHandler selector);
    void setTarget(ax::Object* target, ax::SEL_MenuHandler selector);
};

class CCMenuItemLabel : public ax::MenuItemLabel
{
public:
    static CCMenuItemLabel* create(ax::Node* label);
    static CCMenuItemLabel* create(ax::Node* label, ax::Object* target, ax::SEL_MenuHandler selector);
    void setTarget(ax::Object* target, ax::SEL_MenuHandler selector);
};

/* ---------------------------------------------------------------------------
 * Labels
 * ------------------------------------------------------------------------- */
class CCLabelTTF : public ax::Label
{
public:
    static CCLabelTTF* create();
    static CCLabelTTF* create(const char* str, const char* fontName, float fontSize);
    static CCLabelTTF* create(const char* str, const char* fontName, float fontSize,
                              const ax::Size& dimensions, ax::TextHAlignment hAlignment);
    static CCLabelTTF* create(const char* str, const char* fontName, float fontSize,
                              const ax::Size& dimensions, ax::TextHAlignment hAlignment,
                              ax::TextVAlignment vAlignment);
    void setFontName(const char* fontName) { setSystemFontName(fontName); }
    void setFontSize(float size) { setSystemFontSize(size); }
};

class CCLabelBMFont : public ax::Label
{
public:
    static CCLabelBMFont* create();
    static CCLabelBMFont* create(const char* str, const char* fntFile);
    static CCLabelBMFont* create(const char* str, const char* fntFile, float width,
                                 ax::TextHAlignment alignment = ax::TextHAlignment::LEFT);
};

/* ---------------------------------------------------------------------------
 * HTTP (cocos2d::extension::CCHttpRequest)
 * ------------------------------------------------------------------------- */
class CCHttpRequest : public ax::network::HttpRequest
{
public:
    enum HttpRequestType
    {
        kHttpGet,
        kHttpPost,
        kHttpPut,
        kHttpDelete,
        kHttpUnkown,
    };

    using ax::network::HttpRequest::setRequestType;
    void setRequestType(HttpRequestType type);
    void setResponseCallback(ax::Object* target, ax::SEL_CallFuncND selector);
};

/* ---------------------------------------------------------------------------
 * Audio: CocosDenshion::SimpleAudioEngine on top of ax::AudioEngine
 * ------------------------------------------------------------------------- */
namespace CocosDenshion
{
class SimpleAudioEngine
{
public:
    static SimpleAudioEngine* sharedEngine();
    static void end();

    void preloadBackgroundMusic(const char* path);
    void playBackgroundMusic(const char* path, bool loop = false);
    void stopBackgroundMusic(bool releaseData = false);
    void pauseBackgroundMusic();
    void resumeBackgroundMusic();
    void rewindBackgroundMusic();
    bool willPlayBackgroundMusic() { return true; }
    bool isBackgroundMusicPlaying();
    float getBackgroundMusicVolume() { return _musicVolume; }
    void setBackgroundMusicVolume(float volume);
    float getEffectsVolume() { return _effectsVolume; }
    void setEffectsVolume(float volume);

    unsigned int playEffect(const char* path, bool loop = false);
    void pauseEffect(unsigned int soundId);
    void pauseAllEffects();
    void resumeEffect(unsigned int soundId);
    void resumeAllEffects();
    void stopEffect(unsigned int soundId);
    void stopAllEffects();
    void preloadEffect(const char* path);
    void unloadEffect(const char* path);

private:
    void pruneEffects();

    int _musicId         = ax::AudioEngine::INVALID_AUDIO_ID;
    float _musicVolume   = 1.0f;
    float _effectsVolume = 1.0f;
    std::vector<int> _effectIds;
};
}  // namespace CocosDenshion
