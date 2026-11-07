#pragma once
#include "Characters.h"

// Sai and Deidara share one skill buff (cBuff): value 17 swaps their basic attack,
// value 18 is a ride that drops a monster every half second until it ends. Only the
// monster (Sai's Mouse, Deidara's Spider) differs.
class RidingHero : public Hero
{
protected:
    bool onTransformSkillStart() override;
    bool onTransformSkillEnd() override;
    bool onTransformSkillCancel() override;

    // the monster the ride drops
    virtual const char* rideMonster() const = 0;
    virtual void onRideMonsterSpawned() {}

private:
    void startRide();
    void endRide(bool returnToIdle);
    void spawnRideMonster(float dt);
};
