//
//  main.cpp
//  proj4
//
//  Created by Anirudh Veeraragavan on 3/8/17.
//  Copyright © 2017 Anirudh Veeraragavan. All rights reserved.
//

#include <iostream>
#include <cassert>
#include <string>
#include "MyMap.h"
#include "provided.h"
using namespace std;

void constructor()
{
    // Create new map
    MyMap<int, double> test;
    assert(test.size() == 0);
    
    // Test creating different maps
    MyMap<double, int> test2;
    MyMap<string, int> test3;
    MyMap<int, string> test4;
    MyMap<double, string> test5;
    MyMap<string, double> test6;
    assert(test2.size() == 0);
    assert(test3.size() == 0);
    assert(test4.size() == 0);
    assert(test5.size() == 0);
    assert(test6.size() == 0);
    
    cerr << "construct tests passed" << endl;
}

void associate()
{
    // test map 1
    MyMap<int, string> test;
    // do insertions update size
    for (int i = 0; i < 10; i++)
        test.associate(i, "hi");
    assert(test.size() == 10);
    // can you find insertions
    for (int i = 0; i < 10; i++)
        assert(*test.find(i) == "hi");
    // does clear update size
    test.clear();
    assert(test.size() == 0);
    
    // test map 2
    MyMap<int, char> test2;
    for (int i = 0; i < 10; i++)
        test2.associate(i, 'a');
    assert(test2.size() == 10);
    for (int i = 0; i < 10; i++)
        assert(*test2.find(i) == 'a');
    test2.clear();
    assert(test2.size() == 0);
    // reinsert values
    for (int i = 0; i < 10; i++)
        test2.associate(i, 'a');
    assert(test2.size() == 10);
    for (int i = 0; i < 10; i++)
        assert(*test2.find(i) == 'a');
    // are values updated
    for (int i = 0; i < 10; i++)
        test2.associate(i, 'b');
    assert(test2.size() == 10);
    for (int i = 0; i < 10; i++)
        assert(*test2.find(i) == 'b');
    // are wrong keys handled correctly
    for (int i = 10; i < 20; i++)
        assert(test2.find(i) == nullptr);
    
    cerr << "associate tests passed" << endl;
}

void tree()
{
    // Insert different values
    MyMap<int, char> test;
    test.associate(5, 'a');
    test.associate(6, 'b');
    test.associate(4, 'c');
    test.associate(7, 'd');
    test.associate(3, 'e');
    test.associate(8, 'f');
    test.associate(2, 'g');
    test.associate(9, 'h');
    test.associate(1, 'i');
    
    assert(test.size() == 9);
    assert(*test.find(1) == 'i' && *test.find(2) == 'g' && *test.find(3) == 'e' && *test.find(4) == 'c' && *test.find(5) == 'a' && *test.find(6) == 'b' && *test.find(7) == 'd' && *test.find(8) == 'f' && *test.find(9) == 'h');
    
    for (int i = 1; i < 10; i++)
        test.associate(i, 'j');
    for (int i = 1; i < 10; i++)
        assert(*test.find(i) == 'j');
    
    test.clear();
    assert(test.size() == 0);
    
    cerr << "tree tests passed" << endl;
}

void foo()
{
    MyMap<string,double> nameToGPA; // maps student name to GPA
    
    // add new items to the binary search tree-based map
    nameToGPA.associate("Carey", 3.5); // Carey has a 3.5 GPA
    nameToGPA.associate("David", 3.99); // David beat Carey
    nameToGPA.associate("Abe", 3.2); // Abe has a 3.2 GPA
    
    double* davidsGPA = nameToGPA.find("David");
    if (davidsGPA != nullptr)
        *davidsGPA = 1.5; // after a re-grade of David’s exam
    nameToGPA.associate("Carey", 4.0); // Carey deserves a 4.0
                                       // replaces old 3.5 GPA
    assert(nameToGPA.find("Linda") == nullptr);
    assert(*nameToGPA.find("Carey") == 4.0);
    assert(*nameToGPA.find("David") == 1.5);
    assert(*nameToGPA.find("Abe") == 3.2);
    assert(nameToGPA.size() == 3);
    nameToGPA.clear();
    assert(nameToGPA.size() == 0);
    
    cerr << "foo tests passed" << endl;
}

void loadMap()
{
    MapLoader test;
    assert(test.load("/Users/AniV/Desktop/mapdata.txt"));
    assert(test.getNumSegments() == 19641);
    MapLoader test2;
    assert(! test2.load("/Users/AniV/Desktop/wrong.txt"));
    
    cerr << "load tests passed" << endl;
}

void testMap()
{
    MapLoader test;
    assert(test.load("/Users/AniV/Desktop/test.txt"));
    assert(test.load("/Users/AniV/Desktop/test.txt"));
    assert(test.getNumSegments() == 10);
    
    StreetSegment temp;
    for (int i = 0; i < 10; i++)
    {
        assert(test.getSegment(i, temp));
        
        if (i < 9)
            assert(temp.attractions.size() == 0);
        else
            assert(temp.attractions.size() == 2);
        assert(temp.segment.start.latitude == i);
        assert(temp.segment.end.longitude == 4);
    }
    
    cerr << "tests passed" << endl;
}

void loadAttraction()
{
    MapLoader test;
    assert(test.load("/Users/AniV/Desktop/attest.txt"));
    AttractionMapper test2;
    test2.init(test);
    GeoCoord temp;
    assert(test2.getGeoCoord("blah", temp));
    assert(temp.latitudeText == "1" && temp.longitudeText == "-2");
    assert(test2.getGeoCoord("balh", temp));
    assert(temp.latitudeText == "2" && temp.longitudeText == "1");
    assert(test2.getGeoCoord("a", temp));
    assert(temp.latitudeText == "1" && temp.longitudeText == "1");
    assert(test2.getGeoCoord("b", temp));
    assert(temp.latitudeText == "2" && temp.longitudeText == "-2");
    assert(test2.getGeoCoord("c", temp));
    assert(temp.latitudeText == "33" && temp.longitudeText == "33");
    assert(test2.getGeoCoord("d", temp));
    assert(temp.latitudeText == "44" && temp.longitudeText == "4");
    assert(test2.getGeoCoord("e", temp));
    assert(temp.latitudeText == "5" && temp.longitudeText == "55");
    
    cerr << "load attraction tests passed" << endl;
}

void loadSegment()
{
    MapLoader test;
    assert(test.load("/Users/AniV/Desktop/attest.txt"));
    SegmentMapper test2;
    test2.init(test);
    GeoCoord temp("3", "4");
    vector<StreetSegment> values(test2.getSegments(temp));
    assert(values.size() == 6);
    for (int i = 0; i < 10; i++)
    {
        string test = to_string(i);
        GeoCoord a(test, "2");
        vector<StreetSegment> b(test2.getSegments(a));
        assert(b.size() == 1);
    }
    
    cerr << "load segment tests passed" << endl;
}

void testTree()
{
    constructor();
    associate();
    tree();
    foo();
}

void testMapLoader()
{
    loadMap();
    testMap();
}

int main() {
    testTree();
    testMapLoader();
    loadAttraction();
    loadSegment();
    
    Navigator test;
    test.loadMapData("/Users/AniV/Desktop/mapdata.txt");
    vector<NavSegment> temp;
    
    assert(test.navigate("Los Angeles Country Club", "Zeta Beta Theta Fraternity", temp) == NAV_SUCCESS);
    assert(test.navigate("1031 Broxton Avenue", "1037 Broxton Avenue", temp) == NAV_SUCCESS);
    assert(test.navigate("Drake Stadium", "Parking A", temp) == NAV_SUCCESS);
    assert(test.navigate("Holly", "Cedar", temp) == NAV_SUCCESS);
    assert(test.navigate("Los Angeles Country Club", "Bollywood Bites", temp) == NAV_SUCCESS);
    assert(test.navigate("UCLA Extension - Westwood Village Center", "Zeta Beta Theta Fraternity", temp) == NAV_SUCCESS);
    assert(test.navigate("Saint Sebastian School", "Brentwood Country Mart", temp) == NAV_SUCCESS);
    assert(test.navigate("Acacia", "Zeta Beta Theta Fraternity", temp) == NAV_SUCCESS);
    assert(test.navigate("West Pavilion", "20th Century Fox Studios Galaxy Way", temp) == NAV_SUCCESS);
    
    cerr << "all tests passed" << endl;
}
