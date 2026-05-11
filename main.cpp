#include <bits/stdc++.h>
using namespace std;

vector<int> cards(14, 32);

int main() {
    int c = 52 * 8, count = 0;
    double base = (double)12896 / (double)172640;
    while (true) {
        int total = (c) * (c - 1);
        int pairs = 0;
        for (int i = 1; i <= 13; i++) {
            cout << i << " -> " << cards[i] << "\n";
            pairs += (cards[i]) * (cards[i] - 1);
        }
        double ans = (double)pairs / (double)total;
        // cout << ans * 100 << " " << (ans - base) * 100 << "\n";
        cout << count << " " << (double)count * (double)52 / (double)c << "\n\n";
        int x;
        cin >> x;
        if(x == 0) break;
        cards[x]--;
        c--;
        if(x == 1 || x >= 10) count--;
        if(x >= 2 && x <= 6) count++;
    }
    return 0;
}