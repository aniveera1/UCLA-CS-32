#include "Actor.h"
#include "StudentWorld.h"
#include "GraphObject.h"
#include "GameConstants.h"
#include <list>
using namespace std;

//////////////////////////
// Actor Implementation //
//////////////////////////

Actor::Actor(StudentWorld* field, int startX, int startY, int imageID, Direction dir, int depth, double size)
: GraphObject(imageID, startX, startY, dir, depth, size), m_field(field), m_life(true)
{
}

StudentWorld* Actor::getField() const
{
    return m_field;
}

bool Actor::getLife() const
{
    return m_life;
}

void Actor::killMe()
{
    m_life = false;
}

GraphObject::Direction Actor::randomDir()
{
    int choice = randInt(0, 3);
    Direction dirChoice = none;
    switch (choice)
    {
        case 0:
            dirChoice = up;
            break;
        case 1:
            dirChoice = right;
            break;
        case 2:
            dirChoice = down;
            break;
        case 3:
            dirChoice = left;
            break;
    }
    return dirChoice;
}

//////////////////////////////
// Notmoving Implementation //
//////////////////////////////

Notmoving::Notmoving(StudentWorld* field, int startX, int startY, int imageID, Direction dir, int depth)
: Actor(field, startX, startY, imageID, dir, depth)
{
}

void Notmoving::doSomething()
{
    return;
}

bool Notmoving::doIBlock()
{
    return false;
}

void Notmoving::resetMovement()
{
    return;
}

bool Notmoving::didIMove()
{
    return false;
}

void Notmoving::stunMe()
{
    return;
}

void Notmoving::hurtMe(int amount)
{
    return;
}

int Notmoving::getHealth()
{
    return -1;
}

bool Notmoving::amIFood()
{
    return false;
}

void Notmoving::changeHealth(int amount)
{
    return;
}

bool Notmoving::amIStunableAndPoisonable()
{
    return false;
}

///////////////////////////
// Pebble Implementation //
///////////////////////////

Pebble::Pebble(StudentWorld* field, int startX, int startY)
: Notmoving(field, startX, startY, IID_ROCK, right, 1)
{
}

bool Pebble::doIBlock()
{
    return true;
}

//////////////////////////
// Water Implementation //
//////////////////////////

Water::Water(StudentWorld* field, int startX, int startY)
: Notmoving(field, startX, startY, IID_WATER_POOL, right, 2)
{
}

void Water::doSomething()
{
    getField()->stunAll(getX(), getY());
}

///////////////////////////
// Poison Implementation //
///////////////////////////

Poison::Poison(StudentWorld* field, int startX, int startY)
: Notmoving(field, startX, startY, IID_POISON, right, 2)
{
}

void Poison::doSomething()
{
    getField()->poisonAll(getX(), getY());
}

/////////////////////////
// Food Implementation //
/////////////////////////

Food::Food(StudentWorld* field, int startX, int startY, int startFood)
: Notmoving(field, startX, startY, IID_FOOD, right, 2), m_food(startFood)
{
}

bool Food::amIFood()
{
    return true;
}

int Food::getHealth()
{
    return m_food;
}

void Food::changeHealth(int amount)
{
    m_food += amount;
    if (m_food <= 0)
    {
        killMe();
    }
}

////////////////////////////////
// GrassHopper Implementation //
////////////////////////////////

Grasshopper::Grasshopper(StudentWorld* field, int startX, int startY, int ImageID)
: Actor(field, startX, startY, ImageID), m_moved(false), m_sleep(0)
{
    // Set initial health depending on type
    if (ImageID == IID_ADULT_GRASSHOPPER)
        m_health = 1600;
    else if (ImageID == IID_BABY_GRASSHOPPER)
        m_health = 500;
    
    // Random direction and distance
    Direction dir = randomDir();
    this->setDirection(dir);
    m_distance = randInt(2, 10);
}

bool Grasshopper::doIBlock()
{
    return false;
}

void Grasshopper::resetMovement()
{
    m_moved = false;
}

bool Grasshopper::didIMove()
{
    return m_moved;
}

void Grasshopper::stunMe()
{
    if (this->didIMove())
        m_sleep += 2;
}

void Grasshopper::hurtMe(int amount)
{
    m_health -= amount;
    if (m_health <= 0)
        this->killMe();
}

bool Grasshopper::amIFood()
{
    return false;
}

int Grasshopper::getHealth()
{
    return m_health;
}

void Grasshopper::changeHealth(int amount)
{
    m_health += amount;
}

void Grasshopper::moveMe()
{
    if (m_distance == 0)
    {
        Direction dir = randomDir();
        this->setDirection(dir);
        
        m_distance = randInt(2, 10);
    }
    
    if (getDirection() == GraphObject::up)
    {
        if (getField()->checkPebble(getX(), getY() - 1))
        {
            Direction dir = randomDir();
            this->setDirection(dir);
            
            m_distance = randInt(2, 10);
        }
        else
        {
            m_distance--;
            moveTo(getX(), getY() - 1);
            m_moved = true;
        }
    }
    else if (getDirection() == GraphObject::right)
    {
        if (getField()->checkPebble(getX() + 1, getY()))
        {
            Direction dir = randomDir();
            this->setDirection(dir);
            
            m_distance = randInt(2, 10);
        }
        else
        {
            m_distance--;
            moveTo(getX() + 1, getY());
            m_moved = true;
        }
    }
    else if (getDirection() == GraphObject::down)
    {
        if (getField()->checkPebble(getX(), getY() + 1))
        {
            Direction dir = randomDir();
            this->setDirection(dir);
            
            m_distance = randInt(2, 10);
        }
        else
        {
            m_distance--;
            moveTo(getX(), getY() + 1);
            m_moved = true;
        }
    }
    else if (getDirection() == GraphObject::left)
    {
        if (getField()->checkPebble(getX() - 1, getY()))
        {
            Direction dir = randomDir();
            this->setDirection(dir);
            
            m_distance = randInt(2, 10);
        }
        else
        {
            m_distance--;
            moveTo(getX() - 1, getY());
            m_moved = true;
        }
    }
}

int Grasshopper::getSleep() const
{
    return m_sleep;
}

void Grasshopper::changeSleep(int amount)
{
    m_sleep += amount;
}

bool Grasshopper::amIStunableAndPoisonable()
{
    return true;
}

bool Grasshopper::beforeDoSomethingChecks()
{
    // Ensure a grasshopper only moves
    // once per tick
    if (didIMove())
    {
        resetMovement();
        return true;
    }
    
    // Decrement health and check if dead
    changeHealth(-1);
    if (getHealth() <= 0)
    {
        getField()->addFood(this->getX(), this->getY());
        killMe();
        return true;
    }
    
    // Check whether awake or asleep
    if (getSleep() != 0)
    {
        changeSleep(-1);
        return true;
    }
    return false;
}

////////////////////////////////////
// babyGrassHopper Implementation //
////////////////////////////////////

babyGrasshopper::babyGrasshopper(StudentWorld* field, int startX, int startY)
: Grasshopper(field, startX, startY, IID_BABY_GRASSHOPPER)
{
}

void babyGrasshopper::doSomething()
{
    // Check all preconditions first
    if (beforeDoSomethingChecks())
        return;
    
    // Check whether to transform
    if (getHealth() >= 1600)
    {
        getField()->addGrasshopper(this->getX(), this->getY());
        getField()->addFood(this->getX(), this->getY());
        killMe();
        return;
    }
    
    // Eat food and decide whether to rest
    if (getField()->checkFood(this->getX(), this->getY()))
    {
        getField()->eatFood(this, this->getX(), this->getY(), IID_BABY_GRASSHOPPER);
        if (randInt(1, 2) == 2)
        {
            changeSleep(2);
            return;
        }
    }
    
    // Move around
    moveMe();
    
    // Go to sleep
    changeSleep(2);
}


/////////////////////////////////////
// adultGrassHopper Implementation //
/////////////////////////////////////

adultGrasshopper::adultGrasshopper(StudentWorld* field, int startX, int startY)
: Grasshopper(field, startX, startY, IID_ADULT_GRASSHOPPER)
{
}

void adultGrasshopper::doSomething()
{
    // Check all preconditions first
    if (beforeDoSomethingChecks())
        return;
    
    // Check insect biting
    if (randInt(1, 3) == 3)
    {
        // TODO: Randomly select an insect to bite with 50 damage
        changeSleep(2);
        return;
    }
    
    // Check jumping to another square
    if (randInt(1, 10) == 10)
    {
        // TODO: Jump to a random open square within a circular radius of 10
        changeSleep(2);
        return;
    }
    
    // Eat food and decide whether to rest
    if (getField()->checkFood(this->getX(), this->getY()))
    {
        getField()->eatFood(this, this->getX(), this->getY(), IID_BABY_GRASSHOPPER);
        if (randInt(1, 2) == 2)
        {
            changeSleep(2);
            return;
        }
    }
    
    // Move around
    moveMe();
    
    // Go to sleep
    changeSleep(2);
}

bool adultGrasshopper::amIStunableAndPoisonable()
{
    return false;
}
