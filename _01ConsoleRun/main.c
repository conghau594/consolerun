#define DEFINE_GLOBALS
#include "header.h"


#ifndef TESTING
int main(void) {
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    HWND hWnd = GetConsoleWindow();

    //set font size
    CONSOLE_FONT_INFOEX cfi = { sizeof(cfi) };
    // Populate cfi with the screen buffer's current font info
    GetCurrentConsoleFontEx(hStdout, FALSE, &cfi);

    // Modify the font size in cfi
    //cfi.dwFontSize.X *= 2;
    cfi.dwFontSize.Y = 36;

    // Use cfi to set the screen buffer's new font
    SetCurrentConsoleFontEx(hStdout, FALSE, &cfi);

    //hide cursor
    const CONSOLE_CURSOR_INFO CUR_INFO = { 10, 0 };
    SetConsoleCursorInfo(hStdout, &CUR_INFO);


    //remove console screen buffer. This command must be put after 'SetConsoleWindowInfo',...
    //...and value of 'NewSize' must be greater than CONSOLE_WIDTH and CONSOLE_HEIGHT just 1
    COORD NewSize = { CONSOLE_WIDTH + 1, CONSOLE_HEIGHT + 1 };
    SetConsoleScreenBufferSize(hStdout, NewSize);

    //set size of console
    SMALL_RECT WindowSize = { 0, 0, CONSOLE_WIDTH, CONSOLE_HEIGHT };
    SetConsoleWindowInfo(hStdout, 1, &WindowSize);

    //disable resize console by mouse
    SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_SIZEBOX);

    //disable maximizing
    DeleteMenu(GetSystemMenu(hWnd, FALSE), SC_MAXIMIZE, MF_BYCOMMAND);

    //Hide scroll bar
    ShowScrollBar(hWnd, SB_BOTH, FALSE);

    //disable select text by mouse
    SetConsoleMode(hStdin, ~ENABLE_QUICK_EDIT_MODE);

    //set title of console window
    SetConsoleTitleW(TEXT("Console Run  |  Cong Hau"));

    printf("\n    Press any key to start...");
    short first_key, second_key, third_key;
    first_key = _getch();

    while (restarted) {
        //clear console
        system("cls");

        //clear input buffer
        FlushConsoleInputBuffer(hStdin);

        drawRoute(ROUTE_LINE);
        MPOS man3 = drawStandingMan(START_ROUTE + 2, ROUTE_LINE-1);

        Sleep(500);
        moveForward(&man3, 5);
        FlushConsoleInputBuffer(hStdin);

        int* isDead = &(man3.isDead);

        HANDLE threadHnd1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)checkIfDead, (LPVOID)&man3, 0, NULL);
        HANDLE threadHnd2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)shootBullets, (LPVOID)&man3, 0, NULL);
        HANDLE threadHnd3 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)runEffect, (LPVOID)&man3, 0, NULL);
        //HANDLE threadHnd4 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)testBody, (LPVOID)&man3, 0, NULL);

        for (;;) {

            if (*isDead) {
                FlushConsoleInputBuffer(hStdin);
                break;
            }

            if (_kbhit()) {
                first_key = _getch();
                //printf("\033[1;1H%d", first_key);
                if (first_key == KEY_ESC) {
                    second_key = _getch();
                    //printf("\033[2;1H%d", second_key);

                    third_key = _getch();
                    //printf("\033[3;1H%d", third_key);

                    if (third_key == KEY_UP) {
                        jump(&man3);
                    }
                    else if (third_key == KEY_DOWN) {
                        stoop(&man3, VK_DOWN);
                    }
                    continue;
                }
                //*
                else if (first_key == 'p') {
                    paused = 1;
                                            
                    for (;;) {
                        second_key = _getch();
                        if (second_key == 'p') {
                            paused = 0;
                            break;
                        }

                        if (second_key == 'e') {
                            *isDead = 1;
                            restarted = 0;
                            break;
                        }
                    }
                    
                    continue;
                }
                //*/
            }
        }
        if (threadHnd1) CloseHandle(threadHnd1);
        if (threadHnd2) CloseHandle(threadHnd2);
        if (threadHnd3) CloseHandle(threadHnd3);
        //if (threadHnd4) CloseHandle(threadHnd4);

        for (;;) {
            if (restarted == 0) break;

            first_key = _getch();
            if (first_key == 'r') restarted = 1;
            else if (first_key == 'e') restarted = 0;
            else continue;

            break;
        }
    }
    return 0;
}
#endif /* TESTING */


void drawRoute(INT yFloor) {
    int i;
    for (i = START_ROUTE; i < ROUTE_LENGTH + START_ROUTE + 1; i++)
        printf("\033[%d;%dHT", yFloor + 1, i + 1);

    printf("\033[%d;%dHYour score: ", yFloor + 4, ROUTE_LENGTH + START_ROUTE - 21);
}

void printScore(MPOS man) {
    printf("\033[%d;%dH%.10u",
        man.yM + 5,
        ROUTE_LENGTH + START_ROUTE - 9,
        man.distancePassed + 10 * man.numBulletsPassed
    );
}


void pauseGame(void) {
    while (paused) {
        Sleep(1);
    }
}


void checkIfDead(MPOS* man) {
    int* isDead = &(man->isDead);
    char* arr = man->bodyZone;
    int* deadPnt = &(man->deadPoint);
    INT yM = man->yM;
    INT xM = man->xM;

    while (*isDead == 0) {
        while (paused) {}

        if (arr[*deadPnt] == 1) {
            *isDead = 1;

            printf("\033[%d;%dHGAME OVER", yM - 4, ROUTE_LENGTH / 2 + START_ROUTE - 4);
            printf("\033[%d;%dHPress R to restart, E to exit...", yM - 3, ROUTE_LENGTH / 2 + START_ROUTE - 15);
            if (*deadPnt != -1) printf("\033[%d;%dH$", yM + 1 - *deadPnt / 3, xM + *deadPnt % 3);   //keep printing bullet

        }
    }
}

MPOS drawStandingMan(INT xM, INT yM) {
    printf("\033[%d;%dH o ", yM - 1, xM);
    printf("\033[%d;%dH<O>", yM, xM);
    printf("\033[%d;%dH W ", yM + 1, xM);

    MPOS man = { xM, yM, STANDING, 0, 0, -1, 0, {0,1,0,1,1,1,0,1,0} };
    return man;
}

MPOS drawRunningMan(INT xM, INT yM) {
    printf("\033[%d;%dH p ", yM - 1, xM);
    printf("\033[%d;%dH D/", yM, xM);
    printf("\033[%d;%dH X ", yM + 1, xM);

    MPOS man = { xM, yM, RUNNING, 0, 0, -1, 0, {0,1,0,0,1,1,0,1,0} };
    return man;
}

void drawJumpingMan(INT xM, INT yM) {
    printf("\033[%d;%dH_p/", yM - 1, xM);
    printf("\033[%d;%dH D_", yM, xM);
    printf("\033[%d;%dH/  ", yM + 1, xM);
}

MPOS moveForward(MPOS* manPtr, unsigned short steps) {
    INT x = manPtr->xM, y = manPtr->yM;

    for (int i = 0; i < steps; i++) {
        printf("\033[%d;%dH  p ", y - 1, x + i);
        printf("\033[%d;%dH /D ", y, x + i);
        printf("\033[%d;%dH  I ", y + 1, x + i);
        Sleep(GAME_PACE * 3);

        printf("\033[%d;%dH  p ", y - 1, x + i);
        printf("\033[%d;%dH  D/", y, x + i);
        printf("\033[%d;%dH  X ", y + 1, x + i);
        Sleep(GAME_PACE * 3);
    }
    manPtr->xM += steps;
    return *manPtr;
}

void runEffect(MPOS* man) {
    INT xM = man->xM, yM = man->yM;
    MSTATE* manState = &(man->state);
    char* arr = man->bodyZone;
    int* isDead = &(man->isDead);
    int* deadPnt = &(man->deadPoint);

    while (*isDead == 0) {
        if (paused) continue;


        if (*manState != JUMPING) {
            arr[0] = arr[2] = arr[6] = arr[8] = 0;
            arr[1] = arr[3] = arr[4] = arr[5] = 1;

            // 1st animation
            if (*manState == RUNNING) {
                arr[7] = 1;
                printf("\033[%d;%dH p ", yM - 1, xM);
                printf("\033[%d;%dH/D ", yM, xM);
            }
            else {  // if (*manState==STOOPING)
                printf("\033[%d;%dH-Do", yM, xM);
            }
            printf("\033[%d;%dH H ", yM + 1, xM);
            if (*deadPnt != -1) printf("\033[%d;%dH$", yM + 1 - *deadPnt / 3, xM + *deadPnt % 3);   //keep printing bullet

            if (*isDead == 1) return;
            if (*manState == JUMPING) {
                printf("\033[%d;%dH   ", yM + 1, xM);
                continue;
            }

            Sleep(GAME_PACE);
            if (paused) continue;

            if (*isDead == 1) return;
            if (*manState == JUMPING) {
                printf("\033[%d;%dH   ", yM + 1, xM);
                continue;
            }
            // 2nd animation
            if (*manState == RUNNING) {
                arr[7] = 1;
                printf("\033[%d;%dH p ", yM - 1, xM);
                printf("\033[%d;%dH 0 ", yM, xM);
            }
            else {// if (*manState==STOOPING)
                printf("\033[%d;%dH 0o", yM, xM);
            }
            printf("\033[%d;%dH I ", yM + 1, xM);
            if (*deadPnt != -1) printf("\033[%d;%dH$", yM + 1 - *deadPnt / 3, xM + *deadPnt % 3);   //keep printing bullet

            if (*isDead == 1) return;
            if (*manState == JUMPING) {
                printf("\033[%d;%dH   ", yM + 1, xM);
                continue;
            }

            Sleep(GAME_PACE);
            if (paused) continue;

            
            if (*isDead == 1) return;
            if (*manState == JUMPING) continue;

            // 3rd animation
            if (*manState == RUNNING) {
                arr[7] = 1;
                printf("\033[%d;%dH p ", yM - 1, xM);
                printf("\033[%d;%dH D/", yM, xM);
            }
            else {// if (*manState==STOOPING)
                printf("\033[%d;%dH=Do", yM, xM);
            }
            printf("\033[%d;%dH X ", yM + 1, xM);
            if (*deadPnt != -1) printf("\033[%d;%dH$", yM + 1 - *deadPnt / 3, xM + *deadPnt % 3);   //keep printing bullet

            if (*isDead == 1) return;
            if (*manState == JUMPING) continue;

            Sleep(GAME_PACE);
            if (paused) continue;

            
            if (*isDead == 1) return;
            if (*manState == JUMPING) continue;
        }
        else while (*manState == JUMPING && *isDead == 0);
    }
}

//function just for test
void testBody(MPOS* man) {
    int i;
    char* arr = man->bodyZone;
    int* isDead = &(man->isDead);

    while (*isDead == 0) {
        pauseGame();

        for (i = 0; i < 9; i++) {
            if (man->deadPoint == i) {
                printf("\033[%d;%dH.", man->yM + 6 - i / 3, man->xM + i % 3);
            }
            else printf("\033[%d;%dH%d", man->yM + 6 - i / 3, man->xM + i % 3, arr[i]);
        }
        Sleep(1);
    }
}

void jump(MPOS* man) {
    INT xM = man->xM, yM = man->yM;
    char i;
    char* arr = man->bodyZone;
    int* isDead = &(man->isDead);
    man->state = JUMPING;
    int* deadPnt = &(man->deadPoint);
    arr[0] = arr[4] = arr[5] = arr[6] = arr[7] = arr[8] = 1;
    arr[1] = arr[2] = arr[3] = 0;
    //15 16 17
    //12 13 14
    // 9 10 11
    // 6  7  8
    // 3  4  5
    // 0  1  2

    drawJumpingMan(xM, yM);
    if (*deadPnt != -1) printf("\033[%d;%dH$", yM + 1 - *deadPnt / 3, xM + *deadPnt % 3);   //keep printing bullet

    if (*isDead == 1) return;
    Sleep(GAME_PACE / 2);

    for (i = 0; i < 3; i++) {
        if (*isDead == 1) return;
        arr[3 * i] = arr[3 * i + 1] = arr[3 * i + 2] = arr[3 * i + 4] = arr[3 * i + 5] = 0;
        arr[3 * i + 3] = arr[3 * i + 6] = arr[3 * i + 7] = arr[3 * i + 8] = 1;
        //arr[3*i+9] = arr[3*i+10] = arr[3*i+11] = 1; //=> 

        printf("\033[%d;%dH   ", yM - i + 1, xM);
        drawJumpingMan(xM, yM - i - 1);
        if (*deadPnt != -1) printf("\033[%d;%dH$", yM + 1 - *deadPnt / 3, xM + *deadPnt % 3);   //keep printing bullet

        if (*isDead == 1) return;
        Sleep(GAME_PACE);
    }
    if (*isDead == 1) return;
    Sleep(GAME_PACE);

    for (i = 0; i < 3; i++) {
        if (*isDead == 1) return;
        Sleep(GAME_PACE);
        if (*isDead == 1) return;
        //arr[17-3*i] = arr[16-3*i] = arr[15-3*i] = 0; 
        arr[7 - 3 * i] = arr[8 - 3 * i] = 0;
        arr[12 - 3 * i] = arr[13 - 3 * i] = arr[14 - 3 * i] = 1;
        arr[10 - 3 * i] = arr[11 - 3 * i] = arr[6 - 3 * i] = arr[9 - 3 * i] = 1;

        printf("\033[%d;%dH   ", yM + i - 4, xM);
        drawJumpingMan(xM, yM + i - 2);
        if (*deadPnt != -1) printf("\033[%d;%dH$", yM + 1 - *deadPnt / 3, xM + *deadPnt % 3);   //keep printing bullet
    }
    if (*isDead == 1) return;
    Sleep(GAME_PACE / 2);

    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    man->state = RUNNING;
}

void stoop(MPOS* man, int keyPress) {
    INT xM = man->xM, yM = man->yM;

    man->state = STOOPING;
    char* arr = man->bodyZone;
    int* deadPnt = &(man->deadPoint);

    arr[1] = arr[3] = arr[4] = arr[5] = 1;
    arr[0] = arr[2] = arr[6] = arr[7] = arr[8] = 0;

    printf("\033[%d;%dH   ", yM - 1, xM);
    printf("\033[%d;%dH=0o", yM, xM);
    printf("\033[%d;%dH X ", yM + 1, xM);

    if (*deadPnt != -1) printf("\033[%d;%dH$", yM + 1 - *deadPnt / 3, xM + *deadPnt % 3);   //keep printing bullet
    
    while ((GetKeyState(keyPress) & 0x8000) && man->isDead == 0);
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    man->state = RUNNING;
}


void shootBullets(MPOS* man) {
    const int END_ROUTE = START_ROUTE + ROUTE_LENGTH - 1;
    const INT X_MAN = man->xM,
        Y_MAN = man->yM;
    BLIST bullets = { NULL, NULL };
    int* isDead = &(man->isDead);

    srand((unsigned)time(NULL));
    while (*isDead == 0) {
        pauseGame();

        BPOS* pTailBullet = (BPOS*)malloc(sizeof(BPOS));

        if (pTailBullet == NULL) continue;

        pTailBullet->xB = END_ROUTE;
        pTailBullet->yB = Y_MAN - rand() % 3;
        pTailBullet->nextBulletPtr = NULL;

        insertTail(&bullets, pTailBullet);
        INT nextBulletAfter = rand() % (MAX_DIST_OF_BULLETS - MIN_DIST_OF_BULLETS + 1) + MIN_DIST_OF_BULLETS;

        for (int j = 0; j < nextBulletAfter; j++) {
            while (paused) {}
            BPOS* tmp = bullets.pHead;

            while (tmp != NULL && *isDead == 0) {
                while (paused) {}

                INT* X_BULLET = &(tmp->xB);
                const INT Y_BULLET = tmp->yB;

                if (*X_BULLET > START_ROUTE - 1) {
                    if (*X_BULLET < X_MAN + 2) {
                        if (*X_BULLET > X_MAN - 2) {
                            man->fatalBullet.xB = *X_BULLET;
                            man->fatalBullet.yB = Y_BULLET;

                            //compute the index of array of body zone that the bullet is occupying
                            man->deadPoint = *X_BULLET + 1 - X_MAN + (Y_MAN - Y_BULLET) * 3;
                        }
                        else if (*X_BULLET == X_MAN - 2) {
                            man->numBulletsPassed++;
                            man->deadPoint = -1;        //the bullet was out of body zone
                        }
                    }
                    printf("\033[%d;%dH$ ", 1 + Y_BULLET, 1 + (*X_BULLET)--);
                    tmp = tmp->nextBulletPtr;

                }
                else {
                    printf("\033[%d;%dH ", 1 + Y_BULLET, START_ROUTE + 1);
                    //BPOS *tmp_1 = tmp->nextBulletPtr;
                    removeHead(&bullets);

                    free((void*)tmp);
                    tmp = bullets.pHead;
                }
            }

            if (*isDead == 1) break;
            man->distancePassed++;
            printScore(*man);
            Sleep(GAME_PACE);
            if (*isDead == 1) break;
        }
    }

    BPOS* tmp = bullets.pHead;
    //delete heap pointers when game over
    while (bullets.pHead != NULL) {
        tmp = bullets.pHead;
        bullets.pHead = bullets.pHead->nextBulletPtr;
        free((void*)tmp);
    }
}

void insertTail(BLIST* pList, BPOS* pBullet) {
    if (pList->pTail == NULL) {
        pList->pHead = pBullet;
        pList->pTail = pBullet;
    }
    else {
        pList->pTail->nextBulletPtr = pBullet;
        pList->pTail = pBullet;
    }
}

void removeHead(BLIST* pList) {
    if (pList->pHead != NULL) {
        pList->pHead = (pList->pHead->nextBulletPtr);
        if (pList->pHead == NULL) pList->pTail = NULL;
    }
}
