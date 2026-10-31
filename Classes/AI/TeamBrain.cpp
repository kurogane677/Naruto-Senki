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
    for (auto& b : cache.brain)
        b = TeamBrain{};
    int alive[3] = {0, 0, 0};
    int dead[3] = {0, 0, 0};

    CCObject* obj;
    CCARRAY_FOREACH(game->_CharacterArray, obj)
    {
        ActionManager* u = (ActionManager*)obj;
        if (!isLaneHero(u))
            continue;
        int g = (int)u->group();
        if (u->getActionState() == ACTION_STATE_DEAD)
        {
            dead[g]++;
            continue;
        }
        alive[g]++;
        TeamBrain& b = cache.brain[g];
        float x = u->getPositionX();
        bool ahead = u->group() == Group::Konoha ? x > b.frontX : x < b.frontX;
        if (!b.hasFront || ahead)
        {
            b.frontX = x;
            b.hasFront = true;
        }
    }

    float tile = game->currentMap->getTileSize().width;
    float mapW = game->currentMap->getMapSize().width * tile;
    for (Group side : {Group::Konoha, Group::Akatsuki})
    {
        TeamBrain& b = cache.brain[(int)side];
        int enemy = side == Group::Konoha ? (int)Group::Akatsuki : (int)Group::Konoha;
        b.pushWindow = dead[enemy] >= 2;
        b.outnumbered = alive[enemy] > alive[(int)side] + 1;
        if (!b.hasFront)
            continue;

        float best = 2.0f;
        CCARRAY_FOREACH(game->_CharacterArray, obj)
        {
            ActionManager* u = (ActionManager*)obj;
            if (!isLaneHero(u) || (int)u->group() != enemy || u->getActionState() == ACTION_STATE_DEAD ||
                !u->_isVisable || u->_isWudi)
                continue;
            float x = u->getPositionX();
            if (x < tile * 3 || x > mapW - tile * 3)
                continue;  // inside a base: not worth diving
            if (u->isHurtingTower)
                b.towerThreat = u;
            if (fabsf(x - b.frontX) > winSize.width * 0.6f)
                continue;
            float score = u->getHpPercent();
            if (score < best)
            {
                best = score;
                b.focus = u;
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
