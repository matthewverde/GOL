//
//  creature.h
//  GOL
//
//  Created by Matthew Green on 10/22/17.
//  Copyright (c) 2017 Matthew Green. All rights reserved.
//

#ifndef GOL_creature_h
#define GOL_creature_h

#include <iostream>
#include <vector>
#include <stdlib.h>

using namespace std;
using namespace sf;

class creature
{
public:
    creature()
    {
        
    }
    
    creature(Vector2i startLocation)
    {
        creatureLocation.push_back(startLocation);
        mutationRate = 0.05;
    }
    
    int creatureSize;
    double mutationRate;
    vector<Vector2i> creatureLocation;
    unsigned short score;
    
    vector<Vector2i> makeLocationList(Vector2i loc)
    {
        vector<Vector2i> toRet;
        Vector2i toInsert;
        
        toInsert.x = loc.x;
        toInsert.y = loc.y - 1;// n
        toRet.push_back(toInsert);
        
        toInsert.x = loc.x + 1;
        toInsert.y = loc.y - 1;// ne
        toRet.push_back(toInsert);
        
        toInsert.x = loc.x + 1;
        toInsert.y = loc.y;// e
        toRet.push_back(toInsert);
        
        toInsert.x = loc.x + 1;
        toInsert.y = loc.y + 1;// se
        toRet.push_back(toInsert);
        
        toInsert.x = loc.x;
        toInsert.y = loc.y + 1;// s
        toRet.push_back(toInsert);
        
        toInsert.x = loc.x - 1;
        toInsert.y = loc.y + 1;// sw
        toRet.push_back(toInsert);
        
        toInsert.x = loc.x - 1;
        toInsert.y = loc.y;// w
        toRet.push_back(toInsert);
        
        toInsert.x = loc.x - 1;
        toInsert.y = loc.y - 1;// nw
        toRet.push_back(toInsert);
        
        
        return toRet;
    }
    
    void evolve()
    {
        Vector2i location;
        vector<Vector2i> toAdd, locList;
        int num;
        for(int i = 0; i < creatureLocation.size(); i++)
        {
            //cout << "Size: " << creatureLocation.size() << endl;
            locList = makeLocationList(creatureLocation[i]);
            for(int j = 0; j < 8; j++)
            {
                num = rand() % 1000;
                if(num <= mutationRate * 1000)
                {
                    if(locList[j].x > 0 && locList[j].y > 0)
                    {
                        toAdd.push_back(locList[j]);
                    }
                }
            }
        }
        
        for(int k = 0; k < toAdd.size(); k++)
        {
            creatureLocation.push_back(toAdd[k]);
        }
    }
    
    
};


#endif
