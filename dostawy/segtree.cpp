#include <bits/stdc++.h>
using namespace std;

const int N = 1005;
char g[N][N];
int d[N][N];

struct Seg {
    struct Node { int s, b, bz;};
    vector<Node> t;
    int n;
    static const int NEG = -1e9;

    Node merge(Node a, Node b) {
        return {a.s + b.s, max({b.b, a.b + b.s, (b.s > 0 && a.bz != NEG ? a.bz + b.s : NEG)}),
                max(b.bz, b.s == 0 ? a.bz : NEG)};
    }

    void build(int i, int l, int r, const vector<int>& c) {
        if (l == r) {
            int x = c[l];
            t[i] = {x, x > 0 ? l + x - 1 : NEG, x == 0 ? l - 1 : NEG};
            return;
        }
        int m = (l + r) / 2;
        build(i * 2, l, m, c);
        build(i * 2 + 1, m + 1, r, c);
        t[i] = merge(t[i * 2], t[i * 2 + 1]);
    }

    void init(const vector<int>& c) {
        n = c.size();
        if (!n) {
            t.clear();
            return;
        }
        t.assign(4 * n, {0, NEG, NEG});
        build(1, 0, n - 1, c);
    }

    void upd(int i, int l, int r, int p, int v) {
        if (l == r) {
            t[i].s += v;
            int x = t[i].s;
            t[i].b = x > 0 ? l + x - 1 : NEG;
            t[i].bz = x == 0 ? l - 1 : NEG;
            return;
        }
        int m = (l + r) / 2;
        if (p <= m)
            upd(i * 2, l, m, p, v);
        else
            upd(i * 2 + 1, m + 1, r, p, v);
        t[i] = merge(t[i * 2], t[i * 2 + 1]);
    }

    void upd(int p, int v) { if (n) upd(1, 0, n - 1, p, v); }

    int get() { return n && t[1].s > 0 ? t[1].b : 0; }
};

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n, q;
    cin >> n >> q;
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;
        for (int j = 0; j < n; j++) g[i][j] = s[j], d[i][j] = -1;
    }

    queue<pair<int, int>> qu;
    d[0][0] = 0;
    qu.push({0, 0});
    int dx[4] = {1, -1, 0, 0}, dy[4] = {0, 0, 1, -1};
    while (!qu.empty()) {
        auto [x, y] = qu.front();
        qu.pop();
        for (int k = 0; k < 4; k++) {
            int nx = x + dx[k], ny = y + dy[k];
            if (nx < 0 || ny < 0 || nx >= n || ny >= n || g[nx][ny] == '#' || d[nx][ny] != -1) continue;
            d[nx][ny] = d[x][y] + 1;
            qu.push({nx, ny});
        }
    }

    int md = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (g[i][j] != '#') md = max(md, d[i][j]);

    vector<int> cnt(md + 1, 0);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (g[i][j] == 'F' && d[i][j] != -1) cnt[d[i][j]]++;

    Seg seg;
    seg.init(cnt);
    cout << seg.get() << "\n";

    while (q--) {
        int x, y;
        cin >> x >> y;
        x--;
        y--;
        int dd = d[x][y];
        if (g[x][y] == 'F') {
            if (dd != -1) seg.upd(dd, -1);
            g[x][y] = '.';
        } else {
            if (dd != -1) seg.upd(dd, 1);
            g[x][y] = 'F';
        }
        cout << seg.get() << "\n";
    }
}
