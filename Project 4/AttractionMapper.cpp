// AttractionMapper.cpp

#include "provided.h"
#include "MyMap.h"
#include <string>
#include <cctype>
using namespace std;

////////////////////////////////////////////////
// Declaration
////////////////////////////////////////////////

class AttractionMapperImpl
{
public:
    // Constructor
	AttractionMapperImpl();
    
    // Destructor
	~AttractionMapperImpl();
    
    // Creates data structure of
    // attraction data
	void init(const MapLoader& ml);
    
    // Retrieve the GeoCoord of a
    // specified attraction
	bool getGeoCoord(string attraction, GeoCoord& gc) const;
private:
    // Holds all attraction data
    MyMap<string, GeoCoord>     m_data;
    
    // Keeps track of whether data
    // has been loaded or not
    bool                        m_loadComplete;
};

////////////////////////////////////////////////
// Public Functions
////////////////////////////////////////////////

// Constructor
AttractionMapperImpl::AttractionMapperImpl()
: m_loadComplete(false)
{
}

// Destructor
AttractionMapperImpl::~AttractionMapperImpl()
{
}

// Creates data structure of
// attraction data
void AttractionMapperImpl::init(const MapLoader& ml)
{
    // If the data has already
    // been loaded, do nothing
    if (m_loadComplete)
        return;
    
    // Iterate through MapLoader data
    for (long int i = 0; i < ml.getNumSegments(); i++)
    {
        // Get StreetSegment
        StreetSegment current;
        ml.getSegment(i, current);
        
        // Add all attractions of
        // StreetSegment to BST
        for (int j = 0; j < current.attractions.size(); j++)
            m_data.associate(current.attractions[j].name, current.attractions[j].geocoordinates);
    }
    m_loadComplete = true;
}

// Retrieve the GeoCoord of a
// specified attraction
bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
    // Lowercase attraction name
    // for comparison
    int pos = 0;
    while (pos < attraction.size())
    {
        if (isalpha(attraction[pos]))
            attraction[pos] = tolower(attraction[pos]);
        pos++;
    }
    
    // Attempt to find the specified attraction
    const GeoCoord* temp = m_data.find(attraction);
    
    // If we found the attraction update
    // gc's value, otherwise simply
    // return false
    if (temp != nullptr)
    {
        gc = *temp;
        return true;
    }
    return false;
}

//******************** AttractionMapper functions *****************************

// These functions simply delegate to AttractionMapperImpl's functions.
// You probably don't want to change any of this code.

AttractionMapper::AttractionMapper()
{
	m_impl = new AttractionMapperImpl;
}

AttractionMapper::~AttractionMapper()
{
	delete m_impl;
}

void AttractionMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

bool AttractionMapper::getGeoCoord(string attraction, GeoCoord& gc) const
{
	return m_impl->getGeoCoord(attraction, gc);
}
