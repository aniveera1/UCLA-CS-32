#ifndef ACTOR_H_
#define ACTOR_H_

#include "GameConstants.h"
#include "GraphObject.h"

class StudentWorld;

class Actor : public GraphObject
{
public:
    // Constructor
    Actor(StudentWorld* field, int startX, int startY, int imageID, Direction dir = right, int depth = 0, double size = 0.25);
    
    // Pure virtual functions
    virtual void doSomething() = 0;
    virtual bool doIBlock() = 0;
    virtual void resetMovement() = 0;
    virtual bool didIMove() = 0;
    virtual void stunMe() = 0;
    virtual void hurtMe(int amount = 0) = 0;
    virtual bool amIFood() = 0;
    virtual int getHealth() = 0;
    virtual void changeHealth(int amount) = 0;
    virtual bool amIStunableAndPoisonable() = 0;
    
    // Member functions
    StudentWorld* getField() const;
    bool getLife() const;
    void killMe();
protected:
    // Helper function
    Direction randomDir();
private:
    StudentWorld*   m_field;
    bool            m_life;
};

class Notmoving : public Actor
{
public:
    // Constructor
    Notmoving(StudentWorld* field, int startX, int startY, int imageID, Direction dir, int depth);
    
    // Virtual functions
    virtual void doSomething();
    virtual bool doIBlock();
    virtual void resetMovement();
    virtual bool didIMove();
    virtual void stunMe();
    virtual void hurtMe(int amount = 0);
    virtual bool amIFood();
    virtual int getHealth();
    virtual void changeHealth(int amount);
    virtual bool amIStunableAndPoisonable();
};

class Pebble : public Notmoving
{
public:
    // Constructor
    Pebble(StudentWorld* field, int startX, int startY);
    
    // Virtual functions
    virtual bool doIBlock();
};

class Water : public Notmoving
{
public:
    // Constructor
    Water(StudentWorld* field, int startX, int startY);
    
    // Virtual functions
    virtual void doSomething();
};

class Poison : public Notmoving
{
public:
    // Constructor
    Poison(StudentWorld* field, int startX, int startY);
    
    // Virtual functions
    virtual void doSomething();
};

class Food : public Notmoving
{
public:
    // Constructor
    Food(StudentWorld* field, int startX, int startY, int startFood = 6000);
    
    // Virtual functions
    virtual bool amIFood();
    virtual int getHealth();
    virtual void changeHealth(int amount);
    
    // Member functions
private:
    int     m_food;
};

class Grasshopper : public Actor
{
public:
    // Constructor
    Grasshopper(StudentWorld* field, int startX, int startY, int ImageID);
    
    // Virtual functions
    virtual bool doIBlock();
    virtual void resetMovement();
    virtual bool didIMove();
    virtual void stunMe();
    virtual void hurtMe(int amount = 150);
    virtual bool amIFood();
    virtual int getHealth();
    virtual void changeHealth(int amount);
    virtual bool amIStunableAndPoisonable();
protected:
    void moveMe();
    int getSleep() const;
    void changeSleep(int amount);
    bool beforeDoSomethingChecks();
private:
    int     m_health;
    int     m_distance;
    bool    m_moved;
    int     m_sleep;
};

class babyGrasshopper : public Grasshopper
{
public:
    // Constructor
    babyGrasshopper(StudentWorld* field, int startX, int startY);
    
    // Virtual functions
    virtual void doSomething();
};

class adultGrasshopper : public Grasshopper
{
public:
    // Constructor
    adultGrasshopper(StudentWorld* field, int startX, int startY);
    
    // Virtual functions
    virtual void doSomething();
    virtual bool amIStunableAndPoisonable();
};

#endif // ACTOR_H_
