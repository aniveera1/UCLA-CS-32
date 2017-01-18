//
//  Rat.cpp
//  rats
//
//  Created by Anirudh Veeraragavan on 1/10/17.
//  Copyright © 2017 Anirudh Veeraragavan. All rights reserved.
//
#include "Rat.h"
#include "Arena.h"
#include "globals.h"
#include <iostream>
using namespace std;

// Rat Implementation

Rat::Rat(Arena* ap, int r, int c)
{
    if (ap == nullptr)
    {
        cout << "***** A rat must be created in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        cout << "***** Rat created with invalid coordinates (" << r << ","
        << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_health = INITIAL_RAT_HEALTH;
    m_idleTurnsRemaining = 0;
}

int Rat::row() const
{
    return m_row;
}

int Rat::col() const
{
    return m_col;
}

bool Rat::isDead() const
{
    return m_health == 0;
}

void Rat::move()
{
    if (m_idleTurnsRemaining > 0)
    {
        m_idleTurnsRemaining--;
        return;
    }
    
    // Attempt to move in a random direction; if we can't move, don't move
    if (attemptMove(*m_arena, randInt(0, NUMDIRS-1), m_row, m_col))
    {
        if (m_arena->getCellStatus(m_row, m_col) == HAS_POISON)
        {
            m_arena->setCellStatus(m_row, m_col, EMPTY);
            if (m_health == INITIAL_RAT_HEALTH)
                m_arena->history().record(m_row, m_col);
            m_health--;
        }
    }
    
    if (m_health < INITIAL_RAT_HEALTH)
        m_idleTurnsRemaining = POISONED_IDLE_TIME;
}
