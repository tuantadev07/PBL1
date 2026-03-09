#include "vector.h"

#include <iostream>

using namespace std;


int main () {
    vector_int v;
    init(v, 2, 3); 

    set(v, 0, 5);

    for (int i=0; i<v.size; ++i) {
        cout << get(v, i) <<' ';
    }

    free(v);
    system("pause");
    return 0;
}
