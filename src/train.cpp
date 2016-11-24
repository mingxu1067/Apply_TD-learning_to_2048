#include "td-model.h"

int main(int argc, char const *argv[]) {

    TDModel td_model_2048 = TDModel(0.3);
    td_model_2048.train(1);
    return 0;
}