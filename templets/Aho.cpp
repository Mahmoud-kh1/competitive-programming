#include <bits/stdc++.h>
using namespace std;

struct Aho {
    static const int A = 26;

    struct Node {
       // try use vector (empty ) only resize it when you need it 
        int nxt[A];
        int link = 0;
        int isEnd = 0;
        int near = -1;

        Node() {
            memset(nxt, 0, sizeof(nxt));
        }
    };

    vector<Node> trie;
    vector<vector<int>> pattern;
    vector<vector<int>> all;

    Aho() {
        trie.emplace_back();
        pattern.emplace_back();
    }

    int newNode() {
        trie.emplace_back();
        pattern.emplace_back();
        return (int)trie.size() - 1;
    }

    int addString(const string &s) {
        int v = 0;

        for (char ch : s) {
            int c = ch - 'a';

            if (!trie[v].nxt[c])
                trie[v].nxt[c] = newNode();

            v = trie[v].nxt[c];
        }

        trie[v].isEnd = 1;
        pattern[v].push_back(v);

        return v;
    }

    void build() {
        queue<int> q;

        for (int c = 0; c < A; c++) {
            int u = trie[0].nxt[c];

            if (u) {
                trie[u].link = 0;
                q.push(u);
            }
        }

        while (!q.empty()) {
            int v = q.front();
            q.pop();

            int fail = trie[v].link;

            if (trie[fail].isEnd)
                trie[v].near = fail;
            else
                trie[v].near = trie[fail].near;

            for (int c = 0; c < A; c++) {
                int u = trie[v].nxt[c];

                if (u) {
                    trie[u].link = trie[fail].nxt[c];
                    q.push(u);
                } else {
                    trie[v].nxt[c] = trie[fail].nxt[c];
                }
            }
        }
    }

    int advance(int state, char c) {
        return trie[state].nxt[c - 'a'];
    }

    void compute_all() {
        int sz = trie.size();

        all.resize(sz);

        for (int i = 0; i < sz; i++) {
            for (int v = i; v != -1; v = trie[v].near) {
                for (auto x : pattern[v]) {
                    all[i].push_back(x);
                }
            }
        }
    }
};

signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int q, n;
    cin >> q >> n;

    vector<string> a(n);
    vector<int> end(n);

    Aho aho;

    for (int i = 0; i < n; i++) {
        cin >> a[i];
        end[i] = aho.addString(a[i]);
    }

    aho.build();
    aho.compute_all();

    while (q--) {
        string query;
        cin >> query;

        if (query[0] == '?') {
            int ans = 0;
            int state = 0;

            for (int i = 1; i < (int)query.size(); i++) {
                state = aho.advance(state, query[i]);

                for (auto v : aho.all[state]) {
                    ans += aho.trie[v].isEnd;
                }
            }

            cout << ans << '\n';
        } else {
            int add = (query[0] == '+') ? 1 : 0;

            int id = stoi(query.substr(1));
            int v = end[id - 1];

            aho.trie[v].isEnd = add;
        }
    }

    return 0;
}
