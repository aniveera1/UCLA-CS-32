#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "Compiler.h"

///////////////////////////////////////////////////
// Global Constants
///////////////////////////////////////////////////

const int BASE_HEALTH                       = 100;
const int ANTHILL_STARTING_HEALTH           = 8999;
const int PHEROMONE_STARTING_HEALTH         = 256;
const int ANT_STARTING_HEALTH               = 1500;
const int BABYGRASSHOPPER_STARTING_HEALTH   = 500;
const int ADULTGRASSHOPPER_STARTING_HEALTH  = 1600;

const int ANTHILL_FOOD_EATING_CAPACITY      = 10000;
const int ENERGY_NEEDED_TO_CREATE_ANT       = 2000;
const int ANT_FOOD_EATING_CAPACITY          = 100;
const int ANT_FOOD_PICKUP_CAPACITY          = 400;
const int ANT_FOOD_CAPACITY                 = 1800;

const int INSECT_POISON_DAMAGE              = -150;
const int ANT_BITE_DAMAGE                   = 15;
const int GRASSHOPPER_BITE_DAMAGE           = 50;

const int FOOD_FROM_DEAD_INSECT             = 100;

////////////////////////////////////////////////////

class StudentWorld;

class Actor : public GraphObject
{
public:
    // Constructor
    Actor(StudentWorld* world, int startX, int startY, Direction startDir, int imageID, int depth);
    
    // Pure virtual functions
    virtual void    doSomething() = 0;
    
    // Actions
    virtual void    getBitten(int amt);
    virtual void    getPoisoned();
    virtual void    getStunned();
    virtual void    resetMovement();
    virtual void    changeFood(int amt);
    virtual void    updateEnergy(int amt);
    
    // Checks
    virtual bool    isEdible() const;
    virtual bool    isPheromone(int colony) const;
    virtual bool    isEnemy(int colony) const;
    virtual bool    isDangerous(int colony) const;
    virtual bool    isMyAntHill(int colony) const;
    virtual bool    isPoison() const;
    virtual bool    didIMove() const;
    virtual bool    blocksMovement() const;
    bool            amIAlive() const;
    
    // Accessors
    virtual int     getColony() const;
    virtual void    getFood(int &amt) const;
    virtual int     getEnergy() const;
protected:
    // Helper functions
    StudentWorld*   getWorld() const;
    void            killMe();
    Direction       randomDir();
private:
    StudentWorld*   m_field;
    bool            m_life;
};

class Pebble : public Actor
{
public:
    // Constructor
    Pebble(StudentWorld* sw, int startX, int startY);
    
    // Member functions
    virtual void    doSomething();
    virtual bool    blocksMovement() const;
};

class EnergyHolder : public Actor
{
public:
    // Constructor
    EnergyHolder(StudentWorld* sw, int startX, int startY, Direction startDir, int energy, int imageID, int depth);
    
    virtual void    doSomething();
    
    // Accessors
    virtual int     getEnergy() const;
    virtual void    getFood(int &amt) const;
    
    // Actions
    virtual void    updateEnergy(int amt);
    virtual void    changeFood(int amt);
    void            addFood(int amt);       // Allocates a new food object or adds more to an existing one
    int             pickupFood(int amt);
    int             pickupAndEatFood(int amt);
private:
    int             m_health;
    int             m_food;
};

class Food : public EnergyHolder
{
public:
    // Constructor
    Food(StudentWorld* sw, int startX, int startY, int energy);
    
    // Member functions
    virtual void    doSomething();
    virtual bool    isEdible() const;
};

class AntHill : public EnergyHolder
{
public:
    // Constructor
    AntHill(StudentWorld* sw, int startX, int startY, int colony, Compiler* program);
    
    // Member functions
    virtual void    doSomething();
    virtual bool    isMyAntHill(int colony) const;
private:
    int             m_colony;
    Compiler*       m_behavior;
};

class Pheromone : public EnergyHolder
{
public:
    // Constructor
    Pheromone(StudentWorld* sw, int startX, int startY, int colony, int imageID);
    
    // Member functions
    virtual void    doSomething();
    virtual bool    isPheromone(int colony) const;
private:
    int         m_colony;
};

class TriggerableActor : public Actor
{
public:
    // Constructor
    TriggerableActor(StudentWorld* sw, int x, int y, int imageID);
    
    virtual void doSomething();
    
    // Member functions
    virtual bool    isDangerous(int colony) const;
};

class WaterPool : public TriggerableActor
{
public:
    // Constructor
    WaterPool(StudentWorld* sw, int x, int y);
    
    // Member functions
    virtual void    doSomething();
};

class Poison : public TriggerableActor
{
public:
    // Constructor
    Poison(StudentWorld* sw, int x, int y);
    
    // Member functions
    virtual void    doSomething();
    virtual bool    isPoison() const;
    virtual void    resetMovement();
    virtual bool    didIMove() const;
private:
    bool        m_moved;
};

class Insect : public EnergyHolder
{
public:
    // Constructor
    Insect(StudentWorld* world, int startX, int startY, int energy, int imageID);
    
    virtual void doSomething();
    
    // Actions
    virtual void    getBitten(int amt);
    virtual void    getPoisoned();
    virtual void    getStunned();
    virtual void    resetMovement();
    
    // Checks
    virtual bool    isEnemy(int colony);
    virtual bool    isDangerous(int colony) const;
    virtual bool    didIMove() const;
    
    // Accessor
    virtual int     getColony() const;
protected:
    // Member functions
    int     getSleep() const;
    void    changeSleep(int amount);
    bool    beforeDoSomethingChecks();
    void    IMoved();
private:
    int     m_sleep;
    bool    m_moved;
};

class Ant : public Insect
{
public:
    // Constructor
    Ant(StudentWorld* sw, int startX, int startY, int colony, Compiler* program, int imageID);
    
    // Member functions
    virtual void    doSomething();
    virtual bool    isEnemy(int colony) const;
    virtual int     getColony() const;
    virtual bool    moveForwardIfPossible();
    virtual void    getBitten(int amt);
private:
    // Helper functions
    bool            processCmd(Compiler::Command cmd);
    Compiler*       getFile();
    
    int     m_instruction;
    int     m_random;
    int     m_colony;
    Compiler* m_file;
    bool    m_blocked;
    bool    m_bit;
};

class Grasshopper : public Insect
{
public:
    // Constructor
    Grasshopper(StudentWorld* sw, int startX, int startY, int energy, int imageID);
    
    virtual void doSomething();
protected:
    // Member functions
    void    moveMe();
    void    eatFood();
private:
    int     m_distance;
};

class BabyGrasshopper : public Grasshopper
{
public:
    // Constructor
    BabyGrasshopper(StudentWorld* sw, int startX, int startY);
    
    // Member functions
    virtual void    doSomething();
};

class AdultGrasshopper : public Grasshopper
{
public:
    // Constructor
    AdultGrasshopper(StudentWorld* sw, int startX, int startY);
    
    // Member functions
    virtual void    doSomething();
    virtual void    getPoisoned();
    virtual void    getStunned();
};

#endif // ACTOR_H_
