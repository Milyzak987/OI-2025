#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1005;
char grid[MAXN][MAXN];
int dist[MAXN][MAXN];
int cnt[MAXN * MAXN];
int suf[MAXN * MAXN];

struct SegTree {
    struct Node {
        int sum;
        int best;
        int bestZero;
    };
    static constexpr int NEG = -1000000000;
    int n = 0;
    vector<Node> tree;

    Node merge(const Node &L, const Node &R) const {
        Node res;
        res.sum = L.sum + R.sum;
        int candidateRight = R.best;
        int candidateLeft = (L.best != NEG ? L.best + R.sum : NEG);
        int candidateLeftZero = (R.sum > 0 && L.bestZero != NEG) ? L.bestZero + R.sum : NEG;
        res.best = max(candidateRight, max(candidateLeft, candidateLeftZero));
        int candidateZero = (R.sum == 0 ? L.bestZero : NEG);
        res.bestZero = max(R.bestZero, candidateZero);
        return res;
    }

    void build(int idx, int l, int r, const vector<int> &cnt) {
        if (l == r) {
            int c = cnt[l];
            tree[idx].sum = c;
            tree[idx].best = (c > 0 ? l + c - 1 : NEG);
            tree[idx].bestZero = (c == 0 ? l - 1 : NEG);
            return;
        }
        int mid = (l + r) / 2;
        build(idx * 2, l, mid, cnt);
        build(idx * 2 + 1, mid + 1, r, cnt);
        tree[idx] = merge(tree[idx * 2], tree[idx * 2 + 1]);
    }

    void init(const vector<int> &cnt) {
        n = cnt.size();
        if (n == 0) {
            tree.clear();
            return;
        }
        tree.assign(4 * n, Node{0, NEG, NEG});
        build(1, 0, n - 1, cnt);
    }

    void update(int idx, int l, int r, int pos, int delta) {
        if (l == r) {
            tree[idx].sum += delta;
            int c = tree[idx].sum;
            tree[idx].best = (c > 0 ? l + c - 1 : NEG);
            tree[idx].bestZero = (c == 0 ? l - 1 : NEG);
            return;
        }
        int mid = (l + r) / 2;
        if (pos <= mid) update(idx * 2, l, mid, pos, delta);
        else update(idx * 2 + 1, mid + 1, r, pos, delta);
        tree[idx] = merge(tree[idx * 2], tree[idx * 2 + 1]);
    }

    void update(int pos, int delta) {
        if (n == 0) return;
        update(1, 0, n - 1, pos, delta);
    }

    int getAnswer() const {
        if (n == 0 || tree.empty()) return 0;
        const Node &res = tree[1];
        return res.sum > 0 ? res.best : 0;
    }
};

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

    vector<int> cntDist(maxDist + 1, 0);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (grid[i][j] == 'F' && dist[i][j] != -1)
                cntDist[dist[i][j]]++;

    SegTree seg;
    seg.init(cntDist);

    cout << seg.getAnswer() << "\n";

    for (int i = 0; i < q; i++) {
        int x, y;
        cin >> x >> y;
        x--;
        y--;
        int d = dist[x][y];
        if (grid[x][y] == 'F') {
            if (d != -1) seg.update(d, -1);
            grid[x][y] = '.';
        } else {
            if (d != -1) seg.update(d, 1);
            grid[x][y] = 'F';
        }
        cout << seg.getAnswer() << "\n";
    }
}