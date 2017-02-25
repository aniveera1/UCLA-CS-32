#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "Gameworld.h"
#include "GameConstants.h"
#include <string>
#include <list>

class Actor;
class Field;

class StudentWorld : public GameWorld
{
public:
    // Constructor
    StudentWorld(std::string assetDir);
    
    // Destructor
    ~StudentWorld();
    
    // Loads the field and allocates
    // all necessary objects
    virtual int init();
    
    // Runs one tick
    virtual int move();
    
    // Ends the simulation
    virtual void cleanUp();
    
    // Accessors
    bool checkPebble(int x, int y) const;
    bool checkFood(int x, int y) const;
    
    // Actions
    void stunAll(int x, int y) const;
    void poisonAll(int x, int y) const;
    
    // Modifiers
    void addFood(int x, int y);
    void eatFood(Actor* eater, int x, int y, int type);
    void addGrasshopper(int x, int y);
private:
    // Helper functions for init()
    bool loadField(Field &current);
    void loadObjects(Field &current);
    
    // Helper function for move()
    void resetActorMovements();
    void removeDeadActors();
    
    // 2D Array of Linked Lists
    std::list<Actor*> m_field[VIEW_HEIGHT][VIEW_WIDTH];
    
    // Tick Counter
    int m_ticks;
};

#endif // STUDENTWORLD_H_
