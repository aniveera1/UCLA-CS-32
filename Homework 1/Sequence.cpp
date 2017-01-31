//
//  Sequence.cpp
//  hw1
//
//  Created by Anirudh Veeraragavan on 1/19/17.
//  Copyright © 2017 Anirudh Veeraragavan. All rights reserved.
//

#include "Sequence.h"

// Constructor
Sequence::Sequence()
{
    this->m_size = 0;
}

// Return true if sequence is empty
bool Sequence::empty() const
{
    if (this->m_size == 0)
        return true;
    return false;
}

// Return size
int Sequence::size() const
{
    return this->m_size;
}

// If there is space, insert value into the specificed pos
bool Sequence::insert(int pos, const ItemType& value)
{
    if (pos < 0 || pos > this->size() || this->size() == DEFAULT_MAX_ITEMS)
        return false;
    
    int tracker = this->size();
    
    while (tracker != pos)
    {
        this->m_data[tracker] = this->m_data[tracker - 1];
        tracker--;
    }
    
    this->m_data[pos] = value;
    this->m_size++;
    return true;
}

int Sequence::insert(const ItemType& value)
{
    if (this->size() == DEFAULT_MAX_ITEMS)
        return -1;
    
    int target = this->size();
    
    for (int i = 0; i < this->size(); i++)
        if (value <= this->m_data[i])
        {
            target = i;
            break;
        }
    
    int tracker = this->size();
    
    while (tracker != target)
    {
        this->m_data[tracker] = this->m_data[tracker - 1];
        tracker--;
    }
    
    this->m_data[target] = value;
    this->m_size++;
    return target;
}

// Remove the item at position pos
bool Sequence::erase(int pos)
{
    if (pos < 0 || pos >= this->size())
        return false;
    
    int tracker = pos;
    this->m_size--;
    while (tracker != this->size())
    {
        this->m_data[tracker] = this->m_data[tracker + 1];
        tracker++;
    }
    return true;
}

// Remove all items that == value
int Sequence::remove(const ItemType& value)
{
    int tracker = 0;
    
    for (int i = this->size() - 1; i >= 0; i--)
        if (value == this->m_data[i])
        {
            this->erase(i);
            tracker++;
        }
    
    return tracker;
}

// Copy the item at position pos into value
bool Sequence::get(int pos, ItemType& value) const
{
    if (pos < 0 || pos >= this->size())
        return false;
    
    value = this->m_data[pos];
    return true;
}

// Replace the item at position pos with value
bool Sequence::set(int pos, const ItemType& value)
{
    if (pos < 0 || pos >= this->size())
        return false;
    
    this->m_data[pos] = value;
    return true;
}

// Return the smallest position where value == item at said position
int Sequence::find(const ItemType& value) const
{
    for (int i = 0; i < this->size(); i++)
    {
        if (this->m_data[i] == value)
            return i;
    }
    return -1;
}

// Exchange the contents of this sequence with the other one.
void Sequence::swap(Sequence& other)
{
    int max = this->size();
    if (max < other.m_size)
        max = other.m_size;
    
    ItemType swap;
    for (int i = 0; i < max; i++)
    {
        swap = this->m_data[i];
        this->m_data[i] = other.m_data[i];
        other.m_data[i] = swap;
    }
    
    int temp = this->m_size;
    this->m_size = other.m_size;
    other.m_size = temp;
}
