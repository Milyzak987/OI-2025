#include <bits/stdc++.h>
using namespace std;

int main() {
    long long n, m;
    cin >> n >> m;
    long long g = gcd(n, m);
    if (g == 1) {
        cout << 2 << endl;
        cout << "DP" << endl;
    } else {
        long long dx = -1;
        if (gcd(g - 1, m) == 1) {
            dx = g - 1;
        } else if (gcd(g - 1, n) == 1) {
            dx = 1;
        }
        if (dx == -1) {
            for (long long dxx = 1; dxx < g; dxx++) {
                if (gcd(dxx, m) == 1 && gcd(g - dxx, n) == 1) {
                    dx = dxx;
                    break;
                }
            }
        }
        long long dy = g - dx;
        long long num_hor = dx, num_ver = dy;
        char hor = 'P', ver = 'D';
        if (num_ver > num_hor) {
            swap(num_hor, num_ver);
            swap(hor, ver);
        }
        string s(num_hor, hor);
        s += string(num_ver, ver);
        cout << g << endl;
        cout << s << endl;
    }
    return 0;
}