// Navigator.cpp

#include "provided.h"
#include "MyMap.h"
#include "support.h"
#include <string>
#include <vector>
#include <list>
#include <iostream>
using namespace std;

////////////////////////////////////////////////
// Declaration
////////////////////////////////////////////////

class NavigatorImpl
{
public:
    // Constructor
    NavigatorImpl();
    
    // Destructor
    ~NavigatorImpl();
    
    // Loads required data and
    // initializes data structures
    bool loadMapData(string mapFile);
    
    // Finds the optimal route between
    // two locations, if a route exists,
    // and returns it
    NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
private:
    // Map of attractions to
    // respective GeoCoordinate
    AttractionMapper        m_attractionData;
    
    // Map of GeoCoordinate to
    // all associated StreetSegments
    SegmentMapper           m_streetData;
    
    // Keeps track of whether data
    // has been loaded or not
    bool                    m_loadComplete;
};

////////////////////////////////////////////////
// Helper Functions
////////////////////////////////////////////////

// Finds the node with the lowest fScore
Node findLowest(list<Node> values)
{
    // Define iterators
    list<Node>::iterator current;
    list<Node>::iterator next;
    
    // Start at the first value
    current = values.begin();
    Node lowestSoFar = *current;
    while (current != values.end())
    {
        next = ++current;
        --current;
        Node temp = *current;
        
        // Update smallest Node
        if (temp.fScore < lowestSoFar.fScore)
            lowestSoFar = temp;
        
        // Go to next value
        current = next;
    }
    // Pop node off
    return lowestSoFar;
}

// Returns an iterator to the node
// with the smallest fScore for deletion
list<Node>::iterator popLowest(list<Node> values)
{
    // Define iterators
    list<Node>::iterator current;
    list<Node>::iterator next;
    list<Node>::iterator toDelete;
    
    // Start at the first value
    current = values.begin();
    toDelete = current;
    Node lowestSoFar = *current;
    while (current != values.end())
    {
        next = ++current;
        --current;
        Node temp = *current;
        
        // Update smallest Node
        if (temp.fScore < lowestSoFar.fScore)
        {
            lowestSoFar = temp;
            toDelete = current;
        }
        
        // Go to next value
        current = next;
    }
    // Pop node off
    return toDelete;
}

// Check if a node already exists in a list
// and if so, has a lower fScore. Return true
// if node passes tests, return false otherwise

bool checkForNode(list<Node> set, Node target)
{
    // Define iterators
    list<Node>::iterator current;
    list<Node>::iterator next;
    
    // Start at the first value
    current = set.begin();
    while (current != set.end())
    {
        next = ++current;
        --current;
        Node temp = *current;
        
        // Check if node has same coord as target
        // and if node has lower fScore than target
        if (temp.coordinate == target.coordinate && temp.fScore <= target.fScore)
            return false;
        
        current = next;
    }
    // Node passed check
    return true;
}

////////////////////////////////////////////////
// Public Functions
////////////////////////////////////////////////

// Constructor
NavigatorImpl::NavigatorImpl()
: m_loadComplete(false)
{
}

// Destructor
NavigatorImpl::~NavigatorImpl()
{
}

// Loads required data and
// initializes data structures
bool NavigatorImpl::loadMapData(string mapFile)
{
    // Load data, and return
    // false if error
    MapLoader data;
    if (! data.load(mapFile))
        return false;
    
    // Load attraction data
    m_attractionData.init(data);
    
    // Load StreetSegment data
    m_streetData.init(data);
    
    // Success
    return true;
}

NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
    // Check if specified attractions exist
    GeoCoord startCoordinate;
    GeoCoord endCoordinate;
    if (! (m_attractionData.getGeoCoord(start, startCoordinate)))
        return NAV_BAD_SOURCE;
    if (! (m_attractionData.getGeoCoord(end, endCoordinate)))
        return NAV_BAD_DESTINATION;
    
    // Need to be evaluated
    list<Node> openSet;
    
    // Already evaluated
    list<Node> closedSet;
    
    // Create startNode and add to
    // needs to be evaluated
    Node startNode(0, 0, startCoordinate);
    openSet.push_back(startNode);
    
    while (! openSet.empty())
    {
        // Finds most promising node and pops
        // it off the list
        Node current = findLowest(openSet);
        openSet.remove(current);
        
        // Find neighbors
        vector<StreetSegment> potentialStreets = m_streetData.getSegments(current.coordinate);
        
        for (int i = 0; i < potentialStreets.size(); i++)
        {
            if (! potentialStreets[i].attractions.empty())
                for (int j = 0; j < potentialStreets[i].attractions.size(); j++)
                    if (endCoordinate == potentialStreets[i].attractions[j].geocoordinates)
                        return NAV_SUCCESS;
            
            GeoCoord nodeCoord;
            Node neighbor;
            if (current.coordinate == potentialStreets[i].segment.start)
                nodeCoord = potentialStreets[i].segment.end;
            else if (current.coordinate == potentialStreets[i].segment.end)
                nodeCoord = potentialStreets[i].segment.start;
            else
            {
                {
                    nodeCoord = potentialStreets[i].segment.start;
                    
                    // Set neighbor's values
                    neighbor.coordinate = nodeCoord;
                    neighbor.gScore = current.gScore + distanceEarthMiles(current.coordinate, neighbor.coordinate);
                    neighbor.hScore = distanceEarthMiles(neighbor.coordinate, endCoordinate);
                    neighbor.fScore = neighbor.gScore + neighbor.hScore;
                    
                    // Check if there exists a better path through
                    // this node
                    if (checkForNode(openSet, neighbor) && checkForNode(closedSet, neighbor))
                        openSet.push_back(neighbor);
                }
                {
                    nodeCoord = potentialStreets[i].segment.end;
                    
                    // Set neighbor's values
                    neighbor.coordinate = nodeCoord;
                    neighbor.gScore = current.gScore + distanceEarthMiles(current.coordinate, neighbor.coordinate);
                    neighbor.hScore = distanceEarthMiles(neighbor.coordinate, endCoordinate);
                    neighbor.fScore = neighbor.gScore + neighbor.hScore;
                    
                    // Check if there exists a better path through
                    // this node
                    if (checkForNode(openSet, neighbor) && checkForNode(closedSet, neighbor))
                        openSet.push_back(neighbor);
                }
                continue;
            }
            // Set neighbor's values
            neighbor.coordinate = nodeCoord;
            neighbor.gScore = current.gScore + distanceEarthMiles(current.coordinate, neighbor.coordinate);
            neighbor.hScore = distanceEarthMiles(neighbor.coordinate, endCoordinate);
            neighbor.fScore = neighbor.gScore + neighbor.hScore;
            
            // Check if there exists a better path through
            // this node
            if (checkForNode(openSet, neighbor) && checkForNode(closedSet, neighbor))
                openSet.push_back(neighbor);
        }
        closedSet.push_back(current);
    }
    return NAV_NO_ROUTE;
}

//******************** Navigator functions ************************************

// These functions simply delegate to NavigatorImpl's functions.
// You probably don't want to change any of this code.

Navigator::Navigator()
{
    m_impl = new NavigatorImpl;
}

Navigator::~Navigator()
{
    delete m_impl;
}

bool Navigator::loadMapData(string mapFile)
{
    return m_impl->loadMapData(mapFile);
}

NavResult Navigator::navigate(string start, string end, vector<NavSegment>& directions) const
{
    return m_impl->navigate(start, end, directions);
}

