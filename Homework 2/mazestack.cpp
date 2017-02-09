//
//  mazestack.cpp
//  hw2
//
//  Created by Anirudh Veeraragavan on 2/2/17.
//  Copyright © 2017 Anirudh Veeraragavan. All rights reserved.
//

#include <string>
#include <stack>
using namespace std;

const char DISCOVERED = '*';

// Class for coordinates
class Coord
{
public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
private:
    int m_r;
    int m_c;
};

// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise
bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    stack<Coord> coordStack;
    
    // Start with initial position
    Coord init(sr, sc);
    coordStack.push(init);
    maze[init.r()][init.c()] = DISCOVERED;
    
    while (! coordStack.empty())
    {
        Coord current = coordStack.top();
        coordStack.pop();
        
        // If match, then we have found ending
        if (current.r() == er && current.c() == ec)
            return true;
        
        // Check North
        if (maze[current.r() - 1][current.c()] == '.')
        {
            Coord north(current.r() - 1, current.c());
            coordStack.push(north);
            maze[current.r() - 1][current.c()] = DISCOVERED;
        }
        
        // Check East
        if (maze[current.r()][current.c() + 1] == '.')
        {
            Coord east(current.r(), current.c() + 1);
            coordStack.push(east);
            maze[current.r()][current.c() + 1] = DISCOVERED;
        }
        
        // Check South
        if (maze[current.r() + 1][current.c()] == '.')
        {
            Coord south(current.r() + 1, current.c());
            coordStack.push(south);
            maze[current.r() + 1][current.c()] = DISCOVERED;
        }
        
        // Check West
        if (maze[current.r()][current.c() - 1] == '.')
        {
            Coord west(current.r(), current.c() - 1);
            coordStack.push(west);
            maze[current.r()][current.c() - 1] = DISCOVERED;
        }
    }
    return false;
}
