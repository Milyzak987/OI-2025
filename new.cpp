/**
 * Problem: Mountain Biking (mtb)
 * Contest: Smol PREOI 2025
 * Solution by: Gemini
 * Approach: MST + LCA (Bottleneck Path)
 */

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Maximum number of nodes and log factor for binary lifting
const int MAXN = 200005;
const int LOGN = 19; 

// Stores heights of mountains
int h[MAXN];

// DSU parent array
int parent_dsu[MAXN];

// Adjacency list for the MST: stores pairs {neighbor, weight}
vector<pair<int, int>> adj[MAXN]; 

// LCA and Binary Lifting tables
// up[u][i] stores the 2^i-th ancestor of u
// max_edge[u][i] stores the max edge weight on the path to the 2^i-th ancestor
int up[MAXN][LOGN];
int max_edge[MAXN][LOGN];
int depth[MAXN];
int comp[MAXN]; // Component ID to handle disconnected graphs

// Structure for graph edges
struct Edge {
    int u, v;
    int weight;
    // Sort edges by weight ascending for Kruskal's
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

// DSU Find with path compression
int find_set(int v) {
    if (v == parent_dsu[v])
        return v;
    return parent_dsu[v] = find_set(parent_dsu[v]);
}

// DSU Union
void union_sets(int a, int b) {
    a = find_set(a);
    b = find_set(b);
    if (a != b)
        parent_dsu[b] = a;
}

// DFS to build the LCA table (Binary Lifting Preprocessing)
void dfs(int u, int p, int w, int d, int c_id) {
    depth[u] = d;
    comp[u] = c_id;
    up[u][0] = p;
    max_edge[u][0] = w;

    // Build jump tables
    for (int i = 1; i < LOGN; i++) {
        int ancestor = up[u][i-1];
        up[u][i] = up[ancestor][i-1];
        // The max weight for a jump of 2^i is max of two 2^(i-1) jumps
        max_edge[u][i] = max(max_edge[u][i-1], max_edge[ancestor][i-1]);
    }

    // Traverse children
    for (auto& edge : adj[u]) {
        int v = edge.first;
        int weight = edge.second;
        if (v != p) {
            dfs(v, u, weight, d + 1, c_id);
        }
    }
}

// Query function: Finds max edge weight on the path between u and v
int get_max_on_path(int u, int v) {
    if (u == v) return 0; 

    int res = 0;
    // Ensure u is deeper
    if (depth[u] < depth[v]) swap(u, v);

    // Lift u to the same depth as v
    for (int i = LOGN - 1; i >= 0; i--) {
        if (depth[u] - (1 << i) >= depth[v]) {
            res = max(res, max_edge[u][i]);
            u = up[u][i];
        }
    }

    if (u == v) return res;

    // Lift both u and v until they are just below their LCA
    for (int i = LOGN - 1; i >= 0; i--) {
        if (up[u][i] != up[v][i]) {
            res = max(res, max_edge[u][i]);
            res = max(res, max_edge[v][i]);
            u = up[u][i];
            v = up[v][i];
        }
    }

    // Account for the final edges connecting to the LCA
    res = max(res, max_edge[u][0]);
    res = max(res, max_edge[v][0]);
    return res;
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    // Read heights
    for (int i = 1; i <= n; i++) {
        cin >> h[i];
        parent_dsu[i] = i; // Initialize DSU
    }

    // Read edges and calculate weights
    vector<Edge> edges;
    edges.reserve(m);
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        // The cost to traverse an edge is limited by the higher of the two mountains
        int w = max(h[u], h[v]);
        edges.push_back({u, v, w});
    }

    // Kruskal's Algorithm: Sort edges and build MST
    sort(edges.begin(), edges.end());

    for (const auto& edge : edges) {
        if (find_set(edge.u) != find_set(edge.v)) {
            union_sets(edge.u, edge.v);
            adj[edge.u].push_back({edge.v, edge.weight});
            adj[edge.v].push_back({edge.u, edge.weight});
        }
    }

    // Run DFS to build LCA tables. Loop handles disconnected components (forest).
    int comp_count = 0;
    for (int i = 1; i <= n; i++) {
        if (comp[i] == 0) {
            comp_count++;
            dfs(i, i, 0, 0, comp_count);
        }
    }

    int q;
    cin >> q;
    while (q--) {
        int a, b;
        long long c; // Use long long to prevent overflow during addition
        cin >> a >> b >> c;

        // If start and end are in different components, impossible
        if (comp[a] != comp[b]) {
            cout << "NIE\n";
        } else {
            long long limit = c + h[a];
            int bottleneck;

            if (a == b) {
                bottleneck = h[a];
            } else {
                bottleneck = get_max_on_path(a, b);
            }

            if (bottleneck <= limit) {
                cout << "TAK\n";
            } else {
                cout << "NIE\n";
            }
        }
    }

    return 0;
}
