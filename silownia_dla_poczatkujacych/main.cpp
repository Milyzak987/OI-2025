#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n;
    cin >> n;
    bool ok = false;

    for (int x = 0; x <= min(2, n/8); x++) {
        if ((n - 8*x) % 3 == 0) {
            ok = true;
            break;
        }
    }

    cout << (ok ? "TAK\n" : "NIE\n");
    return 0;
}
