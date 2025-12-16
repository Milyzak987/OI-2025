/**
 * Solution for "Gra w kotki" (PREOI 2025 Day 4)
 *
 * Problem Analysis:
 * We need to find the breakdown point for the loyalty coefficient x.
 * A node u can be 'forced' (bringing its whole subtree) if the rebellion propagates from a child.
 * We compute dp[u] = max x such that u can be forced.
 *
 * Transitions:
 * 1. Base case trigger: A single saboteur in a child's subtree acts as 1 unit.
 * Threshold = 1 / (sz[u] - 1).
 * 2. Recursive trigger: A child v is already forced (bringing sz[v] units).
 * Threshold = min(dp[v], sz[v] / (sz[u] - 1)).
 *
 * Answer: Max dp[u] for all u where sz[u] > k.
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

// Using global variables to avoid stack overflow on deep recursion,
// though iterative approach is used below.
const int MAXN = 500005;
vector<int> children[MAXN];
int sz[MAXN];
double dp[MAXN];

int main() {
    // Optimize I/O operations
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, k;
    if (!(cin >> n >> k)) return 0;

    // Reading input. Note: Node 1 is root.
    // Input format: n-1 lines, i-th line contains parent of i+1.
    [span_3](start_span)// Parents always have smaller indices[span_3](end_span).
    for (int i = 2; i <= n; ++i) {
        int p;
        cin >> p;
        children[p].push_back(i);
    }

    // Initialize arrays
    fill(sz, sz + n + 1, 1);
    fill(dp, dp + n + 1, 0.0);

    double ans = 0.0;

    // Process nodes in reverse topological order (N down to 1).
    // This ensures children are processed before parents.
    for (int i = n; i >= 1; --i) {
        // 1. Compute Subtree Size
        for (int child : children[i]) {
            sz[i] += sz[child];
        }

        // Leaves cannot be "forced" by subordinates (no subordinates),
        // so dp[i] remains 0.0. They can only act as saboteurs (mass 1).
        if (sz[i] == 1) continue;

        // 2. Compute DP
        [span_4](start_span)// Denominator for fraction: (sz[i] - 1)[span_4](end_span)
        double denom = (double)(sz[i] - 1);
        
        // Option A: Triggered by a single saboteur in any child's branch (Mass = 1)
        double max_trigger = 1.0 / denom;

        // Option B: Triggered by a forced child v (Mass = sz[v])
        for (int child : children[i]) {
            // We need the child chain to sustain (dp[child]) 
            // AND the transition to i to hold (sz[child] / denom).
            double chain_val = min(dp[child], (double)sz[child] / denom);
            max_trigger = max(max_trigger, chain_val);
        }

        dp[i] = max_trigger;

        // 3. Update Answer
        // If forcing node i results in > k cats, this x is a valid success threshold.
        // We want the smallest x where it FAILS, which is the boundary of success.
        if (sz[i] > k) {
            ans = max(ans, dp[i]);
        }
    }

    [span_5](start_span)// Output with required precision[span_5](end_span)
    cout << fixed << setprecision(10) << ans << endl;

    return 0;
}
