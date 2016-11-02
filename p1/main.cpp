#include "Arena.h"
#include "History.h"
#include "Player.h"
#include "globals.h"
int main()
{
    Arena a(4, 4);
    a.addPlayer(2, 4);
    a.addRobot(3, 2);
    a.addRobot(2, 3);
    a.addRobot(1, 4);
    a.player()->moveOrAttack(LEFT);
    a.player()->moveOrAttack(UP);
    a.player()->moveOrAttack(LEFT);
    a.player()->moveOrAttack(LEFT);
    a.player()->moveOrAttack(DOWN);
    a.player()->moveOrAttack(DOWN);
    a.player()->moveOrAttack(LEFT);
    a.player()->moveOrAttack(UP);
    a.player()->moveOrAttack(UP);
    a.player()->moveOrAttack(UP);
    a.history().display();
}
