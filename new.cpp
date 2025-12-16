#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>

using namespace std;

const int INF = -1e9;

class DistinctCheckerIterative {
private:
    int n;
    vector<int> arr;
    vector<int> tree; // Segment Tree array
    map<int, set<int>> positions; // Stores indices for each value

    // Iterative Build
    void build_tree(const vector<int>& prev_occurrence) {
        // Initialize leaves
        for (int i = 0; i < n; ++i) {
            tree[n + i] = prev_occurrence[i];
        }
        // Calculate parents
        for (int i = n - 1; i > 0; --i) {
            tree[i] = max(tree[2 * i], tree[2 * i + 1]);
        }
    }

    // Iterative Point Update
    // Updates the value at index `idx` (0-based) to `val`
    void update_tree(int idx, int val) {
        idx += n; // Move to leaf position
        tree[idx] = val;
        
        // Move up to root
        for (idx /= 2; idx >= 1; idx /= 2) {
            tree[idx] = max(tree[2 * idx], tree[2 * idx + 1]);
        }
    }

    // Iterative Range Query
    // Returns max value in range [l, r] inclusive
    int query_max(int l, int r) {
        int res = INF;
        // Loop processes the range. 
        // l is the left bound, r is the right bound.
        for (l += n, r += n; l <= r; l /= 2, r /= 2) {
            // If l is odd, it's the right child, so include it and move inward
            if (l % 2 == 1) res = max(res, tree[l++]);
            // If r is even, it's the left child, so include it and move inward
            if (r % 2 == 0) res = max(res, tree[r--]);
        }
        return res;
    }

public:
    DistinctCheckerIterative(const vector<int>& input_arr) {
        arr = input_arr;
        n = arr.size();
        tree.resize(2 * n, INF); // Size is 2*n for iterative tree
        
        vector<int> prev_occurrence(n);

        // Precompute positions and prev array
        for (int i = 0; i < n; ++i) {
            if (positions[arr[i]].empty()) {
                prev_occurrence[i] = -1;
            } else {
                prev_occurrence[i] = *positions[arr[i]].rbegin();
            }
            positions[arr[i]].insert(i);
        }

        build_tree(prev_occurrence);
    }

    // Returns true if subarray [l, r] has NO repeating values
    bool hasUniqueValues(int l, int r) {
        if (l < 0 || r >= n || l > r) return false;
        
        // Check if the maximum 'prev' index in this range is strictly to the left of 'l'
        int max_prev = query_max(l, r);
        return max_prev < l;
    }

    // Update value at index idx to new_val
    void update(int idx, int new_val) {
        if (idx < 0 || idx >= n) return;
        int old_val = arr[idx];
        if (old_val == new_val) return;

        // --- Step 1: Remove idx from old_val positions ---
        auto it = positions[old_val].find(idx);
        int old_prev = -1;
        int old_next = -1;

        if (it != positions[old_val].begin()) old_prev = *std::prev(it);
        if (std::next(it) != positions[old_val].end()) old_next = *std::next(it);

        positions[old_val].erase(it);

        // If there was a next occurrence, it must now point to old_prev
        if (old_next != -1) {
            update_tree(old_next, old_prev);
        }

        // --- Step 2: Insert idx into new_val positions ---
        arr[idx] = new_val;
        positions[new_val].insert(idx);
        it = positions[new_val].find(idx);

        int new_prev = -1;
        int new_next = -1;

        if (it != positions[new_val].begin()) new_prev = *std::prev(it);
        if (std::next(it) != positions[new_val].end()) new_next = *std::next(it);

        // Update current index's 'prev'
        update_tree(idx, new_prev);

        // If there is a next occurrence, it must now point to idx
        if (new_next != -1) {
            update_tree(new_next, idx);
        }
    }
};

int main() {
    // Example Usage
    vector<int> data = {1, 2, 3, 2, 1, 4};
    DistinctCheckerIterative solver(data);

    cout << "Range [1, 2] (2, 3): " << (solver.hasUniqueValues(1, 2) ? "Unique" : "Repeating") << endl; 
    
    cout << "Range [1, 3] (2, 3, 2): " << (solver.hasUniqueValues(1, 3) ? "Unique" : "Repeating") << endl; 
    
    cout << "Updating index 3 to 5..." << endl;
    solver.update(3, 5); // Array becomes: 1, 2, 3, 5, 1, 4
    
    cout << "Range [1, 3] (2, 3, 5): " << (solver.hasUniqueValues(1, 3) ? "Unique" : "Repeating") << endl;

    return 0;
}
