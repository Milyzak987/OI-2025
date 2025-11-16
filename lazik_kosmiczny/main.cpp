#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    ll n, m;
    cin >> n >> m;

    if (n == 1 && m == 1) {
        cout << 0;
        return 0;
    }
    if (n == 1) {
        cout << 1 << "P";
        return 0;
    }
    if (m == 1) {
        cout << 1 << "D";
        return 0;
    }

    ll g = gcd(n, m);
    if (g == 1) {
        cout << 2 << "\n";
        cout << "DP" << "\n";
        return 0;
    }

    ll dx = -1;
    if (gcd(g - 1, m) == 1) {
        dx = g - 1;
    } else if (gcd(g - 1, n) == 1) {
        dx = 1;
    }
    if (dx == -1) {
        for (ll dxx = 1; dxx < g; dxx++) {
            if (gcd(dxx, m) == 1 && gcd(g - dxx, n) == 1) {
                dx = dxx;
                break;
            }
        }
    }

    ll dy = g - dx;
    ll num_hor = dx, num_ver = dy;
    char hor = 'P', ver = 'D';
    if (num_ver > num_hor) {
        swap(num_hor, num_ver);
        swap(hor, ver);
    }

    string s(num_hor, hor);
    s += string(num_ver, ver);
    cout << g << "\n" << s << "\n";

    return 0;
}