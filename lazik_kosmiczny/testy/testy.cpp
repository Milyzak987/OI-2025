#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    string s;

    if (n == 1 && m == 1) {
        s = "\n";
    }
    if (n == 1) {
        s = "P\n";
    }
    if (m == 1) {
        s = "D\n";
    }

    long long g = gcd(n, m);
    if (g == 1) {
        s = "DP";
    } else {
        s.append(g - 1, 'D');
        s.push_back('P');
        s.append(g - 2, 'G');
    }
    // cout << s << "\n";

    vector<vector<char>> in_cycle(n, vector<char>(m, 0));

    int x = 0, y = 0;
    in_cycle[x][y] = 1;
    for (char c : s) {
        if (c == 'P') y = (y + 1) % m;
        else if (c == 'L') y = (y - 1 + m) % m;
        else if (c == 'G') x = (x - 1 + n) % n;
        else if (c == 'D') x = (x + 1) % n;
        in_cycle[x][y] = 1;
    }

    int vx = x % n;
    int vy = y % m;

    vector<vector<int>> comp(n, vector<int>(m, -1));
    vector<char> comp_has;

    int comp_id = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (comp[i][j] != -1) continue;
            int cx = i, cy = j;
            bool first = true;
            bool has = false;
            while (first || cx != i || cy != j) {
                first = false;
                comp[cx][cy] = comp_id;
                if (in_cycle[cx][cy]) has = true;
                cx = (cx + vx) % n;
                cy = (cy + vy) % m;
            }
            comp_has.push_back(has);
            ++comp_id;
        }
    }

    bool ok = all_of(comp_has.begin(), comp_has.end(), [](char v) { return v; });
    cout << (ok ? "TAK\n" : "NIE\n");
}