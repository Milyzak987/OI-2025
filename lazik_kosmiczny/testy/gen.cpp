#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1e6;

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(1, MAXN);

    for (int i = 1; i <= 10000; i++) {
        string filename = "rand1000/test" + to_string(i) + ".in";
        ofstream out(filename);
        int a = dist(gen), b = dist(gen);
        out << a << " " << b << "\n";
    }

    return 0;
}