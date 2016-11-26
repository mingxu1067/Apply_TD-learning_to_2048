#include "td-model.h"
#include "stdio.h"
#include <fstream> 

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
    srand(time(NULL));

    for (int train_round = 0; train_round < times; train_round++) {
        printf("===========================Trainning Round:%d===========================\n", train_round);
        Game game = Game();
        while (!game.isGameOver()) {
            int **game_board = game.getCopyCheckerboard();

            double update_value = 0;
            int direction = pickMoveDirection(game, update_value);

            printf("Pick direction \"D%d\" with evaluation \"%f\"\n", direction, _learning_rate*update_value);

            game.move(direction);
            game.randomGenerate((rand() % 2) + 1);
            game.printCheckerboard();

            Record record = {game_board, update_value};
            _record_list.push_front(record);
        }

        updateValueMap();
    }
}

int TDModel::test(int times) {
    unsigned long total_sorce = 0;
    for (int train_round = 0; train_round < times; train_round++) {
        Game game = Game();

        unsigned long round_sorce = 0;
        while (!game.isGameOver()) {
            int **game_board = game.getCopyCheckerboard();

            double update_value = 0;
            int direction = pickMoveDirection(game, update_value);
            game.move(direction);
            game.randomGenerate((rand() % 2) + 1);
        }

        total_sorce += game.get_score();
    }
    return total_sorce/times;
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
        key += to_string(tile[i]);
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
    mapIter iter = map.find(key);

    if (iter != map.end()) {
        return iter->second;
    }

    return 0;
}

int TDModel::pickMoveDirection(Game game, double& score) {
    typedef long (*Move)(int **);

    int direction[4] = {Game::kMoveUp, Game::kMoveDown, Game::kMoveLeft, Game::kMoveRight};
    Move move[4] = {&Game::moveUp, &Game::moveDown, &Game::moveLeft, &Game::moveRight};

    int resultDirection = direction[0];
    for (int i=0; i<4; i++) {
        int **board = game.getCopyCheckerboard();
        double reward = move[i](board) + valueOfState((const int**)board);
        if ((Game::isMoveWork()) && (reward >= score)) {
            score = reward;
            resultDirection = direction[i];
        }
    }

    return resultDirection;
}

void TDModel::updateValueMap() {
    while (!_record_list.empty()) {
        Record record = _record_list.front();
        int **board = record.board;
        double update_value = _learning_rate * (record.update_value - valueOfState((const int**)board));

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

        _record_list.pop_front();
    }
}

void TDModel::replaceValueInMap(int tile_type, string key, double update_value) {
    printf("Update %f to key:%s in {Map Type:%d} \n", update_value, key.c_str(), tile_type);
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

