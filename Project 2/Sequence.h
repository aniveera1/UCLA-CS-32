//
//  Sequence.h
//  project2
//
//  Created by Anirudh Veeraragavan on 1/25/17.
//  Copyright © 2017 Anirudh Veeraragavan. All rights reserved.
//

// Header Guards
#ifndef Sequence_h
#define Sequence_h

#include <string>

typedef std::string ItemType;

// Class Declaration
class Sequence
{
public:
    // Constructor
    Sequence();
    
    // Copy Constructor
    Sequence(const Sequence& other);
    
    // Assignment Operator
    Sequence& operator=(const Sequence& source);
    
    // Member Functions
    bool empty() const;
    int size() const;
    bool insert(int pos, const ItemType& value);
    int insert(const ItemType& value);
    bool erase(int pos);
    int remove(const ItemType& value);
    bool get(int pos, ItemType& value) const;
    bool set(int pos, const ItemType& value);
    int find(const ItemType& value) const;
    void swap(Sequence& other);
    
    // Deconstructor
    ~Sequence();
private:
    struct Node
    {
        ItemType value;
        Node* next;
        Node* previous;
    };
    
    class doublyLinkedList
    {
    public:
        // Constructor
        doublyLinkedList();
        
        // Member Function
        void addToFront(const ItemType& value);
        void addToRear(Node* tracker, const ItemType& value);
        void addToPos(int pos, const ItemType& value);
        void deleteItem(int pos);
        ItemType valueAtPos(int pos) const;
        void changeValueAtPos(int pos, const ItemType& value);
        int findItem(const ItemType& value) const;
        
        // Accessors
        void getHead(Node*& value) const;
        void getTail(Node*& value) const;
        
        // Mutators
        void changeHead(Node* update);
        void changeTail(Node* update);
        
        // Destructor
        ~doublyLinkedList();
    private:
        Node* head;
        Node* tail;
    };
    
    doublyLinkedList m_data;
    int m_size;
};

// Utility Function Declarations
int subsequence(const Sequence& seq1, const Sequence& seq2);
void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result);

#endif /* Sequence_h */
