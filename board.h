//
//  board.h
//  GOL
//
//  Created by Matthew Green on 10/22/17.
//  Copyright (c) 2017 Matthew Green. All rights reserved.
//

#ifndef GOL_board_h
#define GOL_board_h

#include <iostream>
#include <cmath>
#include <stdlib.h>
#include "creature.h"

#define UPDATELIMIT 30

using namespace std;
using namespace sf;

class board
{
public:
    board(int size, Vector2i targetLocation)
    {
        Vector2i location;
        boardSize = size;
        curBoard = new bool *[size];
        curCreature = creature(Vector2i(10,10));
        goalLocation = targetLocation;
        
        for(int i = 0; i < size; i++)
        {
            curBoard[i] = new bool [size];
        }
        
        for(int row = 0; row < size; row++)
        {
            for(int col = 0; col < size; col++)
            {
                curBoard[row][col] = false;
            }
        }
        
        for(int i = 0; i < curCreature.creatureLocation.size(); i++)
        {
            location = curCreature.creatureLocation[i];
            curBoard[location.y][location.x] = true;
        }
    }
    
public:
    int boardSize;
    bool **curBoard;
    vector<creature> pastCreature;
    creature curCreature;
    Vector2i goalLocation;
    
    bool simulate(unsigned short &lastScore)
    {
        vector<Vector2i> toChange;
        Vector2i location;
        lastScore = 60000;
        for(int row = 0; row < boardSize; row++)
        {
            for(int col = 0; col < boardSize; col++)
            {
                if(curBoard[row][col])
                {
                    if(getScore(row, col) < lastScore)
                    {
                        lastScore = getScore(row, col);
                    }
                }
                if(determineState(row, col))
                {
                    location.x = col;
                    location.y = row;
                    toChange.push_back(location);
                }
            }
        }
        
        for(int i = 0; i < toChange.size(); i++)
        {
            curBoard[toChange[i].y][toChange[i].x] ? curBoard[toChange[i].y][toChange[i].x] = false : curBoard[toChange[i].y][toChange[i].x] = true;
        }
        
        if(toChange.size() == 0 || curBoard[goalLocation.y][goalLocation.x])
        {
            return false;
        }
        //as long as the board continues to change we keep on simulating
        return true;
    }
    
    bool determineState(int row, int col)
    {
        int numClose = 0;
        bool alive = curBoard[row][col];
        //check right and left
        if(row == 0)
        {
            if(curBoard[row + 1][col])numClose++;
        }
        else if(row == boardSize - 1)
        {
            if(curBoard[row - 1][col])numClose++;
        }
        else
        {
            if(curBoard[row + 1][col])numClose++;
            if(curBoard[row - 1][col])numClose++;
        }
        //check up and down
        if(col == 0)
        {
            if(curBoard[row][col+1])numClose++;
        }
        else if(col == boardSize - 1)
        {
            if(curBoard[row][col-1])numClose++;
        }
        else
        {
            if(curBoard[row][col+1])numClose++;
            if(curBoard[row][col-1])numClose++;
        }
        
        //check diagonoals
        if(row == 0 && col == 0)
        {
            if(curBoard[row+1][col+1])numClose++;
        }
        else if(row == 0 && col == boardSize -1)
        {
            if(curBoard[row+1][col-1])numClose++;
        }
        else if(row == boardSize - 1 && col == 0)
        {
            if(curBoard[row-1][col+1])numClose++;
        }
        else if(row == boardSize - 1 && col == boardSize - 1)
        {
            if(curBoard[row-1][col-1])numClose++;
        }
        else if(row == 0)
        {
            if(curBoard[row+1][col+1])numClose++;
            if(curBoard[row+1][col-1])numClose++;
        }
        else if(row == boardSize - 1)
        {
            if(curBoard[row-1][col+1])numClose++;
            if(curBoard[row-1][col-1])numClose++;
        }
        else if(col == 0)
        {
            if(curBoard[row+1][col+1])numClose++;
            if(curBoard[row-1][col+1])numClose++;
        }
        else if(col == boardSize - 1)
        {
            if(curBoard[row+1][col-1])numClose++;
            if(curBoard[row-1][col-1])numClose++;
        }
        else
        {
            if(curBoard[row+1][col+1])numClose++;
            if(curBoard[row+1][col-1])numClose++;
            if(curBoard[row-1][col+1])numClose++;
            if(curBoard[row-1][col-1])numClose++;
        }
        
        if(alive && numClose < 2)
        {
            return true;
        }
        else if(alive && numClose > 3)
        {
            return true;
        }
        else if(!alive && numClose == 3)
        {
            return true;
        }
        
        return false;
    }
    
    void resetBoard()
    {
        for(int row = 0; row < boardSize; row++)
        {
            for(int col = 0; col < boardSize; col++)
            {
                curBoard[row][col] = false;
            }
        }
    }
    
    void setBoard()
    {
        Vector2i location;
        
        for(int i = 0; i < curCreature.creatureLocation.size(); i++)
        {
            location = curCreature.creatureLocation[i];
            //cout << location.y << " " << location.y << endl;
            curBoard[location.y][location.x] = true;
        }
    }
    
    unsigned short getScore(int row, int col)
    {
        int rowDiff = abs(row - goalLocation.y);
        int colDiff = abs(col - goalLocation.x);
        
        return rowDiff + colDiff;
    }
    
    unsigned short runSimulation(int generationLimit)
    {
        bool cont = true;
        unsigned short lastScore, lowScore;
        int genSinceUpdate = 0;
        lowScore = 60000;
        
        for(int i = 0; i < generationLimit; i++)
        {
            cont = simulate(lastScore);
            
            if(lastScore < lowScore)
            {
                genSinceUpdate = 0;
                lowScore = lastScore;
            }
            else
            {
                genSinceUpdate++;
                
                if(genSinceUpdate > UPDATELIMIT)
                {
                    break;
                }
            }
            
            if(!cont)
            {
                break;
            }
        }
        
        if(curBoard[goalLocation.y][goalLocation.x])
        {
            return 0;
        }
        else
        {
            return lowScore;
        }
    }
    
    creature getBestCreature()
    {
        creature bestCreature;
        unsigned short bestScore = 60000;
        for(int i = 0; i < pastCreature.size(); i++)
        {
            if(pastCreature[i].score < bestScore)
            {
                bestCreature = pastCreature[i];
                bestScore = bestCreature.score;
            }
        }
        
        //cout << "Best Score: " << bestScore << endl;
        
        return bestCreature;
    }
    
    creature runEvolution(int numEvolutions, int iterPerEvolution)
    {
        
        
        for(int j = 0; j < numEvolutions; j++)
        {
        
            curCreature.evolve();
            
            //cout << "size: " << curCreature.creatureLocation.size() << endl;
            setBoard();
        
            curCreature.score = runSimulation(iterPerEvolution);
            pastCreature.push_back(curCreature);
            curCreature = getBestCreature();
            resetBoard();
        }
        
        //return curCreature;
    }
};


#endif
