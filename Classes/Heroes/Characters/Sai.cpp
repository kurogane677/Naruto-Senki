#include "Heroes/Characters/Sai.h"

void Sai::onRideMonsterSpawned()
{
    CCDictionary* sound = CCDictionary::create();
    sound->setObject(CCString::create("Audio/Sai/ink_mouse.mp3"), 1);
    setSound(this, sound);
}
