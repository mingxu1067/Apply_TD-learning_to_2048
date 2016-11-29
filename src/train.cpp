#include "td-model.h"

int main(int argc, char const *argv[]) {

    float l_rate = 0.0025;
    TDModel td_model_2048 = TDModel(l_rate);

    // td_model_2048.inputModel("./td_model_100");
    for (int i = 0; i < 100; i++) {
        int train_times = 100;
        printf("Train with {learning rate: %f} in { %d-th round}\n", l_rate, i+1);
        td_model_2048.train(train_times);

        td_model_2048.storeModel("./td_model_" + std::to_string((i+1) * 100));
    }

    // td_model_2048.inputModel("./td_model");
    // int test_times = 1000;
    // printf("Test in {times:%d}\n", test_times);
    // td_model_2048.test(test_times);

    return 0;
}