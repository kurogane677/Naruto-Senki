#pragma once
#include "Compat/Compat2x.h"
#include "Defines.h"

// Shown between hero select and loading: picks how smart every bot plays
// (allies and enemies alike). The choice is remembered in UserDefault
// "aiDifficulty" and read by GameLayer.
class DifficultyLayer : public CCLayer
{
public:
    DifficultyLayer();
    ~DifficultyLayer();

    // heroes: the CCArray of {character, role, group} dictionaries for LoadLayer
    static void show(CCArray* heroes, bool hardCore, bool randomChar);
    // goes straight to loading (also used by NS_AUTOPLAY)
    static void startLoad(CCArray* heroes, bool hardCore, bool randomChar);

    bool init();
    CREATE_FUNC(DifficultyLayer);

    virtual void keyBackClicked();

private:
    void select(int level);
    void onLevel(CCObject* sender);
    void onStart(CCObject* sender);
    void onBack(CCObject* sender);

    CCArray* _heroes;
    bool _hardCore;
    bool _randomChar;
    bool _started;
    int _level;
    CCMenuItem* _items[3];
    CCSprite* _marker;
    CCLabelTTF* _desc;
};
