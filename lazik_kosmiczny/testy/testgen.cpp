#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(1, 10000);

    for (int i = 1; i <= 10000; i++) {
        string filename = "rand1000/test" + to_string(i) + ".in";
        ofstream out(filename);
        int a = dist(gen), b = dist(gen);
        out << a << " " << b << "\n";
    }

    return 0;
}
