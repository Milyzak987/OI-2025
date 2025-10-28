#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1005;
char grid[MAXN][MAXN];
int dist[MAXN][MAXN];
int cnt[MAXN * MAXN];
int suf[MAXN * MAXN];

int solve(int maxDist) {
    int ans = 0;
    for (int d = 0; d <= maxDist; d++)
        if (suf[d]) ans = max(ans, d + suf[d] - 1);
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, q;
    cin >> n >> q;

    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;
        for (int j = 0; j < n; j++) {
            dist[i][j] = -1;
            grid[i][j] = s[j];
        }
    }

    int dx[4] = {1, -1, 0, 0}, dy[4] = {0, 0, 1, -1};
    queue<pair<int, int>> qu;
    dist[0][0] = 0;
    qu.push({0, 0});
    while (!qu.empty()) {
        auto [x, y] = qu.front();
        qu.pop();
        for (int d = 0; d < 4; d++) {
            int nx = x + dx[d], ny = y + dy[d];
            if (nx < 0 || ny < 0 || nx >= n || ny >= n) continue;
            if (grid[nx][ny] == '#' || dist[nx][ny] != -1) continue;
            dist[nx][ny] = dist[x][y] + 1;
            qu.push({nx, ny});
        }
    }

    int maxDist = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (grid[i][j] != '#' && dist[i][j] > maxDist) maxDist = dist[i][j];

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (grid[i][j] == 'F') cnt[dist[i][j]]++;

    for (int d = maxDist; d >= 0; d--) suf[d] = suf[d + 1] + cnt[d];

    cout << solve(maxDist) << "\n";

    for (int i = 0; i < q; i++) {
        int x, y;
        cin >> x >> y;
        x--;
        y--;
        int d = dist[x][y];
        if (grid[x][y] == 'F') {
            grid[x][y] = '.';
            for (int k = 0; k <= d; k++) suf[k]--;
        } else {
            grid[x][y] = 'F';
            for (int k = 0; k <= d; k++) suf[k]++;
        }
        cout << solve(maxDist) << "\n";
    }
}
