#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<int, int> pii;

const int MAXT = 1000005; // Max value T
const int MAXN = 300005;  // Max N and Q
const int R = 1 << 19;    // Power of 2 > 300,000

// Tree 1: Stores {Max, Min}
// first = Maximum value in range
// second = Minimum value in range
pii treeMinMax[2 * R];

// Tree 2: Stores Maximum "Previous Occurrence" Index
int treePrev[2 * R];

// Sets to track positions of each value 1..T
set<int> positions[MAXT];
// Array to keep track of current values for updates
int S[MAXN];

// --- Segment Tree 1 (Min/Max) Operations ---

void updateMinMax(int i, int x) {
    i += R;
    treeMinMax[i].first = x;   // Max
    treeMinMax[i].second = x;  // Min
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
        if (a % 2 == 0) {
            resMax = max(resMax, treeMinMax[a + 1].first);
            resMin = min(resMin, treeMinMax[a + 1].second);
        }
        if (b % 2 == 1) {
            resMax = max(resMax, treeMinMax[b - 1].first);
            resMin = min(resMin, treeMinMax[b - 1].second);
        }
        a /= 2;
        b /= 2;
    }
    return {resMax, resMin};
}

// --- Segment Tree 2 (Previous Index) Operations ---

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

// --- Main ---

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int n, t, q;
    if (!(cin >> n >> t >> q)) return 0;

    // Initialize MinMax Tree with identity values
    // Important: Leaves must be (0, INT_MAX) initially so min() works correctly
    for (int i = 0; i < 2 * R; i++) {
        treeMinMax[i] = {0, INT_MAX};
        treePrev[i] = 0;
    }

    // Read Input and Build Initial State
    for (int i = 1; i <= n; i++) {
        cin >> S[i];
        updateMinMax(i, S[i]);
        positions[S[i]].insert(i);
    }

    // Build Prev Tree based on initial positions
    for (int val = 1; val <= t; val++) {
        int last_pos = 0;
        for (int pos : positions[val]) {
            updatePrev(pos, last_pos);
            last_pos = pos;
        }
    }

    // Process Queries
    while (q--) {
        int type;
        cin >> type;
        
        if (type == 1) {
            // Check Query: 1 l r
            int l, r;
            cin >> l >> r;

            // Condition 1: Unique Elements
            // If the max previous index in range [l, r] is >= l, it means 
            // a duplicate exists within the range [l, r].
            int max_prev = queryPrev(l, r);
            if (max_prev >= l) {
                cout << "NIE\n";
                continue;
            }

            // Condition 2: Contiguous Range
            // Max - Min must equal Length - 1
            pii mm = queryMinMax(l, r);
            // mm.first is MAX, mm.second is MIN
            if ((mm.first - mm.second) == (r - l)) {
                cout << "TAK\n";
            } else {
                cout << "NIE\n";
            }

        } else {
            // Update Query: 2 p v
            int p, v;
            cin >> p >> v;
            int old_v = S[p];

            if (old_v == v) continue;

            // 1. Remove p from old_v set
            {
                auto it = positions[old_v].find(p);
                int prev_idx = 0; 
                if (it != positions[old_v].begin()) prev_idx = *prev(it);
                
                auto next_it = next(it);
                if (next_it != positions[old_v].end()) {
                    // The element after p now points to p's previous
                    updatePrev(*next_it, prev_idx);
                }
                
                positions[old_v].erase(it);
                updatePrev(p, 0); // Clear p's prev pointer (temp)
            }

            // 2. Add p to v set
            {
                positions[v].insert(p);
                auto it = positions[v].find(p);
                
                int prev_idx = 0;
                if (it != positions[v].begin()) prev_idx = *prev(it);
                
                // Update p to point to its new previous occurrence
                updatePrev(p, prev_idx);
                
                auto next_it = next(it);
                if (next_it != positions[v].end()) {
                    // The element after p now points to p
                    updatePrev(*next_it, p);
                }
            }

            // 3. Update value array and MinMax Tree
            S[p] = v;
            updateMinMax(p, v);
        }
    }

    return 0;
}
