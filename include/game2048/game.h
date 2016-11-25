#ifndef GAME2048_FRAMEWORK_GAME_H_
#define GAME2048_FRAMEWORK_GAME_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "linked-list.h"

#define CHECKERBOARD_LENGTH 4

class Game {
public:
    Game();
    ~Game();

    static const int kMoveUp = 0;
    static const int kMoveDown = 1;
    static const int kMoveLeft = 2;
    static const int kMoveRight = 3;

    void start();

    void move(int direction);

    void printCheckerboard();

    int** getCopyCheckerboard();

    void randomGenerate(int generate_amount);

    bool isGameOver();

    long get_score();

    static long moveUp(int **checkerboard);

    static long moveDown(int **checkerboard);

    static long moveLeft(int **checkerboard);

    static long moveRight(int **checkerboard);

private:
    int** _checkerboard;
    long _sorce;
    LinkedList _empty_list;

    void initCheckerBoard();

    void setupEmptyList();

    static void mergeGrid(int **checkerboard, CheckerboardIndex target, CheckerboardIndex operate, long &sorce, CheckerboardIndex &stop_index);

};

#endif