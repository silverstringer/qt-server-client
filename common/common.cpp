#include "common.h"
#include <random>


int Helper::generateRandomNumber(int start, int end) {
    std::random_device rd;
    std::mt19937 data(rd());
    std::uniform_int_distribution<> distr(start, end);
    return distr(data);
}

