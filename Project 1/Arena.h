//
//  Arena.h
//  rats
//
//  Created by Anirudh Veeraragavan on 1/10/17.
//  Copyright © 2017 Anirudh Veeraragavan. All rights reserved.
//
#ifndef ARENA_H

#define ARENA_H

#include <string>
#include "globals.h"
#include "History.h"

class Player;
class Rat;

class Arena
{
public:
    // Constructor/destructor
    Arena(int nRows, int nCols);
    ~Arena();
    
    // Accessors
    int     rows() const;
    int     cols() const;
    Player* player() const;
    int     ratCount() const;
    int     getCellStatus(int r, int c) const;
    int     numberOfRatsAt(int r, int c) const;
    void    display(std::string msg) const;
    History& history();
    
    // Mutators
    void setCellStatus(int r, int c, int status);
    bool addRat(int r, int c);
    bool addPlayer(int r, int c);
    void moveRats();
    
private:
    int     m_grid[MAXROWS][MAXCOLS];
    int     m_rows;
    int     m_cols;
    Player* m_player;
    Rat*    m_rats[MAXRATS];
    int     m_nRats;
    int     m_turns;
    History m_history;
    
    // Helper functions
    void checkPos(int r, int c) const;
    bool isPosInBounds(int r, int c) const;
};

#endif
