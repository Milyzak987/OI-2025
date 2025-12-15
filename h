/**
 * Mafia Kota Zuzola (MKZ) Solution
 * Based on problem analysis from sources [1-23]
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>

using namespace std;

const int MAXN = 100005;
const int MOD = 1e9 + 7;
const long long INF = 1e18; // Sufficiently large for costs up to 10^9

// Graph and Costs
int n, m;
long long cost[MAXN];
vector<int> adj[MAXN];

// Tarjan's Algorithm Variables
int dfs_num[MAXN], dfs_low[MAXN];
bool onStack[MAXN];
stack<int> st;
int timer = 0;

// Results
long long total_min_cost = 0;
long long total_ways = 1;

void findSCC(int u) {
    dfs_num[u] = dfs_low[u] = ++timer;
    st.push(u);
    onStack[u] = true;

    for (int v : adj[u]) {
        if (dfs_num[v] == 0) {
            // Successor v has not been visited
            findSCC(v);
            dfs_low[u] = min(dfs_low[u], dfs_low[v]);
        } else if (onStack[v]) {
            // v is in the stack and hence in the current SCC
            dfs_low[u] = min(dfs_low[u], dfs_num[v]);
        }
    }

    // Root of an SCC found
    if (dfs_low[u] == dfs_num[u]) {
        long long local_min = INF;
        long long local_count = 0;
        vector<int> component_nodes;

        // Pop all nodes in this SCC
        while (true) {
            int v = st.top();
            st.pop();
            onStack[v] = false;
            component_nodes.push_back(v);
            
            // Track min cost within this SCC
            if (cost[v] < local_min) {
                local_min = cost[v];
                local_count = 1;
            } else if (cost[v] == local_min) {
                local_count++;
            }

            if (u == v) break;
        }

        // Aggregate results
        total_min_cost += local_min;
        total_ways = (total_ways * local_count) % MOD;
    }
}

int main() {
    // Optimize I/O operations
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // 1. Input Reading
    if (!(cin >> n)) return 0; // Check for valid input

    for (int i = 1; i <= n; i++) {
        cin >> cost[i];
    }

    cin >> m;
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
    }

    // 2. Initialize Tarjan's arrays
    fill(dfs_num, dfs_num + n + 1, 0);
    fill(dfs_low, dfs_low + n + 1, 0);
    fill(onStack, onStack + n + 1, false);

    // 3. Process Graph to find SCCs
    for (int i = 1; i <= n; i++) {
        if (dfs_num[i] == 0) {
            findSCC(i);
        }
    }

    // 4. Output Result
    // Sources [19]: Print min cost and number of ways modulo 10^9+7
    cout << total_min_cost << " " << total_ways << endl;

    return 0;
}
