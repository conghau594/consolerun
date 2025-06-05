#include "header.h"
#include <assert.h>

int main(void) {
    MPOS man = drawStandingMan(1, 1);
    drawRoute(ROUTE_LINE);
    printScore(man);
    paused = 0;
    pauseGame();
    man.isDead = 1;
    checkIfDead(&man);
    man = drawRunningMan(2, 2);
    drawJumpingMan(2, 3);
    man = moveForward(&man, 0);
    man.isDead = 1;
    runEffect(&man);
    testBody(&man);
    jump(&man);
    stoop(&man, VK_DOWN);
    shootBullets(&man);

    BLIST list = {NULL, NULL};
    BPOS bullet = {NULL, 5, 5};
    insertTail(&list, &bullet);
    assert(list.pHead == &bullet && list.pTail == &bullet);
    removeHead(&list);
    assert(list.pHead == NULL && list.pTail == NULL);

    return 0;
}
