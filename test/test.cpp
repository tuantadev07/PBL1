#include <iostream>
#include <conio.h>

using namespace std;

int main () {
    cout << "an\n";

    int k = _getch();
    cout << k << "\n";
    if (k == 0 || k == 224) {
        cout << "hu\n";
        k = _getch();
    }
    cout << k << "\nok\n";
}