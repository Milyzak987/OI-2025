/**
 * Task: Oddział Kociaków (Kittens Squad)
 * Solution: Segment Tree for Range Min/Max + Segment Tree for Distinctness (Predecessor check)
 * Complexity: O(Q * log N)
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

const int INF = 1e9;
const int MAXN = 300005;
const int MAXT = 1000005;

// Global Inputs
int n, t_max, q;
int S[MAXN];

// --- Segment Tree 1: Range Minimum and Maximum Query ---
struct NodeMinMax {
    int min_val;
    int max_val;
};

NodeMinMax treeMinMax[4 * MAXN];

void buildMinMax(int node, int start, int end) {
    if (start == end) {
        treeMinMax[node] = {S[start], S[start]};
    } else {
        int mid = (start + end) / 2;
        buildMinMax(2 * node, start, mid);
        buildMinMax(2 * node + 1, mid + 1, end);
        treeMinMax[node].min_val = min(treeMinMax[2 * node].min_val, treeMinMax[2 * node + 1].min_val);
        treeMinMax[node].max_val = max(treeMinMax[2 * node].max_val, treeMinMax[2 * node + 1].max_val);
    }
}

void updateMinMax(int node, int start, int end, int idx, int val) {
    if (start == end) {
        treeMinMax[node] = {val, val};
    } else {
        int mid = (start + end) / 2;
        if (start <= idx && idx <= mid) {
            updateMinMax(2 * node, start, mid, idx, val);
        } else {
            updateMinMax(2 * node + 1, mid + 1, end, idx, val);
        }
        treeMinMax[node].min_val = min(treeMinMax[2 * node].min_val, treeMinMax[2 * node + 1].min_val);
        treeMinMax[node].max_val = max(treeMinMax[2 * node].max_val, treeMinMax[2 * node + 1].max_val);
    }
}

NodeMinMax queryMinMax(int node, int start, int end, int l, int r) {
    if (r < start || end < l) {
        return {INF, -INF};
    }
    if (l <= start && end <= r) {
        return treeMinMax[node];
    }
    int mid = (start + end) / 2;
    NodeMinMax p1 = queryMinMax(2 * node, start, mid, l, r);
    NodeMinMax p2 = queryMinMax(2 * node + 1, mid + 1, end, l, r);
    return {min(p1.min_val, p2.min_val), max(p1.max_val, p2.max_val)};
}

// --- Segment Tree 2: Range Maximum Previous Occurrence ---
// Stores the index of the previous occurrence of the element at position i.
// If max(prev) in range [L, R] < L, then all elements are distinct.
int treePrev[4 * MAXN];
int prev_arr[MAXN]; // Local array to hold current prev values for easier debugging/init

void updatePrevTree(int node, int start, int end, int idx, int val) {
    if (start == end) {
        treePrev[node] = val;
    } else {
        int mid = (start + end) / 2;
        if (start <= idx && idx <= mid) {
            updatePrevTree(2 * node, start, mid, idx, val);
        } else {
            updatePrevTree(2 * node + 1, mid + 1, end, idx, val);
        }
        treePrev[node] = max(treePrev[2 * node], treePrev[2 * node + 1]);
    }
}

int queryMaxPrev(int node, int start, int end, int l, int r) {
    if (r < start || end < l) {
        return -1; 
    }
    if (l <= start && end <= r) {
        return treePrev[node];
    }
    int mid = (start + end) / 2;
    return max(queryMaxPrev(2 * node, start, mid, l, r), 
               queryMaxPrev(2 * node + 1, mid + 1, end, l, r));
}

// --- Position Tracking ---
// We need to track positions of every value to update 'prev' pointers efficiently.
// Using set is O(log N) which fits within limits.
set<int> positions[MAXT]; 

int main() {
    // Optimize I/O operations
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> n >> t_max >> q)) return 0;

    for (int i = 1; i <= n; i++) {
        cin >> S[i];
        positions[S[i]].insert(i);
    }

    // 1. Build MinMax Tree
    buildMinMax(1, 1, n);

    // 2. Build Prev Tree
    // Initialize prev array
    for (int val = 1; val <= t_max; val++) {
        int last_pos = 0;
        for (int pos : positions[val]) {
            prev_arr[pos] = last_pos;
            updatePrevTree(1, 1, n, pos, last_pos);
            last_pos = pos;
        }
    }

    // Process Queries
    while (q--) {
        int type;
        cin >> type;
        if (type == 1) {
            int l, r;
            cin >> l >> r;
            
            // Check 1: Distinctness
            // The previous occurrence of any number in [l, r] must appear BEFORE l.
            int max_prev = queryMaxPrev(1, 1, n, l, r);
            if (max_prev >= l) {
                cout << "NIE\n";
                continue;
            }

            // Check 2: Contiguity
            // Max(range) - Min(range) == Length - 1
            NodeMinMax res = queryMinMax(1, 1, n, l, r);
            if ((res.max_val - res.min_val) == (r - l)) {
                cout << "TAK\n";
            } else {
                cout << "NIE\n";
            }

        } else {
            int p, v;
            cin >> p >> v;
            int old_v = S[p];
            if (old_v == v) continue; // No change

            // --- Update Handling ---
            
            // A. Remove p from old_v set
            {
                auto it = positions[old_v].find(p);
                int prev_idx = 0;
                int next_idx = 0;
                
                // Identify predecessor of p (in terms of position for value old_v)
                if (it != positions[old_v].begin()) {
                    prev_idx = *prev(it);
                }
                
                // Identify successor of p
                auto next_it = next(it);
                if (next_it != positions[old_v].end()) {
                    next_idx = *next_it;
                    // The successor's new previous is p's old previous
                    updatePrevTree(1, 1, n, next_idx, prev_idx);
                }
                
                positions[old_v].erase(it);
            }

            // B. Add p to v (new value) set
            {
                positions[v].insert(p);
                auto it = positions[v].find(p);
                int prev_idx = 0;
                int next_idx = 0;

                // Identify new predecessor for p
                if (it != positions[v].begin()) {
                    prev_idx = *prev(it);
                }
                // Update p's previous value in the tree
                updatePrevTree(1, 1, n, p, prev_idx);

                // Identify successor of p
                auto next_it = next(it);
                if (next_it != positions[v].end()) {
                    next_idx = *next_it;
                    // The successor's new previous is now p
                    updatePrevTree(1, 1, n, next_idx, p);
                }
            }

            // C. Update Value Array and MinMax Tree
            S[p] = v;
            updateMinMax(1, 1, n, p, v);
        }
    }

    return 0;
}
