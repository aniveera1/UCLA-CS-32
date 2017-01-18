//
//  Game.h
//  rats
//
//  Created by Anirudh Veeraragavan on 1/10/17.
//  Copyright © 2017 Anirudh Veeraragavan. All rights reserved.
//
#ifndef GAME_H

#define GAME_H

#include <string>

class Arena;

class Game
{
public:
    // Constructor/destructor
    Game(int rows, int cols, int nRats);
    ~Game();
    
    // Mutators
    void play();
    
private:
    Arena* m_arena;
    
    // Helper functions
    std::string takePlayerTurn();
};

// Helper Functions
bool decodeDirection(char ch, int& dir);
int computeDanger(const Arena& a, int r, int c);
bool recommendMove(const Arena& a, int r, int c, int& bestDir);

#endif
