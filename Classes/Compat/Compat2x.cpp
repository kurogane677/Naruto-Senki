/****************************************************************************
 Compat2x.cpp - implementation of the temporary cocos2d-x 2.2.2 shim.
 See Compat2x.h.
 ****************************************************************************/
#include "Compat/Compat2x.h"

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <cstdlib>

/* ---------------------------------------------------------------------------
 * Logging / time / files
 * ------------------------------------------------------------------------- */
static std::string vformat(const char* format, va_list args)
{
    va_list copy;
    va_copy(copy, args);
    int len = vsnprintf(nullptr, 0, format, copy);
    va_end(copy);
    if (len <= 0)
        return std::string();
    std::string out(len, '\0');
    vsnprintf(&out[0], len + 1, format, args);
    return out;
}

void CCLog(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    std::string msg = vformat(format, args);
    va_end(args);
    AXLOGD("{}", msg);
}

int CCTime::gettimeofdayCocos2d(cc_timeval* tp, void* /*tzp*/)
{
    if (!tp)
        return -1;
    auto now = std::chrono::system_clock::now().time_since_epoch();
    auto us  = std::chrono::duration_cast<std::chrono::microseconds>(now).count();
    tp->tv_sec  = (long)(us / 1000000);
    tp->tv_usec = (long)(us % 1000000);
    return 0;
}

double CCTime::timersubCocos2d(cc_timeval* start, cc_timeval* end)
{
    if (!start || !end)
        return 0;
    return ((end->tv_sec * 1000.0 + end->tv_usec / 1000.0) - (start->tv_sec * 1000.0 + start->tv_usec / 1000.0));
}

namespace
{
// Camera::_visitingCamera is a protected static; a derived type may set it.
struct VisitingCameraScope : ax::Camera
{
    explicit VisitingCameraScope(ax::Camera* cam) : _previous(_visitingCamera) { _visitingCamera = cam; }
    ~VisitingCameraScope() { _visitingCamera = _previous; }

private:
    ax::Camera* _previous;
};
}  // namespace

void ccVisitForSnapshot(ax::Node* node)
{
    if (!node)
        return;
    auto scene  = ax::Director::getInstance()->getRunningScene();
    auto camera = scene ? scene->getDefaultCamera() : nullptr;
    if (!camera)
        camera = ax::Camera::getDefaultCamera();
    VisitingCameraScope scope(camera);
    node->visit();
}

void ccEndSnapshot(ax::RenderTexture* rt)
{
    rt->end();
    ax::Director::getInstance()->getRenderer()->render();
}

unsigned char* ccGetFileData(const char* path, const char* /*mode*/, unsigned long* size)
{
    if (size)
        *size = 0;
    if (!path)
        return nullptr;
    ax::Data data = ax::FileUtils::getInstance()->getDataFromFile(path);
    if (data.isNull())
        return nullptr;
    auto len = data.getSize();
    auto buf = new unsigned char[len + 1];
    memcpy(buf, data.getBytes(), len);
    buf[len] = 0;
    if (size)
        *size = (unsigned long)len;
    return buf;
}

/* ---------------------------------------------------------------------------
 * CCString
 * ------------------------------------------------------------------------- */
CCString* CCString::create(const char* str)
{
    auto ret = new CCString(str ? str : "");
    ret->autorelease();
    return ret;
}

CCString* CCString::create(const std::string& str)
{
    auto ret = new CCString(str);
    ret->autorelease();
    return ret;
}

CCString* CCString::createWithFormat(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    std::string s = vformat(format, args);
    va_end(args);
    return create(s);
}

CCString* CCString::createWithData(const unsigned char* data, unsigned long length)
{
    return create(std::string(reinterpret_cast<const char*>(data), length));
}

int CCString::intValue() const
{
    return m_sString.empty() ? 0 : atoi(m_sString.c_str());
}

unsigned int CCString::uintValue() const
{
    return m_sString.empty() ? 0 : (unsigned int)atoi(m_sString.c_str());
}

float CCString::floatValue() const
{
    return m_sString.empty() ? 0.0f : (float)atof(m_sString.c_str());
}

double CCString::doubleValue() const
{
    return m_sString.empty() ? 0.0 : atof(m_sString.c_str());
}

bool CCString::boolValue() const
{
    if (m_sString.empty())
        return false;
    return !(m_sString == "0" || m_sString == "false");
}

bool CCString::isEqual(const ax::Object* obj) const
{
    auto other = dynamic_cast<const CCString*>(obj);
    return other && other->m_sString == m_sString;
}

/* ---------------------------------------------------------------------------
 * CCArray
 * ------------------------------------------------------------------------- */
CCArray::~CCArray()
{
    for (auto* obj : _data)
        obj->release();
}

CCArray* CCArray::create()
{
    auto ret = new CCArray();
    ret->autorelease();
    return ret;
}

CCArray* CCArray::create(ax::Object* first, ...)
{
    auto ret = create();
    if (first)
    {
        ret->addObject(first);
        va_list args;
        va_start(args, first);
        while (auto obj = va_arg(args, ax::Object*))
            ret->addObject(obj);
        va_end(args);
    }
    return ret;
}

CCArray* CCArray::createWithObject(ax::Object* obj)
{
    auto ret = create();
    ret->addObject(obj);
    return ret;
}

CCArray* CCArray::createWithCapacity(unsigned int capacity)
{
    auto ret = create();
    ret->_data.reserve(capacity);
    return ret;
}

CCArray* CCArray::createWithArray(CCArray* other)
{
    auto ret = create();
    ret->addObjectsFromArray(other);
    return ret;
}

ax::Object* CCArray::objectAtIndex(unsigned int index) const
{
    AXASSERT(index < _data.size(), "CCArray: index out of range");
    return index < _data.size() ? _data[index] : nullptr;
}

ax::Object* CCArray::lastObject() const
{
    return _data.empty() ? nullptr : _data.back();
}

ax::Object* CCArray::randomObject() const
{
    if (_data.empty())
        return nullptr;
    return _data[(size_t)(AXRANDOM_0_1() * _data.size()) % _data.size()];
}

unsigned int CCArray::indexOfObject(ax::Object* obj) const
{
    auto it = std::find(_data.begin(), _data.end(), obj);
    return it == _data.end() ? CC_INVALID_INDEX : (unsigned int)(it - _data.begin());
}

bool CCArray::containsObject(ax::Object* obj) const
{
    return indexOfObject(obj) != CC_INVALID_INDEX;
}

void CCArray::addObject(ax::Object* obj)
{
    AXASSERT(obj, "CCArray: cannot add nullptr");
    if (!obj)
        return;
    obj->retain();
    _data.push_back(obj);
}

void CCArray::addObjectsFromArray(CCArray* other)
{
    if (!other)
        return;
    for (auto* obj : other->_data)
        addObject(obj);
}

void CCArray::insertObject(ax::Object* obj, unsigned int index)
{
    if (!obj)
        return;
    obj->retain();
    _data.insert(_data.begin() + std::min<size_t>(index, _data.size()), obj);
}

void CCArray::removeObject(ax::Object* obj, bool releaseObj)
{
    auto it = std::find(_data.begin(), _data.end(), obj);
    if (it == _data.end())
        return;
    _data.erase(it);
    if (releaseObj)
        obj->release();
}

void CCArray::removeObjectAtIndex(unsigned int index, bool releaseObj)
{
    if (index >= _data.size())
        return;
    auto obj = _data[index];
    _data.erase(_data.begin() + index);
    if (releaseObj)
        obj->release();
}

void CCArray::removeLastObject(bool releaseObj)
{
    if (!_data.empty())
        removeObjectAtIndex((unsigned int)_data.size() - 1, releaseObj);
}

void CCArray::removeAllObjects()
{
    auto old = std::move(_data);
    _data.clear();
    for (auto* obj : old)
        obj->release();
}

void CCArray::fastRemoveObject(ax::Object* obj)
{
    removeObject(obj);
}

void CCArray::fastRemoveObjectAtIndex(unsigned int index)
{
    removeObjectAtIndex(index);
}

void CCArray::exchangeObjectAtIndex(unsigned int a, unsigned int b)
{
    if (a < _data.size() && b < _data.size())
        std::swap(_data[a], _data[b]);
}

void CCArray::reverseObjects()
{
    std::reverse(_data.begin(), _data.end());
}

/* ---------------------------------------------------------------------------
 * CCDictionary
 * ------------------------------------------------------------------------- */
CCDictionary::~CCDictionary()
{
    removeAllObjects();
}

CCDictionary* CCDictionary::create()
{
    auto ret = new CCDictionary();
    ret->autorelease();
    return ret;
}

CCDictionary* CCDictionary::createWithDictionary(CCDictionary* src)
{
    auto ret = create();
    if (src)
    {
        for (auto& el : src->_elements)
        {
            if (el._isIntKey)
                ret->setObject(el._object, el._intKey);
            else
                ret->setObject(el._object, el._strKey);
        }
    }
    return ret;
}

static ax::Object* valueToObject(const ax::Value& value);

static CCDictionary* valueMapToDictionary(const ax::ValueMap& map)
{
    auto dict = CCDictionary::create();
    for (auto& [key, value] : map)
    {
        if (auto obj = valueToObject(value))
            dict->setObject(obj, std::string(key));
    }
    return dict;
}

static ax::Object* valueToObject(const ax::Value& value)
{
    switch (value.getTypeFamily())
    {
    case ax::Value::Type::MAP:
        return valueMapToDictionary(value.asValueMap());
    case ax::Value::Type::VECTOR:
    {
        auto arr = CCArray::create();
        for (auto& v : value.asValueVector())
            if (auto obj = valueToObject(v))
                arr->addObject(obj);
        return arr;
    }
    case ax::Value::Type::NONE:
        return nullptr;
    default:
        return CCString::create(value.asString());
    }
}

CCArray* ccArrayFromValueVector(const ax::ValueVector& values)
{
    auto arr = CCArray::create();
    for (auto& v : values)
        if (auto obj = valueToObject(v))
            arr->addObject(obj);
    return arr;
}

CCDictionary* CCDictionary::createWithContentsOfFile(const char* path)
{
    return valueMapToDictionary(ax::FileUtils::getInstance()->getValueMapFromFile(path));
}

CCArray* CCDictionary::allKeys() const
{
    auto arr = CCArray::create();
    for (auto& el : _elements)
    {
        if (el._isIntKey)
            arr->addObject(CCString::createWithFormat("%ld", (long)el._intKey));
        else
            arr->addObject(CCString::create(el._strKey));
    }
    return arr;
}

CCDictElement* CCDictionary::find(const std::string& key)
{
    for (auto& el : _elements)
        if (!el._isIntKey && el._strKey == key)
            return &el;
    return nullptr;
}

CCDictElement* CCDictionary::find(intptr_t key)
{
    for (auto& el : _elements)
        if (el._isIntKey && el._intKey == key)
            return &el;
    return nullptr;
}

const CCDictElement* CCDictionary::find(const std::string& key) const
{
    return const_cast<CCDictionary*>(this)->find(key);
}

const CCDictElement* CCDictionary::find(intptr_t key) const
{
    return const_cast<CCDictionary*>(this)->find(key);
}

void CCDictionary::setObject(ax::Object* obj, const std::string& key)
{
    if (!obj)
        return;
    obj->retain();
    if (auto el = find(key))
    {
        el->_object->release();
        el->_object = obj;
        return;
    }
    CCDictElement el;
    el._strKey = key;
    el._object = obj;
    _elements.push_back(std::move(el));
}

void CCDictionary::setObject(ax::Object* obj, intptr_t key)
{
    if (!obj)
        return;
    obj->retain();
    if (auto el = find(key))
    {
        el->_object->release();
        el->_object = obj;
        return;
    }
    CCDictElement el;
    el._intKey   = key;
    el._isIntKey = true;
    el._object   = obj;
    _elements.push_back(std::move(el));
}

ax::Object* CCDictionary::objectForKey(const std::string& key) const
{
    auto el = find(key);
    return el ? el->_object : nullptr;
}

ax::Object* CCDictionary::objectForKey(intptr_t key) const
{
    auto el = find(key);
    return el ? el->_object : nullptr;
}

const CCString* CCDictionary::valueForKey(const std::string& key) const
{
    if (auto str = dynamic_cast<CCString*>(objectForKey(key)))
        return str;
    return CCString::create("");
}

const CCString* CCDictionary::valueForKey(intptr_t key) const
{
    if (auto str = dynamic_cast<CCString*>(objectForKey(key)))
        return str;
    return CCString::create("");
}

void CCDictionary::removeObjectForKey(const std::string& key)
{
    for (auto it = _elements.begin(); it != _elements.end(); ++it)
    {
        if (!it->_isIntKey && it->_strKey == key)
        {
            auto obj = it->_object;
            _elements.erase(it);
            obj->release();
            return;
        }
    }
}

void CCDictionary::removeObjectForKey(intptr_t key)
{
    for (auto it = _elements.begin(); it != _elements.end(); ++it)
    {
        if (it->_isIntKey && it->_intKey == key)
        {
            auto obj = it->_object;
            _elements.erase(it);
            obj->release();
            return;
        }
    }
}

void CCDictionary::removeAllObjects()
{
    auto old = std::move(_elements);
    _elements.clear();
    for (auto& el : old)
        el._object->release();
}

/* ---------------------------------------------------------------------------
 * CCNotificationCenter
 * ------------------------------------------------------------------------- */
CCNotificationCenter* CCNotificationCenter::sharedNotificationCenter()
{
    static CCNotificationCenter instance;
    return &instance;
}

void CCNotificationCenter::addObserver(ax::Object* target,
                                       ax::SEL_CallFuncO selector,
                                       const char* name,
                                       ax::Object* obj)
{
    for (auto& o : _observers)
        if (o.target == target && o.name == name)
            return;
    _observers.push_back({target, selector, name, obj});
}

void CCNotificationCenter::removeObserver(ax::Object* target, const char* name)
{
    _observers.erase(std::remove_if(_observers.begin(), _observers.end(),
                                    [&](const Observer& o) { return o.target == target && o.name == name; }),
                     _observers.end());
}

int CCNotificationCenter::removeAllObservers(ax::Object* target)
{
    auto before = _observers.size();
    _observers.erase(std::remove_if(_observers.begin(), _observers.end(),
                                    [&](const Observer& o) { return o.target == target; }),
                     _observers.end());
    return (int)(before - _observers.size());
}

void CCNotificationCenter::postNotification(const char* name)
{
    postNotification(name, nullptr);
}

void CCNotificationCenter::postNotification(const char* name, ax::Object* obj)
{
    // Observers may remove themselves (or others) while being notified.
    auto snapshot = _observers;
    for (auto& o : snapshot)
    {
        if (o.name != name)
            continue;
        if (!(o.object == obj || o.object == nullptr || obj == nullptr))
            continue;
        bool stillRegistered = std::any_of(_observers.begin(), _observers.end(), [&](const Observer& cur) {
            return cur.target == o.target && cur.name == o.name;
        });
        if (stillRegistered)
            (o.target->*o.selector)(obj);
    }
}

/* ---------------------------------------------------------------------------
 * Touch dispatcher
 * ------------------------------------------------------------------------- */
CCTouchDispatcher* CCTouchDispatcher::sharedDispatcher()
{
    static CCTouchDispatcher instance;
    return &instance;
}

// 2.x menus used priority -128 and everything else sorted after them.
// Axmol runs fixed-priority listeners < 0 before scene-graph listeners (menus)
// and > 0 after them, so shift 2.x priorities above -128 into the positive range.
static int toAxmolPriority(int legacyPriority)
{
    return legacyPriority > -128 ? legacyPriority + 200 : legacyPriority;
}

void CCTouchDispatcher::addTargetedDelegate(CCTouchDelegate* delegate, int priority, bool swallowsTouches)
{
    if (!delegate || _listeners.count(delegate))
        return;

    auto listener = ax::EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(swallowsTouches);
    listener->onTouchBegan     = [delegate](ax::Touch* t, ax::Event* e) { return delegate->ccTouchBegan(t, e); };
    listener->onTouchMoved     = [delegate](ax::Touch* t, ax::Event* e) { delegate->ccTouchMoved(t, e); };
    listener->onTouchEnded     = [delegate](ax::Touch* t, ax::Event* e) { delegate->ccTouchEnded(t, e); };
    listener->onTouchCancelled = [delegate](ax::Touch* t, ax::Event* e) { delegate->ccTouchCancelled(t, e); };

    // 2.x touch handlers retained their delegate.
    if (auto obj = dynamic_cast<ax::Object*>(delegate))
        obj->retain();

    ax::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener,
                                                                                         toAxmolPriority(priority));
    _listeners[delegate] = listener;
}

void CCTouchDispatcher::removeDelegate(CCTouchDelegate* delegate)
{
    auto it = _listeners.find(delegate);
    if (it == _listeners.end())
        return;
    ax::Director::getInstance()->getEventDispatcher()->removeEventListener(it->second);
    _listeners.erase(it);
    if (auto obj = dynamic_cast<ax::Object*>(delegate))
        obj->release();
}

/* ---------------------------------------------------------------------------
 * CCLayer
 * ------------------------------------------------------------------------- */
void CCLayer::setKeypadEnabled(bool enabled)
{
    if (enabled == (_keypadListener != nullptr))
        return;

    auto dispatcher = _eventDispatcher;
    if (enabled)
    {
        _keypadListener               = ax::EventListenerKeyboard::create();
        _keypadListener->onKeyReleased = [this](ax::EventKeyboard::KeyCode key, ax::Event*) {
            if (key == ax::EventKeyboard::KeyCode::KEY_BACK)
                keyBackClicked();
            else if (key == ax::EventKeyboard::KeyCode::KEY_MENU)
                keyMenuClicked();
        };
        dispatcher->addEventListenerWithSceneGraphPriority(_keypadListener, this);
    }
    else
    {
        dispatcher->removeEventListener(_keypadListener);
        _keypadListener = nullptr;
    }
}

void CCLayer::onExit()
{
    setKeypadEnabled(false);
    ax::Layer::onExit();
}

/* ---------------------------------------------------------------------------
 * Sequence / Spawn
 * ------------------------------------------------------------------------- */
ax::Sequence* CCSequence::create(CCArray* actions)
{
    return ax::Sequence::create(actions ? actions->toVector<ax::FiniteTimeAction*>() : ax::Vector<ax::FiniteTimeAction*>());
}

ax::Sequence* CCSequence::create(ax::FiniteTimeAction* action1, ...)
{
    va_list args;
    va_start(args, action1);
    auto ret = ax::Sequence::createWithVariableList(action1, args);
    va_end(args);
    return ret;
}

ax::Spawn* CCSpawn::create(CCArray* actions)
{
    return ax::Spawn::create(actions ? actions->toVector<ax::FiniteTimeAction*>() : ax::Vector<ax::FiniteTimeAction*>());
}

ax::Spawn* CCSpawn::create(ax::FiniteTimeAction* action1, ...)
{
    va_list args;
    va_start(args, action1);
    auto ret = ax::Spawn::createWithVariableList(action1, args);
    va_end(args);
    return ret;
}

/* ---------------------------------------------------------------------------
 * Actions
 * ------------------------------------------------------------------------- */
// 2.x CCCallFunc* retained its target for the lifetime of the action.
CCCallFunc* CCCallFunc::create(ax::Object* target, ax::SEL_CallFunc selector)
{
    auto ret = new CCCallFunc();
    ax::RefPtr<ax::Object> keep(target);
    ret->initWithFunction([keep, selector]() { (keep.get()->*selector)(); });
    ret->autorelease();
    return ret;
}

CCCallFuncN* CCCallFuncN::create(ax::Object* target, ax::SEL_CallFuncN selector)
{
    auto ret = new CCCallFuncN();
    ax::RefPtr<ax::Object> keep(target);
    ret->initWithFunction([keep, selector](ax::Node* node) { (keep.get()->*selector)(node); });
    ret->autorelease();
    return ret;
}

CCCallFuncND* CCCallFuncND::create(ax::Object* target, ax::SEL_CallFuncND selector, void* data)
{
    auto ret = new CCCallFuncND();
    ax::RefPtr<ax::Object> keep(target);
    ret->initWithFunction([keep, selector, data](ax::Node* node) { (keep.get()->*selector)(node, data); });
    ret->autorelease();
    return ret;
}

/* ---------------------------------------------------------------------------
 * Menu items
 * ------------------------------------------------------------------------- */
static ax::ccMenuCallback makeMenuCallback(ax::Object* target, ax::SEL_MenuHandler selector)
{
    if (!target || !selector)
        return nullptr;
    return [target, selector](ax::Object* sender) { (target->*selector)(sender); };
}

CCMenuItemSprite* CCMenuItemSprite::create(ax::Node* normal, ax::Node* selected, ax::Node* disabled)
{
    return create(normal, selected, disabled, nullptr, nullptr);
}

CCMenuItemSprite* CCMenuItemSprite::create(ax::Node* normal,
                                           ax::Node* selected,
                                           ax::Object* target,
                                           ax::SEL_MenuHandler selector)
{
    return create(normal, selected, nullptr, target, selector);
}

CCMenuItemSprite* CCMenuItemSprite::create(ax::Node* normal,
                                           ax::Node* selected,
                                           ax::Node* disabled,
                                           ax::Object* target,
                                           ax::SEL_MenuHandler selector)
{
    auto ret = new CCMenuItemSprite();
    ret->initWithNormalSprite(normal, selected, disabled, makeMenuCallback(target, selector));
    ret->autorelease();
    return ret;
}

void CCMenuItemSprite::setTarget(ax::Object* target, ax::SEL_MenuHandler selector)
{
    setCallback(makeMenuCallback(target, selector));
}

CCMenuItemLabel* CCMenuItemLabel::create(ax::Node* label)
{
    return create(label, nullptr, nullptr);
}

CCMenuItemLabel* CCMenuItemLabel::create(ax::Node* label, ax::Object* target, ax::SEL_MenuHandler selector)
{
    auto ret = new CCMenuItemLabel();
    ret->initWithLabel(label, makeMenuCallback(target, selector));
    ret->autorelease();
    return ret;
}

void CCMenuItemLabel::setTarget(ax::Object* target, ax::SEL_MenuHandler selector)
{
    setCallback(makeMenuCallback(target, selector));
}

/* ---------------------------------------------------------------------------
 * Labels
 * ------------------------------------------------------------------------- */
CCLabelTTF* CCLabelTTF::create()
{
    return create("", "Arial", 12);
}

CCLabelTTF* CCLabelTTF::create(const char* str, const char* fontName, float fontSize)
{
    return create(str, fontName, fontSize, ax::Size::ZERO, ax::TextHAlignment::CENTER, ax::TextVAlignment::TOP);
}

CCLabelTTF* CCLabelTTF::create(const char* str,
                               const char* fontName,
                               float fontSize,
                               const ax::Size& dimensions,
                               ax::TextHAlignment hAlignment)
{
    return create(str, fontName, fontSize, dimensions, hAlignment, ax::TextVAlignment::TOP);
}

CCLabelTTF* CCLabelTTF::create(const char* str,
                               const char* fontName,
                               float fontSize,
                               const ax::Size& dimensions,
                               ax::TextHAlignment hAlignment,
                               ax::TextVAlignment vAlignment)
{
    auto ret = new CCLabelTTF();
    ret->setHorizontalAlignment(hAlignment);
    ret->setVerticalAlignment(vAlignment);
    ret->setSystemFontName(fontName ? fontName : "Arial");
    ret->setSystemFontSize(fontSize);
    ret->setDimensions(dimensions.width, dimensions.height);
    ret->setString(str ? str : "");
    ret->autorelease();
    return ret;
}

CCLabelBMFont* CCLabelBMFont::create()
{
    auto ret = new CCLabelBMFont();
    ret->autorelease();
    return ret;
}

CCLabelBMFont* CCLabelBMFont::create(const char* str, const char* fntFile)
{
    return create(str, fntFile, 0.0f, ax::TextHAlignment::LEFT);
}

CCLabelBMFont* CCLabelBMFont::create(const char* str, const char* fntFile, float width, ax::TextHAlignment alignment)
{
    auto ret = new CCLabelBMFont();
    if (fntFile && ret->setBMFontFilePath(fntFile))
    {
        ret->setHorizontalAlignment(alignment);
        ret->setMaxLineWidth(width);
        ret->setString(str ? str : "");
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

/* ---------------------------------------------------------------------------
 * SimpleAudioEngine
 * ------------------------------------------------------------------------- */
namespace CocosDenshion
{
static SimpleAudioEngine* s_audioEngine = nullptr;

SimpleAudioEngine* SimpleAudioEngine::sharedEngine()
{
    if (!s_audioEngine)
        s_audioEngine = new SimpleAudioEngine();
    return s_audioEngine;
}

void SimpleAudioEngine::end()
{
    if (s_audioEngine)
    {
        ax::AudioEngine::stopAll();
        delete s_audioEngine;
        s_audioEngine = nullptr;
    }
}

void SimpleAudioEngine::preloadBackgroundMusic(const char* path)
{
    if (path)
        ax::AudioEngine::preload(path);
}

void SimpleAudioEngine::playBackgroundMusic(const char* path, bool loop)
{
    if (!path)
        return;
    stopBackgroundMusic();
    _musicId = ax::AudioEngine::play2d(path, loop, _musicVolume);
}

void SimpleAudioEngine::stopBackgroundMusic(bool releaseData)
{
    if (_musicId != ax::AudioEngine::INVALID_AUDIO_ID)
    {
        ax::AudioEngine::stop(_musicId);
        _musicId = ax::AudioEngine::INVALID_AUDIO_ID;
    }
}

void SimpleAudioEngine::pauseBackgroundMusic()
{
    if (_musicId != ax::AudioEngine::INVALID_AUDIO_ID)
        ax::AudioEngine::pause(_musicId);
}

void SimpleAudioEngine::resumeBackgroundMusic()
{
    if (_musicId != ax::AudioEngine::INVALID_AUDIO_ID)
        ax::AudioEngine::resume(_musicId);
}

void SimpleAudioEngine::rewindBackgroundMusic()
{
    if (_musicId != ax::AudioEngine::INVALID_AUDIO_ID)
        ax::AudioEngine::setCurrentTime(_musicId, 0);
}

bool SimpleAudioEngine::isBackgroundMusicPlaying()
{
    return _musicId != ax::AudioEngine::INVALID_AUDIO_ID &&
           ax::AudioEngine::getState(_musicId) == ax::AudioEngine::AudioState::PLAYING;
}

void SimpleAudioEngine::setBackgroundMusicVolume(float volume)
{
    _musicVolume = std::clamp(volume, 0.0f, 1.0f);
    if (_musicId != ax::AudioEngine::INVALID_AUDIO_ID)
        ax::AudioEngine::setVolume(_musicId, _musicVolume);
}

void SimpleAudioEngine::setEffectsVolume(float volume)
{
    _effectsVolume = std::clamp(volume, 0.0f, 1.0f);
    pruneEffects();
    for (int id : _effectIds)
        ax::AudioEngine::setVolume(id, _effectsVolume);
}

void SimpleAudioEngine::pruneEffects()
{
    _effectIds.erase(std::remove_if(_effectIds.begin(), _effectIds.end(),
                                    [](int id) {
                                        auto state = ax::AudioEngine::getState(id);
                                        return state != ax::AudioEngine::AudioState::PLAYING &&
                                               state != ax::AudioEngine::AudioState::PAUSED;
                                    }),
                     _effectIds.end());
}

unsigned int SimpleAudioEngine::playEffect(const char* path, bool loop)
{
    if (!path)
        return 0;
    pruneEffects();
    int id = ax::AudioEngine::play2d(path, loop, _effectsVolume);
    if (id == ax::AudioEngine::INVALID_AUDIO_ID)
        return 0;
    _effectIds.push_back(id);
    return (unsigned int)id;
}

void SimpleAudioEngine::pauseEffect(unsigned int soundId)
{
    ax::AudioEngine::pause((int)soundId);
}

void SimpleAudioEngine::pauseAllEffects()
{
    pruneEffects();
    for (int id : _effectIds)
        ax::AudioEngine::pause(id);
}

void SimpleAudioEngine::resumeEffect(unsigned int soundId)
{
    ax::AudioEngine::resume((int)soundId);
}

void SimpleAudioEngine::resumeAllEffects()
{
    pruneEffects();
    for (int id : _effectIds)
        ax::AudioEngine::resume(id);
}

void SimpleAudioEngine::stopEffect(unsigned int soundId)
{
    ax::AudioEngine::stop((int)soundId);
}

void SimpleAudioEngine::stopAllEffects()
{
    for (int id : _effectIds)
        ax::AudioEngine::stop(id);
    _effectIds.clear();
}

void SimpleAudioEngine::preloadEffect(const char* path)
{
    if (path)
        ax::AudioEngine::preload(path);
}

void SimpleAudioEngine::unloadEffect(const char* path)
{
    if (path)
        ax::AudioEngine::uncache(path);
}
}  // namespace CocosDenshion
