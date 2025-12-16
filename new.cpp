#include <bits/stdc++.h>
using namespace std;

const int MAXN = 5e5 + 7;
vector<int> children[MAXN];
vector<int> sz(MAXN, 1);
vector<double> dp(MAXN, 0.0);

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    int n, k;
    cin >> n >> k;

    for (int i = 2; i <= n; ++i) {
        int p;
        cin >> p;
        children[p].push_back(i);
    }

    double ans = 0.0;

    for (int i = n; i >= 1; --i) {
        for (int ch : children[i]) {
            sz[i] += sz[ch];
        }

        if (sz[i] == 1) continue;

        double denom = (double)(sz[i] - 1);

        double trig = 1.0 / denom;

        for (int ch : children[i]) {
            double cval = min(dp[ch], (double)sz[ch] / denom);
            trig = max(trig, cval);
        }

        dp[i] = trig;

        if (sz[i] > k) {
            ans = max(ans, dp[i]);
        }
    }

    cout << fixed << setprecision(10) << ans << "\n";

    return 0;
}
