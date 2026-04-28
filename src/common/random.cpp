#include "common/random.h"

#include <random>

namespace {
    void _swap_int (int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}
} // namespace 


int random_int (int left, int right) {

    // std::random_device seed;
    // std::mt19937 gen(seed());

    static std::mt19937 genrator(std::random_device{}());
    
    if (left > right) {
        _swap_int(left, right);
    }
    
    std::uniform_int_distribution<int> uni(left, right);

    return uni(genrator);    
}

int gieo_xuc_xac () {
    return random_int(1, 6);
}