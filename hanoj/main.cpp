#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1e6 + 7;

struct Tower {
    int top, bottom, size;
};

vector<Tower> towers(MAXN);
int arr[MAXN];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, m;
    cin >> n >> m;

    bool anySorted = false;
    int maxSorted = -1, maxSortedIdx;

    int idx = 0;
    for (int i = 0; i < m; i++) {
        int c;
        cin >> c;
        towers[i].size = c;

        bool tempSorted = true;
        for (int j = 0; j < c; j++) {
            int x;
            cin >> x;
            if (j == 0) towers[i].top = x;
            towers[i].bottom = x;
            arr[x] = i;

            if (j != x - 1) tempSorted = false;
        }
        if (tempSorted) {
            anySorted = true;
            if (c > maxSorted) {
                maxSorted = c;
                maxSortedIdx = i;
            }
        }
    }

    int minBottom = INT_MAX, minIdx = -1;
    for (int i = 0; i < m; i++) {
        if (towers[i].size != 0) {
            if (towers[i].bottom < minBottom) {
                minBottom = towers[i].bottom;
                minIdx = i;
            }
        } else {
            minBottom = 0;
            minIdx = i;
        }
    }

    int maxTop = -1, maxSize = INT_MAX, maxIdx;
    for (int i = 0; i < m; i++) {
        if (towers[i].size != 0) {
            if (towers[i].top > minBottom && towers[i].size < maxSize) {
                maxTop = towers[i].top;
                maxSize = towers[i].size;
                maxIdx = i;
            }
        }
    }

    if (maxTop == -1) {
        cout << -1 << "\n";
        return 0;
    }

    if (anySorted) {
        cout << n - maxSorted << "\n";
        for (int i = 1; i <= n; i++) {
            if (arr[i] != maxSortedIdx) {
                cout << arr[i] + 1 << " " << maxSortedIdx + 1 << "\n";
            }
        }
    } else {
        cout << maxSize + n << "\n";
        for (int i = 0; i < maxSize; i++) {
            cout << maxIdx + 1 << " " << minIdx + 1 << "\n";
        }
        for (int i = 1; i <= n; i++) {
            if (arr[i] != maxIdx) {
                cout << arr[i] + 1 << " " << maxIdx + 1 << "\n";
            } else {
                cout << minIdx + 1 << " " << maxIdx + 1 << "\n";
            }
        }
    }

    return 0;
}
