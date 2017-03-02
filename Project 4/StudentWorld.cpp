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
: GameWorld(assetDir), m_ticks(2000), m_winner(-1)
{
    // Set initial ant count to 0
    for (int i = 0; i < 4; i++)
        m_ants[i] = 0;
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
    // Load field file
    Field current;
    if (! loadField(current))
        return GWSTATUS_LEVEL_ERROR;
    
    // Load ant files
    if (! loadAnts(m_files))
        return GWSTATUS_LEVEL_ERROR;
    
    // Allocate necessary actors
    loadObjects(current, m_files);
    
    // Start game
    return GWSTATUS_CONTINUE_GAME;
}

// Runs one tick
int StudentWorld::move()
{
    // Decrement tick and set game text
    m_ticks--;
    setGameStatText(formatDisplayText());
    
    // Reset all actors, so that all
    // will do something this tick
    resetActorMovements();
    
    // Iterate through each actor calling
    // doSomething
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
                
                // If an actor moved, update the
                // data structure
                if (temp->didIMove())
                {
                    m_field[temp->getY()][temp->getX()].push_back(*current);
                    m_field[i][j].erase(current);
                }
                current = next;
            }
        }
    
    // Remove any actors that may have died
    removeDeadActors();
    
    // Check if the simulation has ended
    if (m_ticks == 0)
    {
        // No suitable winner was found
        if (m_winner == -1)
            return GWSTATUS_NO_WINNER;
        
        // Otherwise, we have a winner
        setWinner(m_files[m_winner].getColonyName());
        return GWSTATUS_PLAYER_WON;
    }
    return GWSTATUS_CONTINUE_GAME;
}

// Ends the simulation
void StudentWorld::cleanUp()
{
    // Iterates through deleting
    // each actor
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
}

///////////////////////////////////////////////////
// Accessors
///////////////////////////////////////////////////

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

///////////////////////////////////////////////////
// Actions
///////////////////////////////////////////////////

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
        
        // Ensures that the same actor
        // is not stunned more than once
        if (current->didIMove())
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
    
    // Iterates through linked list
    // adding all actors to an array
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
    
    // Randomly pick an actor to bite
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
            // Use cos and sin to find a possible space
            // within a 10 radius circle
            double tempX = i * cos(j * PI / 180);
            int checkX = roundAwayFromZero(tempX);
            double tempY = i * sin(j * PI / 180);
            int checkY = roundAwayFromZero(tempY);
            
            // Check that the space exists within the grid
            if (x + checkX >= 0 && x + checkX < VIEW_WIDTH && y + checkY >= 0 && y + checkY < VIEW_HEIGHT)
                if (m_field[y + checkY][x + checkX].empty())
                {
                    Coord temp;
                    temp.x = x + checkX;
                    temp.y = y + checkY;
                    
                    // Add coordinate to possibilities
                    possiblePlaces.push_back(temp);
                }
            
            // Check that the space exists within the grid
            if (x - checkX >= 0 && x - checkX < VIEW_WIDTH && y - checkY >= 0 && y - checkY < VIEW_HEIGHT)
                if (m_field[y - checkY][x - checkX].empty())
                {
                    Coord temp;
                    temp.x = x - checkX;
                    temp.y = y - checkY;
                    
                    // Add coordinate to possibilities
                    possiblePlaces.push_back(temp);
                }
        }
    
    // Randomly pick a coordinate
    int target = randInt(0, possiblePlaces.size() - 1);
    Coord move = possiblePlaces[target];
    
    // Move to that coordinate
    jumper->moveTo(move.x, move.y);
}

///////////////////////////////////////////////////
// Modifiers
///////////////////////////////////////////////////

// Adds a new food item, or increases the amt
// if a food item is already there
void StudentWorld::addFood(int x, int y, int amt)
{
    if (amt == 0)
        return;
    Actor* food = getFoodAt(x, y);
    
    // Check if there is already a food
    // object there or not
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
    
    // Iterate through till we find the
    // food object
    while (*temp != nullptr && temp != m_field[y][x].end())
    {
        next = ++temp;
        --temp;
        current = *temp;
        if (current->isEdible())
            break;
        temp = next;
    }
    
    // Attempt to eat 200 food, if not enough
    // eat rest of food
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

// Add ant of specified colony
void StudentWorld::addAnt(int x, int y, int colony)
{
    // Find approporiate imageID
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
    
    // Increment ant count
    m_ants[colony]++;
}

// Add pheromone of specified colony
void StudentWorld::addPheromone(int x, int y, int colony)
{
    // Check whether to allocate a new
    // pheromone, or simply increase
    // the strength of an existing one
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
    
    // Find approporiate imageID
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

///////////////////////////////////////////////////
// Checks
///////////////////////////////////////////////////

// Determines whether it is possible
// to move to a specified location
bool StudentWorld::canMoveTo(int x, int y) const
{
    if (x < 0 || x >= VIEW_WIDTH || y < 0 || y >= VIEW_HEIGHT)
        return false;
    if (m_field[y][x].front() != nullptr && m_field[y][x].front()->blocksMovement())
        return false;
    return true;
}

// Determines whether there is an open space
// to jump to within a 10 radius circle
bool StudentWorld::checkJumpSpace(int x, int y) const
{
    const int PI = 3.14159265;
    for (int i = 1; i<= 10; i++)
        for (int j = 6; j <= 180; j+=6)
        {
            // Use cos and sin to find a possible space
            // within a 10 radius circle
            double tempX = i * cos(j * PI / 180);
            int checkX = roundAwayFromZero(tempX);
            double tempY = i * sin(j * PI / 180);
            int checkY = roundAwayFromZero(tempY);
            
            // If there is an available space,
            // return true
            if (x + checkX >= 0 && x + checkX < VIEW_WIDTH && y + checkY >= 0 && y + checkY < VIEW_HEIGHT)
                if (m_field[y + checkY][x + checkX].empty())
                    return true;
            if (x - checkX >= 0 && x - checkX < VIEW_WIDTH && y - checkY >= 0 && y - checkY < VIEW_HEIGHT)
                if (m_field[y - checkY][x - checkX].empty())
                    return true;
        }
    return false;
}

// Determines whether there is an
// enemy at an idicated point
bool StudentWorld::isEnemyAt(int x, int y, int colony) const
{
    list<Actor*>::const_iterator temp;
    list<Actor*>::const_iterator next;
    temp = m_field[y][x].begin();
    
    // Iterate through linked list
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

// Determines whether there is
// danger at an idicated point
bool StudentWorld::isDangerAt(int x, int y, int colony) const
{
    list<Actor*>::const_iterator temp;
    list<Actor*>::const_iterator next;
    temp = m_field[y][x].begin();
    
    // Iterate through linked list
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

// Determines whether there is an
// antHill of the indicated colony
// at some point
bool StudentWorld::isAntHillAt(int x, int y, int colony) const
{
    list<Actor*>::const_iterator temp;
    list<Actor*>::const_iterator next;
    temp = m_field[y][x].begin();
    
    // Iterate through linked list
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

///////////////////////////////////////////////////
// Helper Functions
///////////////////////////////////////////////////

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
    
    // If any of the ant files fail
    // to compile, setError
    for (int i = 0; i < ants.size(); i++)
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
    vector<string> ants;
    ants = getFilenamesOfAntPrograms();
    
    // Iterate through grid, allocating
    // objects wherever indicated
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
            if (item == Field::anthill0 && 0 < ants.size())
            {
                m_field[i][j].push_front(new AntHill(this, j, i, 0, &antFiles[0]));
            }
            if (item == Field::anthill1 && 1 < ants.size())
            {
                m_field[i][j].push_front(new AntHill(this, j, i, 1, &antFiles[1]));
            }
            if (item == Field::anthill2 && 2 < ants.size())
            {
                m_field[i][j].push_front(new AntHill(this, j, i, 2, &antFiles[2]));
            }
            if (item == Field::anthill3 && 3 < ants.size())
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
    
    // Iterate through grid resetting all
    // actors
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

std::string StudentWorld::formatDisplayText()
{
    // Get all ants in order to determine
    // if there is currently a winner
    vector<string> ants;
    ants = getFilenamesOfAntPrograms();
    
    // If there is only one ant,
    // check if it produced more
    // than 6 ants in total
    if (ants.size() == 1 && m_ants[0] >= 6)
        m_winner = 0;
    
    // Iterate through all the ants,
    // and determine if any meet
    // the winning criterion
    int mostAntsSoFar = 0;
    for (int i = 0; i < ants.size(); i++)
    {
        if (m_ants[i] > mostAntsSoFar && m_ants[i] >= 6)
        {
            mostAntsSoFar = m_ants[i];
            m_winner = i;
        }
    }
    
    // Declare variables
    ostringstream ticksText, ant1Text, ant2Text, ant3Text, ant4Text;
    
    const string REGULAR = ": ";
    const string WINNER =  "*: ";
    
    // Set ticks text
    ticksText << "Ticks:" << setw(5) << m_ticks << " - ";
    
    if (0 < ants.size())
    {
        // Get number of ants for this colony
        ostringstream temp;
        temp << setfill('0') << setw(2) << m_ants[0] << " ants  ";
        string numAnts = temp.str();
        
        // Determine if this colony is currently
        // the winner
        if (0 != m_winner)
            ant1Text << m_files[0].getColonyName() << REGULAR << numAnts;
        else
            ant1Text << m_files[0].getColonyName() << WINNER << numAnts;
    }
    if (1 < ants.size())
    {
        // Get number of ants for this colony
        ostringstream temp;
        temp << setfill('0') << setw(2) << m_ants[1] << " ants  ";
        string numAnts = temp.str();
        
        // Determine if this colony is currently
        // the winner
        if (1 != m_winner)
            ant2Text << m_files[1].getColonyName() << REGULAR << numAnts;
        else
            ant2Text << m_files[1].getColonyName() << WINNER << numAnts;
    }if (2 < ants.size())
    {
        // Get number of ants for this colony
        ostringstream temp;
        temp << setfill('0') << setw(2) << m_ants[2] << " ants  ";
        string numAnts = temp.str();
        
        // Determine if this colony is currently
        // the winner
        if (2 != m_winner)
            ant3Text << m_files[2].getColonyName() << REGULAR << numAnts;
        else
            ant3Text << m_files[2].getColonyName() << WINNER << numAnts;
    }if (3 < ants.size())
    {
        // Get number of ants for this colony
        ostringstream temp;
        temp << setfill('0') << setw(2) << m_ants[3] << " ants  ";
        string numAnts = temp.str();
        
        // Determine if this colony is currently
        // the winner
        if (3 != m_winner)
            ant4Text << m_files[3].getColonyName() << REGULAR << numAnts;
        else
            ant4Text << m_files[3].getColonyName() << WINNER << numAnts;
    }
    
    // Compile final display text together
    string finalText = ticksText.str() + ant1Text.str() + ant2Text.str() + ant3Text.str() + ant4Text.str();
    return finalText;
}
