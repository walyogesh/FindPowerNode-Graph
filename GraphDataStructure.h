#pragma once
#include "PathNode.h"
#include "PowerUp.h"
#include <queue>
#include <mutex>
#include <unordered_set>

//////////////////////////////////////////////////////////////////////////////////////////
// GraphData class contains node information and its distance from start node
//////////////////////////////////////////////////////////////////////////////////////////
class GraphData
{
public:
    GraphData(PathNode* currentNode, float distanceFromStartNode)
        : mDistanceFromStartNode(distanceFromStartNode)
        , mCurrentNode(currentNode)
    {}

    bool operator<(const GraphData& rhs) const
    {
        return (rhs.mDistanceFromStartNode < mDistanceFromStartNode);
    }

    bool operator==(const GraphData& rhs) const
    {
        return (rhs.mDistanceFromStartNode == mDistanceFromStartNode);
    }

    PathNode* const GetCurrentNode() const { return mCurrentNode; }
    float GetDistanceFromStartNode() const { return mDistanceFromStartNode; }
private:
    float mDistanceFromStartNode;
    PathNode* mCurrentNode;
};

// Comparison function for prioritizing queue data, based on distance from stat node 
struct ComapreGraphData
{
    bool operator()(const GraphData& lhs, const GraphData& rhs) const
    {
        return (rhs.GetDistanceFromStartNode() < lhs.GetDistanceFromStartNode());
    }
};

//////////////////////////////////////////////////////////////////////////////////////////
//  UniquePriorityQueue is Data Structure which holds unique GraphData in priority manner
//  Priority is decided on shortest distance from given start node (distance between start
//  node and graph node))
//   Time Complexity :
//         1) PUSH: log(n) => insert into set: O(1), insert into Priority queue/heap: log(n)
//                            where n is size of queue
//         2) PUSH: O(1)
//         3) POP: O(1)
///////////////////////////////////////////////////////////////////////////////////////////

class UniquePriorityQueue
{
private:
    std::priority_queue<GraphData, std::vector<GraphData>, ComapreGraphData> mQueue;

    // Set is used to achieve uniqueness of GraphData inside mQueue
    std::unordered_set<unsigned int> mSet;

    //To achieve thread synchronization. 
    mutable std::mutex mMutex;
public:
    bool push(const GraphData& data)
    {
        std::lock_guard lockGuard(mMutex);
        if (mSet.insert(data.GetCurrentNode()->GetNodeId()).second) // uniqueness achieved
        {
            mQueue.push(data);
            return true;
        }
        return false;
    }
    bool empty() const { return mQueue.empty(); }
    const GraphData& top() const { return mQueue.top(); }
    void pop() 
    {
        std::lock_guard lockGuard(mMutex);
        mQueue.pop(); 
    }
};


