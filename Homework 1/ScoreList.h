//
//  ScoreList.h
//  hw1
//
//  Created by Anirudh Veeraragavan on 1/19/17.
//  Copyright © 2017 Anirudh Veeraragavan. All rights reserved.
//

#ifndef ScoreList_h
#define ScoreList_h

#include "Sequence.h"
#include <limits>

const unsigned long NO_SCORE = std::numeric_limits<unsigned long>::max();

class ScoreList
{
public:
    ScoreList();       // Create an empty score list.
    
    bool add(unsigned long score);
    // If the score is valid (a value from 0 to 100) and the score list
    // has room for it, add it to the score list and return true.
    // Otherwise, leave the score list unchanged and return false.
    
    bool remove(unsigned long score);
    // Remove one instance of the specified score from the score list.
    // Return true if a score was removed; otherwise false.
    
    int size() const;  // Return the number of scores in the list.
    
    unsigned long minimum() const;
    // Return the lowest score in the score list.  If the list is
    // empty, return NO_SCORE.
    
    unsigned long maximum() const;
    // Return the highest score in the score list.  If the list is
    // empty, return NO_SCORE.
    
private:
    Sequence m_data;
    int m_size;
};

#endif /* ScoreList_h */
