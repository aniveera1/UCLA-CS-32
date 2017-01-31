//
//  newSequence.cpp
//  hw1
//
//  Created by Anirudh Veeraragavan on 1/20/17.
//  Copyright © 2017 Anirudh Veeraragavan. All rights reserved.
//

#include "newSequence.h"

// Creates a sequence that can hold DEFAULT_MAX_ITEMS items
Sequence::Sequence()
{
    this->m_capacity = DEFAULT_MAX_ITEMS;
    this->m_size = 0;
    this->m_data = new ItemType[DEFAULT_MAX_ITEMS];
    
}

// Creates a sequence that can hold numberItems items
Sequence::Sequence(int numberItems)
{
    this->m_capacity = numberItems;
    this->m_size = 0;
    this->m_data = new ItemType[numberItems];
}

// Destructor
Sequence::~Sequence()
{
    delete [] m_data;
}

// Copy Constructor
Sequence::Sequence(const Sequence &old)
{
    this->m_size = old.m_size;
    this->m_capacity = old.m_capacity;
    this->m_data = new ItemType[m_capacity];
    for (int i = 0; i < old.m_size; i++)
        this->m_data[i] = old.m_data[i];
}

// Assignment Operator
Sequence& Sequence::operator=(const Sequence& src)
{
    if (&src == this)
        return (*this);
    delete [] this->m_data;
    this->m_size = src.m_size;
    this->m_capacity = src.m_capacity;
    this->m_data = new ItemType[m_capacity];
    for (int i = 0; i < src.m_size; i++)
        this->m_data[i] = src.m_data[i];
    return (*this);
}

// Return true if sequence is empty
bool Sequence::empty() const
{
    if (m_size == 0)
        return true;
    return false;
}

// Return size
int Sequence::size() const
{
    return m_size;
}

bool Sequence::insert(int pos, const ItemType& value)
{
    if (pos < 0 || pos > this->size() || this->size() == this->m_capacity)
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
    if (this->size() == this->m_capacity)
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
    int temp = this->m_size;
    this->m_size = other.m_size;
    other.m_size = temp;
    
    int temp2 = this->m_capacity;
    this->m_capacity = other.m_capacity;
    other.m_capacity = temp2;
    
    ItemType* tempPtr = other.m_data;
    other.m_data = this->m_data;
    this->m_data = tempPtr;
}
