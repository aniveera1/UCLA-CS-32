#ifndef SUPPORT_INCLUDED
#define SUPPORT_INCLUDED

// support.h

#include "provided.h"

////////////////////////////////////////////////
// Helper Struct
////////////////////////////////////////////////

struct Node
{
    // Constructor
    Node(double length, double distance, GeoCoord location);
    
    // Default Constructor
    Node();
    
    // Cost of getting from start to this node
    double         gScore;
    
    // Estimated cost to get to end from node
    double          hScore;
    
    // Total node cost
    double         fScore;
    
    // Node value
    GeoCoord    coordinate;
};

////////////////////////////////////////////////
// Comparison Operators
////////////////////////////////////////////////

// Comparison operator for GeoCoord
bool operator== (const GeoCoord& a, const GeoCoord& b);

// Reverse ordering so that lowest
// fScore will be first
bool operator<(Node a, Node b);

// Used to determine if nodes have
// already been added
bool operator==(const Node a, const Node b);

#endif // SUPPORT_INCLUDED
