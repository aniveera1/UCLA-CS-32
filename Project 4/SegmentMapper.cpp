// SegmentMapper.cpp

#include "provided.h"
#include "MyMap.h"
#include <vector>
using namespace std;

////////////////////////////////////////////////
// Declaration
////////////////////////////////////////////////

class SegmentMapperImpl
{
public:
    // Constructor
	SegmentMapperImpl();
    
    // Destructor
	~SegmentMapperImpl();
    
    // Creates data structure of all
    // StreetSegments associated with
    // each GeoCoordinate
	void init(const MapLoader& ml);
    
    // Retrieves all StreetSegments
    // associated with a specified
    // GeoCoordinate
	vector<StreetSegment> getSegments(const GeoCoord& gc) const;
private:
    // Associations of StreetSegments with
    // a specified GeoCoordinate
    MyMap<string, vector<StreetSegment>>    m_data;
    
    // Keeps track of whether data
    // has been loaded or not
    bool                        m_loadComplete;
};

////////////////////////////////////////////////
// Public Functions
////////////////////////////////////////////////

// Constructor
SegmentMapperImpl::SegmentMapperImpl()
: m_loadComplete(false)
{
}

// Destructor
SegmentMapperImpl::~SegmentMapperImpl()
{
}

// Creates data structure of all
// StreetSegments associated with
// each GeoCoordinate
void SegmentMapperImpl::init(const MapLoader& ml)
{
    // Only load data once
    if (m_loadComplete)
        return;
    
    // Iterate through all StreetSegments
    for (long int i = 0; i < ml.getNumSegments(); i++)
    {
        // Keys are compared by a string
        // of latitude value + longitude value
        
        // Get StreetSegment
        StreetSegment current;
        ml.getSegment(i, current);
        
        // Determine if there is
        // already an association
        string currentStartText = current.segment.start.latitudeText + current.segment.start.longitudeText;
        vector<StreetSegment>* values = m_data.find(currentStartText);
        
        // If none, create new array
        // and add it to map
        if (values == nullptr)
        {
            vector<StreetSegment> temp;
            temp.push_back(current);
            m_data.associate(currentStartText, temp);
        }
        // Otherwise, add to existing array
        else
            values->push_back(current);
        
        // Repeat for all datasets
        string currentEndText = current.segment.end.latitudeText + current.segment.end.longitudeText;
        values = m_data.find(currentEndText);
        
        if (values == nullptr)
        {
            vector<StreetSegment> temp;
            temp.push_back(current);
            m_data.associate(currentEndText, temp);
        }
        else
            values->push_back(current);
        
        //Iterate through all attractions
        for (int j = 0; j < current.attractions.size(); j++)
        {
            string attractionText = current.attractions[j].geocoordinates.latitudeText + current.attractions[j].geocoordinates.longitudeText;
            values = m_data.find(attractionText);
            
            if (values == nullptr)
            {
                vector<StreetSegment> temp;
                temp.push_back(current);
                m_data.associate(attractionText, temp);
            }
            else
                values->push_back(current);
        }
    }
    m_loadComplete = true;
}

// Retrieves all StreetSegments
// associated with a specified
// GeoCoordinate
vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
    // Look for StreetSegments
    string searchText = gc.latitudeText + gc.longitudeText;
    const vector<StreetSegment>* values = m_data.find(searchText);
    
    // Check against nullptr accessing
    if (values == nullptr)
    {
        vector<StreetSegment> temp;
        return temp;
    }
    vector<StreetSegment> temp(*values);
    return temp;
}

//******************** SegmentMapper functions ********************************

// These functions simply delegate to SegmentMapperImpl's functions.
// You probably don't want to change any of this code.

SegmentMapper::SegmentMapper()
{
	m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
	delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
	return m_impl->getSegments(gc);
}
