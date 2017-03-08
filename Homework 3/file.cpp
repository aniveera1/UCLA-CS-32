//
//  main.cpp
//  hw3
//
//  Created by Anirudh Veeraragavan on 2/7/17.
//  Copyright © 2017 Anirudh Veeraragavan. All rights reserved.
//

#include <iostream>
#include <string>
using namespace std;

// File Class Declaration
class File
{
public:
    // Constructor
    File(string name);
    
    // Destructor
    virtual ~File() = 0;
    
    // Member Functions
    string name() const;
    virtual void open() const = 0;
    virtual void redisplay() const = 0;
private:
    string m_name;
};

// File Class Implementation
File::File(string name)
 : m_name(name){}

File::~File()
{}

string File::name() const
{
    return m_name;
}

// TextMsg Class Declaration
class TextMsg: public File
{
public:
    // Constructor
    TextMsg(string name);
    
    // Destructor
    virtual ~TextMsg();
    
    // Member Functions
    virtual void open() const;
    virtual void redisplay() const;
};

// TextMsg Class Implementation
TextMsg::TextMsg(string name)
: File(name){}

TextMsg::~TextMsg()
{
    cout << "Destroying " << this->name() << ", a text message" << endl;
}

void TextMsg::open() const
{
    cout << "open text message";
}

void TextMsg::redisplay() const
{
    cout << "refresh the screen";
}

// Video Class Declaration
class Video: public File
{
public:
    // Constructor
    Video(string name, int time);
    
    // Destructor
    ~Video();
    
    // Member Functions
    virtual void open() const;
    virtual void redisplay() const;
private:
    int m_time;
};

// Video Class Implementation
Video::Video(string name, int time)
: File(name), m_time(time){}

Video::~Video()
{
    cout << "Destroying " << this->name() << ", a video" << endl;
}

void Video::open() const
{
    cout << "play " << m_time << " second video";
}

void Video::redisplay() const
{
    cout << "replay video";
}

// Picture Class Declaration
class Picture: public File
{
public:
    // Constructor
    Picture(string name);
    
    // Destructor
    ~Picture();
    
    // Member Functions
    virtual void open() const;
    virtual void redisplay() const;
};

// Picture Class Implementation
Picture::Picture(string name)
: File(name){}

Picture::~Picture()
{
    cout << "Destroying the picture " << this->name() << endl;
}

void Picture::open() const
{
    cout << "show picture";
}

void Picture::redisplay() const
{
    cout << "refresh the screen";
}
