#include <bits/stdc++.h>
using namespace std;

int main() {
    cin.tie(0)->sync_with_stdio(0);

    int n;
    cin >> n;
    vector<string> in;
    for (int i = 0; i < n; i++) {
        string g;
        cin >> g;
        in.push_back(g);
    }

    long long k;
    cin >> k;
    string ops;
    cin >> ops;

    sort(in.begin(), in.end());

    vector<string> vis;
    string cur;

    for (auto op : ops) {
        if ('a' <= op and op <= 'z') {
            cur.push_back(op);
        }
        else if (op == 'B') {
            if (!cur.empty()) cur.pop_back();
        }
        else if (op == 'E') {
            vis.push_back(cur);
            cur.clear();
        }
        else if (op == 'T') {
            for (int i = vis.size() - 1; i >= 0; i--) {
                string s = vis[i];
                if (s.size() >= cur.size() and s.compare(0, cur.size(), cur) == 0) {
                    cur = s;
                    break;
                }
            }
        }
    }

    sort(vis.begin(), vis.end());

    if (vis == in) cout << "C";
    else cout << "I";

    return 0;
}
