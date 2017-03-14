//  support.cpp

#include "support.h"
#include "provided.h"

////////////////////////////////////////////////
// Helper Struct
////////////////////////////////////////////////

// Constructor
Node::Node(double length, double distance, GeoCoord location)
: coordinate(location)
{
    gScore = length;
    hScore = distance;
    fScore = gScore + hScore;
}
    
// Default Constructor
Node::Node()
: gScore(0), hScore(0), fScore(0)
{
}

////////////////////////////////////////////////
// Comparison Operators
////////////////////////////////////////////////

// Comparison operator for GeoCoord
bool operator== (const GeoCoord& a, const GeoCoord& b)
{
    return (a.latitude == b.latitude) && (a.longitude == b.longitude);
}

// Reverse ordering so that lowest
// fScore will be first
bool operator<(Node a, Node b)
{
    return a.fScore > b.fScore;
}

// Used to determine if nodes have
// already been added
bool operator==(const Node a, const Node b)
{
    return a.coordinate == b.coordinate;
}
