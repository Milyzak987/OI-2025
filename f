/**
 * Problem: Mafia Kota Zuzola (SCC Decomposition & Greedy Selection)
 * Approach:
 * 1. Find all Strongly Connected Components (SCCs) using Tarjan's Algorithm.
 * 2. For each SCC, identify the minimum recruitment cost among its nodes.
 * 3. Count how many nodes in that SCC share this minimum cost.
 * 4. Accumulate the total cost and the product of the counts (ways).
 *
 * Time Complexity: O(N + M)
 * Space Complexity: O(N)
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>

using namespace std;

// Constants
const int MAXN = 100005;
const int MOD = 1e9 + 7;
const int INF = 2e9 + 7; // Sufficiently larger than max cost 10^9

// Global variables for Graph and Costs
int n, m;
int cost[MAXN];
vector<int> adj[MAXN];

// Global variables for Tarjan's Algorithm
int disc[MAXN];   // Discovery time of nodes
int low[MAXN];    // Lowest discovery time reachable
int timer;        // Timer for discovery times
bool onStack[MAXN]; // To check if node is currently in the recursion stack
stack<int> st;    // Stack for Tarjan's

// Result variables
long long total_min_cost = 0;
long long total_ways = 1;

void tarjan(int u) {
    disc[u] = low[u] = ++timer;
    st.push(u);
    onStack[u] = true;

    for (int v : adj[u]) {
        if (disc[v] == -1) {
            // If v is not visited, recurse
            tarjan(v);
            low[u] = min(low[u], low[v]);
        } else if (onStack[v]) {
            // If v is in stack, it's a back-edge
            low[u] = min(low[u], disc[v]);
        }
    }

    // If u is the root of an SCC
    if (low[u] == disc[u]) {
        int min_c = INF;
        int count_c = 0;
        
        // Temporary buffer to store nodes of the current SCC
        // (Optimization: We can process them directly while popping)
        
        // Process all nodes in this SCC
        while (true) {
            int v = st.top();
            st.pop();
            onStack[v] = false;

            // Update min cost and count for this specific SCC
            if (cost[v] < min_c) {
                min_c = cost[v];
                count_c = 1;
            } else if (cost[v] == min_c) {
                count_c++;
            }

            if (u == v) break;
        }

        // Aggregate results
        total_min_cost += min_c;
        total_ways = (total_ways * count_c) % MOD;
    }
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // 1. Input Reading
    if (!(cin >> n)) return 0;

    for (int i = 1; i <= n; i++) {
        cin >> cost[i];
        // Initialize Tarjan's arrays
        disc[i] = -1; 
        low[i] = -1;
        onStack[i] = false;
    }

    cin >> m;
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
    }

    // 2. Process all nodes (Graph might be disconnected)
    timer = 0;
    for (int i = 1; i <= n; i++) {
        if (disc[i] == -1) {
            tarjan(i);
        }
    }

    // 3. Output
    cout << total_min_cost << " " << total_ways << endl;

    return 0;
}
