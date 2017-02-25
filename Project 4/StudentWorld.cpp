#include "StudentWorld.h"
#include "GameWorld.h"
#include "GameConstants.h"
#include "Field.h"
#include "Actor.h"
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
    return new StudentWorld(assetDir);
}

// Constructor
StudentWorld::StudentWorld(std::string assetDir)
: GameWorld(assetDir), m_ticks(2000)
{
}

// Destructor
StudentWorld::~StudentWorld()
{
    this->cleanUp();
}

// Loads the field and allocates
// all necessary objects
int StudentWorld::init()
{
    Field current;
    if (! loadField(current))
        return GWSTATUS_LEVEL_ERROR;
    
    loadObjects(current);
    
    return GWSTATUS_CONTINUE_GAME;
}

// Runs one tick
int StudentWorld::move()
{
    m_ticks--;
    
    ostringstream text;
    text << "Ticks:" << setw(5) << m_ticks;
    string test = text.str();
    setGameStatText(test);
    
    // String form
    // Ticks: 1134 - AmyAnt: 32 BillyAnt: 33 SuzieAnt*: 77 IgorAnt: 05
    
    resetActorMovements();
    
    list<Actor*>::iterator current;
    for (int i = 0; i < VIEW_HEIGHT; i++)
        for (int j = 0; j < VIEW_WIDTH; j++)
        {
            current = m_field[i][j].begin();
            while (*current != nullptr && current != m_field[i][j].end())
            {
                Actor* temp = *current;
                temp->doSomething();
                if (temp->didIMove())
                {
                    m_field[temp->getY()][temp->getX()].push_back(*current);
                    m_field[i][j].erase(current);
                }
                current++;
            }
        }
    
    removeDeadActors();
    
    return GWSTATUS_CONTINUE_GAME;
}

// Ends the simulation
void StudentWorld::cleanUp()
{
    list<Actor*>::iterator current;
    for (int i = 0; i < VIEW_HEIGHT; i++)
        for (int j = 0; j < VIEW_WIDTH; j++)
        {
            current = m_field[i][j].begin();
            while (*current != nullptr && current != m_field[i][j].end())
            {
                delete *current;
                m_field[i][j].erase(current);
                current++;
            }
        }
}

// Check movement
bool StudentWorld::checkPebble(int x, int y) const
{
   if (m_field[y][x].front() != nullptr && m_field[y][x].front()->doIBlock())
       return true;
    return false;
}

bool StudentWorld::checkFood(int x, int y) const
{
    list<Actor*>::const_iterator temp;
    temp = m_field[y][x].begin();
    while (*temp != nullptr && temp != m_field[y][x].end())
    {
        Actor* current = *temp;
        if (current->amIFood())
            return true;
        temp++;
    }
    return false;
}

// Stun everything on a certain point
void StudentWorld::stunAll(int x, int y) const
{
    list<Actor*>::const_iterator temp;
    temp = m_field[y][x].begin();
    while (*temp != nullptr && temp != m_field[y][x].end())
    {
        Actor* current = *temp;
        if (current->amIStunableAndPoisonable())
            current->stunMe();
        temp++;
    }
}

// Poison everything on a certain point
void StudentWorld::poisonAll(int x, int y) const
{
    list<Actor*>::const_iterator temp;
    temp = m_field[y][x].begin();
    while (*temp != nullptr && temp != m_field[y][x].end())
    {
        Actor* current = *temp;
        if (current->amIStunableAndPoisonable())
            current->hurtMe();
        temp++;
    }
}

// Add food due to a dead insect
void StudentWorld::addFood(int x, int y)
{
    m_field[y][x].push_back(new Food(this, x, y, 100));
}

// Eat some food
void StudentWorld::eatFood(Actor* eater, int x, int y, int type)
{
    list<Actor*>::const_iterator temp;
    Actor* current = nullptr;
    temp = m_field[y][x].begin();
    while (*temp != nullptr && temp != m_field[y][x].end())
    {
        current = *temp;
        if (current->amIFood())
            break;
        temp++;
    }
    
    if (type == IID_BABY_GRASSHOPPER || type == IID_ADULT_GRASSHOPPER)
    {
        int foodAmount = current->getHealth();
        if (foodAmount < 200)
        {
            eater->changeHealth(foodAmount);
            current->changeHealth(-foodAmount);
        }
        else
        {
            eater->changeHealth(200);
            current->changeHealth(-200);
        }
    }
}

// Change a baby to an adult
void StudentWorld::addGrasshopper(int x, int y)
{
    m_field[y][x].push_back(new adultGrasshopper(this, x, y));
}

///////////////////////
// Private Functions //
///////////////////////

// Helper functions for init()
bool StudentWorld::loadField(Field &current)
{
    string fieldFile = getFieldFilename();
    string error;
    
    if (current.loadField(fieldFile, error) != Field::LoadResult::load_success)
    {
        setError(fieldFile + " " + error);
        return false;
    }
    return true;
}

void StudentWorld::loadObjects(Field &current)
{
    for (int i = 0; i < VIEW_HEIGHT; i++)
        for (int j = 0; j < VIEW_WIDTH; j++)
        {
            Field::FieldItem item = current.getContentsOf(j, i);
            if (item == Field::rock)
            {
                m_field[i][j].push_front(new Pebble(this, j, i));
            }
            if (item == Field::grasshopper)
            {
                m_field[i][j].push_front(new babyGrasshopper(this, j, i));
            }
            if (item == Field::water)
            {
                m_field[i][j].push_front(new Water(this, j, i));
            }
            if (item == Field::poison)
            {
                m_field[i][j].push_front(new Poison(this, j, i));
            }
            if (item == Field::food)
            {
                m_field[i][j].push_front(new Food(this, j, i));
            }
        }
}

// Helper function for move()
void StudentWorld::resetActorMovements()
{
    list<Actor*>::iterator temp;
    for (int i = 0; i < VIEW_HEIGHT; i++)
        for (int j = 0; j < VIEW_WIDTH; j++)
        {
            temp = m_field[i][j].begin();
            while (*temp != nullptr && temp != m_field[i][j].end())
            {
                Actor* current = *temp;
                current->resetMovement();
                temp++;
            }
        }
}

void StudentWorld::removeDeadActors()
{
    list<Actor*>::iterator current;
    for (int i = 0; i < VIEW_HEIGHT; i++)
        for (int j = 0; j < VIEW_WIDTH; j++)
        {
            current = m_field[i][j].begin();
            while (*current != nullptr && current != m_field[i][j].end())
            {
                Actor* temp = *current;
                if (! temp->getLife())
                {
                    delete temp;
                    m_field[i][j].erase(current);
                }
                current++;
            }
        }
}
