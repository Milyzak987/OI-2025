/**
 * Problem: Chatownicy (Chatters)
 * Role: World Finals Competitive Programmer
 * Strategy: Linear Path Decomposition + Greedy Matching
 * Complexity: O(N) Time, O(N) Space, N-1 Queries
 */

#include <vector>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <cassert>

// ------------------------------------------------------------------
// Local Testing Stub
// ------------------------------------------------------------------
// Define LOCAL to enable the internal testing library.
// In the judging environment, this is not defined, and "chalib.h" is used.
// #define LOCAL 

#ifdef LOCAL
    #include <random>
    #include <map>
    #include <set>

    namespace chalib {
        // Simulation parameters
        static int N_VAL = 20; 
        // Use a map to simulate the hidden graph state consistently
        static std::map<std::pair<int,int>, bool> adj;
        static std::mt19937 rng(1337);

        int daj_n() {
            return N_VAL;
        }

        bool zapytaj(int i, int j) {
            if (i < 1 || j < 1 || i > N_VAL || j > N_VAL || i == j) {
                std::cerr << "Invalid query: " << i << ", " << j << std::endl;
                exit(1);
            }
            if (i > j) std::swap(i, j);
            
            // If edge generated previously, return consistency. Else generate random.
            if (adj.find({i, j}) == adj.end()) {
                adj[{i, j}] = std::uniform_int_distribution<int>(0, 1)(rng);
            }
            return adj[{i, j}];
        }

        void odpowiedz(std::vector<std::pair<int, int>> a) {
            std::cout << "--------------------------------------------------\n";
            std::cout << "Simulator: Received " << a.size() << " pairs.\n";
            
            if (a.empty() && N_VAL >= 4) {
                std::cout << "Simulator: FAIL (Empty answer for N >= 4)\n";
                exit(0);
            }

            std::set<int> used;
            int type = -1; // 0 for enemy, 1 for friend
            bool first = true;

            for (const auto& p : a) {
                // Check disjointness
                if (used.count(p.first) || used.count(p.second)) {
                    std::cout << "Simulator: FAIL (Vertices not disjoint: " 
                              << p.first << "-" << p.second << ")\n";
                    exit(0);
                }
                used.insert(p.first);
                used.insert(p.second);

                // Check relation type consistency
                int u = p.first;
                int v = p.second;
                if (u > v) std::swap(u, v);
                
                // Retrieve the relation from our simulated map
                // Note: In a real run, the solver only knows what it queried.
                // Here we assume the solver logic is correct and uses queried edges.
                bool relation = zapytaj(u, v); 

                if (first) {
                    type = relation;
                    first = false;
                } else {
                    if (relation != type) {
                        std::cout << "Simulator: FAIL (Mixed relationship types)\n";
                        exit(0);
                    }
                }
            }

            // Check quantity threshold floor(n/4)
            int required = N_VAL / 4;
            if ((int)a.size() >= required) {
                std::cout << "Simulator: SUCCESS (Pairs: " << a.size() 
                          << ", Required: " << required << ", Type: " 
                          << (type ? "Friends" : "Enemies") << ")\n";
            } else {
                std::cout << "Simulator: FAIL (Not enough pairs. Got " << a.size() 
                          << ", Needed " << required << ")\n";
            }
            std::cout << "--------------------------------------------------\n";
            exit(0);
        }
    }
    // Bring simulation into global scope for local testing
    using namespace chalib;

#else
    // Official inclusion
    #include "chalib.h"
#endif

// ------------------------------------------------------------------
// Solution
// ------------------------------------------------------------------

void solve() {
    int n = daj_n();

    // Edge case: Not enough people to form pairs
    if (n < 2) {
        odpowiedz({});
        return;
    }

    // We will query the path 1-2, 2-3, ..., (n-1)-n.
    // We store the relationship type for each edge.
    // path_rels[i] corresponds to the edge between node (i+1) and node (i+2).
    // 0 = Enemy, 1 = Friend
    std::vector<int> path_rels;
    path_rels.reserve(n - 1);

    int count_friends = 0;
    int count_enemies = 0;

    // Phase 1: Query the linear path
    for (int i = 1; i < n; ++i) {
        bool is_friend = zapytaj(i, i + 1);
        if (is_friend) {
            path_rels.push_back(1);
            count_friends++;
        } else {
            path_rels.push_back(0);
            count_enemies++;
        }
    }

    // Phase 2: Determine Majority Type
    // Pigeonhole Principle: One type appears at least (n-1)/2 times.
    int target_type = (count_friends >= count_enemies) ? 1 : 0;

    // Phase 3: Greedy Matching
    // Iterate through the path again. If an edge matches the target type
    // and its vertices are not yet used, select it.
    std::vector<std::pair<int, int>> result;
    std::vector<bool> used(n + 1, false);

    // Iterating edges (i, i+1) where i ranges from 1 to n-1
    // The type of edge (i, i+1) is stored at path_rels[i-1]
    for (int i = 1; i < n; ++i) {
        if (path_rels[i - 1] == target_type) {
            // Check if both u and v are free
            if (!used[i] && !used[i + 1]) {
                result.push_back({i, i + 1});
                
                // Mark vertices as used
                used[i] = true;
                used[i + 1] = true;
            }
        }
    }

    // Phase 4: Submit Answer
    odpowiedz(result);
}

int main() {
    // Optimize I/O operations (though not strictly necessary given the interactive nature)
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    #ifdef LOCAL
        // If testing locally, we can loop to test various N or specific cases
        // chalib::N_VAL = 10; 
    #endif

    solve();

    return 0;
}
