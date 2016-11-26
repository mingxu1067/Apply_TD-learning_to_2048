#include "td-model.h"

int main(int argc, char const *argv[]) {

    float l_rate = 0.1;
    int train_times = 2000;
    printf("Train with {learning rate: %f} in {times:%d}\n", l_rate, train_times);
    TDModel td_model_2048 = TDModel(l_rate);
    td_model_2048.train(train_times);

    td_model_2048.inputModel("./td_model");

    // int test_times = 1;
    // printf("Test in {times:%d}\n", test_times);
    // unsigned long average_scorce = td_model_2048.test(test_times);
    // printf("Test Result: %lu\n", average_scorce);

    return 0;
}