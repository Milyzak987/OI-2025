#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(1, 50);

    for (int i = 1; i <= 1000; i++) {
        string filename = "testy/rand1000/test" + to_string(i) + ".in";
        ofstream out(filename);
        if (!out) {
            cerr << "Error creating " << filename << "\n";
            continue;
        }
        int a = dist(gen), b = dist(gen);
        out << a << " " << b << "\n";
    }

    return 0;
}
