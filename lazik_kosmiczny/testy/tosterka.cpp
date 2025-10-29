#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    string s;
    int l;
    cin >> l >> s;

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
    cout << (ok ? "C" : "I");
}