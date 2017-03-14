// MapLoader.cpp

#include "provided.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cctype>
using namespace std;

////////////////////////////////////////////////
// Declaration
////////////////////////////////////////////////

class MapLoaderImpl
{
public:
    // Constructor
	MapLoaderImpl();
    
    // Destructor
	~MapLoaderImpl();
    
    // Returns true if load was
    // successful, otherwise false
	bool load(string mapFile);
    
    // Returns number of StreetSegments
	size_t getNumSegments() const;
    
    // Returns contents of a specified
    // StreetSegment
	bool getSegment(size_t segNum, StreetSegment& seg) const;
private:
    // Holds all StreetSegment data
    vector<StreetSegment*>   m_data;
    
    // Keeps track of whether data
    // has been loaded or not
    bool                    m_loadComplete;
    
    // Helper function for load()
    void getCoords(const string& coords, string& startingLat, string& startingLong, string& endingLat, string& endingLong);
    
    // Helper function for load()
    void getAttractionInfo(const string& info, string& name, string& latitude, string& longitude);
};

////////////////////////////////////////////////
// Public Functions
////////////////////////////////////////////////

// Constructor
MapLoaderImpl::MapLoaderImpl()
: m_loadComplete(false)
{
}

// Destructor
MapLoaderImpl::~MapLoaderImpl()
{
    for (long int i = m_data.size() - 1; i >= 0; i--)
        delete m_data[i];
}

// Returns true if load was
// successful, otherwise false
bool MapLoaderImpl::load(string mapFile)
{
    // If the data file has already
    // been loaded then we have
    // nothing to do
    if (m_loadComplete)
        return true;
    
    // Attempt to load file and return
    // false if failure
    ifstream datafile(mapFile);
    if (! datafile)
        return false;
    
    // Process until we reach the end
    // of the data file
    while (true)
    {
        // Get segment name
        string segmentName;
        getline(datafile, segmentName);
        
        // Check if we have reached the
        // end of the data file
        if (! datafile)
            break;
        
        // Create new StreetSegment and
        // update its name
        StreetSegment* currentStreet = new StreetSegment;
        (*currentStreet).streetName = segmentName;
        
        // Get the starting and ending
        // coordinates for the street
        string segmentCoord;
        getline(datafile, segmentCoord);
        
        string startingLat = "";
        string startingLong = "";
        string endingLat = "";
        string endingLong = "";
        
        // Parse the string to obtain the
        // proper starting and ending
        // coordinates
        getCoords(segmentCoord, startingLat, startingLong, endingLat, endingLong);
        
        GeoCoord start(startingLat, startingLong);
        GeoCoord end(endingLat, endingLong);
        GeoSegment length(start, end);
        
        // Update the StreetSegment's
        // GeoSegment
        (*currentStreet).segment = length;
        
        // Get the number of attractions
        // on the street
        int numAttractions;
        datafile >> numAttractions;
        datafile.ignore(10000, '\n');
        
        // Parse through the attractions
        for (int count = 0; count < numAttractions; count++)
        {
            Attraction current;
            
            // Get all the attraction info
            // for future parsing
            string attractionInfo;
            getline(datafile, attractionInfo);
            
            string attractionName = "";
            string attractionLatitude = "";
            string attractionLongitude = "";
            
            // Parse the string to obtain the
            // attraction's name and starting
            // coordinate
            getAttractionInfo(attractionInfo, attractionName, attractionLatitude, attractionLongitude);
            
            GeoCoord position(attractionLatitude, attractionLongitude);
            
            // Update the attraction's data
            current.name = attractionName;
            current.geocoordinates = position;
            
            // Add it to the vector
            (*currentStreet).attractions.push_back(current);
        }
        // Add the street
        m_data.push_back(currentStreet);
    }
    m_loadComplete = true;
    return true;
}

// Returns number of StreetSegments
size_t MapLoaderImpl::getNumSegments() const
{
    return m_data.size();
}

// Returns contents of a specified
// StreetSegment
bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
    // Check specified position
    if (segNum >= getNumSegments())
        return false;
    
    // Get the specified street segment
    seg = *(m_data[segNum]);
    return true;
}

////////////////////////////////////////////////
// Helper Functions
////////////////////////////////////////////////

// Helper function for load()
void MapLoaderImpl::getCoords(const string& coords, string& startingLat, string& startingLong, string& endingLat, string& endingLong)
{
    // Get latitude for the
    // starting coordinate
    int pos = 0;
    while (coords[pos] != ',')
    {
        startingLat += coords[pos];
        pos++;
    }
    
    while ((!isdigit(coords[pos])) && coords[pos] != '-')
    {
        pos++;
    }
    
    // Get longitude for the
    // starting coordinate
    while (coords[pos] != ' ')
    {
        startingLong += coords[pos];
        pos++;
    }
    
    while ((!isdigit(coords[pos])) && coords[pos] != '-')
    {
        pos++;
    }
    
    // Get latitude for the
    // ending coordinate
    while (coords[pos] != ',')
    {
        endingLat += coords[pos];
        pos++;
    }
    
    while ((!isdigit(coords[pos])) && coords[pos] != '-')
    {
        pos++;
    }
    
    // Get longitude for the
    // ending coordinate
    while (pos < coords.size())
    {
        endingLong += coords[pos];
        pos++;
    }
}

// Helper function for load()
void MapLoaderImpl::getAttractionInfo(const string& info, string& name, string& latitude, string& longitude)
{
    // Get attraction name
    int pos = 0;
    while (info[pos] != '|')
    {
        name += info[pos];
        pos++;
    }
    
    // Store attraction name
    // in lowercase form
    int namePos = 0;
    while (namePos < name.size())
    {
        if (isalpha(name[namePos]))
            name[namePos] = tolower(name[namePos]);
        namePos++;
    }
    
    while ((!isdigit(info[pos])) && info[pos] != '-')
    {
        pos++;
    }
    
    // Get latitude
    while (info[pos] != ',')
    {
        latitude += info[pos];
        pos++;
    }
    
    while ((!isdigit(info[pos])) && info[pos] != '-')
    {
        pos++;
    }
    
    // Get longitude
    while (pos < info.size())
    {
        longitude += info[pos];
        pos++;
    }
}

//******************** MapLoader functions ************************************

// These functions simply delegate to MapLoaderImpl's functions.
// You probably don't want to change any of this code.

MapLoader::MapLoader()
{
	m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
	delete m_impl;
}

bool MapLoader::load(string mapFile)
{
	return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
	return m_impl->getNumSegments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
   return m_impl->getSegment(segNum, seg);
}
