//
//  Player.h
//  rats
//
//  Created by Anirudh Veeraragavan on 1/10/17.
//  Copyright © 2017 Anirudh Veeraragavan. All rights reserved.
//
#ifndef PLAYER_H

#define PLAYER_H

#include <string>

class Arena;  // This is needed to let the compiler know that Arena is a
              // type name, since it's mentioned in the Player declaration.

class Player
{
public:
    // Constructor
    Player(Arena* ap, int r, int c);
    
    // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;
    
    // Mutators
    std::string dropPoisonPellet();
    std::string move(int dir);
    void   setDead();
    
private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    bool   m_dead;
};

#endif
