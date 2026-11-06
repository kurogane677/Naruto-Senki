#include "AI/TeamBrain.h"
#include "ActionManager.h"
#include "GameLayer.h"

namespace ai
{

bool isLaneHero(ActionManager* unit)
{
    return (unit->role() == Role::Player || unit->role() == Role::Com) && !unit->isCharacter("Roshi") &&
           !unit->isCharacter("Han");
}

namespace
{

struct Cache
{
    GameLayer* game = nullptr;
    unsigned int frame = 0;
    TeamBrain brain[3];
};

void compute(GameLayer* game, Cache& cache)
{
    for (auto& brain : cache.brain)
        brain = TeamBrain{};
    int alive[3] = {0, 0, 0};
    int dead[3] = {0, 0, 0};

    CCObject* object;
    CCARRAY_FOREACH(game->_CharacterArray, object)
    {
        ActionManager* unit = (ActionManager*)object;
        if (!isLaneHero(unit))
            continue;
        int groupIndex = (int)unit->group();
        if (unit->getActionState() == ACTION_STATE_DEAD)
        {
            dead[groupIndex]++;
            continue;
        }
        alive[groupIndex]++;
        TeamBrain& brain = cache.brain[groupIndex];
        float unitX = unit->getPositionX();
        bool ahead = unit->group() == Group::Konoha ? unitX > brain.frontX : unitX < brain.frontX;
        if (!brain.hasFront || ahead)
        {
            brain.frontX = unitX;
            brain.hasFront = true;
        }
    }

    float tileWidth = game->currentMap->getTileSize().width;
    float mapWidth = game->currentMap->getMapSize().width * tileWidth;
    for (Group side : {Group::Konoha, Group::Akatsuki})
    {
        TeamBrain& brain = cache.brain[(int)side];
        int enemy = side == Group::Konoha ? (int)Group::Akatsuki : (int)Group::Konoha;
        brain.pushWindow = dead[enemy] >= 2;
        brain.outnumbered = alive[enemy] > alive[(int)side] + 1;
        if (!brain.hasFront)
            continue;

        float best = 2.0f;
        CCARRAY_FOREACH(game->_CharacterArray, object)
        {
            ActionManager* unit = (ActionManager*)object;
            if (!isLaneHero(unit) || (int)unit->group() != enemy || unit->getActionState() == ACTION_STATE_DEAD ||
                !unit->_isVisable || unit->_isWudi)
                continue;
            float unitX = unit->getPositionX();
            if (unitX < tileWidth * 3 || unitX > mapWidth - tileWidth * 3)
                continue;  // inside a base: not worth diving
            if (unit->isHurtingTower)
                brain.towerThreat = unit;
            if (fabsf(unitX - brain.frontX) > kGameWidth * 0.6f)
                continue;
            float score = unit->getHpPercent();
            if (score < best)
            {
                best = score;
                brain.focus = unit;
            }
        }
    }
}

}  // namespace

const TeamBrain& TeamBrain::get(GameLayer* game, Group group)
{
    static Cache cache;
    unsigned int frame = CCDirector::sharedDirector()->getTotalFrames();
    if (cache.game != game || frame - cache.frame >= 6)
    {
        cache.game = game;
        cache.frame = frame;
        compute(game, cache);
    }
    return cache.brain[(int)group];
}

}  // namespace ai
