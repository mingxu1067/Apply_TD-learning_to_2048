#include "td-model.h"

int main(int argc, char const *argv[]) {

    TDModel td_model_2048 = TDModel(0.01);
    td_model_2048.train(100);
    return 0;
}