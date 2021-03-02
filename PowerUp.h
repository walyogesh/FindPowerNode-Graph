#ifndef POWER_UP_H
#define POWER_UP_H

#include "Vertex.h"
#include <string.h>

enum class PowerUpType
{
    WEAPON = 0,
    ARMOUR,
    HEALTH
};

///////////////////////////////////////////////////////////////////////////////
// This Class holds Path Node data.
//Improvements Done:
//   1. Passed function parameters(class objects) by const reference to avoid extra copy
//   2. All Getters are const functions
//   3. Member variables are declared and initialized as per their size i.e smallest first 
//   4. Constructor's initialization is done in the Initializer list
//   5. deleted memory by delete [] as we are declaring it with new []
//   6. Added null checks before we use pointer
//   7. Used scoped enum by using enum class
//Improvements Can be Done:
// We could create PowerUp.cpp file to separate declaration from definition of the class
// we could add guards to use class in multi threaded environment with the help of lock_guard & mutex 
// We could add copy/move/assignment operators methods as per rule of 5
////////////////////////////////////////////////////////////////////////////////

class PowerUp
{
public:
    PowerUp(const char* name, const Vertex& position, PowerUpType type)
        : mType(type)
        , mPosition(position)
        , mName(nullptr)
    {
        if (name)
        {
            size_t size = strlen(name);
            mName = new char[size +1];
            strncpy_s(mName, size + 1, name, size);
        }
    }

    ~PowerUp()
    {
        delete[] mName;
        mName = nullptr;
    } 

    PowerUpType GetPowerUpType() const
    {
        return(mType);
    };

    const Vertex& GetPosition() const
    {
        return(mPosition);
    }

protected:
    PowerUpType mType;
    Vertex      mPosition;
    char*       mName;
};

#endif // POWER_UP_H
