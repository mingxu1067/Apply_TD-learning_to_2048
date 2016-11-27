#include "string"
#include "map"
#include "list"

#include "game2048/game.h"

using namespace std;

typedef struct {
    int** board;
    double update_value;
}Record;

class TDModel {
public:
    TDModel(float learning_rate);
    ~TDModel();

    void train(int times);
    int test(int times);

    void storeModel(string path);
    void inputModel(string path);

    void set_learning_rate(float rate);

private:
    const static int kTileType14 = 0;
    const static int kTileType23 = 1;
    const static string kModelName;

    map<string, double> _value_table_14;
    map<string, double> _value_table_23;

    list<Record> _record_list;

    typedef map<string, double>::iterator MapIter;

    float _learning_rate;

    double valueOfState(const int** board);

    double evaluteTile(int tile_type, const int* tile);

    string changeTileToKey(const int *tile);

    int getTileType(int index);

    double getValueByMap(map<string, double> map, string key);

    int pickMoveDirection(Game game, double& score);

    void updateValueMap();

    void replaceValueInMap(int tile_type, string key, double update_value);
};