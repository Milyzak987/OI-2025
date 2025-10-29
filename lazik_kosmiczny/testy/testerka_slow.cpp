#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    string s;
    // cin >> s;

    int g = gcd(n, m);

    if (n > m) {
        s = string(max(g - 1, 1), 'P');
        s += 'G';
    } else {
        s = string(max(g - 1, 1), 'G');
        s += 'P';
    }
    // cout << s << "\n";

    vector<vector<char>> in_cycle(n, vector<char>(m, 0));
    vector<vector<char>> comp(n, vector<char>(m, 1));

    int x = 0, y = 0, cnt = 0;
    in_cycle[x][y] = 1;
    while (in_cycle != comp && cnt <= 1000000) {
        for (char c : s) {
            if (c == 'P')
                y = (y + 1) % m;
            else if (c == 'L')
                y = (y - 1 + m) % m;
            else if (c == 'G')
                x = (x - 1 + n) % n;
            else if (c == 'D')
                x = (x + 1) % n;
            in_cycle[x][y] = 1;
        }
        cnt++;
    }

    cout << (in_cycle == comp ? "TAK\n" : "NIE\n");
}