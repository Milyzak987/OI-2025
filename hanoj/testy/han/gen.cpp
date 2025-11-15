#include <bits/stdc++.h>
using namespace std;

const int NUM_TESTS = 1000;
const int N = 1000000;
const int M = 1000;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(1, M);

    for (int t = 1; t <= NUM_TESTS; t++) {
        vector<vector<int>> stos(M + 1); // allocate on heap
        string filename = "in3/test" + to_string(t) + ".in";
        ofstream out(filename);

        out << N << " " << M << "\n";

        for (int i = 1; i <= N; i++) {
            int ind = dist(gen);
            stos[ind].push_back(i);
        }

        for (int i = 1; i <= M; i++) {
            out << stos[i].size() << " ";
            for (int x : stos[i]) out << x << " ";
            out << "\n";
        }

        out.close();
        cout << "Generated " << filename << "\n";
    }
}
