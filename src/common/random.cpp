#include "random.h"


#include "common_utils.h"

#include <random>



int random_int (int left, int right) {

    // std::random_device seed;
    // std::mt19937 gen(seed());

    static std::mt19937 genrator(std::random_device{}());
    
    if (left > right) {
        swap_int(left, right);
    }
    
    std::uniform_int_distribution<int> uni(left, right);

    return uni(genrator);    
}

int gieo_xuc_xac () {
    return random_int(1, 6);
}