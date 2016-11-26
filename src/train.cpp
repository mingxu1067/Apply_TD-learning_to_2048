#include "td-model.h"

int main(int argc, char const *argv[]) {

    float l_rate = 0.09;
    TDModel td_model_2048 = TDModel(l_rate);

    // for (int i = 0; i < 20; i++) {
    //     td_model_2048.inputModel("./td_model");
    //     int train_times = 100;
    //     printf("Train with {learning rate: %f} in {times:%d}\n", l_rate, train_times);
    //     td_model_2048.train(train_times);

    //     td_model_2048.storeModel("./td_model");
    // }

    td_model_2048.inputModel("./td_model");
    int test_times = 30;
    printf("Test in {times:%d}\n", test_times);
    unsigned long average_scorce = td_model_2048.test(test_times);
    printf("Test Average Result: %lu\n", average_scorce);

    return 0;
}