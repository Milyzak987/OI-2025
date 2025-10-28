#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1005;
int grid[MAXN][MAXN];
int dist[MAXN][MAXN];
int cnt[MAXN * MAXN];

struct cmp {
    bool operator()(const pair<int, int>& a, const pair<int, int>& b) const {
        if (dist[a.first][a.second] != dist[b.first][b.second])
            return dist[a.first][a.second] > dist[b.first][b.second];
        return a < b;
    }
};
set<pair<int, int>, cmp> forts;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, q;
    cin >> n >> q;
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;
        for (int j = 0; j < n; j++) {
            if (s[j] == 'F') {
                grid[i][j] = 2;
                dist[i][j] = -1;
            } else {
                grid[i][j] = (s[j] == '#' ? 1 : 0);
                dist[i][j] = -1;
            }
        }
    }

    int dx[4] = {1, -1, 0, 0};
    int dy[4] = {0, 0, 1, -1};
    queue<pair<int, int>> qu;
    qu.push({0, 0});
    dist[0][0] = 0;

    while (!qu.empty()) {
        auto [x, y] = qu.front();
        qu.pop();

        for (int d = 0; d < 4; d++) {
            int nx = x + dx[d];
            int ny = y + dy[d];
            if (nx < 0 || ny < 0 || nx >= n || ny >= n) continue;
            if (grid[nx][ny] == 1 || dist[nx][ny] != -1) continue;

            dist[nx][ny] = dist[x][y] + 1;
            qu.push({nx, ny});
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (grid[i][j] == 2) {
                cnt[dist[i][j]]++;
                forts.insert({i, j});
            }
        }
    }

    int furthest = dist[(*forts.begin()).first][(*forts.begin()).second];
    int added = 0, idx = furthest, b = 0;
    for(auto [x, y] : forts) {
        int d = dist[x][y];
        added += max(0, d - idx - b);
        b = max(b, d - idx);
        idx--;
    }
    cout << furthest + added << "\n";

    return 0;
}
