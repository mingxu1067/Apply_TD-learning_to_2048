#include "td-model.h"
#include <stdio.h>
#include <fstream> 
#include <math.h>
/*
*   Constant Variable Initialize
*/

const string TDModel::kModelName = ".tdm";

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
    for (int train_round = 0; train_round < times; train_round++) {
        printf("===========================Trainning Round:%d===========================\n", train_round+1);
        Game game = Game();
        int random_count = 2;

        while (!game.isGameOver()) {
            int **game_board = game.getCopyCheckerboard();
            game.randomGenerate(random_count);
            random_count = 1;

            double update_value = 0;
            int direction = pickMoveDirection(game, update_value);


            printf("Pick direction \"D%d\" with evaluation \"%f\"\n", direction, update_value);
            if (direction != -1)
                game.move(direction);
            // game.printCheckerboard();
            // printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

            Record record = {game_board, valueOfState((const int**)game_board), update_value};
            _record_list.push_front(record);
        }

        printf("\n");
        updateValueMap();
    }
}

void TDModel::test(int times) {
    unsigned long total_sorce = 0;
    unsigned long max_source = 0;
    int max_number = 0;
    for (int train_round = 0; train_round < times; train_round++) {
        Game game = Game();
        game.randomGenerate(2);

        unsigned long round_sorce = 0;
        while (!game.isGameOver()) {
            double update_value = 0;
            int direction = pickMoveDirection(game, update_value);
            game.move(direction);
            game.randomGenerate(1);
        }
        printf("Time:%d  sorce:%ld\n", train_round, game.get_score());
        max_source = game.get_score() >= max_source? game.get_score():max_source;
        total_sorce += game.get_score();

        int **board = game.getCopyCheckerboard();
        for(int row=0; row<CHECKERBOARD_LENGTH; row++) {
            for(int col=0; col<CHECKERBOARD_LENGTH; col++) {
                if (board[row][col] > max_number) {
                    max_number = board[row][col];
                }
            }
        }
        delete [] board;
    }
    
    printf("The maximum number %d\n", max_number);
    printf("The maximum score: %lu\n", max_source);
    printf("Test Average Result: %lu\n", total_sorce/times);
}

void TDModel::storeModel(string path) {
    path += kModelName;
    ofstream fout(path);
    if (!fout){
        printf("There are diretories which doesn't exist.\n");
    }

    printf("Store model to %s\n", path.c_str());
    fout << kTileType14 << "\t" << kTileType14 << endl;
    for(map<string, double>::iterator iter = _value_table_14.begin(); iter != _value_table_14.end(); iter++) {
        fout << iter->first << "\t" << iter->second << endl;
    }

    fout << kTileType23 << "\t" << kTileType23 << endl;
    for(map<string, double>::iterator iter = _value_table_23.begin(); iter != _value_table_23.end(); iter++) {
        fout << iter->first << "\t" << iter->second << endl;
    }

    fout.close();
}

void TDModel::inputModel(string path) {
    path += kModelName;
    ifstream fin(path); 

    if (!fin){
        printf("There are diretories which doesn't exist.\n");
    }

    printf("Input model to %s\n", path.c_str());
    string type;
    double val;
    fin >> type >> val;
    while(!fin.eof()) { 
        fin >> type >> val;
        if (type.length() < CHECKERBOARD_LENGTH) {
            break;
        }
        _value_table_14.insert(pair<string, double>(type, val));
    }

    while(!fin.eof()) { 
        fin >> type >> val;
        if (type.length() < CHECKERBOARD_LENGTH) {
            break;
        }
        _value_table_23.insert(pair<string, double>(type, val));
    }

}

void TDModel::set_learning_rate(float rate) {
    _learning_rate = rate;
}

/*
*   Private finction
*/
double TDModel::valueOfState(const int **board) {
    double value = 0.0;
    for (int row = 0; row < CHECKERBOARD_LENGTH; row++) {
        value += evaluteTile(getTileType(row), *(board+row));
    }

    for (int col = 0; col < CHECKERBOARD_LENGTH; col++) {
        int *tile = new int[CHECKERBOARD_LENGTH];
        for (int row = 0; row < CHECKERBOARD_LENGTH; row++) {
            *(tile+row) = board[row][col];
        }
        value += evaluteTile(getTileType(col), tile);
        delete [] tile;
    }
    return value;
}

double TDModel::evaluteTile(int tile_type, const int* tile) {
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
        if (tile[i] == 0) {
            key += "0";
            continue;
        }
        key += to_string((int)(log(tile[i]) / log(2)));
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

double TDModel::getValueByMap(map<string, double> map, string key) {
    MapIter iter = map.find(key);

    if (iter != map.end()) {
        return iter->second;
    }

    return 0;
}

int TDModel::pickMoveDirection(Game &game, double& score) {
    typedef long (*Move)(int **);

    int direction[4] = {Game::kMoveUp, Game::kMoveDown, Game::kMoveLeft, Game::kMoveRight};
    Move move[4] = {&Game::moveUp, &Game::moveDown, &Game::moveLeft, &Game::moveRight};

    int resultDirection = -1;
    for (int i=0; i<4; i++) {
        int **board = game.getCopyCheckerboard();
        double reward = move[i](board);
        // printf("%d get reward: %f v(s-next): %f\n",direction[i], reward, valueOfState((const int**)board));
        reward += valueOfState((const int**)board);

        if ((Game::isMoveWork())) {
            if (resultDirection == -1) {
                score = reward;
                resultDirection = direction[i];
            } else {
                if (reward >= score) {
                    score = reward;
                    resultDirection = direction[i];
                }
            }
        }

        delete [] board;
    }
    if (resultDirection == -1) {
        printf("Terminal state.\n");
    }
    return resultDirection;
}

void TDModel::updateValueMap() {
    while (!_record_list.empty()) {
        Record record = _record_list.front();
        int **board = record.board;
        // for (int r=0;r<4;r++) {
        //     for(int c=0;c<4;c++){
        //         printf("%4d", board[r][c]);
        //     }
        //     printf("\n");
        // }

        double update_value = _learning_rate * (record.update_value - record.origin_state_values);
        // printf("%f = %f*(%f - %f)\n", update_value, _learning_rate, record.update_value, record.origin_state_values);
        for (int row = 0; row < CHECKERBOARD_LENGTH; row++) {
            string key = changeTileToKey((const int*)(*(board+row)));
            replaceValueInMap(getTileType(row), key, update_value);
        }

        for (int col = 0; col < CHECKERBOARD_LENGTH; col++) {
            int *tile = new int[CHECKERBOARD_LENGTH];
            for (int row = 0; row < CHECKERBOARD_LENGTH; row++) {
                *(tile+row) = board[row][col];
            }
            string key = changeTileToKey((const int*)tile);
            replaceValueInMap(getTileType(col), key, update_value);
            delete [] tile;
        }
        delete [] board;
        _record_list.pop_front();
    }
}

void TDModel::replaceValueInMap(int tile_type, string key, double update_value) {
    // printf("Update %f to key:%s in {Map Type:%d} \n", update_value, key.c_str(), tile_type);
    if (tile_type == kTileType14) {
        std::map<string, double>::iterator it = _value_table_14.find(key); 
        if (it != _value_table_14.end()) {
            it->second = it->second + update_value;
        } else {
            _value_table_14.insert(pair<string, double>(key, update_value));
        }
    } else if (tile_type == kTileType23) {
        std::map<string, double>::iterator it = _value_table_23.find(key); 
        if (it != _value_table_23.end()) {
            it->second = it->second + update_value;
        } else {
            _value_table_23.insert(pair<string, double>(key, update_value));
        }
    }
}

