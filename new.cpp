#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<int, int> pii;

const int MAXT = 1000005; // Max value T
const int MAXN = 300005;  // Max N and Q
const int R = 1 << 19;    // Power of 2 > 300,000

// Tree 1: {Max, Min}
pii treeMinMax[2 * R];

// Tree 2: Max Previous Index
int treePrev[2 * R];

// Sets for positions
set<int> positions[MAXT];
int S[MAXN]; // Store current values

// --- Helper Functions to avoid Iterator Spaghetti ---

// Find the index of 'val' that appears immediately BEFORE 'idx'
// Returns 0 if no such index exists.
int getPredecessor(int val, int idx) {
    // lower_bound returns iterator to the first element >= idx
    auto it = positions[val].lower_bound(idx);
    
    // If it's the beginning, there is no predecessor
    if (it == positions[val].begin()) return 0;
    
    // Otherwise, step back one position
    it--;
    return *it;
}

// Find the index of 'val' that appears immediately AFTER 'idx'
// Returns 0 if no such index exists.
int getSuccessor(int val, int idx) {
    // upper_bound returns iterator to the first element > idx
    auto it = positions[val].upper_bound(idx);
    
    // If it's the end, there is no successor
    if (it == positions[val].end()) return 0;
    
    return *it;
}

// --- Segment Tree Operations (Your Structure) ---

void updateMinMax(int i, int x) {
    i += R;
    treeMinMax[i].first = x;    // Max
    treeMinMax[i].second = x;   // Min
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

    // Initialization: Fill MinMax tree with Safe Defaults
    // (0, INT_MAX) ensures unused leaves don't break min/max queries
    for(int i = 0; i < 2 * R; i++) {
        treeMinMax[i] = {0, INT_MAX};
        treePrev[i] = 0;
    }

    // Build Initial State
    for(int i = 1; i <= n; i++) {
        cin >> S[i];
        updateMinMax(i, S[i]);
        positions[S[i]].insert(i);
    }

    // Initialize Prev Tree
    for (int val = 1; val <= t; val++) {
        int last_pos = 0;
        for (int pos : positions[val]) {
            updatePrev(pos, last_pos);
            last_pos = pos;
        }
    }

    while (q--) {
        int type;
        cin >> type;
        
        if (type == 1) {
            int l, r;
            cin >> l >> r;
            
            // 1. Uniqueness Check: Max(Prev) < l
            if (queryPrev(l, r) >= l) {
                cout << "NIE\n";
                continue;
            }

            // 2. Contiguity Check: Max - Min == Length - 1
            pii mm = queryMinMax(l, r);
            if ((mm.first - mm.second) == (r - l)) {
                cout << "TAK\n";
            } else {
                cout << "NIE\n";
            }
        } 
        else {
            int p, v;
            cin >> p >> v;
            int old_v = S[p];

            if (old_v == v) continue;

            // --- Step A: Remove p from old_v ---
            // Before removing p, we must connect its neighbors:
            // Predecessor(p) <--- connected to ---> Successor(p)
            int old_pre = getPredecessor(old_v, p);
            int old_suc = getSuccessor(old_v, p);

            if (old_suc != 0) {
                // The element after p now points to p's old predecessor
                updatePrev(old_suc, old_pre);
            }
            positions[old_v].erase(p);
            // Reset p's prev pointer to 0 temporarily
            updatePrev(p, 0); 

            // --- Step B: Add p to v ---
            positions[v].insert(p);
            
            // Now p is in the set, find its new neighbors
            int new_pre = getPredecessor(v, p);
            int new_suc = getSuccessor(v, p);

            // p now points to its new predecessor
            updatePrev(p, new_pre);

            if (new_suc != 0) {
                // The element after p now points to p
                updatePrev(new_suc, p);
            }

            // --- Step C: Update Value ---
            S[p] = v;
            updateMinMax(p, v);
        }
    }

    return 0;
}
