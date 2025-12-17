#include <vector>
#include <algorithm>
#include <cmath>
#include <iostream>

using namespace std;

const int MAXN = 100005;
const int LOG = 20; // Sufficient for N <= 10^5 or 10^6

vector<int> adj[MAXN];
int weights[MAXN];
int up[MAXN][LOG];      // up[u][i] is the 2^i-th ancestor of u
int max_path[MAXN][LOG]; // max_path[u][i] is max weight on path u -> up[u][i]
int depth[MAXN];
int N;

// DFS to setup depths and direct parents (2^0 ancestors)
void dfs(int u, int p, int d) {
    depth[u] = d;
    up[u][0] = p;
    // Max weight path of length 2^0 (1 edge) includes u and parent
    // Note: If u is root, p should be u or handled separately
    if (p != -1)
        max_path[u][0] = max(weights[u], weights[p]);
    else
        max_path[u][0] = weights[u];

    for (int v : adj[u]) {
        if (v != p) {
            dfs(v, u, d + 1);
        }
    }
}

// Precompute the binary lifting tables
void precompute() {
    for (int j = 1; j < LOG; j++) {
        for (int i = 1; i <= N; i++) {
            if (up[i][j-1] != -1) {
                up[i][j] = up[up[i][j-1]][j-1];
                // Combine the max of the first half and the second half
                max_path[i][j] = max(max_path[i][j-1], max_path[up[i][j-1]][j-1]);
            } else {
                up[i][j] = -1;
                max_path[i][j] = max_path[i][j-1]; // Fallback
            }
        }
    }
}

// Helper to get max weight on path from u to an ancestor at distinct 'h' above
int get_max_up(int u, int dist) {
    int res = weights[u]; // Start with u's own weight
    for (int j = 0; j < LOG; j++) {
        if ((dist >> j) & 1) {
            res = max(res, max_path[u][j]);
            u = up[u][j];
        }
    }
    return res;
}

// Standard LCA finding
int get_lca(int u, int v) {
    if (depth[u] < depth[v]) swap(u, v);
    
    // Bring u to the same depth as v
    for (int j = LOG - 1; j >= 0; j--) {
        if (up[u][j] != -1 && depth[up[u][j]] >= depth[v]) {
            u = up[u][j];
        }
    }
    
    if (u == v) return u;
    
    // Lift both until just below LCA
    for (int j = LOG - 1; j >= 0; j--) {
        if (up[u][j] != up[v][j]) {
            u = up[u][j];
            v = up[v][j];
        }
    }
    return up[u][0];
}

// Main query function
int query(int u, int v) {
    int lca = get_lca(u, v);
    int res_u = get_max_up(u, depth[u] - depth[lca]);
    int res_v = get_max_up(v, depth[v] - depth[lca]);
    return max(res_u, res_v);
}
