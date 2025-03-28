#include "bits/stdc++.h"
using namespace std;

void fast() {
    ios::sync_with_stdio(0);
    cin.tie(NULL);
    cout.tie(0);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
}

#define int long long
const int N = 4e3;
vector<int> adj[N];

struct LCA {
    int LG, n;
    vector<int> lvl, pre;
    vector<vector<int>> anc;

    void dfs(int node, int par) {
        for (auto v : adj[node]) {
            if (v == par) continue;
            anc[0][v] = node;
            lvl[v] = lvl[node] + 1;
            dfs(v, node);
        }
    }

    void init(int nn, int root) {
        n = nn;
        LG = __lg(n) + 1;
        lvl.assign(n + 1, 0);
        pre.assign(n + 1, 0);
        anc.assign(LG, vector<int>(n + 1, 0));

        dfs(root, 0);

        for (int mask = 1; mask < LG; mask++) {
            for (int u = 1; u <= n; u++) {
                if (anc[mask - 1][u]) {
                    anc[mask][u] = anc[mask - 1][anc[mask - 1][u]];
                }
            }
        }
    }

    int kth(int u, int k) {
        for (int i = 0; k > 0 && u; i++, k >>= 1) {
            if (k & 1) {
                u = anc[i][u];
            }
        }
        return u ? u : -1;
    }

    int lca(int u, int v) {
        if (lvl[u] > lvl[v]) swap(u, v);
        int dif = lvl[v] - lvl[u];
        v = kth(v, dif);
        if (u == v) return u;

        for (int i = LG - 1; i >= 0; i--) {
            if (anc[i][u] != anc[i][v]) {
                u = anc[i][u];
                v = anc[i][v];
            }
        }
        return anc[0][u];
    }

    int dist2(int u, int v) {
        int lc = lca(u, v);
        return lvl[u] + lvl[v] - 2 * lvl[lc];
    }
};
LCA lca;
int next(int u, int v) {
    if(u == v) return u;
    int l = lca.lca(u, v);
    if(l == u) {
        int d = lca.lvl[v] - lca.lvl[u];
        return lca.kth(v, d - 1);
    } else {
        return lca.kth(u, 1);
    }
}

string s;
vector<vector<int>> dp;

int solve(int u, int v) {
    if(u == -1 || v == -1) return 0;
    if (u == v) return dp[u][v] = 1;
    int &ret = dp[u][v];
    if(ret!= -1) return ret;

    if(u == v) return dp[u][v] = 1;

    if(lca.dist2(u, v) == 1) {
        return dp[u][v] = (s[u-1] == s[v-1] ? 2 : 1);
    }

    int nu = next(u, v);
    int nv = next(v, u);
    int res = 0;
    if(s[u-1] == s[v-1])
        res = max(res, solve(nu, nv) + 2);

    res = max(res, solve(nu, v));
    res = max(res, solve(u, nv));

    dp[u][v] = res;
    return res;
}

signed main() {
//    fast();
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        cin >> s;

        for (int i = 1; i <= n; i++) adj[i].clear();

        for (int i = 0; i < n - 1; i++) {
            int u, v;
            cin >> u >> v;
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        dp.assign(n + 1, vector<int>(n + 1, -1));

        lca.init(n, 1);

        int ans = 1;
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                if (dp[i][j] == -1) {
                    ans = max(ans, solve(i, j));
                }
            }
        }

        cout << ans << endl;
    }

    return 0;
}
