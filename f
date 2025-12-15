#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void solve() {
    int n, k;
    if (!(cin >> n >> k)) return;

    vector<int> a(n);
    vector<int> vals;
    vals.reserve(n);

    // 1. Read Input
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
        vals.push_back(a[i]);
    }

    // 2. Coordinate Compression
    // Map arbitrary values to range [0, distinct_count-1]
    sort(vals.begin(), vals.end());
    vals.erase(unique(vals.begin(), vals.end()), vals.end());

    for (int i = 0; i < n; ++i) {
        a[i] = lower_bound(vals.begin(), vals.end(), a[i]) - vals.begin();
    }

    // 3. Sliding Window
    // We need a window with at most (k + 1) distinct numbers.
    // Inside such a window, we want the max frequency of the current number.
    
    // Frequency array for numbers in current window
    // Size is vals.size() which is <= n
    vector<int> counts(vals.size(), 0);
    
    int l = 0;
    int current_distinct = 0;
    int max_len = 0;
    int limit_distinct = k + 1;

    for (int r = 0; r < n; ++r) {
        // Add a[r] to window
        if (counts[a[r]] == 0) {
            current_distinct++;
        }
        counts[a[r]]++;

        // Shrink window if we have too many distinct numbers
        while (current_distinct > limit_distinct) {
            counts[a[l]]--;
            if (counts[a[l]] == 0) {
                current_distinct--;
            }
            l++;
        }

        // Update answer
        // We only care about the count of a[r] because only a[r]'s 
        // frequency increased in this step. If the max length is provided 
        // by another number, it was already handled when R was at that position.
        if (counts[a[r]] > max_len) {
            max_len = counts[a[r]];
        }
    }

    cout << max_len << "\n";
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    solve();

    return 0;
}
