#include "string"
#include "map"
#include "list"

#include "game2048/game.h"

using namespace std;

typedef struct {
    int** board;
    int update_value;
}record;

class TDModel {
public:
    TDModel(float learning_rate);
    ~TDModel();

    void train(int times);
    int test(int times);

private:
    const static int kTileType14 = 0;
    const static int kTileType23 = 1;

    map<string, int> _value_table_14;
    map<string, int> _value_table_23;

    list<record> record_list;

    typedef map<string, int>::iterator mapIter;

    float _learning_rate;

    int valueOfState(const int** board);

    int evaluteTile(int tile_type, const int* tile);

    string changeTileToKey(const int *tile);

    int getTileType(int index);

    int getValueByMap(map<string, int> map, string key);
};