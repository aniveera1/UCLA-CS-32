#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;
class Compiler;

class Actor : public GraphObject
{
public:
    // Constructor
    Actor(StudentWorld* world, int startX, int startY, Direction startDir, int imageID, int depth);
    
    // Pure virtual functions
    virtual void    doSomething() = 0;
    
    // Virtual functions
    virtual bool    blocksMovement() const;
    virtual void    getBitten(int amt);
    virtual void    getPoisoned();
    virtual void    getStunned();
    virtual bool    isEdible() const;
    virtual bool    isPheromone(int colony) const;
    virtual bool    isEnemy(int colony) const;
    virtual bool    isDangerous(int colony) const;
    virtual bool    isMyAntHill(int colony) const;
    virtual bool    didIMove() const;
    virtual void    resetMovement();
    virtual void    getFood(int &amt) const;
    virtual void    changeFood(int amt);
    virtual int     getEnergy() const;
    virtual void    updateEnergy(int amt);
    virtual bool    isPoison() const;
    
    // Member function
    StudentWorld*   getWorld() const;
    bool            amIAlive() const;
    void            killMe();
protected:
    // Helper function
    Direction randomDir();
private:
    StudentWorld*   m_field;
    bool            m_life;
};

class Pebble : public Actor
{
public:
    Pebble(StudentWorld* sw, int startX, int startY);
    virtual void    doSomething();
    virtual bool    blocksMovement() const;
};

class EnergyHolder : public Actor
{
public:
    EnergyHolder(StudentWorld* sw, int startX, int startY, Direction startDir, int energy, int imageID, int depth);
    
    // Get this actor's amount of energy (for a Pheromone, same as strength).
    virtual int     getEnergy() const;
    
    // Adjust this actor's amount of energy upward or downward.
    virtual void    updateEnergy(int amt);
    
    // Get this actor's amount of food
    virtual void    getFood(int &amt) const;
    
    // Adjust this actor's amount of food
    virtual void    changeFood(int amt);
    
    // Add an amount of food to this actor's location.
    void            addFood(int amt);
    
    // Have this actor pick up an amount of food.
    int             pickupFood(int amt);
    
    // Have this actor pick up an amount of food and eat it.
    int             pickupAndEatFood(int amt);
    
    // Does this actor become food when it dies?
    virtual bool    becomesFoodUponDeath() const;
private:
    int             m_health;
    int             m_food;
};

class Food : public EnergyHolder
{
public:
    Food(StudentWorld* sw, int startX, int startY, int energy);
    virtual void    doSomething();
    virtual bool    isEdible() const;
};

class AntHill : public EnergyHolder
{
public:
    AntHill(StudentWorld* sw, int startX, int startY, int colony, Compiler* program);
    virtual void    doSomething();
    virtual bool    isMyAntHill(int colony) const;
private:
    int             m_colony;
    Compiler*       m_behavior;
};

class Pheromone : public EnergyHolder
{
public:
    Pheromone(StudentWorld* sw, int startX, int startY, int colony, int imageID);
    virtual void doSomething();
    virtual bool isPheromone(int colony) const;
    
    // Increase the strength (i.e., energy) of this pheromone.
    void increaseStrength();
private:
    int         m_colony;
};

class TriggerableActor : public Actor
{
public:
    TriggerableActor(StudentWorld* sw, int x, int y, int imageID);
    virtual bool isDangerous(int colony) const;
};

class WaterPool : public TriggerableActor
{
public:
    WaterPool(StudentWorld* sw, int x, int y);
    virtual void doSomething();
};

class Poison : public TriggerableActor
{
public:
    Poison(StudentWorld* sw, int x, int y);
    virtual void doSomething();
};

class Insect : public EnergyHolder
{
public:
    Insect(StudentWorld* world, int startX, int startY, int energy, int imageID);
    virtual void getBitten(int amt);
    virtual void getPoisoned();
    virtual void getStunned();
    virtual bool isEnemy(int colony);
    virtual bool becomesFoodUponDeath() const;
    virtual bool    isDangerous(int colony) const;
    virtual int     getColony() const;
protected:
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
    Ant(StudentWorld* sw, int startX, int startY, int colony, Compiler* program, int imageID);
    virtual void    doSomething();
    virtual bool    isEnemy(int colony) const;
    virtual int     getColony() const;
    virtual bool    moveForwardIfPossible();
    virtual void    getBitten(int amt);
    
    // Member function
    Compiler*       getFile();
private:
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
    Grasshopper(StudentWorld* sw, int startX, int startY, int energy, int imageID);
protected:
    void    moveMe();
    void    eatFood();
private:
    int     m_distance;
};

class BabyGrasshopper : public Grasshopper
{
public:
    BabyGrasshopper(StudentWorld* sw, int startX, int startY);
    
    // Virtual functions
    virtual void    doSomething();
};

class AdultGrasshopper : public Grasshopper
{
public:
    AdultGrasshopper(StudentWorld* sw, int startX, int startY);
    
    // Virtual functions
    virtual void    doSomething();
    virtual void getPoisoned();
    virtual void getStunned();
};

#endif // ACTOR_H_
