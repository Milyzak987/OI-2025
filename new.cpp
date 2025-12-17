/**
 * Finansowanie dróg (Road Funding)
 * Solution by Gemini
 * * Approach:
 * 1. Build the full tree using all 'S' operations.
 * 2. Precompute LCA (Lowest Common Ancestor) for O(log N) distance queries.
 * 3. Use DSU (Disjoint Set Union) to manage connected components.
 * 4. For each component, maintain the endpoints of its diameter.
 * When merging components, the new diameter is found by comparing distances
 * between the endpoints of the two merging components.
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

// Constants
const int MAXN = 200005;
const int LOGN = 19; // 2^19 > 200,000

// Graph and LCA storage
vector<int> adj[MAXN];
int up[MAXN][LOGN];
int depth[MAXN];

// DSU storage
int parent[MAXN];
int rnk[MAXN]; // Rank for union by rank
pair<int, int> diam_end[MAXN]; // Endpoints of diameter for component rooted at i

// Input storage
struct Operation {
    char type;
    int u, v; // v is 0 for T operations
};
vector<Operation> ops;

int n, q;

// ---------------- LCA / Tree Preprocessing ----------------

// DFS to calculate depths and binary lifting parents
void dfs(int u, int p, int d) {
    depth[u] = d;
    up[u][0] = p;
    for (int v : adj[u]) {
        if (v != p) {
            dfs(v, u, d + 1);
        }
    }
}

// Build binary lifting table
void precompute_lca() {
    for (int j = 1; j < LOGN; j++) {
        for (int i = 1; i <= n; i++) {
            if (up[i][j - 1] != 0)
                up[i][j] = up[up[i][j - 1]][j - 1];
            else
                up[i][j] = 0;
        }
    }
}

// Find Lowest Common Ancestor
int get_lca(int u, int v) {
    if (depth[u] < depth[v]) swap(u, v);
    
    // Lift u to same depth as v
    for (int j = LOGN - 1; j >= 0; j--) {
        if (depth[u] - (1 << j) >= depth[v]) {
            u = up[u][j];
        }
    }
    
    if (u == v) return u;
    
    // Lift both until they are children of LCA
    for (int j = LOGN - 1; j >= 0; j--) {
        if (up[u][j] != up[v][j]) {
            u = up[u][j];
            v = up[v][j];
        }
    }
    return up[u][0];
}

// Distance between two nodes in the tree
int dist(int u, int v) {
    return depth[u] + depth[v] - 2 * depth[get_lca(u, v)];
}

// ---------------- DSU Implementation ----------------

void make_set(int v) {
    parent[v] = v;
    rnk[v] = 0;
    diam_end[v] = {v, v}; // A single node has diameter 0, endpoints {v, v}
}

int find_set(int v) {
    if (v == parent[v]) return v;
    return parent[v] = find_set(parent[v]); // Path compression
}

void union_sets(int a, int b) {
    a = find_set(a);
    b = find_set(b);
    
    if (a != b) {
        // We are merging components led by 'a' and 'b'.
        // New diameter endpoints will be chosen from {ends_a, ends_b}.
        int p1 = diam_end[a].first;
        int p2 = diam_end[a].second;
        int p3 = diam_end[b].first;
        int p4 = diam_end[b].second;
        
        int candidates[4] = {p1, p2, p3, p4};
        int max_d = -1;
        pair<int, int> best_pair = {p1, p2};

        // Check distance between all pairs of candidates
        for(int i = 0; i < 4; ++i) {
            for(int j = i + 1; j < 4; ++j) {
                int d = dist(candidates[i], candidates[j]);
                if (d > max_d) {
                    max_d = d;
                    best_pair = {candidates[i], candidates[j]};
                }
            }
        }

        // Union by rank
        if (rnk[a] < rnk[b]) swap(a, b);
        parent[b] = a;
        if (rnk[a] == rnk[b]) rnk[a]++;
        
        // Update diameter of the new root
        diam_end[a] = best_pair;
    }
}

// ---------------- Main ----------------

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> n >> q)) return 0;

    // 1. Read Operations and Build Full Tree
    for (int i = 0; i < q; i++) {
        char type;
        cin >> type;
        if (type == 'S') {
            int u, v;
            cin >> u >> v;
            ops.push_back({type, u, v});
            // Add edge to adjacency list for LCA preprocessing
            adj[u].push_back(v);
            adj[v].push_back(u);
        } else {
            int v;
            cin >> v;
            ops.push_back({type, v, 0});
        }
    }

    // 2. Preprocess LCA (Root at 1)
    dfs(1, 0, 0);
    precompute_lca();

    // 3. Initialize DSU
    for (int i = 1; i <= n; i++) {
        make_set(i);
    }

    // 4. Process Operations Chronologically
    for (const auto& op : ops) {
        if (op.type == 'S') {
            union_sets(op.u, op.v);
        } else {
            int root = find_set(op.u);
            // The longest path from u must end at one of the diameter endpoints
            int d1 = dist(op.u, diam_end[root].first);
            int d2 = dist(op.u, diam_end[root].second);
            cout << max(d1, d2) << "\n";
        }
    }

    return 0;
}
