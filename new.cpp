#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
typedef pair<int, int> pii;

const int MAXT = 1e6 + 7;
const int R = 1 << 19;
pii treeMinMax[2 * R];
int treePrev[2 * R];
set<int> positions[MAXT];

void updateMinMax(int i, int x) {
    i += R;
    treeMinMax[i].first = x;
    treeMinMax[i].second = x;
    i /= 2;
    while (i > 0) {
        treeMinMax[i].first = max(treeMinMax[i * 2].first, treeMinMax[i * 2 + 1].first);
        treeMinMax[i].second = min(treeMinMax[i * 2].second, treeMinMax[i * 2 + 1].second);
        i /= 2;
    }
}

pii queryMinMax(int a, int b) {
    int resMax = 0;
    int resMin = INT_MAX;
    a += R - 1;
    b += R + 1;
    while (a / 2 != b / 2) {
        if (a % 2 == 0) resMax = max(resMax, treeMinMax[a + 1].first);
        if (b % 2 == 1) resMax = max(resMax, treeMinMax[b - 1].first);
        if (a % 2 == 0) resMin = min(resMin, treeMinMax[a + 1].second);
        if (b % 2 == 1) resMin = min(resMin, treeMinMax[b - 1].second);
        a /= 2;
        b /= 2;
    }
    return {resMax, resMin};
}

void updatePrev(int i, int x) {
    i += R;
    treePrev[i] = x;
    i /= 2;
    while (i > 0) {
        treePrev[i] = max(treePrev[i * 2], treePrev[i * 2 + 1]);
        i /= 2;
    }
}

int queryPrev(int a, int b) {
    int res = 0;
    a += R - 1;
    b += R + 1;
    while (a / 2 != b / 2) {
        if (a % 2 == 0) res = max(res, treePrev[a + 1]);
        if (b % 2 == 1) res = max(res, treePrev[b - 1]);
        a /= 2;
        b /= 2;
    }
    return res;
}



int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int n, t, q;
    cin >> n >> t >> q;

    for(int i = 1; i <= n; i++) {
        int x;
        cin >> x;
        updateMinMax(i, x);
        positions[x].insert(i);
    }

    for (int val = 1; val <= q; val++) {
        int last_pos = 0;
        for (int pos : positions[val]) {
            updatePrev(pos, last_pos);
            last_pos = pos;
        }
    }

    while (t--) {
        int a, b, c;
        cin >> a >> b >> c;
        if (a == 1) {
            cout << queryMinMax(b, c).first << " " << queryMinMax(b, c).second << "\n";
        } else {
            updateMinMax(b, c);
        }
    }

    return 0;
}
