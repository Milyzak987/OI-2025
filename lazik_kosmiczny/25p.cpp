#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n, m;
    cin >> n >> m;

    if (n == 1 && m == 1) {
        cout << "\n";
        return 0;
    }
    if (n == 1) {
        cout << "P\n";
        return 0;
    }
    if (m == 1) {
        cout << "D\n";
        return 0;
    }

    long long g = gcd(n, m);
    string s;
    if (g == 1) {
        s = "DP";
    } else {
        s.append(g - 1, 'D');
        s.push_back('P');
        s.append(g - 2, 'G');
    }
    cout << s.length() << "\n" << s << "\n";
}