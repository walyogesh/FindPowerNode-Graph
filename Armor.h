#ifndef ARMOUR_H
#define ARMOUR_H

#include "PowerUp.h"

class Armor : public PowerUp
{
public:
    Armor(const char* name, const Vertex& position) 
        : PowerUp(name, position, PowerUpType::ARMOUR)
        , mClanTag(nullptr)
    {
    }

    ~Armor()
    {
        delete[] mClanTag; // Array delete
    }

    const char* GetClanTag() const
    {
        return(mClanTag);
    }

    void SetClanTag(char* clanTag)
    {
        delete[] mClanTag;
        size_t  clagTagSize = strlen(clanTag);
        mClanTag = new char[clagTagSize+1];
        strncpy_s(mClanTag, clagTagSize +1, clanTag, clagTagSize);
    }

protected:
    char* mClanTag;
};

#endif // ARMOUR_H

