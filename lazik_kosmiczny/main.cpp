#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, m;
    cin >> n >> m;

    int g = gcd(n, m);

    if (n > m) {
        string s(max(g - 1, 1), 'P');
        s += 'G';
        cout << s.size() << "\n" << s << "\n";
    } else {
        string s(max(g - 1, 1), 'G');
        s += 'P';
        cout << s.size() << "\n" << s << "\n";
    }

    return 0;
}
