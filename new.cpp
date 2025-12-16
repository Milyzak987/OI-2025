/**
 * Problem: Wojna Gangow (Gang War)
 * Solution: Geometry + Sweep-line / Binary Search
 * Time Complexity: O(N log N)
 * Space Complexity: O(N)
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>

using namespace std;

// Use long double for high precision geometric calculations
using Real = long double;

const Real PI = acos(-1.0L);
const Real TWO_PI = 2.0L * PI;
const Real EPS = 1e-11L; // Epsilon for floating point comparisons

// Normalize any angle to the range [0, 2*PI)
Real normalize(Real angle) {
    angle = fmod(angle, TWO_PI);
    if (angle < 0) angle += TWO_PI;
    return angle;
}

struct Interval {
    Real L, R;
    bool wraps;
};

int main() {
    // Optimize standard I/O operations for speed
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    long long R_in;
    if (!(cin >> N >> R_in)) return 0;
    
    Real R = (Real)R_in;

    vector<Real> start_points;
    start_points.reserve(N);
    vector<Interval> intervals;
    intervals.reserve(N);

    // 1. Geometry Phase: Transform points into angular intervals
    for (int i = 0; i < N; ++i) {
        long long x, y;
        cin >> x >> y;

        // Calculate polar coordinates
        Real dist_sq = (Real)x * x + (Real)y * y;
        Real dist = sqrt(dist_sq);
        
        // Base angle of the point
        Real base_angle = atan2((Real)y, (Real)x);
        
        // Angular spread of the tangents: arccos(R / dist)
        // If dist is very close to R (should not happen per constraints), clamp to 0
        Real spread = 0.0;
        if (dist > R) {
            spread = acos(R / dist);
        }

        // Define the interval [L, R]
        Real s = normalize(base_angle - spread);
        Real e = normalize(base_angle + spread);

        start_points.push_back(s);
        
        // Determine if the interval wraps around the 0/2PI boundary
        // Since width < PI, if start > end, it must be wrapping
        bool wraps = (s > e);
        intervals.push_back({s, e, wraps});
    }

    // 2. Sorting Phase: Prepare for binary search
    sort(start_points.begin(), start_points.end());

    long long total_intersections = 0;

    // 3. Counting Phase: Sum how many start points fall into each interval
    for (const auto& interval : intervals) {
        long long count = 0;
        
        if (!interval.wraps) {
            // Standard Interval [L, R]: Count points x such that L <= x <= R
            // We use EPS to handle precision issues at boundaries
            auto it_start = lower_bound(start_points.begin(), start_points.end(), interval.L - EPS);
            auto it_end = upper_bound(start_points.begin(), start_points.end(), interval.R + EPS);
            count = distance(it_start, it_end);
        } else {
            // Wrapping Interval [L, 2PI) U [0, R]
            
            // Part A: Points >= L
            auto it_start = lower_bound(start_points.begin(), start_points.end(), interval.L - EPS);
            count += distance(it_start, start_points.end());
            
            // Part B: Points <= R
            auto it_end = upper_bound(start_points.begin(), start_points.end(), interval.R + EPS);
            count += distance(start_points.begin(), it_end);
        }
        
        total_intersections += count;
    }

    // Subtract N because every interval contains its own start point (self-intersection)
    // The remaining sum represents the total number of valid pairs.
    // Note: Due to the geometry (width < PI), exact asymmetry holds:
    // Pair (i, j) is counted exactly once either when checking interval i or interval j.
    cout << total_intersections - N << endl;

    return 0;
}
