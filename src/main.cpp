#include <iostream>
#include <random>
using namespace std;

int main () {

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> rng(1, 10);

    int n; cin >> n;
    int a[n];
    
    for (int i=0; i<n; ++i) {
        a[i] = rng(gen);
    }

    int sum = 0;
    for (int i=0; i<n; ++i) {
        sum += a[i];
    }

    string s[2] = {
        "You",
        "AI"
    };

    int luot = 0;
    while (sum) {
        luot = 1 - luot;
        
        for (int i=0; i<n; ++i) {
            cout << "Dong " << i+1 <<": " <<a[i] << '\n';
        }

        cout << '\n';

        cout << "Den luoc " << s[luot] << '\n';
        if (!luot) {
            int p;
            cout << "chon dong: ";
            cin >> p;

            int cnt;
            cout << "cho so luong: ";
            cin >> cnt;

            a[p-1] -= cnt;
            sum -= cnt;
        }else {
            int p;
            uniform_int_distribution<int> rng1(1, n);

            do {
                p = rng1(gen);
            } while (!a[p-1]);

            uniform_int_distribution<int> rng2(1, a[p-1]);
            int cnt = rng2(gen);

            a[p-1] -= cnt;
            sum -= cnt;

            cout << "AI chon dong " << p << '\n';
            cout << "So luong: " << cnt << '\n';   
        }
        cout << '\n';
    }
    cout << s[luot] << " boc cuoi cung\n";
    cout << s[1-luot] << " Win\n"; 

    system("pause");
    cin >> n;

    return 0;
}

