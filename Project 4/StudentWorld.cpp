#include "StudentWorld.h"
#include "GameWorld.h"
#include "GameConstants.h"
#include "Field.h"
#include "Actor.h"
#include "Compiler.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <vector>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
    return new StudentWorld(assetDir);
}

// Constructor
StudentWorld::StudentWorld(std::string assetDir)
: GameWorld(assetDir), m_ticks(2000)
{
    m_files = new Compiler[4];
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
    
    if (! loadAnts(m_files))
        return GWSTATUS_LEVEL_ERROR;
    
    loadObjects(current, m_files);
    
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
    list<Actor*>::iterator next;
    for (int i = 0; i < VIEW_HEIGHT; i++)
        for (int j = 0; j < VIEW_WIDTH; j++)
        {
            current = m_field[i][j].begin();
            while (*current != nullptr && current != m_field[i][j].end())
            {
                next = ++current;
                --current;
                Actor* temp = *current;
                temp->doSomething();
                if (temp->didIMove())
                {
                    m_field[temp->getY()][temp->getX()].push_back(*current);
                    m_field[i][j].erase(current);
                }
                current = next;
            }
        }
    
    removeDeadActors();
    
    return GWSTATUS_CONTINUE_GAME;
}

// Ends the simulation
void StudentWorld::cleanUp()
{
    list<Actor*>::iterator current;
    list<Actor*>::iterator next;
    for (int i = 0; i < VIEW_HEIGHT; i++)
        for (int j = 0; j < VIEW_WIDTH; j++)
        {
            current = m_field[i][j].begin();
            while (*current != nullptr && current != m_field[i][j].end())
            {
                next = ++current;
                --current;
                delete *current;
                m_field[i][j].erase(current);
                current = next;
            }
        }
    delete []m_files;
}

///////////////
// Accessors //
///////////////

// Returns null if none
Actor* StudentWorld::getFoodAt(int x, int y) const
{
    list<Actor*>::const_iterator temp;
    list<Actor*>::const_iterator next;
    temp = m_field[y][x].begin();
    while (*temp != nullptr && temp != m_field[y][x].end())
    {
        next = ++temp;
        --temp;
        Actor* current = *temp;
        if (current->isEdible())
            return current;
        temp = next;
    }
    return nullptr;
}

// Returns null if none
Actor* StudentWorld::getPheromoneAt(int x, int y, int colony) const
{
    list<Actor*>::const_iterator temp;
    list<Actor*>::const_iterator next;
    temp = m_field[y][x].begin();
    while (*temp != nullptr && temp != m_field[y][x].end())
    {
        next = ++temp;
        --temp;
        Actor* current = *temp;
        if (current->isPheromone(colony))
            return current;
        temp = next;
    }
    return nullptr;
}

/////////////
// Actions //
/////////////

// Poisons everything at a certain point
void StudentWorld::poisonAll(int x, int y)
{
    list<Actor*>::const_iterator temp;
    list<Actor*>::const_iterator next;
    temp = m_field[y][x].begin();
    while (*temp != nullptr && temp != m_field[y][x].end())
    {
        next = ++temp;
        --temp;
        Actor* current = *temp;
        current->getPoisoned();
        temp = next;
    }
}

// Stuns everything at a certain point
void StudentWorld::stunAll(int x, int y)
{
    list<Actor*>::const_iterator temp;
    list<Actor*>::const_iterator next;
    temp = m_field[y][x].begin();
    while (*temp != nullptr && temp != m_field[y][x].end())
    {
        next = ++temp;
        --temp;
        Actor* current = *temp;
        current->getStunned();
        temp = next;
    }
}

// Bites a random insect
void StudentWorld::biteEnemyAt(Actor* me, int colony, int biteDamage)
{
    list<Actor*>::const_iterator temp;
    list<Actor*>::const_iterator next;
    temp = m_field[me->getY()][me->getX()].begin();
    Actor** container = new Actor*[m_field[me->getY()][me->getX()].size()];
    int containerSize = 0;
    while (*temp != nullptr && temp != m_field[me->getY()][me->getX()].end())
    {
        next = ++temp;
        --temp;
        Actor* current = *temp;
        if (current->isEnemy(colony) && current != me)
        {
            container[containerSize] = current;
            containerSize++;
        }
        temp = next;
    }
    if (containerSize != 0)
    {
        int target = randInt(0, containerSize - 1);
        container[target]->updateEnergy(-biteDamage);
    }
    delete []container;
}

// Randomly picks an open space to jump to
void StudentWorld::jumpSomewhere(int x, int y, Actor* jumper)
{
    struct Coord
    {
        int x;
        int y;
    };
    vector<Coord> possiblePlaces;
    
    const int PI = 3.14159265;
    for (int i = 1; i<= 10; i++)
        for (int j = 6; j <= 180; j+=6)
        {
            double tempX = i * cos(j * PI / 180);
            int checkX = roundAwayFromZero(tempX);
            double tempY = i * sin(j * PI / 180);
            int checkY = roundAwayFromZero(tempY);
            if (x + checkX >= 0 && x + checkX <= 63 && y + checkY >= 0 && y + checkY <= 63)
                if (m_field[y + checkY][x + checkX].empty())
                {
                    Coord temp;
                    temp.x = x + checkX;
                    temp.y = y + checkY;
                    possiblePlaces.push_back(temp);
                }
            if (x - checkX >= 0 && x - checkX <= 63 && y - checkY >= 0 && y - checkY <= 63)
                if (m_field[y - checkY][x - checkX].empty())
                {
                    Coord temp;
                    temp.x = x - checkX;
                    temp.y = y - checkY;
                    possiblePlaces.push_back(temp);
                }
        }
    
    int target = randInt(0, possiblePlaces.size() - 1);
    Coord move = possiblePlaces[target];
    
    jumper->moveTo(move.x, move.y);
}

///////////////
// Modifiers //
///////////////

// Adds a new food item, or increases the amt
// if a food item is already there
void StudentWorld::addFood(int x, int y, int amt)
{
    Actor* food = getFoodAt(x, y);
    if (food == nullptr)
        m_field[y][x].push_back(new Food(this, x, y, amt));
    else
    {
        food->changeFood(amt);
    }
}

// Eats food
void StudentWorld::eatFood(Actor* eater, int x, int y)
{
    list<Actor*>::const_iterator temp;
    list<Actor*>::const_iterator next;
    Actor* current = nullptr;
    temp = m_field[y][x].begin();
    while (*temp != nullptr && temp != m_field[y][x].end())
    {
        next = ++temp;
        --temp;
        current = *temp;
        if (current->isEdible())
            break;
        temp = next;
    }
    
    int foodAmount = current->getEnergy();
    if (foodAmount < 200)
    {
        eater->updateEnergy(foodAmount);
        current->updateEnergy(-foodAmount);
    }
    else
    {
        eater->updateEnergy(200);
        current->updateEnergy(-200);
    }
}

// Change a baby into an adult
void StudentWorld::addGrasshopper(int x, int y)
{
    m_field[y][x].push_back(new AdultGrasshopper(this, x, y));
}

// Add ant of specified type
void StudentWorld::addAnt(int x, int y, int colony)
{
    int image = 0;
    switch (colony)
    {
        case 0:
            image = IID_ANT_TYPE0;
            break;
        case 1:
            image = IID_ANT_TYPE1;
            break;
        case 2:
            image = IID_ANT_TYPE2;
            break;
        case 3:
            image = IID_ANT_TYPE3;
            break;
    }
    m_field[y][x].push_back(new Ant(this, x, y, colony, &m_files[colony], image));
}

// Add pheromone of specified colony
void StudentWorld::addPheromone(int x, int y, int colony)
{
    Actor* pheromone = getPheromoneAt(x, y, colony);
    if (pheromone != nullptr)
    {
        int amt = 768 - pheromone->getEnergy();
        if (amt < 256)
            pheromone->updateEnergy(amt);
        else
            pheromone->updateEnergy(256);
        return;
    }
    int image = 0;
    switch (colony)
    {
        case 0:
            image = IID_PHEROMONE_TYPE0;
            break;
        case 1:
            image = IID_PHEROMONE_TYPE1;
            break;
        case 2:
            image = IID_PHEROMONE_TYPE2;
            break;
        case 3:
            image = IID_PHEROMONE_TYPE3;
            break;
    }
    m_field[y][x].push_back(new Pheromone(this, x, y, colony, image));
}

////////////
// Checks //
////////////

bool StudentWorld::canMoveTo(int x, int y) const
{
    if (m_field[y][x].front() != nullptr && m_field[y][x].front()->blocksMovement())
        return false;
    return true;
}

bool StudentWorld::checkJumpSpace(int x, int y) const
{
    const int PI = 3.14159265;
    for (int i = 1; i<= 10; i++)
        for (int j = 6; j <= 180; j+=6)
        {
            double tempX = i * cos(j * PI / 180);
            int checkX = roundAwayFromZero(tempX);
            double tempY = i * sin(j * PI / 180);
            int checkY = roundAwayFromZero(tempY);
            if (x + checkX >= 0 && x + checkX <= 63 && y + checkY >= 0 && y + checkY <= 63)
                if (m_field[y + checkY][x + checkX].empty())
                    return true;
            if (x - checkX >= 0 && x - checkX <= 63 && y - checkY >= 0 && y - checkY <= 63)
                if (m_field[y - checkY][x - checkX].empty())
                    return true;
        }
    return false;
}

bool StudentWorld::isEnemyAt(int x, int y, int colony) const
{
    list<Actor*>::const_iterator temp;
    list<Actor*>::const_iterator next;
    temp = m_field[y][x].begin();
    while (*temp != nullptr && temp != m_field[y][x].end())
    {
        next = ++temp;
        --temp;
        Actor* current = *temp;
        if (current->isEnemy(colony))
            return true;
        temp = next;
    }
    return false;
}

bool StudentWorld::isDangerAt(int x, int y, int colony) const
{
    list<Actor*>::const_iterator temp;
    list<Actor*>::const_iterator next;
    temp = m_field[y][x].begin();
    while (*temp != nullptr && temp != m_field[y][x].end())
    {
        next = ++temp;
        --temp;
        Actor* current = *temp;
        if (current->isEnemy(colony) || current->isDangerous(colony))
            return true;
        temp = next;
    }
    return false;
}

bool StudentWorld::isAntHillAt(int x, int y, int colony) const
{
    list<Actor*>::const_iterator temp;
    list<Actor*>::const_iterator next;
    temp = m_field[y][x].begin();
    while (*temp != nullptr && temp != m_field[y][x].end())
    {
        next = ++temp;
        --temp;
        Actor* current = *temp;
        if (current->isMyAntHill(colony))
            return true;
        temp = next;
    }
    return false;
}

//////////////////
// Housekeeping //
//////////////////

void StudentWorld::increaseScore(int colony)
{
    return;
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
        setError(error);
        return false;
    }
    return true;
}

bool StudentWorld::loadAnts(Compiler antFiles[])
{
    vector<string> ants;
    ants = getFilenamesOfAntPrograms();
    string error;
    for (int i = 0; i < 4; i++)
    {
        if (! antFiles[i].compile(ants[i], error))
        {
            setError(ants[i] + " " + error);
            return false;
        }
    }
    return true;
}

void StudentWorld::loadObjects(Field &current, Compiler antFiles[])
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
                m_field[i][j].push_front(new BabyGrasshopper(this, j, i));
            }
            if (item == Field::water)
            {
                m_field[i][j].push_front(new WaterPool(this, j, i));
            }
            if (item == Field::poison)
            {
                m_field[i][j].push_front(new Poison(this, j, i));
            }
            if (item == Field::food)
            {
                m_field[i][j].push_front(new Food(this, j, i, 6000));
            }
            if (item == Field::anthill0)
            {
                m_field[i][j].push_front(new AntHill(this, j, i, 0, &antFiles[0]));
            }
            if (item == Field::anthill1)
            {
                m_field[i][j].push_front(new AntHill(this, j, i, 1, &antFiles[1]));
            }
            if (item == Field::anthill2)
            {
                m_field[i][j].push_front(new AntHill(this, j, i, 2, &antFiles[2]));
            }
            if (item == Field::anthill3)
            {
                m_field[i][j].push_front(new AntHill(this, j, i, 3, &antFiles[3]));
            }
        }
}

// Helper function for move()
void StudentWorld::resetActorMovements()
{
    list<Actor*>::iterator temp;
    list<Actor*>::iterator next;
    for (int i = 0; i < VIEW_HEIGHT; i++)
        for (int j = 0; j < VIEW_WIDTH; j++)
        {

            temp = m_field[i][j].begin();
            while (*temp != nullptr && temp != m_field[i][j].end())
            {
                next = ++temp;
                --temp;
                Actor* current = *temp;
                current->resetMovement();
                temp = next;
            }
        }
}

void StudentWorld::removeDeadActors()
{
    list<Actor*>::iterator current;
    list<Actor*>::iterator next;
    for (int i = 0; i < VIEW_HEIGHT; i++)
        for (int j = 0; j < VIEW_WIDTH; j++)
        {
            current = m_field[i][j].begin();
            while (*current != nullptr && current != m_field[i][j].end())
            {
                next = ++current;
                --current;
                Actor* temp = *current;
                if (! temp->amIAlive())
                {
                    delete temp;
                    m_field[i][j].erase(current);
                }
                current = next;
            }
        }
}
