//StudentWorld.h

#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <vector>

using namespace std;

const int KEY_PRESS_NONE = 0;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir);
	~StudentWorld();
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	bool isDirt(int x, int y) const;
	bool isDirt(int x, int y, int direction) const;
	bool isBoulder(int x, int y, int direction) const;
	bool removeDirt(int x, int y, int direction);
	void removeDirt(int x, int y);
	void killFrackMan();
	void annoyProtester(Actor* a, int howMuch);
	void annoyFrackMan(); 
	void setBarrel(int howMuch);
	void setGold(int howMuch);
	void setBribe(int howMuch);
	void addBribe(Actor* a);
	void addWaterSquirt(Actor* a, Actor::Direction direction);
	void setSonars(int howMuch);
	void setSquirts(int howMuch);
	double radius(int x1, int y1, int x2, int y2) const;
	int randInt(int min, int max);
	bool isNearFrackMan(Actor* a, int radius) const;
	bool isNearProtester(Actor* a, int radius);
	bool isNearBoulder(int x, int y, int r);
	void isNearActor(Actor* a, int r);
	bool facingTowardFrackMan(Actor* a) const;
	GraphObject::Direction lineOfSightToFrackMan(Actor* a) const;
	//GraphObject::Direction DirToFrackMan(Actor* a) const;
	bool canActorMoveTo(Actor* a, int x, int y) const;
    int getCurrentLevel() const;
    bool noDirt(int startX, int startY) const;
    bool checkRadius(int x, int y);

private:
	std::vector<Actor*> m_actor;
	FrackMan* m_frackMan;
	Dirt* m_dirt[VIEW_WIDTH][VIEW_HEIGHT];
	void createDirt();
	void addInitialActors();
	void addMoreActors();
	bool isPlaceable(int x, int y);
	void updateDisplayText();
	int m_currentLevel;
	int m_ticks;
	int m_barrels;
	int m_protesters;

};

#endif // STUDENTWORLD_H_
