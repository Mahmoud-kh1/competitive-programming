// count paths k1 <= path <= k2
#include <bits/stdc++.h>
using namespace std;

#define int long long
#define all(a) a.begin(), a.end()
#define sz(u) (out[u] - in[u] + 1)

struct BIT {
    vector<int> bit;
    int n;

    BIT(int size) : n(size + 2) {
        bit.assign(n, 0);
    }

    void add(int idx, int val = 1) {
        idx++;
        while (idx < n) {
            bit[idx] += val;
            idx += idx & -idx;
        }
    }

    void erase(int idx, int val = 1) {
        add(idx, -val);
    }

    int sum(int idx) {
        idx++;
        int res = 0;
        while (idx > 0) {
            res += bit[idx];
            idx -= idx & -idx;
        }
        return res;
    }

    int sumrange(int l, int r) {
        return sum(r) - sum(l - 1);
    }
};

signed main() {
    int k1, k2;
    int n; cin >> n >> k1 >> k2;
    vector<int> adj[n + 1];
    for (int i = 1; i < n; i++) {
        int u, v; cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    int in[n + 1], out[n + 1], timer = 0, tree[n + 1], lvl[n + 1]{};
    function<void(int, int)> dfs = [&](int u, int p) {
        tree[timer] = u;
        in[u] = timer++;
        for (auto v : adj[u]) {
            if (v == p) continue;
            lvl[v] = lvl[u] + 1;
            dfs(v, u);
        }
        out[u] = timer - 1;
    };

    dfs(1, 0);

    BIT bit(n);
    int freq[n + 1]{}, ans = 0, res[n + 1]{};

    auto add = [&](int u) {
        freq[lvl[u]]++;
        bit.add(lvl[u]);
    };

    auto erase = [&](int u) {
        freq[lvl[u]]--;
        bit.erase(lvl[u]);
    };

    function<void(int, int, int)> solve = [&](int u, int p, int keep) {
        int mx = -1, bg = -1;
        for (auto v : adj[u]) {
            if (v == p) continue;
            if (bg == -1 || sz(v) > mx) {
                bg = v;
                mx = sz(v);
            }
        }

        for (auto v : adj[u]) {
            if (v == p || v == bg) continue;
            solve(v, u, false);
        }

        if (~bg) solve(bg, u, true);

        int l = lvl[u] + k1;
        int r = lvl[u] + k2;
        r = min(r, n - 1);
        if (r >= l) ans += bit.sumrange(l, r);

        add(u);

        for (auto v : adj[u]) {
            if (v == p || v == bg) continue;
            for (int i = in[v]; i <= out[v]; i++) {
                int x = tree[i];
                int reqL = k1 - lvl[x] + 2 * lvl[u];
                int reqR = k2 - lvl[x] + 2 * lvl[u];
                reqR = min(reqR, n - 1);
                if (reqL <= reqR) {
                    ans += bit.sumrange(reqL, reqR);
                }
            }
            for (int i = in[v]; i <= out[v]; i++) {
                add(tree[i]);
            }
        }

        if (!keep) {
            for (int i = in[u]; i <= out[u]; i++) {
                erase(tree[i]);
            }
        }
    };

    solve(1, 0, 1);
    cout << ans << endl;

    return 0;
}
