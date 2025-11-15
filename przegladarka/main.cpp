#include <bits/stdc++.h>
using namespace std;

struct Node {
    int next[26];
    int term = -1;
    int depth = 0;
};

const int MAXN = 1e6 + 7;
string a[MAXN];
vector<Node> trie;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    long long S = 0;
    for (int i = 0; i < n; i++) {
        cin >> a[i];
        S += a[i].size();
    }

    Node root;
    fill(begin(root.next), end(root.next), -1);
    root.term = -1;
    root.depth = 0;
    trie.push_back(root);

    for (int i = 0; i < n; i++) {
        int v = 0;
        for (char ch : a[i]) {
            int c = ch - 'a';
            if (trie[v].next[c] == -1) {
                Node nd;
                fill(begin(nd.next), end(nd.next), -1);
                nd.term = -1;
                nd.depth = trie[v].depth + 1;
                trie[v].next[c] = (int)trie.size();
                trie.push_back(nd);
            }
            v = trie[v].next[c];
        }
        trie[v].term = i;
    }

    int m = (int)trie.size();
    vector<int> maxDepth(m, -1);

    vector<pair<int, int>> st;
    st.emplace_back(0, 0);
    vector<char> visited(m, 0);
    while (!st.empty()) {
        auto [v, state] = st.back();
        st.pop_back();
        if (state == 0) {
            st.emplace_back(v, 1);
            for (int c = 0; c < 26; c++) {
                int u = trie[v].next[c];
                if (u != -1) st.emplace_back(u, 0);
            }
        } else {
            int md = -1;
            if (trie[v].term != -1) md = trie[v].depth;
            for (int c = 0; c < 26; c++) {
                int u = trie[v].next[c];
                if (u != -1) md = max(md, maxDepth[u]);
            }
            maxDepth[v] = md;
        }
    }

    vector<vector<int>> children(m);
    for (int v = 0; v < m; ++v) {
        for (int c = 0; c < 26; c++) {
            int u = trie[v].next[c];
            if (u != -1) children[v].push_back(u);
        }
        sort(children[v].begin(), children[v].end(), [&](int x, int y) {
            if (maxDepth[x] != maxDepth[y]) return maxDepth[x] < maxDepth[y];
            return trie[x].depth < trie[y].depth;
        });
    }

    vector<int> order;
    vector<pair<int, int>> st2;
    st2.emplace_back(0, -1);
    while (!st2.empty()) {
        int v = st2.back().first;
        int& idx = st2.back().second;
        if (idx == -1) {
            if (trie[v].term != -1) order.push_back(trie[v].term);
            idx = 0;
        } else if (idx < (int)children[v].size()) {
            int u = children[v][idx++];
            st2.emplace_back(u, -1);
        } else {
            st2.pop_back();
        }
    }

    string ops;
    if (!order.empty()) {
        const string& s0 = a[order[0]];
        for (char ch : s0) ops.push_back(ch);
        ops.push_back('E');
        for (int i = 1; i < (int)order.size(); ++i) {
            const string& last = a[order[i - 1]];
            const string& cur = a[order[i]];
            int l = 0;
            int lim = min((int)last.size(), (int)cur.size());
            while (l < lim && last[l] == cur[l]) ++l;
            long long costA = (long long)cur.size() + 1;
            long long costB = 1 + (long long)(last.size() - l) + (long long)(cur.size() - l) + 1;
            if (costB <= costA) {
                ops.push_back('T');
                for (int b = 0; b < (int)last.size() - l; ++b) ops.push_back('B');
                for (int p = l; p < (int)cur.size(); ++p) ops.push_back(cur[p]);
                ops.push_back('E');
            } else {
                for (char ch : cur) ops.push_back(ch);
                ops.push_back('E');
            }
        }
    }

    cout << ops.size() << '\n';
    cout << ops << '\n';
    return 0;
}
