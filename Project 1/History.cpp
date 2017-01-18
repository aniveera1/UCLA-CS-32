//
//  History.cpp
//  rats
//
//  Created by Anirudh Veeraragavan on 1/10/17.
//  Copyright © 2017 Anirudh Veeraragavan. All rights reserved.
//

#include "History.h"
#include "globals.h"
#include <iostream>
using namespace std;

// History Implementation

History::History(int nRows, int nCols)
{
    m_rows = nRows;
    m_cols = nCols;
    for (int r = 0; r < m_rows; r++)
        for (int c = 0; c < m_cols; c++)
            m_grid[r][c] = 0;
}

bool History::record(int r, int c)
{
    if (r <= 0 || c <= 0 || r > m_rows || c > m_cols)
        return false;
    m_grid[r-1][c-1]++;
    return true;
}

void History::display() const
{
    char displayGrid[MAXROWS][MAXCOLS];
    for (int r = 0; r < m_rows; r++)
        for (int c = 0; c < m_cols; c++)
            switch (m_grid[r][c])
            {
                case 0: displayGrid[r][c] = '.'; break;
                case 1: displayGrid[r][c] = 'A'; break;
                case 2: displayGrid[r][c] = 'B'; break;
                case 3: displayGrid[r][c] = 'C'; break;
                case 4: displayGrid[r][c] = 'D'; break;
                case 5: displayGrid[r][c] = 'E'; break;
                case 6: displayGrid[r][c] = 'F'; break;
                case 7: displayGrid[r][c] = 'G'; break;
                case 8: displayGrid[r][c] = 'H'; break;
                case 9: displayGrid[r][c] = 'I'; break;
                case 10: displayGrid[r][c] = 'J'; break;
                case 11: displayGrid[r][c] = 'K'; break;
                case 12: displayGrid[r][c] = 'L'; break;
                case 13: displayGrid[r][c] = 'M'; break;
                case 14: displayGrid[r][c] = 'N'; break;
                case 15: displayGrid[r][c] = 'O'; break;
                case 16: displayGrid[r][c] = 'P'; break;
                case 17: displayGrid[r][c] = 'Q'; break;
                case 18: displayGrid[r][c] = 'R'; break;
                case 19: displayGrid[r][c] = 'S'; break;
                case 20: displayGrid[r][c] = 'T'; break;
                case 21: displayGrid[r][c] = 'U'; break;
                case 22: displayGrid[r][c] = 'V'; break;
                case 23: displayGrid[r][c] = 'W'; break;
                case 24: displayGrid[r][c] = 'X'; break;
                case 25: displayGrid[r][c] = 'Y'; break;
                default: displayGrid[r][c] = 'Z'; break;
            }
    
    clearScreen();
    for (int i = 0; i < m_rows; i++)
    {
        for (int j = 0; j < m_cols; j++)
            cout << displayGrid[i][j];
        cout << endl;
    }
    cout << endl;
}
