#include<bits/stdc++.h>

using namespace std;


const int MAX_N = 10; // số trạng thái tối đa
int dp[MAX_N];


bool isInP(int u) {
    if (dp[u] != -1) // nếu u đã được tính trước đó thì
        return dp[u];

    if (u == 0) { // u = 0 là trạng thái kết thúc nên thuộc P
        dp[u] = 1;
        return 1;
    }

    // Từ u chỉ có thể đi tới các v hợp lệ
    for (int v = u - 1; v >= max(u - 3, 0); v--)
        if (isInP(v)) {
            dp[u] = 0;
            return false;
        }

    // u không đi được trạng thái nào thuộc P
    dp[u] = 1;
    return true;
}

int main() {
    int n;

    cin >> n;

    fill(dp, dp + n + 1, -1);
    if (!isInP(n)) cout << "A win";
    else cout << "B win";

    return 0;
}