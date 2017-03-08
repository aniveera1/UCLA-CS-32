#include "Actor.h"
#include "StudentWorld.h"
#include "GraphObject.h"
#include "GameConstants.h"
#include "Compiler.h"
#include <list>
using namespace std;

///////////////////////////////////////////////////
// Actor Implementation
///////////////////////////////////////////////////

// Constructor
Actor::Actor(StudentWorld* world, int startX, int startY, Direction startDir, int imageID, int depth)
: GraphObject(imageID, startX, startY, startDir, depth), m_field(world), m_life(true)
{
}

// Actions
void Actor::getBitten(int amt)
{
    return;
}

void Actor::getPoisoned()
{
    return;
}

void Actor::getStunned()
{
    return;
}

void Actor::resetMovement()
{
    return;
}

void Actor::changeFood(int amt)
{
    return;
}

void Actor::updateEnergy(int amt)
{
    return;
}

// Checks
bool  Actor::isEdible() const
{
    return false;
}

bool Actor::isPheromone(int colony) const
{
    return false;
}

bool Actor::isEnemy(int colony) const
{
    return false;
}

bool Actor::isDangerous(int colony) const
{
    return false;
}

bool Actor::isMyAntHill(int colony) const
{
    return false;
}

bool Actor::isPoison() const
{
    return false;
}

bool Actor::didIMove() const
{
    return false;
}

bool Actor::blocksMovement() const
{
    return false;
}

bool Actor::amIAlive() const
{
    return m_life;
}

// Accessors
int Actor::getColony() const
{
    return INVALID_COLONY_NUMBER;
}

void Actor::getFood(int &amt) const
{
    return;
}

int Actor::getEnergy() const
{
    return BASE_HEALTH;
}

// Helper functions
StudentWorld* Actor::getWorld() const
{
    return m_field;
}


void  Actor::killMe()
{
    m_life = false;
}

// Randomly picks a direction
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

///////////////////////////////////////////////////
// Pebble Implementation
///////////////////////////////////////////////////

// Constructor
Pebble::Pebble(StudentWorld* sw, int startX, int startY)
: Actor(sw, startX, startY, right, IID_ROCK, 1)
{
}

// Member functions
void Pebble::doSomething()
{
    // Do nothing
    return;
}

bool Pebble::blocksMovement() const
{
    return true;
}

///////////////////////////////////////////////////
// EnergyHolder Implementation
///////////////////////////////////////////////////

// Constructor
EnergyHolder::EnergyHolder(StudentWorld* sw, int startX, int startY, Direction startDir, int energy, int imageID, int depth)
: Actor(sw, startX, startY, startDir, imageID, depth), m_health(energy), m_food(0)
{
}

void EnergyHolder::doSomething()
{
    return;
}

// Accessors
int EnergyHolder::getEnergy() const
{
    return m_health;
}

void EnergyHolder::getFood(int &amt) const
{
    amt = m_food;
}

// Actions
void EnergyHolder::updateEnergy(int amt)
{
    m_health += amt;
}

void EnergyHolder::changeFood(int amt)
{
    m_food += amt;
}

void EnergyHolder::addFood(int amt)
{
    getWorld()->addFood(this->getX(), this->getY(), amt);
}

int EnergyHolder::pickupFood(int amt)
{
    // Check whether a food object
    // actually exists at the current
    // position
    Actor* temp = getWorld()->getFoodAt(this->getX(), this->getY());
    if (temp == nullptr)
        return 0;
    
    // If so, retrieve its current amount
    int foodAmount = temp->getEnergy();
    
    // If trying to pickup more food than
    // available, simply pickup rest of food
    if (amt >= foodAmount)
    {
        this->changeFood(foodAmount);
        getWorld()->getFoodAt(this->getX(), this->getY())->updateEnergy(-foodAmount);
        return foodAmount;
    }
    this->changeFood(amt);
    getWorld()->getFoodAt(this->getX(), this->getY())->updateEnergy(-amt);
    return amt;
}

int EnergyHolder::pickupAndEatFood(int amt)
{
    // Pick up the specified amount
    // of food
    int health = pickupFood(amt);
    
    // Eat it
    updateEnergy(health);
    return health;
}

///////////////////////////////////////////////////
// Food Implementation
///////////////////////////////////////////////////

// Constructor
Food::Food(StudentWorld* sw, int startX, int startY, int energy)
: EnergyHolder(sw, startX, startY, right, energy, IID_FOOD, 2)
{
}

// Member functions
void Food::doSomething()
{
    // Reduce energy and check if dead
    if (getEnergy() <= 0)
        killMe();
}

bool Food::isEdible() const
{
    return true;
}

///////////////////////////////////////////////////
// AntHill Implementation
///////////////////////////////////////////////////

// Constructor
AntHill::AntHill(StudentWorld* sw, int startX, int startY, int colony, Compiler* program)
: EnergyHolder(sw, startX, startY, right, ANTHILL_STARTING_HEALTH, IID_ANT_HILL, 2), m_colony(colony), m_behavior(program)
{
}

// Member functions
void AntHill::doSomething()
{
    // Make sure the Actor
    // is still alive
    if (! amIAlive())
        return;
    
    // Reduce energy and check if dead
    updateEnergy(-1);
    if (getEnergy() <= 0)
    {
        killMe();
        return;
    }
    
    // Eat any food placed on anthill
    if (getWorld()->getFoodAt(this->getX(), this->getY()) != nullptr)
    {
        pickupAndEatFood(ANTHILL_FOOD_EATING_CAPACITY);
        return;
    }
    
    // Create new ants if enough energy
    if (getEnergy() >= ENERGY_NEEDED_TO_CREATE_ANT)
    {
        getWorld()->addAnt(this->getX(), this->getY(), m_colony);
        updateEnergy(-ANT_STARTING_HEALTH);
    }
}

bool AntHill::isMyAntHill(int colony) const
{
    return m_colony == colony;
}

///////////////////////////////////////////////////
// Pheromone Implementation
///////////////////////////////////////////////////

// Constructor
Pheromone::Pheromone(StudentWorld* sw, int startX, int startY, int colony, int imageID)
: EnergyHolder(sw, startX, startY, right, PHEROMONE_STARTING_HEALTH, imageID, 2), m_colony(colony)
{
}

// Member functions
void Pheromone::doSomething()
{
    // Reduce energy and check if dead
    updateEnergy(-1);
    if (getEnergy() <= 0)
        killMe();
}

bool Pheromone::isPheromone(int colony) const
{
    return m_colony == colony;
}

///////////////////////////////////////////////////
// TriggerableActor Implementation
///////////////////////////////////////////////////

// Constructor
TriggerableActor::TriggerableActor(StudentWorld* sw, int x, int y, int imageID)
: Actor(sw, x, y, right, imageID, 2)
{
}

void TriggerableActor::doSomething()
{
    return;
}

// Member functions
bool TriggerableActor::isDangerous(int colony) const
{
    if (this->isPoison())
        return true;
    return false;
}

///////////////////////////////////////////////////
// Water Implementation
///////////////////////////////////////////////////

// Constructor
WaterPool::WaterPool(StudentWorld* sw, int x, int y)
: TriggerableActor(sw, x, y, IID_WATER_POOL)
{
}

// Member functions
void WaterPool::doSomething()
{
    // Stun everything on its spot
    getWorld()->stunAll(this->getX(), this->getY());
}

///////////////////////////////////////////////////
// Poison Implementation
///////////////////////////////////////////////////

// Constructor
Poison::Poison(StudentWorld* sw, int x, int y)
: TriggerableActor(sw, x, y, IID_POISON), m_moved(false)
{
}

// Member functions
void Poison::doSomething()
{
    // Poison everything on its spot
    // once per tick
    if (! didIMove())
    {
        getWorld()->poisonAll(this->getX(), this->getY());
        m_moved = true;
    }
}

bool Poison::isPoison() const
{
    return true;
}

void Poison::resetMovement()
{
    m_moved = false;
}

bool Poison::didIMove() const
{
    return m_moved;
}

///////////////////////////////////////////////////
// Insect Implementation
///////////////////////////////////////////////////

// Constructor
Insect::Insect(StudentWorld* world, int startX, int startY, int energy, int imageID)
: EnergyHolder(world, startX, startY, randomDir(), energy, imageID, 1), m_sleep(0), m_moved(false)
{
}

void Insect::doSomething()
{
    return;
}

// Actions
void Insect::getBitten(int amt)
{
    updateEnergy(-amt);
}

void Insect::getPoisoned()
{
    updateEnergy(INSECT_POISON_DAMAGE);
}

void Insect::getStunned()
{
    m_sleep += 2;
}

void Insect::resetMovement()
{
    m_moved = false;
}

// Checks
bool Insect::isEnemy(int colony)
{
    return true;
}

bool Insect::isDangerous(int colony) const
{
    if (colony == INVALID_COLONY_NUMBER)
        return true;
    return ! (colony == this->getColony());
}

bool Insect::didIMove() const
{
    return m_moved;
}

// Accessor
int Insect::getColony() const
{
    return INVALID_COLONY_NUMBER;
}

// Member functions
int Insect::getSleep() const
{
    return m_sleep;
}

void Insect::changeSleep(int amount)
{
    m_sleep += amount;
}

bool Insect::beforeDoSomethingChecks()
{
    // Check that the actor is
    // actually alive
    if (! amIAlive())
        return true;
    
    // Ensure a Insect only moves
    // once per tick
    if (didIMove())
    {
        resetMovement();
        return true;
    }
    
    // Decrement health and check if dead
    updateEnergy(-1);
    if (getEnergy() <= 0)
    {
        getWorld()->addFood(this->getX(), this->getY(), FOOD_FROM_DEAD_INSECT);
        killMe();
        return true;
    }
    
    // Check whether awake or asleep
    if (getSleep() != 0)
    {
        changeSleep(-1);
        return true;
    }
    // All checks passed
    return false;
}

void Insect::IMoved()
{
    m_moved = true;
}

///////////////////////////////////////////////////
// Ant Implementation
///////////////////////////////////////////////////

// Constructor
Ant::Ant(StudentWorld* sw, int startX, int startY, int colony, Compiler* program, int imageID)
: Insect(sw, startX, startY, ANT_STARTING_HEALTH, imageID), m_instruction(0), m_random(0), m_colony(colony), m_file(program), m_blocked(false), m_bit(false)
{
}

// Member functions
void Ant::doSomething()
{
    // Check all preconditions first
    if (beforeDoSomethingChecks())
        return;
    
    // Process commands
    while (true)
    {
        // Get next command and kill
        // ant if failure to retrieve
        Compiler::Command cmd;
        if (! getFile()->getCommand(m_instruction, cmd))
        {
            killMe();
            return;
        }
        
        // Execute command
        if (processCmd(cmd))
            break;
    }
}

bool Ant::isEnemy(int colony) const
{
    if (colony == INVALID_COLONY_NUMBER)
        return true;
    return ! (colony == m_colony);
}

int Ant::getColony() const
{
    return m_colony;
}

Compiler* Ant::getFile()
{
    return m_file;
}

bool Ant::moveForwardIfPossible()
{
    Direction dir = this->getDirection();
    
    // Check movement based on current direction and
    // if possible, move ant
    if (dir == GraphObject::up)
    {
        if (getWorld()->canMoveTo(this->getX(), this->getY() - 1))
        {
            this->moveTo(this->getX(), this->getY() - 1);
            return true;
        }
        return false;
    }
    else if (dir == GraphObject::right)
    {
        if (getWorld()->canMoveTo(this->getX() + 1, this->getY()))
        {
            this->moveTo(this->getX() + 1, this->getY());
            return true;
        }
        return false;
    }
    else if (dir == GraphObject::down)
    {
        if (getWorld()->canMoveTo(this->getX(), this->getY() + 1))
        {
            this->moveTo(this->getX(), this->getY() + 1);
            return true;
        }
        return false;
    }
    else if (dir == GraphObject::left)
    {
        if (getWorld()->canMoveTo(this->getX() - 1, this->getY()))
        {
            this->moveTo(this->getX() - 1, this->getY());
            return true;
        }
        return false;
    }
    return false;
}

void Ant::getBitten(int amt)
{
    m_bit = true;
    updateEnergy(-amt);
}

// Helper functions
bool Ant::processCmd(Compiler::Command cmd)
{
    // Check and execute command
    if (cmd.opcode == Compiler::moveForward)
    {
        ++m_instruction;
        if (this->moveForwardIfPossible())
        {
            m_blocked = false;
            m_bit = false;
            this->IMoved();
        }
        else
            m_blocked = true;
        return true;
    }
    else if (cmd.opcode == Compiler::eatFood)
    {
        ++m_instruction;
        int food;
        this->getFood(food);
        
        // Attempt to eat 100 food, if
        // not eat rest of food
        if (food <= ANT_FOOD_EATING_CAPACITY)
        {
            changeFood(-food);
            updateEnergy(food);
            return true;
        }
        changeFood(-ANT_FOOD_EATING_CAPACITY);
        updateEnergy(ANT_FOOD_EATING_CAPACITY);
        return true;
    }
    else if (cmd.opcode == Compiler::dropFood)
    {
        ++m_instruction;
        int food;
        this->getFood(food);
        getWorld()->addFood(this->getX(), this->getY(), food);
        this->changeFood(-food);
        return true;
    }
    else if (cmd.opcode == Compiler::bite)
    {
        ++m_instruction;
        if (getWorld()->isEnemyAt(this->getX(), this->getY(), m_colony))
        {
            getWorld()->biteEnemyAt(this, m_colony, ANT_BITE_DAMAGE);
        }
        return true;
    }
    else if (cmd.opcode == Compiler::pickupFood)
    {
        ++m_instruction;
        int currentFood;
        this->getFood(currentFood);
        
        // Determine how much capacity is left
        int foodCapacity = ANT_FOOD_CAPACITY - currentFood;
        
        // Pickup 400 if enough capacity, otherwise
        // pickup enough food to reach 1800 capacity
        if (foodCapacity < ANT_FOOD_PICKUP_CAPACITY)
        {
            pickupFood(foodCapacity);
            return true;
        }
        pickupFood(ANT_FOOD_PICKUP_CAPACITY);
        return true;
    }
    else if (cmd.opcode == Compiler::emitPheromone)
    {
        ++m_instruction;
        getWorld()->addPheromone(this->getX(), this->getY(), m_colony);
        return true;
    }
    else if (cmd.opcode == Compiler::faceRandomDirection)
    {
        ++m_instruction;
        Direction dir = randomDir();
        this->setDirection(dir);
        return true;
    }
    else if (cmd.opcode == Compiler::rotateClockwise)
    {
        ++m_instruction;
        Direction dir = this->getDirection();
        
        // Adjust direction based on
        // current direction
        if (dir == GraphObject::up)
            setDirection(GraphObject::right);
        else if (dir == GraphObject::right)
            setDirection(GraphObject::down);
        else if (dir == GraphObject::down)
            setDirection(GraphObject::left);
        else if (dir == GraphObject::left)
            setDirection(GraphObject::up);
        return true;
    }
    else if (cmd.opcode == Compiler::rotateCounterClockwise)
    {
        ++m_instruction;
        Direction dir = this->getDirection();
        
        // Adjust direction based on
        // current direction
        if (dir == GraphObject::up)
            setDirection(GraphObject::left);
        else if (dir == GraphObject::right)
            setDirection(GraphObject::up);
        else if (dir == GraphObject::down)
            setDirection(GraphObject::right);
        else if (dir == GraphObject::left)
            setDirection(GraphObject::down);
        return true;
    }
    else if (cmd.opcode == Compiler::generateRandomNumber)
    {
        ++m_instruction;
        int upperBound = stoi(cmd.operand1);
        if (upperBound == 0)
            m_random = 0;
        else
            m_random = randInt(0, upperBound - 1);
        return false;
    }
    else if (cmd.opcode == Compiler::goto_command)
    {
        m_instruction = stoi(cmd.operand1);
        return false;
    }
    else if (cmd.opcode == Compiler::if_command)
    {
        ++m_instruction;
        struct Coord
        {
            int x;
            int y;
        };
        
        // Store the position directly
        // one ahead of the ant for
        // future reference
        Coord oneSpotAhead;
        if (this->getDirection() == GraphObject::up)
        {
            oneSpotAhead.x = this->getX();
            oneSpotAhead.y = this->getY() - 1;
        }
        else if (this->getDirection() == GraphObject::right)
        {
            oneSpotAhead.x = this->getX() + 1;
            oneSpotAhead.y = this->getY();
        }
        else if (this->getDirection() == GraphObject::down)
        {
            oneSpotAhead.x = this->getX();
            oneSpotAhead.y = this->getY() + 1;
        }
        else if (this->getDirection() == GraphObject::left)
        {
            oneSpotAhead.x = this->getX() - 1;
            oneSpotAhead.y = this->getY();
        }
        
        // Process and execute the indicated if command
        int code = stoi(cmd.operand1);
        if (code == Compiler::last_random_number_was_zero)
        {
            if (m_random == 0)
                m_instruction = stoi(cmd.operand2);
        }
        else if (code == Compiler::i_am_carrying_food)
        {
            int food;
            this->getFood(food);
            if (food > 0)
                m_instruction = stoi(cmd.operand2);
        }
        else if (code == Compiler::i_am_hungry)
        {
            if (this->getEnergy() <= 25)
                m_instruction = stoi(cmd.operand2);
        }
        else if (code == Compiler::i_am_standing_with_an_enemy)
        {
            if (getWorld()->isEnemyAt(this->getX(), this->getY(), m_colony))
                m_instruction = stoi(cmd.operand2);
        }
        else if (code == Compiler::i_am_standing_on_food)
        {
            Actor* foodObject = getWorld()->getFoodAt(this->getX(), this->getY());
            if (foodObject != nullptr)
                m_instruction = stoi(cmd.operand2);
        }
        else if (code == Compiler::i_am_standing_on_my_anthill)
        {
            if (getWorld()->isAntHillAt(this->getX(), this->getY(), m_colony))
                m_instruction = stoi(cmd.operand2);
        }
        else if (code == Compiler::i_smell_pheromone_in_front_of_me)
        {
            Actor* pheromone = getWorld()->getPheromoneAt(oneSpotAhead.x, oneSpotAhead.y, m_colony);
            if (pheromone != nullptr)
                m_instruction = stoi(cmd.operand2);
        }
        else if (code == Compiler::i_smell_danger_in_front_of_me)
        {
            if (getWorld()->isDangerAt(oneSpotAhead.x, oneSpotAhead.y, m_colony))
                m_instruction = stoi(cmd.operand2);
        }
        else if (code == Compiler::i_was_bit)
        {
            if (m_bit)
                m_instruction = stoi(cmd.operand2);
        }
        else if (code == Compiler::i_was_blocked_from_moving)
        {
            if (m_blocked)
                m_instruction = stoi(cmd.operand2);
        }
        return false;
    }
    
    // If 10 commands were executed,
    // we are done
    if (m_instruction == 10)
        return true;
    return false;
}

///////////////////////////////////////////////////
// GrassHopper Implementation
///////////////////////////////////////////////////

// Constructor
Grasshopper::Grasshopper(StudentWorld* sw, int startX, int startY, int energy, int imageID)
: Insect(sw, startX, startY, energy, imageID)
{
    // Get a random distance
    m_distance = randInt(2, 10);
}

void Grasshopper::doSomething()
{
    return;
}

// Member functions
void Grasshopper::moveMe()
{
    // If finished moving in indicated direction,
    // get a new direction and distance
    if (m_distance == 0)
    {
        Direction dir = randomDir();
        this->setDirection(dir);
        
        m_distance = randInt(2, 10);
    }
    
    // Check movement based on indicated direction
    if (getDirection() == GraphObject::up)
    {
        // If unable to move, get a new direction
        // and distance
        if (! getWorld()->canMoveTo(getX(), getY() - 1))
        {
            Direction dir = randomDir();
            this->setDirection(dir);
            
            m_distance = randInt(2, 10);
        }
        else
        {
            m_distance--;
            moveTo(getX(), getY() - 1);
            IMoved();
        }
    }
    else if (getDirection() == GraphObject::right)
    {
        if (! getWorld()->canMoveTo(getX() + 1, getY()))
        {
            Direction dir = randomDir();
            this->setDirection(dir);
            
            m_distance = randInt(2, 10);
        }
        else
        {
            m_distance--;
            moveTo(getX() + 1, getY());
            IMoved();
        }
    }
    else if (getDirection() == GraphObject::down)
    {
        if (! getWorld()->canMoveTo(getX(), getY() + 1))
        {
            Direction dir = randomDir();
            this->setDirection(dir);
            
            m_distance = randInt(2, 10);
        }
        else
        {
            m_distance--;
            moveTo(getX(), getY() + 1);
            IMoved();
        }
    }
    else if (getDirection() == GraphObject::left)
    {
        if (! getWorld()->canMoveTo(getX() - 1, getY()))
        {
            Direction dir = randomDir();
            this->setDirection(dir);
            
            m_distance = randInt(2, 10);
        }
        else
        {
            m_distance--;
            moveTo(getX() - 1, getY());
            IMoved();
        }
    }
}

void Grasshopper::eatFood()
{
    if (getWorld()->getFoodAt(this->getX(), this->getY()) != nullptr)
    {
        getWorld()->eatFood(this, this->getX(), this->getY());
        if (randInt(1, 2) == 2)
        {
            changeSleep(2);
            return;
        }
    }
}

///////////////////////////////////////////////////
// babyGrassHopper Implementation
///////////////////////////////////////////////////

// Constructor
BabyGrasshopper::BabyGrasshopper(StudentWorld* sw, int startX, int startY)
: Grasshopper(sw, startX, startY, BABYGRASSHOPPER_STARTING_HEALTH, IID_BABY_GRASSHOPPER)
{
}

// Member functions
void BabyGrasshopper::doSomething()
{
    // Check all preconditions first
    if (beforeDoSomethingChecks())
        return;
    
    // Check whether to transform
    if (getEnergy() >= ADULTGRASSHOPPER_STARTING_HEALTH)
    {
        getWorld()->addGrasshopper(this->getX(), this->getY());
        getWorld()->addFood(this->getX(), this->getY(), FOOD_FROM_DEAD_INSECT);
        killMe();
        return;
    }
    
    // Eat food and decide whether to rest
    eatFood();
    
    // Move around
    moveMe();
    
    // Go to sleep
    changeSleep(2);
}


///////////////////////////////////////////////////
// adultGrassHopper Implementation
///////////////////////////////////////////////////

// Constructor
AdultGrasshopper::AdultGrasshopper(StudentWorld* sw, int startX, int startY)
: Grasshopper(sw, startX, startY, ADULTGRASSHOPPER_STARTING_HEALTH, IID_ADULT_GRASSHOPPER)
{
}

// Member functions
void AdultGrasshopper::doSomething()
{
    // Check all preconditions first
    if (beforeDoSomethingChecks())
        return;
    
    // Check insect biting
    if (getWorld()->isEnemyAt(this->getX(), this->getY(), INVALID_COLONY_NUMBER) && randInt(1, 3) == 3)
    {
        getWorld()->biteEnemyAt(this, INVALID_COLONY_NUMBER, GRASSHOPPER_BITE_DAMAGE);
        changeSleep(2);
        return;
    }
    
    // Check jumping to another square
    if (getWorld()->checkJumpSpace(this->getX(), this->getY()) && randInt(1, 10) == 10)
    {
        getWorld()->jumpSomewhere(this->getX(), this->getY(), this);
        changeSleep(2);
        return;
    }
    
    // Eat food and decide whether to rest
    eatFood();
    
    // Move around
    moveMe();
    
    // Go to sleep
    changeSleep(2);
}

void AdultGrasshopper::getPoisoned()
{
    return;
}

void AdultGrasshopper::getStunned()
{
    return;
}
