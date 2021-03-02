#include "PathNode.h"
#include "PowerUp.h"
#include "Weapon.h"
#include "Health.h"
#include "Armor.h"
#include "GraphDataStructure.h"

#include <stdio.h>
#include <algorithm>
#include <map>
#include <cmath>
#include <mutex>

static PathNodes sPathNodes;
static PowerUps sPowerUps;
std::mutex gMutex;

/////////////////////////////////////////////////////////////////////////////////////////////////////
// - To find nearest powerUp for the given node I have created a data structure which is Unique, Priority Queue
// - It holds GraphData which has information about node and its distance from given node(start node)
// - This function also has a local map which holds nodes parent/previous node value
//   **** Working of the Funtion: ****
//   -Add start node into the queue
//   -Iterate till queue is empty
//         i) Initialize the current node with top of the queue which always has shortest distance from start node
//        ii) Check if Current Node's linked power up is matching with given Power Up ; Return true if it mathes
//        iii) if power up is not matching then Push its(Current Node's) linked node into the queue
//             - Also add current node as linked nodes parent node as linked nodes are successor of current node
//        iv)  pop the queue, it will pop current node.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
bool FindPowerUp(PathNodes& path, PowerUpType mType, PathNode *start)
{
    if (start) //null check
    {
        UniquePriorityQueue pathQueue;
        GraphData startNodeData(start, 0.0f);
        pathQueue.push(startNodeData);

        //map of current node and its parent/previous node at the time of finding path
        std::map<PathNode, std::unique_ptr<PathNode> > visted;
        {
            std::lock_guard<std::mutex> lock(gMutex);
            visted[*start] = nullptr; //Added start node as start point with null as its parent node
        }

        while (!pathQueue.empty())
        {
            PathNode* const currentNode = pathQueue.top().GetCurrentNode();
            
            for (const auto& powerUp : currentNode->GetPowerUps())
            {
                if (mType == powerUp->GetPowerUpType())
                {
                    path.push_back(currentNode);

                    if (auto ptrNode = std::move(visted.at(*currentNode)))
                    {
                        while (ptrNode->GetNodeId() != start->GetNodeId())
                        {
                            PathNode ptrNodeValue = *ptrNode.get();
                            PathNode* temp = new PathNode(ptrNodeValue);
                            path.push_back(temp);
                            ptrNode = std::move(visted.at(ptrNodeValue));
                        }
                    }
                    else
                    {
                        return true; // founded path is start node 
                    }
                    path.push_back(start);
                    return true;
                }
            }

            for (const auto& currentNodeLink : currentNode->GetLinks())
            {
                //ToDo: We could shift it to function into utlis to calculate distance between any two points
                float distanceFromStartNode = sqrt(pow((start->GetPosition().x - currentNodeLink->GetPosition().x), 2) + pow((start->GetPosition().y - currentNodeLink->GetPosition().y), 2) + pow((start->GetPosition().z - currentNodeLink->GetPosition().z), 2));
                GraphData nodeData(currentNodeLink, distanceFromStartNode);
                if (pathQueue.push(nodeData))
                {
                    {
                        std::lock_guard<std::mutex> lock(gMutex);
                        visted[*currentNodeLink] = std::move(currentNode->Clone());
                    }
                }
            }

            pathQueue.pop();
        }
    }
    return(false); // No path found.
}

// For this example, all links are symmetric.
inline void LinkNodes(PathNode *n1, PathNode *n2)
{
    if (n1&&n2)
    {
        n1->AddLink(n2);
        n2->AddLink(n1);
    }
}

int main(int, char*[])
{
    sPathNodes.push_back(new PathNode("Node0", Vertex(300, 60, 0)));
    sPathNodes.push_back(new PathNode("Node1", Vertex(100, 60, 0)));
    sPathNodes.push_back(new PathNode("Node2", Vertex(80, 560, 0)));
    sPathNodes.push_back(new PathNode("Node3", Vertex(280, 650, 0)));
    sPathNodes.push_back(new PathNode("Node4", Vertex(300, 250, 0)));
    sPathNodes.push_back(new PathNode("Node5", Vertex(450, 400, 0)));
    sPathNodes.push_back(new PathNode("Node6", Vertex(450, 60, 0)));
    sPathNodes.push_back(new PathNode("Node7", Vertex(450, 400, 0)));

    LinkNodes(sPathNodes[1], sPathNodes[4]);
    LinkNodes(sPathNodes[0], sPathNodes[1]);
    LinkNodes(sPathNodes[0], sPathNodes[6]);
    LinkNodes(sPathNodes[0], sPathNodes[4]);
    LinkNodes(sPathNodes[7], sPathNodes[4]);
    LinkNodes(sPathNodes[7], sPathNodes[5]);
    LinkNodes(sPathNodes[2], sPathNodes[4]);
    LinkNodes(sPathNodes[2], sPathNodes[3]);
    LinkNodes(sPathNodes[3], sPathNodes[5]);

    sPowerUps.push_back(new Weapon("Weapon0", Vertex(340, 670, 0)));
    sPathNodes[3]->AddPowerUp(sPowerUps[0]);    
    sPowerUps.push_back(new Weapon("Weapon1", Vertex(500, 220, 0)));
    sPathNodes[7]->AddPowerUp(sPowerUps[1]);    

    sPowerUps.push_back(new Health("Health0", Vertex(490, 10, 0)));
    sPathNodes[6]->AddPowerUp(sPowerUps[2]);    
    sPowerUps.push_back(new Health("Health1", Vertex(120, 20, 0)));
    sPathNodes[1]->AddPowerUp(sPowerUps[3]);    

    sPowerUps.push_back(new Armor("Armour0", Vertex(500, 360, 0)));
    sPathNodes[5]->AddPowerUp(sPowerUps[4]);    
    sPowerUps.push_back(new Armor("Armour1", Vertex(180, 525, 0)));
    sPathNodes[2]->AddPowerUp(sPowerUps[5]);    

    PathNodes path;

    if(!FindPowerUp(path, PowerUpType::WEAPON, sPathNodes[4]))
    {
        printf("No path found: IMPOSSIBLE!\n");
    }
    else
    {
        printf("Path found: ");

        for(PathNodes::const_reverse_iterator i = path.rbegin(); i != path.rend(); ++i)
        {
            if (const char* name = (*i)->GetName())
            {
                printf("%s ", name);
            }
            else
            {
                printf("Name is not Provided");
            }
        }

        printf("\n");
    }
       
    return(0);
}
