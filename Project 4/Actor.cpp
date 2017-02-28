#include "Actor.h"
#include "StudentWorld.h"
#include "GraphObject.h"
#include "GameConstants.h"
#include "Compiler.h"
#include <list>
using namespace std;

//////////////////////////
// Actor Implementation //
//////////////////////////

Actor::Actor(StudentWorld* world, int startX, int startY, Direction startDir, int imageID, int depth)
: GraphObject(imageID, startX, startY, startDir, depth), m_field(world), m_life(true)
{
}

// Virtual functions
bool Actor::blocksMovement() const
{
    return false;
}

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

bool Actor::didIMove() const
{
    return false;
}

void Actor::resetMovement()
{
    return;
}

void Actor::getFood(int &amt) const
{
    return;
}

void Actor::changeFood(int amt)
{
    return;
}

int Actor::getEnergy() const
{
    return 100;
}

void Actor::updateEnergy(int amt)
{
    return;
}


bool Actor::isPoison() const
{
    return false;
}

// Member functions
StudentWorld* Actor::getWorld() const
{
    return m_field;
}

bool Actor::amIAlive() const
{
    return m_life;
}

void  Actor::killMe()
{
    m_life = false;
}

// Protected function
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

///////////////////////////
// Pebble Implementation //
///////////////////////////

Pebble::Pebble(StudentWorld* sw, int startX, int startY)
: Actor(sw, startX, startY, right, IID_ROCK, 1)
{
}

void Pebble::doSomething()
{
    return;
}

bool Pebble::blocksMovement() const
{
    return true;
}

/////////////////////////////////
// EnergyHolder Implementation //
/////////////////////////////////

EnergyHolder::EnergyHolder(StudentWorld* sw, int startX, int startY, Direction startDir, int energy, int imageID, int depth)
: Actor(sw, startX, startY, startDir, imageID, depth), m_health(energy), m_food(0)
{
}

// Get this actor's amount of energy (for a Pheromone, same as strength).
int EnergyHolder::getEnergy() const
{
    return m_health;
}

// Adjust this actor's amount of energy upward or downward.
void EnergyHolder::updateEnergy(int amt)
{
    m_health += amt;
}

// Get this actor's amount of food
void EnergyHolder::getFood(int &amt) const
{
    amt = m_food;
}

// Adjust this actor's amount of food upward or downward
void EnergyHolder::changeFood(int amt)
{
    m_food += amt;
}

// Add an amount of food to this actor's location.
void EnergyHolder::addFood(int amt)
{
    getWorld()->addFood(this->getX(), this->getY(), amt);
}

// Have this actor pick up an amount of food.
int EnergyHolder::pickupFood(int amt)
{
    int foodAmount = getWorld()->getFoodAt(this->getX(), this->getY())->getEnergy();
    if (amt >= foodAmount)
    {
        this->changeFood(foodAmount);
        getWorld()->getFoodAt(this->getX(), this->getY())->updateEnergy(-foodAmount);
        return foodAmount;
    }
    else
    {
        this->changeFood(amt);
        getWorld()->getFoodAt(this->getX(), this->getY())->updateEnergy(-amt);
        return amt;
    }
}

// Have this actor pick up an amount of food and eat it.
int EnergyHolder::pickupAndEatFood(int amt)
{
    int health = pickupFood(amt);
    updateEnergy(health);
    return health;
}

// Does this actor become food when it dies?
bool EnergyHolder::becomesFoodUponDeath() const
{
    return false;
}

/////////////////////////
// Food Implementation //
/////////////////////////

Food::Food(StudentWorld* sw, int startX, int startY, int energy)
: EnergyHolder(sw, startX, startY, right, energy, IID_FOOD, 2)
{
}

void Food::doSomething()
{
    return;
}

bool Food::isEdible() const
{
    return true;
}

////////////////////////////
// AntHill Implementation //
////////////////////////////

AntHill::AntHill(StudentWorld* sw, int startX, int startY, int colony, Compiler* program)
: EnergyHolder(sw, startX, startY, right, 8999, IID_ANT_HILL, 2), m_colony(colony), m_behavior(program)
{
}

void AntHill::doSomething()
{
    updateEnergy(-1);
    if (getEnergy() <= 0)
    {
        killMe();
        return;
    }
    
    if (getWorld()->getFoodAt(this->getX(), this->getY()) != nullptr)
    {
        pickupAndEatFood(10000);
        return;
    }
    
    if (getEnergy() >= 2000)
    {
        getWorld()->addAnt(this->getX(), this->getY(), m_colony);
        updateEnergy(-1500);
        getWorld()->increaseScore(m_colony);
    }
}

bool AntHill::isMyAntHill(int colony) const
{
    return m_colony == colony;
}

//////////////////////////////
// Pheromone Implementation //
//////////////////////////////

Pheromone::Pheromone(StudentWorld* sw, int startX, int startY, int colony, int imageID)
: EnergyHolder(sw, startX, startY, right, 256, imageID, 2), m_colony(colony)
{
}

void Pheromone::doSomething()
{
    updateEnergy(-1);
    if (getEnergy() <= 0)
        killMe();
}

bool Pheromone::isPheromone(int colony) const
{
    return m_colony == colony;
}

void Pheromone::increaseStrength()
{
    if (getEnergy() >= 512)
    {
        updateEnergy(768 - getEnergy());
        return;
    }
    updateEnergy(256);
}

/////////////////////////////////////
// TriggerableActor Implementation //
/////////////////////////////////////

TriggerableActor::TriggerableActor(StudentWorld* sw, int x, int y, int imageID)
: Actor(sw, x, y, right, imageID, 2)
{
}

bool TriggerableActor::isDangerous(int colony) const
{
    if (this->isPoison())
        return true;
    return false;
}

//////////////////////////
// Water Implementation //
//////////////////////////

WaterPool::WaterPool(StudentWorld* sw, int x, int y)
: TriggerableActor(sw, x, y, IID_WATER_POOL)
{
}

void WaterPool::doSomething()
{
    getWorld()->stunAll(this->getX(), this->getY());
}

///////////////////////////
// Poison Implementation //
///////////////////////////

Poison::Poison(StudentWorld* sw, int x, int y)
: TriggerableActor(sw, x, y, IID_POISON)
{
}

void Poison::doSomething()
{
    getWorld()->poisonAll(this->getX(), this->getY());
}

///////////////////////////
// Insect Implementation //
///////////////////////////

Insect::Insect(StudentWorld* world, int startX, int startY, int energy, int imageID)
: EnergyHolder(world, startX, startY, randomDir(), energy, imageID, 1), m_sleep(0), m_moved(false)
{
}

// Virtual functions
void Insect::getBitten(int amt)
{
    updateEnergy(-amt);
}

void Insect::getPoisoned()
{
    updateEnergy(-150);
}

void Insect::getStunned()
{
    m_sleep += 2;
}

bool Insect::isEnemy(int colony)
{
    return true;
}

bool Insect::becomesFoodUponDeath() const
{
    return true;
}

bool Insect::isDangerous(int colony) const
{
    if (colony == -1)
        return true;
    return ! (colony == this->getColony());
}

int Insect::getColony() const
{
    return -1;
}

// Protected Functions
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
        getWorld()->addFood(this->getX(), this->getY(), 100);
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

void Insect::IMoved()
{
    m_moved = true;
}

////////////////////////
// Ant Implementation //
////////////////////////

Ant::Ant(StudentWorld* sw, int startX, int startY, int colony, Compiler* program, int imageID)
: Insect(sw, startX, startY, 1500, imageID), m_instruction(0), m_random(0), m_colony(colony), m_file(program), m_blocked(false), m_bit(false)
{
}

void Ant::doSomething()
{
    // Check all preconditions first
    if (beforeDoSomethingChecks())
        return;
    
    while (true)
    {
    // Get next command
    Compiler::Command cmd;
    if (! m_file->getCommand(m_instruction, cmd))
    {
        killMe();
        return;
    }
    
    if (cmd.opcode == Compiler::moveForward)
    {
        ++m_instruction;
        if (this->moveForwardIfPossible())
        {
            m_blocked = false;
            m_bit = false;
        }
        else
            m_blocked = true;
        return;
    }
    else if (cmd.opcode == Compiler::eatFood)
    {
        ++m_instruction;
        int food;
        this->getFood(food);
        if (food <= 100)
        {
            changeFood(-food);
            updateEnergy(food);
        }
        else
        {
            changeFood(-100);
            updateEnergy(100);
        }
        return;
    }
    else if (cmd.opcode == Compiler::dropFood)
    {
        ++m_instruction;
        int food;
        this->getFood(food);
        getWorld()->addFood(this->getX(), this->getY(), food);
        return;
    }
    else if (cmd.opcode == Compiler::bite)
    {
        ++m_instruction;
        if (getWorld()->isEnemyAt(this->getX(), this->getY(), m_colony))
        {
            getWorld()->biteEnemyAt(this, m_colony, 15);
        }
        return;
    }
    else if (cmd.opcode == Compiler::pickupFood)
    {
        ++m_instruction;
        int currentFood;
        this->getFood(currentFood);
        int foodCapacity = 1800 - currentFood;
        if (foodCapacity < 400)
        {
            pickupFood(foodCapacity);
        }
        else
        {
            pickupFood(400);
        }
        return;
    }
    else if (cmd.opcode == Compiler::emitPheromone)
    {
        ++m_instruction;
        getWorld()->addPheromone(this->getX(), this->getY(), m_colony);
        return;
    }
    else if (cmd.opcode == Compiler::faceRandomDirection)
    {
        ++m_instruction;
        // Random direction
        Direction dir = randomDir();
        this->setDirection(dir);
        return;
    }
    else if (cmd.opcode == Compiler::generateRandomNumber)
    {
        ++m_instruction;
        int upperBound = stoi(cmd.operand1);
        if (upperBound == 0)
            m_random = 0;
        else
            m_random = randInt(0, upperBound - 1);
    }
    else if (cmd.opcode == Compiler::goto_command)
    {
        m_instruction = stoi(cmd.operand1);
    }
    else if (cmd.opcode == Compiler::if_command)
    {
        ++m_instruction;
        struct Coord
        {
            int x;
            int y;
        };
        Coord temp;
        if (this->getDirection() == GraphObject::up)
        {
            temp.x = this->getX();
            temp.y = this->getY() - 1;
        }
        else if (this->getDirection() == GraphObject::right)
        {
            temp.x = this->getX() + 1;
            temp.y = this->getY();
        }
        else if (this->getDirection() == GraphObject::down)
        {
            temp.x = this->getX();
            temp.y = this->getY() + 1;
        }
        else if (this->getDirection() == GraphObject::left)
        {
            temp.x = this->getX() - 1;
            temp.y = this->getY();
        }
        
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
            Actor* pheromone = getWorld()->getPheromoneAt(temp.x, temp.y, m_colony);
            if (pheromone != nullptr)
                m_instruction = stoi(cmd.operand2);
        }
        else if (code == Compiler::i_smell_danger_in_front_of_me)
        {
            if (getWorld()->isDangerAt(temp.x, temp.y, m_colony))
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
    }
    if (m_instruction == 10)
        return;
    }
}

bool Ant::isEnemy(int colony) const
{
    if (colony == -1)
        return true;
    return ! (colony == m_colony);
}

Compiler* Ant::getFile()
{
    return m_file;
}

int Ant::getColony() const
{
    return m_colony;
}

bool Ant::moveForwardIfPossible()
{
    Direction dir = this->getDirection();
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

////////////////////////////////
// GrassHopper Implementation //
////////////////////////////////

Grasshopper::Grasshopper(StudentWorld* sw, int startX, int startY, int energy, int imageID)
: Insect(sw, startX, startY, energy, imageID)
{
    // Random distance
    m_distance = randInt(2, 10);
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

////////////////////////////////////
// babyGrassHopper Implementation //
////////////////////////////////////

BabyGrasshopper::BabyGrasshopper(StudentWorld* sw, int startX, int startY)
: Grasshopper(sw, startX, startY, 500, IID_BABY_GRASSHOPPER)
{
}

void BabyGrasshopper::doSomething()
{
    // Check all preconditions first
    if (beforeDoSomethingChecks())
        return;
    
    // Check whether to transform
    if (getEnergy() >= 1600)
    {
        getWorld()->addGrasshopper(this->getX(), this->getY());
        getWorld()->addFood(this->getX(), this->getY(), 100);
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


/////////////////////////////////////
// adultGrassHopper Implementation //
/////////////////////////////////////

AdultGrasshopper::AdultGrasshopper(StudentWorld* sw, int startX, int startY)
: Grasshopper(sw, startX, startY, 1600, IID_ADULT_GRASSHOPPER)
{
}

void AdultGrasshopper::doSomething()
{
    // Check all preconditions first
    if (beforeDoSomethingChecks())
        return;
    
    // Check insect biting
    if (getWorld()->isEnemyAt(this->getX(), this->getY(), -1) && randInt(1, 3) == 3)
    {
        getWorld()->biteEnemyAt(this, -1, 50);
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
