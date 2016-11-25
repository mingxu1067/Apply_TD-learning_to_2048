#include "td-model.h"
#include "stdio.h"

/*
*   Constructor
*/
TDModel::TDModel(float learning_rate) {
    _learning_rate = learning_rate;
}

TDModel::~TDModel() {}

/*
*   Public finction
*/
void TDModel::train(int times) {
    srand(time(NULL));

    for (int train_round = 0; train_round < times; train_round++) {
        Game game = Game();
        while (!game.isGameOver()) {
            int **game_board = game.getCopyCheckerboard();

            int update_value = 0;
            int direction = pickMoveDirection(game, update_value);

            printf("Pick direction \"D%d\" with evaluation \"%d\"\n", direction, update_value);

            game.move(direction);
            game.randomGenerate((rand() % 2) + 1);
            game.printCheckerboard();
            printf("==================================\n");

            Record record = {game_board, update_value};
            record_list.push_front(record);
        }
    }
}

int TDModel::test(int times) {
    return 0;
}

/*
*   Private finction
*/
int TDModel::valueOfState(const int **board) {
    int value = 0;
    for (int row = 0; row < CHECKERBOARD_LENGTH; row++) {
        value += evaluteTile(getTileType(row), *(board+row));
    }

    return value;
}

int TDModel::evaluteTile(int tile_type, const int* tile) {
    string key = changeTileToKey(tile);
    
    if (tile_type == kTileType14) {
        return getValueByMap(_value_table_14, key);
    } else if (tile_type == kTileType23) {
        return getValueByMap(_value_table_23, key);
    }

    return 0;
}

string TDModel::changeTileToKey(const int *tile) {
    string key = string("");
    for (int i = 0; i < CHECKERBOARD_LENGTH; i++) {
        key += tile[i];
    }

    return key;
}

int TDModel::getTileType(int index) {
    index = index + 1;

    switch(index) {
        case 1:
        case 4:
            return kTileType14;
        case 2:
        case 3:
            return kTileType23;
    }

    return kTileType14;
}

int TDModel::getValueByMap(map<string, int> map, string key) {
    mapIter iter = map.find(key);

    if (iter != map.end()) {
        return iter->second;
    }

    return 0;
}

int TDModel::pickMoveDirection(Game game, int& score) {
    typedef int (*Move)(int **);

    int direction[4] = {Game::kMoveUp, Game::kMoveDown, Game::kMoveLeft, Game::kMoveRight};
    Move move[4] = {&Game::moveUp, &Game::moveDown, &Game::moveLeft, &Game::moveRight};

    int resultDirection = direction[0];
    for (int i=0; i<4; i++) {
        int **board = game.getCopyCheckerboard();
        int reward = move[i](board);
        reward += valueOfState((const int**)board);

        if (reward >= score) {
            score = reward;
            resultDirection = direction[i];
        }
    }

    return resultDirection;
}



