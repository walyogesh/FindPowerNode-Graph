#ifndef PATH_NODE_H
#define PATH_NODE_H

#include "Vertex.h"

#include <vector>
#include <algorithm>
#include <atomic>
#include <memory>

class PathNode;
typedef std::vector<PathNode*> PathNodes;

class PowerUp;
typedef std::vector<PowerUp*> PowerUps;

///////////////////////////////////////////////////////////////////////////
// This Class holds Path Node data.
// Improvements Done:
//   1. Passed function parameters(class objects) by const reference to avoid extra copy
//   2. All Getters are const functions
//   3. Member variables are declared and initialized as per their size i.e smallest first 
//   4. Written All necessary constructors as per rule of 5
//   5. Added unique ID for each node to achieve uniqueness of each node
//   6. Added null checks before we use pointer
// Improvements Can be Done:
//  We could create PathNode.cpp file to separate declaration from definition of the class
//  we could add guard to mLinks & mPowerUps to use class in multi threaded environment with the help of lock_guard & mutex 
///////////////////////////////////////////////////////////////////////////
class PathNode
{
public:
    PathNode(const char* name, Vertex const& position)//Q1.-2 const&
        : mId(CreateAndGetUniquePowerUpId())
        , mName(nullptr)
        , mPosition(position)
    {
        if (name)
        {
            size_t size = strlen(name);
            mName = new char[size+1];
            strncpy_s(mName, size + 1, name, size);
        }
    }

    //copy constructor
    PathNode(const PathNode& other)
        : mId(other.mId)
        , mName(nullptr)
        , mPosition(other.mPosition)
        , mLinks(other.mLinks)
        , mPowerUps(other.mPowerUps)
    {
        if (other.mName)
        {
            size_t size = strlen(other.mName);
            mName = new char[size + 1];
            strncpy_s(mName, size + 1, other.mName, size);
        }
    }

    // clones object and returns cloned object
    std::unique_ptr<PathNode> Clone()
    {
        std::unique_ptr<PathNode> clone(new PathNode() );
        clone->mId = mId;
        clone->mPosition = mPosition;
        clone->mLinks = mLinks;
        clone->mPowerUps = mPowerUps;
        if (mName)
        {
            size_t size = strlen(mName);
            clone->mName = new char[size + 1];
            strncpy_s(clone->mName, size + 1, mName, size);
        }
        return clone;
    }

    PathNode& operator=(const PathNode& other)
    {
        if (this==&other)//self check
        {
            return *this;
        }
        if (mName)
        {
            delete[] mName;
            mName = nullptr;
            mLinks.clear();
            mPowerUps.clear();
        }
        mId = other.mId;
        mPosition = other.mPosition;

        mLinks = other.mLinks; 
        mPowerUps = other.mPowerUps;

        if (other.GetName())
        {
            size_t size = strlen(other.GetName());
            mName = new char[size + 1];
            strncpy_s(mName, size + 1, other.GetName(), size);
        }
        return *this;
    }

    bool operator<(const PathNode& rhs) const
    {
        return rhs.GetNodeId() < GetNodeId();
    }

    //move constructor
    PathNode(PathNode&& other) noexcept
        : mId(other.mId)
        , mName(nullptr)
        , mPosition(other.mPosition)
        , mLinks(std::move(other.mLinks))
        , mPowerUps(std::move(other.mPowerUps)) 
    {
        other.mName = nullptr;
    }

    ~PathNode() 
    {
        delete[] mName; //Q1.-1
        mName = nullptr;
    }

    void AddLink(PathNode *pathNode)
    {
        if (pathNode)
        {
            mLinks.push_back(pathNode);
        }
    }
    
    bool RemoveLink(PathNode *pathNode)
    {
        PathNodes::iterator i = std::find(mLinks.begin(), mLinks.end(), pathNode);

        if (i != mLinks.end())
        {
            mLinks.erase(i);
            return true;
        }
        return false;
    }

    void AddPowerUp(PowerUp *powerUp)
    {
        if (powerUp)
        {
            mPowerUps.push_back(powerUp);
        }
    }
    
    bool RemovePowerUp(PowerUp *powerUp)
    {
        PowerUps::iterator itr = std::find(mPowerUps.begin(), mPowerUps.end(), powerUp);
        if (itr != mPowerUps.end())
        {
            mPowerUps.erase(itr);
            return true;
        }
        return false;
    }

    const char* GetName() const
    {
        return(mName);
    }

    const PathNodes& GetLinks() const
    {
        return(mLinks);
    }

    const PowerUps& GetPowerUps() const
    {
        return(mPowerUps); 
    }

    unsigned int GetNodeId() const 
    {
        return mId; 
    }

    const Vertex& GetPosition() const 
    { 
        return mPosition; 
    }

protected:
    unsigned int mId;
    char*        mName;
    Vertex       mPosition;
    PathNodes    mLinks;
    PowerUps     mPowerUps;

private:
     PathNode()
     : mName(nullptr)
     , mId(0)
     {}

     const static unsigned int CreateAndGetUniquePowerUpId()
     {
         static std::atomic<unsigned int> id{ 0 };
         return ++id;
     }
};

#endif // PATH_NODE_H
