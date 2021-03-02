#ifndef HEALTH_H
#define HEALTH_H

#include "PowerUp.h"

class Health : public PowerUp
{
public:
    Health(const char* name, const Vertex& position)
        : PowerUp(name, position, PowerUpType::HEALTH)
        , mHealing(0.0f)
    {
    }
    
protected:
    float mHealing;
};

#endif // HEALTH_H

