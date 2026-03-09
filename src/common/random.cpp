#include "random.h"


#include "common_utils.h"

#include <random>



int random_int (int left, int right) {
    static std::mt19937 genrator(std::random_device{}());
    
    if (left > right) {
        swap_int(left, right);
    }
    
    std::uniform_int_distribution<int> uni(left, right);

    return uni(genrator);    
}