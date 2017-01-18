//
//  History.h
//  rats
//
//  Created by Anirudh Veeraragavan on 1/10/17.
//  Copyright © 2017 Anirudh Veeraragavan. All rights reserved.
//

#ifndef HISTORY_H

#define HISTORY_H
#include "globals.h"

class History
{
public:
    // Constructor
    History(int nRows, int nCols);
    
    // Accessor
    void    display() const;
    
    // Mutators
    bool    record(int r, int c);
private:
    int     m_grid[MAXROWS][MAXCOLS];
    int     m_rows;
    int     m_cols;
};

#endif
