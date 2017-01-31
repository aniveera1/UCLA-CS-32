//
//  Sequence.cpp
//  project2
//
//  Created by Anirudh Veeraragavan on 1/25/17.
//  Copyright © 2017 Anirudh Veeraragavan. All rights reserved.
//

#include "Sequence.h"

////////////////////////////////////////
// Doubly Linked List Implementations //
////////////////////////////////////////

// Constructor
Sequence::doublyLinkedList::doublyLinkedList()
{
    head = nullptr;
    tail = nullptr;
}

// Destructor
Sequence::doublyLinkedList::~doublyLinkedList()
{
    if (tail == nullptr)
        return;
    Node* killMe = tail;
    Node* temp;
    
    // Traverse backwards through the linked list
    // deallocating all nodes
    while (killMe != nullptr)
    {
        temp = killMe->previous;
        delete killMe;
        killMe = temp;
    }
}

// Adds a node to the front of Sequence
void Sequence::doublyLinkedList::addToFront(const ItemType& value)
{
    Node* temp = new Node;
    
    temp->value = value;
    temp->next = head;
    temp->previous = nullptr;
    
    this->head = temp;
    
    // Check if there was already an existing
    // node that now needs to point back at this
    // new node
    if (temp->next != nullptr)
    {
        temp->next->previous = temp;
        return;
    }
    this->tail = temp;
}

// Adds a node to the end of Sequence
void Sequence::doublyLinkedList::addToRear(Node* tracker, const ItemType& value)
{
    Node* temp = new Node;
    
    temp->value = value;
    temp->next = nullptr;
    temp->previous = tracker;
    
    tracker->next = temp;
    tail = temp;
}

// Adds a node to the sequence at pos
void Sequence::doublyLinkedList::addToPos(int pos, const ItemType& value)
{
    if (head == nullptr)
        addToFront(value);
    else if (pos == 0)
        addToFront(value);
    else
    {
        // Increment till we point right at
        // the pos where we want to insert
        Node* tracker = head;
        for (int i = 0; i < pos; i++)
        {
            if (tracker->next != nullptr)
                tracker = tracker->next;
            else
                break;
        }
        
        // If pos is the last position,
        // simply add to rear
        if (tracker->next == nullptr)
        {
            addToRear(tracker, value);
            return;
        }
        
        // Allocate a new node and fix all
        // the corresponding pointers
        Node* temp = new Node;
        
        temp->value = value;
        temp->next = tracker;
        temp->previous = tracker->previous;
        
        tracker->previous = temp;
        temp->previous->next = temp;
    }
}

// Deletes the node at pos
void Sequence::doublyLinkedList::deleteItem(int pos)
{
    // If linked list is empty, return
    if (head == nullptr)
        return;
    
    // If we are deleting first item
    if (pos == 0)
    {
        // If there is only one item in list
        if (head->next == nullptr)
        {
            Node* killMe = head;
            head = nullptr;
            tail = nullptr;
            delete killMe;
            return;
        }
        
        Node* killMe = head;
        head = killMe->next;
        head->previous = nullptr;
        delete killMe;
        return;
    }
    
    Node* tracker = head;
    int count = 0;
    
    // Traverse till we reach node right before
    // the node we wish to delete
    while (tracker != nullptr)
    {
        if (tracker->next != nullptr && count == pos - 1)
            break;
        tracker = tracker->next;
        count++;
    }
    
    if (tracker != nullptr)
    {
        // If the node we wish to delete is the last node
        if (tracker->next->next != nullptr)
        {
            Node* remove = tracker->next;
            tracker->next = remove->next;
            remove->next->previous = tracker;
            delete remove;
            return;
        }
        
        Node* remove = tracker->next;
        tracker->next = nullptr;
        tail = tracker;
        delete remove;
    }
}

// Returns the value at pos
ItemType Sequence::doublyLinkedList::valueAtPos(int pos) const
{
    Node* tracker = head;
    for (int i = 0; i < pos && tracker->next != nullptr; i++)
        tracker = tracker->next;
    
    return tracker->value;
}

// Changes the item at pos to value
void Sequence::doublyLinkedList::changeValueAtPos(int pos, const ItemType& value)
{
    Node* tracker = head;
    for (int i = 0; i < pos; i++)
        tracker = tracker->next;
    
    tracker->value = value;
}

// Returns the pos where value is located
// or -1 if it is not found anywhere
int Sequence::doublyLinkedList::findItem(const ItemType& value) const
{
    int counter = 0;
    Node* tracker = head;
    
    while (tracker != nullptr)
    {
        if (tracker->value == value)
            return counter;
        tracker = tracker->next;
        counter++;
    }
    
    return -1;
}

// Accessor
void Sequence::doublyLinkedList::getHead(Node*& value) const
{
    value = head;
}

// Accessor
void Sequence::doublyLinkedList::getTail(Node*& value) const
{
    value = tail;
}

// Mutator
void Sequence::doublyLinkedList::changeHead(Node* update)
{
    head = update;
}

// Mutator
void Sequence::doublyLinkedList::changeTail(Node* update)
{
    tail = update;
}

//////////////////////////////
// Sequence Implementations //
//////////////////////////////

// Constructor
Sequence::Sequence()
{
    // Initial size is 0
    this->m_size = 0;
}

// Destructor
Sequence::~Sequence()
{
    // All this needs to do is call
    // the linked list destructor
}

// Copy Constructor
Sequence::Sequence(const Sequence& other)
{
    this->m_size = 0;
    
    // Copy over all the values from other
    for (int i = 0; i < other.m_size; i++)
    {
        ItemType insert;
        other.get(i, insert);
        this->insert(i, insert);
    }
}

// Assignment Operator
Sequence& Sequence::operator=(const Sequence& source)
{
    // Check that you are not assigning
    // the same sequence to itself
    if (this != &source)
    {
        for (int i = this->m_size - 1; i >= 0; i--)
        {
            this->erase(i);
        }
        
        this->m_size = 0;
        for (int i = 0; i < source.m_size; i++)
        {
            ItemType insert;
            source.get(i, insert);
            this->insert(i, insert);
        }
    }
    return *this;
}

// Returns true if the sequence is empty
bool Sequence::empty() const
{
    if (this->m_size == 0)
        return true;
    return false;
}

// Returns the size of the sequence
int Sequence::size() const
{
    return this->m_size;
}

// Returns true if successfully able to insert value
// into pos of the sequence
bool Sequence::insert(int pos, const ItemType& value)
{
    if (pos < 0 || pos > this->size())
        return false;
    m_data.addToPos(pos, value);
    m_size++;
    return true;
}

// Inserts value into the first position where
// value <= the value of the node at said
// position
int Sequence::insert(const ItemType& value)
{
    if (size() == 0)
    {
        m_data.addToFront(value);
        m_size++;
        return 0;
    }
    
    // Increment till we find our pos
    int pos;
    for (pos = 0; pos < size()  &&  value > m_data.valueAtPos(pos); pos++)
        ;
    m_data.addToPos(pos, value);
    m_size++;
    return pos;
}

// Deletes the item at position pos
bool Sequence::erase(int pos)
{
    if (pos < 0  ||  pos >= size())
        return false;
    m_data.deleteItem(pos);
    m_size--;
    return true;
}

// Deletes all items that have a value
// equal to value
int Sequence::remove(const ItemType& value)
{
    int count = 0;
    for (int i = m_size - 1; i >= 0; i--)
        if (m_data.valueAtPos(i) == value)
        {
            this->erase(i);
            count++;
        }
    
    return count;
}

// Gets the value at position pos and stores
// it in value
bool Sequence::get(int pos, ItemType& value) const
{
    if (pos < 0  ||  pos >= this->size())
        return false;
    value = m_data.valueAtPos(pos);
    return true;
}

// Changes the value at position pos to value
bool Sequence::set(int pos, const ItemType& value)
{
    if (pos < 0  ||  pos >= size())
        return false;
    m_data.changeValueAtPos(pos, value);
    return true;
}

// Finds the first pos in the list where the node
// equals value; if none, returns -1
int Sequence::find(const ItemType& value) const
{
    return m_data.findItem(value);
}

// Switches the contents of two sequences
void Sequence::swap(Sequence& other)
{
    // Switch sizes
    int temp = other.m_size;
    other.m_size = this->m_size;
    this->m_size = temp;
    
    // Switch head pointers
    Node* holder;
    Node* change;
    this->m_data.getHead(holder);
    other.m_data.getHead(change);
    this->m_data.changeHead(change);
    other.m_data.changeHead(holder);
    
    // Change tail pointers
    this->m_data.getTail(holder);
    other.m_data.getTail(change);
    this->m_data.changeTail(change);
    other.m_data.changeTail(holder);
}

/////////////////////////
// Sequence Algorithms //
/////////////////////////

// If seq2 is a consecutive of seq1, returns the first int
// where this starts; if not, returns -1
int subsequence(const Sequence& seq1, const Sequence& seq2)
{
    for (int pos2 = 0;;)
    {
        int pos1 = 0;
        
        // Get the first value of each seq
        ItemType seq1Tracker, seq2Tracker;
        seq1.get(pos1, seq1Tracker);
        seq2.get(pos2, seq2Tracker);
        
        // Increment through each value of seq1 until you find
        // one that matches the first value of seq2
        while (seq2Tracker != seq1Tracker && pos1 < seq1.size())
        {
            pos1++;
            seq1.get(pos1, seq1Tracker);
        }

        // If no values matched and we incremented through all
        // of seq1, break out
        if (pos1 == seq1.size())
            break;
        
        int tracker = pos1;
        
        // Increment through each value of seq1 and seq2 and
        // compare them
        while (pos1 < seq1.size() && pos2 < seq2.size())
        {
            if (seq1Tracker != seq2Tracker)
                return -1;
            
            pos1++;
            pos2++;
            seq1.get(pos1, seq1Tracker);
            seq2.get(pos2, seq2Tracker);
        }
        
        // If we have successfully incremented through all of
        // seq2, return the starting pos
        if (pos2 == seq2.size())
            return tracker;
        break;
    }
    return -1;
}

// Creates a sequence result that contains the values from
// seq1 and seq2 in interweaving order
void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result)
{
    if (seq1.size() == 0 && seq2.size() == 0)
    {
        // If both seq1 and seq2 are empty, clear result
        for (int i = result.size() - 1; i >= 0; i--)
            result.erase(i);
        return;
    }
    else if (seq1.size() == 0)
    {
        // If seq1 is empty, set result to seq2
        Sequence temp = seq2;
        result = temp;
        return;
    }
    else if (seq2.size() == 0)
    {
        // If seq2 is empty, set result to seq1
        Sequence temp = seq1;
        result = temp;
        return;
    }
    
    // Copy construct two temp seq
    Sequence temp1 = seq1;
    Sequence temp2 = seq2;
    
    // Clear result
    for (int i = result.size() - 1; i >= 0; i--)
        result.erase(i);
    
    int pos1 = 0;
    int pos2 = 0;
    int resultPos = 0;
    ItemType insert;
    
    // Increment through both seq, assigning
    // their values to result
    while (resultPos < temp1.size() + temp2.size())
    {
        if (pos1 < temp1.size())
        {
            temp1.get(pos1, insert);
            result.insert(resultPos, insert);
            resultPos++;
            pos1++;
        }
        
        if (pos2 < temp2.size())
        {
            temp2.get(pos2, insert);
            result.insert(resultPos, insert);
            resultPos++;
            pos2++;
        }
    }
}
