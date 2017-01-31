//
//  ScoreList.cpp
//  hw1
//
//  Created by Anirudh Veeraragavan on 1/19/17.
//  Copyright © 2017 Anirudh Veeraragavan. All rights reserved.
//

#include "ScoreList.h"
#include "Sequence.h"
using namespace std;

ScoreList::ScoreList()
{
    this->m_size = m_data.size();
}

// If the score is valid (a value from 0 to 100) and the score list
// has room for it, add it to the score list and return true.
// Otherwise, leave the score list unchanged and return false.
bool ScoreList::add(unsigned long score)
{
    if (score > 100)
        return false;
    return this->m_data.insert(this->m_data.size(), score);
}

// Remove one instance of the specified score from the score list.
// Return true if a score was removed; otherwise false.
bool ScoreList::remove(unsigned long score)
{
    int pos = this->m_data.find(score);
    if (pos == -1)
        return false;
    return this->m_data.erase(pos);
}

// Return the number of scores in the list.
int ScoreList::size() const
{
    return this->m_data.size();
}

// Return the lowest score in the score list.  If the list is
// empty, return NO_SCORE.
unsigned long ScoreList::minimum() const
{
    if (this->m_data.size() == 0)
        return NO_SCORE;
    
    unsigned long comparison;
    this->m_data.get(0, comparison);
    
    unsigned long value;
    for (int i = 1; i < this->m_data.size(); i++)
    {
        this->m_data.get(i, value);
        if (value < comparison)
            comparison = value;
    }

    return comparison;
}

// Return the highest score in the score list.  If the list is
// empty, return NO_SCORE.
unsigned long ScoreList::maximum() const
{
    if (m_data.size() == 0)
        return NO_SCORE;
    
    unsigned long comparison;
    this->m_data.get(0, comparison);
    
    unsigned long value;
    for (int i = 1; i < this->m_data.size(); i++)
    {
        this->m_data.get(i, value);
        if (value > comparison)
            comparison = value;
    }
    
    return comparison;
}
