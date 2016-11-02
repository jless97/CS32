#include "Actor.h"
#include "StudentWorld.h"
#include <iostream>
#include <algorithm>
using namespace std;

///////////////////////////////////////////////////////////////////////////
//  Actor implementation
///////////////////////////////////////////////////////////////////////////

Actor::Actor(int imageID, int startX, int startY, Direction startDirection,
	float size, unsigned int depth, StudentWorld* world)
	:GraphObject(imageID, startX, startY, startDirection, size, depth),
	m_world(world), m_isAlive(true)
{
	setVisible(false);
}

Actor::~Actor()
{
	m_isAlive = false;
	setVisible(false);
}

StudentWorld* Actor::returnWorld() const
{
	return m_world;
}

bool Actor::isAlive() const
{
	if (m_isAlive == true)
        return true;
    return false;
    
}

void Actor::setAlive(bool value)
{
	m_isAlive = false;
}

bool Actor::canActorsPassThroughMe() const
{
	return false;
}

bool Actor::moveDir(int direction, int numMoves)
{
	int x = getX();
	int y = getY();
    //Check to see if I'm allowed to move in a given direction
	if (returnWorld()->isBoulder(x, y, direction))
		return false;
	if (returnWorld()->isDirt(x, y, direction))
		return false;
	if (!returnWorld()->canActorMoveTo(this, x, y))
		return false;

    //If so, then move in the specified direction
	switch (direction)
	{
	case GraphObject::up:
		moveTo(x, y + numMoves);
		break;
	case GraphObject::down:
		moveTo(x, y - numMoves);
		break;
	case GraphObject::right:
		moveTo(x + numMoves, y);
		break;
	case GraphObject::left:
		moveTo(x - numMoves, y);
		break;
	default:
		break;
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////
//  FrackMan implementation
///////////////////////////////////////////////////////////////////////////

FrackMan::FrackMan(int x, int y, StudentWorld* world)
:Actor(IID_PLAYER, x, y, right, 1.0, 0, world), m_squirts(5), m_sonars(1), m_gold(0), m_health(10)
{
    setVisible(true);
}

FrackMan::~FrackMan()
{}

void FrackMan::doSomething()
{
    if (!isAlive()) //Check to see if FrackMan is alive
        return;
    int x = getX();
    int y = getY();
    returnWorld()->removeDirt(x, y); //If FrackMan spawns in a dirt location, remove the dirt
    int ch;
    StudentWorld* world = returnWorld();
    if (validPosition(x, y) == false)
        return;
    if (returnWorld()->getKey(ch) == true) //Check to see if a valid key was input by the user
    {
        switch (ch) //To see what FrackMan is going to do in the given turn (tick)
        {
            case KEY_PRESS_LEFT: //Direction checks to see if FrackMan can move in given direction
                if (getDirection() == left)
                {
                    if (x != 0)
                    {
                        if (!world->isNearBoulder(x - 1, y, 3))
                        {
                            moveTo(x - 1, y);
                            if (world->removeDirt(x - 1, y, KEY_PRESS_LEFT))
                                world->playSound(SOUND_DIG);
                        }
                        else if (world->canActorMoveTo(this, x - 1, y))
                            setDirection(left);
                    }
                    if (x == 0)
                        moveTo(x, y);
                }
                else
                    setDirection(left);
                break;
            case KEY_PRESS_RIGHT:
                if (getDirection() == right)
                {
                    if (x != 60)
                    {
                        if (!world->isNearBoulder(x + 1, y, 3))
                        {
                            moveTo(x + 1, y);
                            if (world->removeDirt(x + 1, y, KEY_PRESS_RIGHT))
                                world->playSound(SOUND_DIG);
                        }
                        else if (world->canActorMoveTo(this, x + 1, y))
                            setDirection(right);
                    }
                    if (x == 60)
                        moveTo(x, y);
                }
                else
                    setDirection(right);
                break;
            case KEY_PRESS_DOWN:
                if (getDirection() == down)
                {
                    if (y != 0)
                    {
                        if (!world->isNearBoulder(x, y - 1, 3))
                        {
                            moveTo(x, y - 1);
                            if (world->removeDirt(x, y - 1, KEY_PRESS_DOWN))
                                world->playSound(SOUND_DIG);
                        }
                        else if (world->canActorMoveTo(this, x, y - 1))
                            setDirection(down);
                    }
                    if (y == 0)
                        moveTo(x, y);
                }
                else
                    setDirection(down);
                break;
            case KEY_PRESS_UP:
                if (getDirection() == up)
                {
                    if (y != 60)
                    {
                        if (!world->isNearBoulder(x, y + 1, 3))
                        {
                            moveTo(x, y + 1);
                            if (world->removeDirt(x, y + 1, KEY_PRESS_UP))
                                world->playSound(SOUND_DIG);
                        }
                        else if (world->canActorMoveTo(this, x, y + 1))
                            setDirection(up);
                    }
                    if (y == 60)
                        moveTo(x, y);
                }
                else
                    setDirection(up);
                break;
            case KEY_PRESS_SPACE: //Shoots a squirt object (by creating a squirt object in oil field)
                if (m_squirts >= 1)
                {
                    m_squirts--;
                    returnWorld()->addWaterSquirt(this, getDirection());
                    returnWorld()->playSound(SOUND_PLAYER_SQUIRT);
                }
                break;
            case KEY_PRESS_TAB: //Drops gold (by creating a bribe object in oil field)
                if (m_gold >= 1)
                {
                    m_gold--;
                    returnWorld()->addBribe(this);
                }
                break;
            case KEY_PRESS_ESCAPE: //resets the level (or if no lives left, ends game)
                setDead();
                break;
            case 'Z': //Uses a sonar to check for goodies in surrounding region
            case 'z':
                if (m_sonars >= 1)
                {
                    m_sonars--;
                    returnWorld()->isNearActor(this, 12);
                    returnWorld()->playSound(SOUND_SONAR);
                }
                break;
            default:
                break;
        }
    }
}

void FrackMan::getAnnoyed(int howMuch)
{
    m_health -= 2;
    if (m_health <= 0)
    {
        setDead();
        returnWorld()->playSound(SOUND_PLAYER_GIVE_UP);
    }
}

bool FrackMan::validPosition(int x, int y) const
{
    if (x < 0 || x > 60 || y < 0 || y > 60)
        return false;
    return true;
}

int FrackMan::getSquirts() const
{
    return m_squirts;
}

int FrackMan::getSonars() const
{
    return m_sonars;
}

int FrackMan::getGold() const
{
    return m_gold;
}

int FrackMan::getHealth() const
{
    return m_health;
}

void FrackMan::setSquirts(int howMuch)
{
    m_squirts += howMuch;
}

void FrackMan::setSonars(int howMuch)
{
    m_sonars += howMuch;
}

void FrackMan::setGold(int howMuch)
{
    m_gold += howMuch;
}

void FrackMan::setDead()
{
    returnWorld()->playSound(SOUND_PLAYER_GIVE_UP);
    setVisible(false);
    m_health = 0;
    setAlive(false);
}

void FrackMan::setBribe()
{}


///////////////////////////////////////////////////////////////////////////
//  Protester implementation (base class for regular and hardcore)
///////////////////////////////////////////////////////////////////////////

Protester::Protester(int imageID, int startX, int startY, Direction startDirection, float size,
	unsigned int depth, StudentWorld* world, int health)
	:Actor(imageID, startX, startY, startDirection, size, depth, world), m_health(health)
{
    numMoves = world->randInt(8, 60);
    restingTicks = max(0, 3 - world->getCurrentLevel() / 4);
    waitingTicksToShout = 15;
    leaveField = false;
    setVisible(true);
}

Protester::~Protester()
{}

int Protester::getHealth() const
{
	return m_health;
}

bool Protester::getLeaveOilFieldState() const
{
    return leaveField == true;
}


void Protester::decHealth(int hitpoints) {
	m_health -= hitpoints;
}

void Protester::setDead()
{
	if (getID() == IID_PLAYER)
		returnWorld()->playSound(SOUND_PLAYER_GIVE_UP);
	else if (getID() == IID_HARD_CORE_PROTESTER || getID() == IID_PROTESTER)
		returnWorld()->playSound(SOUND_PROTESTER_GIVE_UP);

	setVisible(false);
	m_health = 0;
	setAlive(false);
}

void Protester::setBribe()
{
    leaveField = true;
}

void Protester::setLeaveOilFieldState(bool value)
{
    leaveField = value;
}

void Protester::doesTheSomething()
{
    int x = getX();
    int y = getY();
    
    if (!isAlive())
        return;
    if (leaveField == true)
        restingTicks = 0;

    if (restingTicks > 0 && newRestingTicks > 0)	// rest state
    {
        restingTicks--;
        newRestingTicks--;
        return;
    }
    if (restingTicks > 0)
    {
        restingTicks--;
        return;
    }
    if (newRestingTicks > 0)
    {
        newRestingTicks--;
        return;
    }
    if (restingTicks == 0)
        restingTicks = max(0, 3 - returnWorld()->getCurrentLevel() / 4);
    
    waitingTicksToShout--;
    
    if (leaveField == true)
    {
        if (getX() == 60 && getY() == 60) { // exit point
            setDead();
            restingTicks = INT_MAX;
            return;
        }
        // move one square closer to its exit point(at x = 60, y = 60)
        setDirection(up);
        if (returnWorld()->canActorMoveTo(this, getX(), getY()))
            doMove(x, y, getDirection());
        else {
            setDirection(right);
            doMove(x, y, getDirection());
        }
        
        restingTicks = max(0, 3 - returnWorld()->getCurrentLevel() / 4);
        return;
    }
    
    //If the protestor is within the proper distance of the frackman, then annoy him
    //Reset the ticks to wait before you can shout again, and reset resting ticks
    if (returnWorld()->isNearFrackMan(this, 4)
        && returnWorld()->facingTowardFrackMan(this)
        && waitingTicksToShout <= 0)
    {
        returnWorld()->playSound(SOUND_PROTESTER_YELL);
        returnWorld()->annoyFrackMan();
        waitingTicksToShout = 15;
        newRestingTicks = max(50, 100 - returnWorld()->getCurrentLevel() * 10);
        return;
    }
    
    //If the protester is right next to the frackman, then he should stop moving
    if (returnWorld()->isNearFrackMan(this, 3))
        return;
    
    /// if frackman is in line of sight, move 1 toward
    
    GraphObject::Direction dir = returnWorld()->lineOfSightToFrackMan(this);
    
    if (dir != GraphObject::none && !returnWorld()->isNearFrackMan(this, 3) )
    {
        // face frackman and take a step towards him if possible
        
        if (canMove(x, y, dir))
        {
            setDirection(dir);
            doMove(x, y, dir);
            numMoves = 0;
            return;
        }
    }
    
    if (numMoves > 0 && canMove(x, y, getDirection()))
    {
        doMove(x, y, getDirection());
        return;
    }
    else
    {
        //If number of moves is 0, and you can't move in a given direction, change direction
        changeDirectionToMove();
    }
    
    return;

}

void Protester::getAnnoyed(int howMuch)
{
    decHealth(howMuch);
    
    //If dead, then prepare to leave the oil field
    if (getHealth() <= 0)
    {
        leaveField = true;
        returnWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
        restingTicks = 0;
        returnWorld()->increaseScore(100);
    }
    else //Just annoy the protester
    {
        returnWorld()->playSound(SOUND_PROTESTER_ANNOYED);
        restingTicks = max(50, 100 - returnWorld()->getCurrentLevel() * 10);
    }
}

void Protester::changeDirectionToMove()
{
    int x = getX();
    int y = getY();
    numMoves = 0;
    Direction direction = getDirection();
    do
    {
        int randNum = rand()%4;
        if (randNum == 0)
            direction = left;
        if (randNum == 1)
            direction = right;
        if (randNum == 2)
            direction = up;
        if (randNum == 3)
            direction = down;
    } while (canMove(x,y,direction) == false); //keep changind directions, until a valid direction is found
    setDirection(direction);
    numMoves = returnWorld()->randInt(8, 60); //reset the number of moves
}

bool Protester::canMove(int x, int y, int direction)
{
    //Check to see if the protester can move in a given direction
    if (direction == left)
        if (returnWorld()->canActorMoveTo(this, x - 1, y))
            return true;
    if (direction == right)
        if (returnWorld()->canActorMoveTo(this, x + 1, y))
            return true;
    if (direction == up)
        if (returnWorld()->canActorMoveTo(this, x, y + 1))
            return true;
    if (direction == down)
        if (returnWorld()->canActorMoveTo(this, x, y - 1))
            return true;
    return false;
}

void Protester::doMove(int x, int y, int direction)
{
    //If you can move in a given direction, then move in that direction
    if (direction == left)
        if (canMove(x - 1, y, left))
            moveTo(x - 1, y);
    if (direction == right)
        if (canMove(x - 1, y, right))
            moveTo(x + 1, y);
    if (direction == up)
        if (canMove(x, y + 1, up))
            moveTo(x, y + 1);
    if (direction == down)
        if (canMove(x, y - 1, down))
            moveTo(x, y - 1);
}
///////////////////////////////////////////////////////////////////////////
//  RegularProtester implementation
///////////////////////////////////////////////////////////////////////////

RegularProtester::RegularProtester(int x, int y, StudentWorld* world)
	:Protester(IID_PROTESTER, x, y, left, 1.0, 0, world, 5)
{}

RegularProtester::~RegularProtester()
{}

void RegularProtester::doSomething()
{
    //Call protester's method to initiate the move
    doesTheSomething();
}

///////////////////////////////////////////////////////////////////////////
//  HardcoreProtester implementation
///////////////////////////////////////////////////////////////////////////

HardcoreProtester::HardcoreProtester(int x, int y, StudentWorld* world)
	:Protester(IID_HARD_CORE_PROTESTER, x, y, left, 1.0, 0, world, 20)
{}

HardcoreProtester::~HardcoreProtester()
{}

void HardcoreProtester::doSomething()
{
    //Call the protester's method to intiate the move
    doesTheSomething();
}

void HardcoreProtester::setBribe()
{
    setLeaveOilFieldState(false);
}

///////////////////////////////////////////////////////////////////////////
//  Boulder implementation
///////////////////////////////////////////////////////////////////////////

Boulder::Boulder(int x, int y, StudentWorld* world)
	:Actor(IID_BOULDER, x, y, down, 1.0, 1, world), m_state(STABLE_STATE), m_waitingStateCounter(0)
{
	setVisible(true);
}

Boulder::~Boulder()
{}

int Boulder::getWaitingStateCounter() const
{
	return m_waitingStateCounter;
}

bool Boulder::isDirtBelow() const
{
    //Check to see if there is dirt below the boulder
	int x = getX();
	int y = getY();
	bool dirtBelow = false;
	for (int i = 0; i < 4; i++)
	{
		if (y != 0 && returnWorld()->isDirt(x + i, y - 1))
			dirtBelow = true;
	}
	if (!dirtBelow)
	{
		return false;
	}
	return true;
}

void Boulder::moveBoulder()
{
	bool frackManBelow = false;
	bool ProtesterBelow = false;
	bool boulderBelow = false;

	int x = getX();
	int y = getY();

	moveTo(x, y - 1);

    //Check to see if the frackman, protester, or another boulder is below the boulder
	if (returnWorld()->isNearFrackMan(this, 3))
		frackManBelow = true;
	if (isDirtBelow() || boulderBelow == true || y == -1)
	{
		setAlive(false);
	}
	if (ProtesterBelow == true)
	{
		returnWorld()->annoyProtester(this, 20);
        returnWorld()->increaseScore(500);
	}
	if (frackManBelow == true)
	{
		returnWorld()->killFrackMan();
	}
}

void Boulder::doSomething()
{
	if (!isAlive())
		return;

    //Initial state when there is dirt below the boulder
	if (m_state == STABLE_STATE)
	{
		if (!isDirtBelow())
		{
			m_state = WAITING_STATE;
			m_waitingStateCounter++;
		}
		else
			return;
	}
	if (m_state == WAITING_STATE) //If dirt was removed below the boulder, change to waiting state
	{
		if (getWaitingStateCounter() == 30)
		{
			m_state = FALLING_STATE;
			returnWorld()->playSound(SOUND_FALLING_ROCK);
		}
		else
			m_waitingStateCounter++;
	}
	if (m_state == FALLING_STATE) //If the boulder is in a falling state check to see if it can move down
	{
		moveBoulder();
	}
}

void Boulder::getAnnoyed(int howMuch)
{}

bool Boulder::canActorsPassThroughMe() const
{
	return false;
}

void Boulder::setBribe()
{}
///////////////////////////////////////////////////////////////////////////
//  Barrel implementation
///////////////////////////////////////////////////////////////////////////

Barrel::Barrel(int x, int y, StudentWorld* world)
	:Actor(IID_BARREL, x, y, right, 1.0, 2, world)
{}

Barrel::~Barrel()
{}

void Barrel::doSomething()
{
	if (!isAlive())
		return;

	if (!isVisible() && returnWorld()->isNearFrackMan(this, 4)) //Activate visibility
	{
		setVisible(true);
		return;
	}
	if (returnWorld()->isNearFrackMan(this, 3)) //Frackman picks up barrel
	{
		setAlive(false);
		returnWorld()->setBarrel(-1);
		returnWorld()->playSound(SOUND_FOUND_OIL);
		returnWorld()->increaseScore(1000);
		return;
	}

}

void Barrel::getAnnoyed(int howMuch)
{}

void Barrel::setBribe()
{}

///////////////////////////////////////////////////////////////////////////
//  Gold implementation
///////////////////////////////////////////////////////////////////////////

Gold::Gold(int x, int y, StudentWorld* world)
	:Actor(IID_GOLD, x, y, right, 1.0, 2, world)
{}

Gold::~Gold()
{}

void Gold::doSomething()
{
	if (!isAlive())
		return;
	if (!isVisible() && returnWorld()->isNearFrackMan(this, 4)) //Activate visibility
	{
		setVisible(true);
		return;
	}
	if (returnWorld()->isNearFrackMan(this, 3)) //FrackMan picks up gold
	{
		setAlive(false);
        setVisible(false);
		returnWorld()->playSound(SOUND_GOT_GOODIE);
		returnWorld()->increaseScore(10);
		returnWorld()->setGold(1);
	}

}

void Gold::getAnnoyed(int howMuch)
{}

void Gold::setBribe()
{}

///////////////////////////////////////////////////////////////////////////
//  Bribe implementation
///////////////////////////////////////////////////////////////////////////

Bribe::Bribe(int x, int y, StudentWorld* world)
	:Actor(IID_GOLD, x, y, right, 1.0, 2, world)
{
	setVisible(true);
	m_ticks = 100;
}

void Bribe::doSomething()
{
    if (!isAlive())
        return;
    m_ticks--;
    if (m_ticks <= 0)
    {
        setAlive(false);
        setVisible(false);
        return;
    }
	if (returnWorld()->isNearProtester(this, 3)) //Protester picks up bribe
	{
		setAlive(false);
		returnWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
		returnWorld()->increaseScore(25);
		returnWorld()->setBribe(1);
	}
}

void Bribe::getAnnoyed(int howMuch)
{}

Bribe::~Bribe()
{}

void Bribe::setBribe()
{}
///////////////////////////////////////////////////////////////////////////
//  Sonar implementation
///////////////////////////////////////////////////////////////////////////

Sonar::Sonar(int x, int y, StudentWorld* world)
	:Actor(IID_SONAR, x, y, right, 1.0, 2, world)
{
	setVisible(true);
	m_ticks = max(100, 300 - 10 * returnWorld()->getCurrentLevel());
}

Sonar::~Sonar()
{}

void Sonar::doSomething()
{
	if (!isAlive())
		return;
    m_ticks--;
	if (m_ticks <= 0) //If ticks expire, remove from oil field
	{
		setAlive(false);
        setVisible(false);
        return;
	}
	if (returnWorld()->isNearFrackMan(this, 3)) //FrackMan picks up sonar
	{
		setAlive(false);
		returnWorld()->playSound(SOUND_GOT_GOODIE);
		returnWorld()->setSonars(1);
		returnWorld()->increaseScore(75);
	}
}

void Sonar::getAnnoyed(int howMuch)
{}

void Sonar::setBribe()
{}

///////////////////////////////////////////////////////////////////////////
//  WaterSquirt implementation
///////////////////////////////////////////////////////////////////////////

WaterSquirt::WaterSquirt(int x, int y, Direction direction, StudentWorld* world)
	:Actor(IID_WATER_SPURT, x, y, direction, 1.0, 1, world), m_distance(5)
{
	setVisible(true);
}

WaterSquirt::~WaterSquirt()
{}

void WaterSquirt::doSomething()
{
	int direction = getDirection();
	StudentWorld* world = returnWorld();
	int x = getX();
	int y = getY();
	bool isDirt = false;

	if (!isAlive())
		return;

	if (returnWorld()->isNearProtester(this, 3)) //If next to protester, stop moving
	{
		returnWorld()->annoyProtester(this, 2);
		setAlive(false);
		setVisible(false);
	}

	if (m_distance == 0) //If squirt traveled its distance, then remove squirt
	{
		setAlive(false);
		setVisible(false);
	}

	switch (direction) //Check to see if squirt can move in the given direction
	{
	case left:
		if (world->isDirt(x, y, left) || (x < 0))
			isDirt = true;
		break;
	case right:
		if (world->isDirt(x, y, right) || (x > 60))
			isDirt = true;
		break;
	case down:
		if (world->isDirt(x, y, down) || (y < 0))
			isDirt = true;
		break;
	case up:
		if (world->isDirt(x, y, up) || (y > 60))
			isDirt = true;
		break;
	}
	if (isDirt == true ||
		returnWorld()->isNearBoulder(getX(), getY(), 3))
	{
		setAlive(false);
		setVisible(false);
	}

	else
	{
		moveDir(getDirection(), 1); //If the squirt moves, then reduce it's allowed travel distance
		m_distance--;
	}

}

void WaterSquirt::getAnnoyed(int howMuch)
{}

void WaterSquirt::setBribe()
{}

///////////////////////////////////////////////////////////////////////////
//  WaterPool implementation
///////////////////////////////////////////////////////////////////////////

WaterPool::WaterPool(int x, int y, StudentWorld* world)
	:Actor(IID_WATER_POOL, x, y, right, 1.0, 2, world)
{
	setVisible(true);
	m_ticks = max(100, 300 - 10 * returnWorld()->getCurrentLevel());
}

WaterPool::~WaterPool()
{}

void WaterPool::doSomething()
{
	m_ticks--;
	if (!isAlive())
		return;
	if (m_ticks <= 0) //If ticks runs out, remove pool from oil field
	{
		setAlive(false);
        setVisible(false);
        return;
	}
	if (returnWorld()->isNearFrackMan(this, 3)) //FrackMan picks up the pool of water
	{
		setAlive(false);
		returnWorld()->playSound(SOUND_GOT_GOODIE);
		returnWorld()->setSquirts(5);
		returnWorld()->increaseScore(100);
	}
}

void WaterPool::getAnnoyed(int howMuch)
{}

void WaterPool::setBribe()
{}

///////////////////////////////////////////////////////////////////////////
//  Dirt implementation
///////////////////////////////////////////////////////////////////////////

Dirt::Dirt(int x, int y, StudentWorld* world)
	:Actor(IID_DIRT, x, y, right, 0.25, 3, world)
{
	setVisible(true);
}

Dirt::~Dirt()
{}

void Dirt::doSomething()
{}

void Dirt::getAnnoyed(int howMuch)
{}

bool Dirt::canActorsPassThroughMe() const
{
	return false;
}

void Dirt::setBribe()
{}