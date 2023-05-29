#ifndef CH_CONSOLE_RUN_H
#define CH_CONSOLE_RUN_H

#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>



#define GAME_PACE 100
#define ROUTE_LENGTH 60
#define MIN_DIST_OF_BULLETS 10 //gia tri ban dau la 10
#define MAX_DIST_OF_BULLETS 30 //gia tri ban dau la 30

#define START_ROUTE 3
#define ROUTE_LINE 8

#define CONSOLE_WIDTH (ROUTE_LENGTH + 2 * START_ROUTE)
#define CONSOLE_HEIGHT (ROUTE_LINE + 5)

#define KEY_UP 65
#define KEY_DOWN 66
#define KEY_LEFT 67
#define KEY_RIGHT 68

#define KEY_R 114
#define KEY_ESC 27
#define KEY_ENT 13


short restarted = 1;
short paused = 0;

typedef enum _MAN_STATE {
    STOOPING = -1,
    STANDING = 0,
    RUNNING = 0,
    JUMPING = 1
} MSTATE;

typedef struct _BULLET_POSITION {
    struct _BULLET_POSITION*    nextBulletPtr;
    INT                         xB;
    INT                         yB;
} BPOS;

typedef struct _BULLET_SINGLE_LINKED_LIST {
    BPOS* pHead;
    BPOS* pTail;
} BLIST;

typedef struct _MAN_POSITION {
    INT         xM;
    INT         yM;
    MSTATE   state;
    UINT distancePassed;
    UINT numBulletsPassed;
    INT      deadPoint;
    INT      isDead;
    BYTE     bodyZone[16]; //bodyZone[i] = 0 if none of body part in there
    BPOS     fatalBullet;
} MPOS;

void drawRoute(INT yFloor);
void printScore(MPOS man);

MPOS drawStandingMan(INT xM, INT yM);
MPOS drawRunningMan(INT xM, INT yM);
void drawJumpingMan(INT xM, INT yM);

void runEffect(MPOS* man);
void checkIfDead(MPOS* man);

void jump(MPOS* man);
void stoop(MPOS* man, int keyPress);
MPOS moveForward(MPOS* man, unsigned short steps);

void shootBullets(MPOS* man);

void insertTail(BLIST* pList, BPOS* pBullet);
void removeHead(BLIST* pList);

void testBody(MPOS* man); //delete after

#endif