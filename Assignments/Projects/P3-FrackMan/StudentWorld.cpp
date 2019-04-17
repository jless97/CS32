#include "StudentWorld.h"
#include <string>
#include <utility>
#include <cstdlib>
#include <algorithm>
#include <random>
#include <cmath>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

StudentWorld::StudentWorld(std::string assetDir)
	:GameWorld(assetDir), m_currentLevel(0)
{}

StudentWorld::~StudentWorld()
{
	cleanUp();                                      // release memory just like in the cleanup function
}

int StudentWorld::init()
{
	m_ticks = 0;                                                // initialize member variables
	m_barrels = 0;
	m_protesters = 0;

	m_frackMan = new FrackMan(30, 60, this);

	createDirt();

	addInitialActors();                                         // Add initial objects

	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	m_ticks++;

	updateDisplayText();                                    // Update status text (see new function)

	m_frackMan->doSomething();
	if (!m_frackMan->isAlive())
	{
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}

	for (unsigned int i = 0; i < m_actor.size(); i++)	// Give each Actor a chance to do something
	{
		if (m_actor.at(i)->isAlive())
			m_actor.at(i)->doSomething();
		else
		{
			int id = m_actor.at(i)->getID();			/// update counts
			if (id == IID_BARREL)
				m_barrels--;
			if (id == IID_PROTESTER || id == IID_HARD_CORE_PROTESTER)
				m_protesters--;
		}
	}

	bool frackManDied = false;
	if (!m_frackMan->isAlive())
	{
		frackManDied = true;
		decLives();
	}
	for (unsigned int i = 0; i < m_actor.size(); i++) // Remove newly-dead actors after each tick
	{

		if (!m_actor.at(i)->isAlive())
		{
			delete m_actor.at(i);
			vector<Actor*>::iterator it = m_actor.begin();
			for (unsigned int j = 0; j < i; j++)
				it++;
			m_actor.erase(it);
			i--;
		}
	}

	if (m_barrels == 0)										/// Check if end of level
	{
		playSound(SOUND_FINISHED_LEVEL);
		advanceToNextLevel();
		m_currentLevel++;
		return GWSTATUS_FINISHED_LEVEL;
	}

	if (frackManDied)
		return GWSTATUS_PLAYER_DIED;

	addMoreActors();									// add protesters, waterpools, sonars

	return GWSTATUS_CONTINUE_GAME;
}


void StudentWorld::cleanUp()
{
	delete m_frackMan;

	for (unsigned int i = 0; i < m_actor.size(); i++)		// Delete the actors in the vector container
		delete m_actor.at(i);
	m_actor.clear();

	//Delete all dirt objects
	for (int i = 0; i < VIEW_WIDTH; i++)
		for (int j = 0; j < VIEW_HEIGHT; j++)
			if (m_dirt[i][j] != nullptr)
				delete m_dirt[i][j];
}

void StudentWorld::killFrackMan()
{
	m_frackMan->setDead();
}

void StudentWorld::annoyFrackMan()
{
	m_frackMan->getAnnoyed(2);
}

void StudentWorld::annoyProtester(Actor* a, int howMuch)
{
    //Check to find protester, and if it is the right protester, annoy him
	for (unsigned int i = 0; i < m_actor.size(); i++)
	{
		int imageID = m_actor.at(i)->getID();
		if (imageID == IID_PROTESTER || imageID == IID_HARD_CORE_PROTESTER)
        {
            if (radius(a->getX(), a->getY(), m_actor.at(i)->getX(), m_actor.at(i)->getY()) <= 4)
                m_actor.at(i)->getAnnoyed(howMuch);
        }
	}
}

void StudentWorld::setBarrel(int howMuch)
{
	m_barrels += howMuch;
}

void StudentWorld::setSquirts(int howMuch)
{
	m_frackMan->setSquirts(howMuch);
}

void StudentWorld::setSonars(int howMuch)
{
	m_frackMan->setSonars(howMuch);
}

void StudentWorld::setGold(int howMuch)
{
	m_frackMan->setGold(howMuch);
}

void StudentWorld::setBribe(int howMuch)
{
    //Find the protester, and give him the gold (bribe)
	int count = 0;
	vector<Actor*>::iterator it = m_actor.begin();

	while (it != m_actor.end())
	{
		if (count == 1)
			break;
		if ((*it)->getID() == IID_PROTESTER || (*it)->getID() == IID_HARD_CORE_PROTESTER)
		{
			(*it)->setBribe();
			count++;
		}
		it++;
	}
}

void StudentWorld::addBribe(Actor* a)
{
    // Add gold (bribe) to the oil field
	m_actor.push_back(new Bribe(a->getX(), a->getY(), this));
}

void StudentWorld::addWaterSquirt(Actor* a, Actor::Direction direction)
{
    //Add a squirt object to the oil field, depending on coordinates and direction of FrackMan
	int x = a->getX();
	int y = a->getY();

	if (direction == Actor::left)
		if (!(isDirt(x - 3, y) && !(isBoulder(x - 3, y, Actor::left))))
			m_actor.push_back(new WaterSquirt(a->getX() - 3, a->getY(), direction, this));
	if (direction == Actor::right)
		if (!(isDirt(x + 3, y) && !(isBoulder(x + 3, y, Actor::right))))
			m_actor.push_back(new WaterSquirt(a->getX() + 3, a->getY(), direction, this));
	if (direction == Actor::down)
		if (!(isDirt(x, y - 3) && !(isBoulder(x, y - 3, Actor::up))))
			m_actor.push_back(new WaterSquirt(a->getX(), a->getY() - 3, direction, this));
	if (direction == Actor::up)
		if (!(isDirt(x, y + 3) && !(isBoulder(x, y + 3, Actor::up))))
			m_actor.push_back(new WaterSquirt(a->getX(), a->getY() + 3, direction, this));
}

void StudentWorld::addInitialActors()
{
	// Add a Protester on very first tick
	m_actor.push_back(new RegularProtester(60, 60, this));

	// add gold nuggets
	int gold = max(5 - m_currentLevel / 2, 2);
	for (int i = 0; i < gold;)
	{
		int x = randInt(0, 60);
		int y = randInt(20, 56);
		if (isPlaceable(x, y))
		{
			m_actor.push_back(new Gold(x, y, this));
			i++;
		}
	}

	// add oil barrels
	int barrels = min(2 + m_currentLevel, 20);
	for (int i = 0; i < barrels;)
	{
		int x = randInt(0, 60);
		int y = randInt(20, 56);
		if (isPlaceable(x, y))
		{
			m_actor.push_back(new Barrel(x, y, this));
			m_barrels++;
			i++;
		}
	}

	// add boulders
	int boulders = min(m_currentLevel / 2 + 2, 6);
	for (int i = 0; i < boulders;)
	{
		int x = randInt(0, 60);
		int y = randInt(20, 56);
		if (isPlaceable(x, y))
		{
			m_actor.push_back(new Boulder(x, y, this));

			// Remove dirt around boulder
			removeDirt(x, y);
			i++;
		}
	}
}

void StudentWorld::addMoreActors()
{
	// add protester
	if (m_ticks >= max(25, 200 - m_currentLevel))
	{
		if (m_protesters < min(15, 2 + (int)(m_currentLevel*1.5)))
		{
			int odds = min(90, m_currentLevel * 10 + 30);
			if (rand() % 100 < odds)
				m_actor.push_back(new HardcoreProtester(60, 60, this));
			else
				m_actor.push_back(new RegularProtester(60, 60, this));
			m_protesters++;
			m_ticks = 0;    // reset ticks (mainly used by this function)
		}
	}
	// add sonar or waterpool
    int G = m_currentLevel * 25 + 300;
	if ((rand() % G == 0)) //1 in G chance to add sonar or waterpool
	{
		if ((rand() % 5) == 0) //1 in 5 chance
			m_actor.push_back(new Sonar(0, 60, this));
		else
		{
			int x, y;
			do {
				x = rand() % 60;
				y = rand() % 56;
            } while ((noDirt(x,y) == false) || (checkRadius(x,y) == false));

			m_actor.push_back(new WaterPool(x, y, this));
		}
	}
}

bool StudentWorld::isPlaceable(int x, int y)
{
    //Used for initial placement of boulders
	vector<Actor*>::iterator it = m_actor.begin();

	while (it != m_actor.end())
	{
		if (radius(x, y, (*it)->getX(), (*it)->getY()) <= 6)
			return false;
		it++;
	}
	if ((x >= 27 && x <= 33) && (y >= 4))
		return false;
	return true;
}

bool StudentWorld::checkRadius(int x, int y) 
{
    //Check to make sure there isn't another actor occupying a given location
    vector<Actor*>::iterator it = m_actor.begin();
    
    while (it != m_actor.end())
    {
        if (radius(x, y, (*it)->getX(), (*it)->getY()) <= 6)
            return false;
        it++;
    }
    return true;
}

bool StudentWorld::isNearFrackMan(Actor* a, int r) const
{
	if (radius(m_frackMan->getX(), m_frackMan->getY(), a->getX(), a->getY()) <= r)
		return true;
	return false;
}

bool StudentWorld::isNearProtester(Actor* a, int r)
{
	vector<Actor*>::iterator it = m_actor.begin();

	while (it != m_actor.end())
	{
		if ((*it)->getID() == IID_PROTESTER || (*it)->getID() == IID_HARD_CORE_PROTESTER)
			if (radius(a->getX(), a->getY(), (*it)->getX(), (*it)->getY()) <= r)
			{
				return true;
			}
		it++;
	}
	return false;
}

void StudentWorld::isNearActor(Actor* a, int r)
{
    //Check to see if frackman is near a barrel or gold
	vector<Actor*>::iterator it = m_actor.begin();

	while (it != m_actor.end())
	{
		if ((*it)->getID() == IID_BARREL || (*it)->getID() == IID_GOLD)
			if (radius(a->getX(), a->getY(), (*it)->getX(), (*it)->getY()) <= r)
			{
				(*it)->setVisible(true);
			}
		it++;
	}
}

bool StudentWorld::isNearBoulder(int x, int y, int r)
{
    //Check to see if an actor is near a boulder
	vector<Actor*>::iterator it = m_actor.begin();

	while (it != m_actor.end())
	{
		if ((*it)->getID() == IID_BOULDER)
			if (radius(x, y, (*it)->getX(), (*it)->getY()) <= r)
			{
				return true;
			}
		it++;
	}
	return false;
}

bool StudentWorld::facingTowardFrackMan(Actor* a) const
{
	return true;  /// to be done
}

GraphObject::Direction StudentWorld::lineOfSightToFrackMan(Actor* a) const
{
    //If the protester is in line with the FrackMan, then set its direction to that given direction
	int x = a->getX();
	int y = a->getY();

	GraphObject::Direction dir;

	int targetx = m_frackMan->getX();
	int targety = m_frackMan->getY();

	dir = GraphObject::up;
	for (int i = y; i < 61; i++) {
		if (i == targety && x == targetx)
			return dir;
	}
	dir = GraphObject::down;
	for (int i = y; i > -1; i--) {
		if (i == targety && x == targetx)
			return dir;
	}
	dir = GraphObject::left;
	for (int i = x; i > -1; i--) {
		if (i == targetx && y == targety)
			return dir;
	}
	dir = GraphObject::right;
	for (int i = x; i < 61; i++) {
		if (i == targetx && y == targety)
			return dir;
	}
    //If not in line, return direction none
	return GraphObject::none;
    
}

/*
GraphObject::Direction StudentWorld::DirToFrackMan(Actor* a) const
{
	int x = a->getX();
	int y = a->getY();
	
	GraphObject::Direction dir;

	int targetx = m_frackMan->getX();
	int targety = m_frackMan->getY();

	dir = GraphObject::up;
	for (int i = y; i < 60; i++) {
		if (i == targety)
			return dir;
	}
	dir = GraphObject::down;
	for (int i = y; i > 0; i--) {
		if (i == targety)
			return dir;
	}
	dir = GraphObject::left;
	for (int i = x; i > 0; i--) {
		if (i == targetx)
			return dir;
	}
	dir = GraphObject::right;
	for (int i = x; i < 60; i++) {
		if (i == targetx)
			return dir;
	}

	return GraphObject::none;
}
*/

bool StudentWorld::canActorMoveTo(Actor* a, int x, int y) const
{
	if (x<0 || x>60 || y<0 || y>60)
		return false;

	if (isDirt(x, y))
		return false;

    if (isBoulder(x,y, a->getDirection()))
        return false;

	return true;
}


void StudentWorld::updateDisplayText()
{
	int score = getScore();
	if (score < 0)
		score = 0;
	string text =
		"Scr: " + std::string(6 - to_string(score).length(), '0') + to_string(score) + "  "
		+ "Lvl: " + std::string(2 - to_string(m_currentLevel).length(), ' ') + to_string(m_currentLevel) + "  "
		+ "Lives: " + to_string(getLives()) + "  "
		+ "Hlth: " + std::string(3 - to_string(m_frackMan->getHealth() * 10).length(), ' ') + to_string(m_frackMan->getHealth() * 10) + "%  "
		+ "Wtr: " + std::string(2 - to_string(m_frackMan->getSquirts()).length(), ' ') + to_string(m_frackMan->getSquirts()) + "  "
		+ "Gld: " + std::string(2 - to_string(m_frackMan->getGold()).length(), ' ') + to_string(m_frackMan->getGold()) + "  "
		+ "Sonar: " + std::string(2 - to_string(m_frackMan->getSonars()).length(), ' ') + to_string(m_frackMan->getSonars()) + "  "
		+ "Oil Left: " + std::string(2 - to_string(m_barrels).length(), ' ') + to_string(m_barrels);

	setGameStatText(text);
}

int StudentWorld::getCurrentLevel() const
{
    //Was having a problem with the getLevel()
    return m_currentLevel;
}

double StudentWorld::radius(int x1, int y1, int x2, int y2) const
{
	int xdif = x2 - x1;
	int ydif = y2 - y1;
	int xsquared = xdif * xdif;
	int ysquared = ydif * ydif;
	double distance = sqrt(xsquared + ysquared);

	return distance;
}

int StudentWorld::randInt(int min, int max)
{
    //Generate a random number (taken from project 1)
	if (max < min)
		swap(max, min);
	static random_device rd;
	static mt19937 generator(rd());
	uniform_int_distribution<> distro(min, max);
	return distro(generator);
}

void StudentWorld::createDirt()
{
	for (int i = 0; i < 30; i++)   //Create all the dirt objects and set their visible/invisible status
	{
		for (int j = 0; j < VIEW_HEIGHT - 4; j++)
		{
			m_dirt[i][j] = new Dirt(i, j, this);
		}
		for (int j = VIEW_HEIGHT - 4; j < VIEW_HEIGHT; j++)
		{
			m_dirt[i][j] = new Dirt(i, j, this);
			m_dirt[i][j]->setVisible(false);
		}
	}
	for (int i = 30; i < 34; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m_dirt[i][j] = new Dirt(i, j, this);
		}
		for (int j = 4; j < VIEW_HEIGHT; j++)
		{
			m_dirt[i][j] = new Dirt(i, j, this);
			m_dirt[i][j]->setVisible(false);
		}
	}
	for (int i = 34; i < VIEW_WIDTH; i++)
	{
		for (int j = 0; j < VIEW_HEIGHT - 4; j++)
		{
			m_dirt[i][j] = new Dirt(i, j, this);
		}
		for (int j = VIEW_HEIGHT - 4; j < VIEW_HEIGHT; j++)
		{
			m_dirt[i][j] = new Dirt(i, j, this);
			m_dirt[i][j]->setVisible(false);
		}
	}
}

bool StudentWorld::isDirt(int x, int y) const
{
    if (x < 0 || x > 60 || y < 0 || y > 60)
        return false;
	if (m_dirt[x][y] != nullptr && m_dirt[x][y]->isVisible())
		return true;
	return false;
}

bool StudentWorld::isDirt(int x, int y, int direction) const
{
    if (x < 0 || x > 60 || y < 0 || y > 60)
        return false;
    
    //Based off of direction and coordinates, check fours squares in that direction for dirt
	bool dirtPresent = false;
	switch (direction)
	{
        case GraphObject::left:
            if (m_dirt[x - 1][y] != nullptr && m_dirt[x - 1][y]->isVisible())
                dirtPresent = true;
            if (m_dirt[x - 1][y + 1] != nullptr && m_dirt[x - 1][y + 1]->isVisible())
                dirtPresent = true;
            if (m_dirt[x - 1][y + 2] != nullptr && m_dirt[x - 1][y + 2]->isVisible())
                dirtPresent = true;
            if (m_dirt[x - 1][y + 3] != nullptr && m_dirt[x - 1][y + 3]->isVisible())
                dirtPresent = true;
            break;
        case GraphObject::right:
            if (m_dirt[x + 4][y] != nullptr && m_dirt[x + 4][y]->isVisible())
                dirtPresent = true;
            if (m_dirt[x + 4][y + 1] != nullptr && m_dirt[x + 4][y + 1]->isVisible())
                dirtPresent = true;
            if (m_dirt[x + 4][y + 2] != nullptr && m_dirt[x + 4][y + 2]->isVisible())
                dirtPresent = true;
            if (m_dirt[x + 4][y + 3] != nullptr && m_dirt[x + 4][y + 3]->isVisible())
                dirtPresent = true;
            break;
        case GraphObject::down:
            if (m_dirt[x][y - 1] != nullptr && m_dirt[x][y - 1]->isVisible())
                dirtPresent = true;
            if (m_dirt[x + 1][y - 1] != nullptr && m_dirt[x + 1][y - 1]->isVisible())
                dirtPresent = true;
            if (m_dirt[x + 2][y - 1] != nullptr && m_dirt[x + 2][y - 1]->isVisible())
                dirtPresent = true;
            if (m_dirt[x + 3][y - 1] != nullptr && m_dirt[x + 3][y - 1]->isVisible())
                dirtPresent = true;
            break;
        case GraphObject::up:
            if (m_dirt[x][y + 4] != nullptr && m_dirt[x][y + 4]->isVisible())
                dirtPresent = true;
            if (m_dirt[x + 1][y + 4] != nullptr && m_dirt[x + 1][y + 4]->isVisible())
                dirtPresent = true;
            if (m_dirt[x + 2][y + 4] != nullptr && m_dirt[x + 2][y + 4]->isVisible())
                dirtPresent = true;
            if (m_dirt[x + 3][y + 4] != nullptr && m_dirt[x + 3][y + 4]->isVisible())
                dirtPresent = true;
            break;
	}
	if (dirtPresent == true)
		return true;
	return false;
}


bool StudentWorld::noDirt(int startX, int startY) const
{
	if (startX> VIEW_WIDTH - 4 || startY> VIEW_HEIGHT - 4)
	{
		return false;
	}
	for (int i = 0; i<4; i++)
	{
		for (int j = 0; j<4; j++)
		{
			if (isDirt(startX + i, startY + j) == true)
			{
				return false;
			}
		}
	}
	return true;
}

bool StudentWorld::isBoulder(int x, int y, int direction) const
{
    if (x < 0 || x > 60 || y < 0 || y > 60)
        return false;
    //Similar to 3 parameter is dirt, check a 4x4 region for a boulder
	bool boulderPresent = false;
	for (unsigned int i = 0; i < m_actor.size(); i++)
	{
		int imageID = m_actor.at(i)->getID();
		if (imageID == IID_BOULDER)
		{
			int a = m_actor.at(i)->getX();
			int b = m_actor.at(i)->getY();
			switch (direction)
			{
			case KEY_PRESS_LEFT:
				for (int i = 0; i < 4; i++)
				{
					if ((x - 3) == a && (y + i) == b)
						boulderPresent = true;
					if ((x - 3) == a && (y + i) == b + 1)
						boulderPresent = true;
					if ((x - 3) == a && (y + i) == b + 1)
						boulderPresent = true;
					if ((x - 3) == a && (y + i) == b + 3)
						boulderPresent = true;
				}
				break;
			case KEY_PRESS_RIGHT:
				for (int i = 0; i < 4; i++)
				{
					if ((x + 3) == a && (y + i) == b)
						boulderPresent = true;
					if ((x + 3) == a && (y + i) == b + 1)
						boulderPresent = true;
					if ((x + 3) == a && (y + i) == b + 1)
						boulderPresent = true;
					if ((x + 3) == a && (y + i) == b + 3)
						boulderPresent = true;
				}
				break;
			case KEY_PRESS_UP:
				for (int i = 0; i < 4; i++)
				{
					if ((x + i) == a && (y + 3) == b)
						boulderPresent = true;
					if ((x + i) == a + 1 && (y + 3) == b)
						boulderPresent = true;
					if ((x + i) == a + 2 && (y + 3) == b)
						boulderPresent = true;
					if ((x + i) == a + 3 && (y + 3) == b)
						boulderPresent = true;
				}
				break;
			case KEY_PRESS_DOWN:
				for (int i = 0; i < 4; i++)
				{
					if ((x + i) == a && (y - 3) == b)
						boulderPresent = true;
					if ((x + i) == a + 1 && (y - 3) == b)
						boulderPresent = true;
					if ((x + i) == a + 2 && (y - 3) == b)
						boulderPresent = true;
					if ((x + i) == a + 3 && (y - 3) == b)
						boulderPresent = true;
				}
				break;
			}
		}
	}
	if (boulderPresent == true)
		return true;
	return false;
}

bool StudentWorld::removeDirt(int x, int y, int direction)
{
	bool playSound = false; //To keep track of when dirt is being mined (set to invisible)

	for (int i = 0; i < 4; i++) //Check to see if FrackMan spawns in dirt location
	{
		if (isDirt(x, y + i)) //If FrackMan were to be placed in dirt to begin, set all dirt to invisible
		{
			m_dirt[x][y + i]->setVisible(false);
			playSound = true;
		}
		if (isDirt(x + 1, y + i))
		{
			m_dirt[x + 1][y + i]->setVisible(false);
			playSound = true;
		}
		if (isDirt(x + 2, y + i))
		{
			m_dirt[x + 2][y + i]->setVisible(false);
			playSound = true;
		}
		if (isDirt(x + 3, y + i))
		{
			m_dirt[x + 3][y + i]->setVisible(false);
			playSound = true;
		}
	}

	switch (direction) //Upon moving either left,right,up, or down, check to see if there is dirt there
	{
	case KEY_PRESS_LEFT:
		for (int i = 0; i < 4; i++)
			if (isDirt(x, y + i))
			{
				playSound = true;
				m_dirt[x][y + i]->setVisible(false);
			}
		break;
	case KEY_PRESS_RIGHT:
		for (int i = 0; i < 4; i++)
			if (isDirt(x + 3, y + i))
			{
				m_dirt[x + 3][y + i]->setVisible(false);
				playSound = true;
			}
		break;
	case KEY_PRESS_UP:
		for (int i = 0; i < 4; i++)
			if (isDirt(x + i, y + 3))
			{
				m_dirt[x + i][y + 3]->setVisible(false);
				playSound = true;
			}
		break;
	case KEY_PRESS_DOWN:
		for (int i = 0; i < 4; i++)
			if (isDirt(x + i, y))
			{
				m_dirt[x + i][y]->setVisible(false);
				playSound = true;
			}
		break;
	}
	if (playSound == true) //If dirt was ever mined by FrackMan (set to invisible), return true to play sound
		return true;
	return false;
}

void StudentWorld::removeDirt(int x, int y)
{
	for (int i = x; i < x + 4; i++)
	{
		for (int j = y; j < y + 4; j++)
		{
			m_dirt[i][j]->setVisible(false);
		}
	}
}