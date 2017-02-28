#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <list>

class Actor;
class Compiler;
class Field;

class StudentWorld : public GameWorld
{
public:
    // Constructor
    StudentWorld(std::string assetDir);
    
    // Destructor
    virtual ~StudentWorld();
    
    // Loads the field and allocates
    // all necessary objects
    virtual int init();
    
    // Runs one tick
    virtual int move();
    
    // Ends the simulation
    virtual void cleanUp();

    // Add an actor to the world
    void    addActor(Actor* a);
    
    // Accessors
    Actor*  getFoodAt(int x, int y) const;   // Returns null if none =>DONE
    Actor*  getPheromoneAt(int x, int y, int colony) const;  // Returns null if none =>DONE
    
    // Actions
    void    poisonAll(int x, int y);
    void    stunAll(int x, int y);
    void    biteEnemyAt(Actor* me, int colony, int biteDamage);
    void    jumpSomewhere(int x, int y, Actor* jumper);
    
    // Modifiers
    void    addFood(int x, int y, int amt);
    void    eatFood(Actor* eater, int x, int y);
    void    addGrasshopper(int x, int y);
    void    addAnt(int x, int y, int colony);
    void    addPheromone(int x, int y, int colony);
    
    // Checks
    bool    canMoveTo(int x, int y) const;
    bool    checkJumpSpace(int x, int y) const;
    bool    isEnemyAt(int x, int y, int colony) const;
    bool    isDangerAt(int x, int y, int colony) const;
    bool    isAntHillAt(int x, int y, int colony) const;
    
    // Housekeeping
    void    increaseScore(int colony);      // TODO
private:
    // Helper functions for init()
    bool    loadField(Field &current);
    void    loadObjects(Field &current, Compiler antFiles[]);
    bool    loadAnts(Compiler antFiles[]);
    
    // Helper function for move()
    void    resetActorMovements();
    void    removeDeadActors();
    
    // 2D Array of Linked Lists
    std::list<Actor*> m_field[VIEW_HEIGHT][VIEW_WIDTH];
    
    // Tick Counter
    int     m_ticks;
    
    // Bugs Files
    Compiler* m_files;
};

#endif // STUDENTWORLD_H_
