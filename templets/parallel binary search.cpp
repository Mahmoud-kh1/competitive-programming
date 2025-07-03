#include <bits/stdc++.h>
using namespace std;
const int MOD = 998244353;

struct BIT {
    int n;
    vector<int> f;
    BIT(int _n): n(_n), f(n+2,0) {}
    inline void add(int i, int v) {
        for (i++; i <= n+1; i += i&-i) f[i] += v;
    }
    inline int sum(int i) {
        int s = 0;
        for (i++; i > 0; i -= i&-i) s += f[i];
        return s;
    }
    inline void range_add(int l, int r, int v) {
        add(l, v);
        add(r+1, -v);
    }
};

int timer = 0;
vector<vector<int>> adj;
vector<int> in, out;
void dfs(int u, int p) {
    in[u] = timer++;
    for (int v : adj[u]) if (v != p) dfs(v, u);
    out[u] = timer - 1;
}

struct LCA {
    int LG, n;
    vector<int> lvl, first, log_2, euler, depth_euler;
    vector<vector<int>> anc, st;

    void dfs_euler(int u, int p) {
        anc[0][u] = p;
        lvl[u] = p ? lvl[p] + 1 : 0;
        first[u] = euler.size();
        euler.push_back(u);
        depth_euler.push_back(lvl[u]);
        for (int v : adj[u]) {
            if (v == p) continue;
            dfs_euler(v, u);
            euler.push_back(u);
            depth_euler.push_back(lvl[u]);
        }
    }

    void init(int nn) {
        n = nn;
        LG = __lg(n) + 1;
        lvl.assign(n+1, 0);
        first.assign(n+1, 0);
        anc.assign(LG, vector<int>(n+1, 0));
        log_2.clear();
        euler.clear();
        depth_euler.clear();
        dfs_euler(1, 0);
        for (int k = 1; k < LG; k++)
            for (int u = 1; u <= n; u++)
                anc[k][u] = anc[k-1][anc[k-1][u]];
        int m = euler.size();
        log_2.assign(m+1, 0);
        for (int i = 2; i <= m; i++)
            log_2[i] = log_2[i/2] + 1;
        int K = log_2[m] + 1;
        st.assign(K, vector<int>(m));
        for (int i = 0; i < m; i++) st[0][i] = i;
        for (int k = 1; k < K; k++)
            for (int i = 0; i + (1 << k) <= m; i++) {
                int x = st[k-1][i];
                int y = st[k-1][i + (1 << (k-1))];
                st[k][i] = (depth_euler[x] < depth_euler[y] ? x : y);
            }
    }

    int lca(int u, int v) {
        int L = first[u], R = first[v];
        if (L > R) swap(L, R);
        int len = R - L + 1;
        int k = log_2[len];
        int x = st[k][L];
        int y = st[k][R - (1 << k) + 1];
        return depth_euler[x] < depth_euler[y] ? euler[x] : euler[y];
    }

    int kth(int u, int k) {
        for (int i = 0; k; i++, k >>= 1)
            if (k & 1) u = anc[i][u];
        return u ? u : -1;
    }
};

vector<pair<int,int>> A;
vector<array<int,5>> queries;
vector<int> ans;
LCA lc;

void solve(int L, int R, vector<int>& idxs, BIT& bit) {
    if (idxs.empty()) return;
    if (L == R) {
        for (int qi : idxs)
            ans[queries[qi][3]] = A[L].first;
        return;
    }
    int M = (L + R) / 2;
    static int cur = 0;
    while (cur <= M) {
        bit.range_add(in[A[cur].second], out[A[cur].second], 1);
        cur++;
    }
    while (cur - 1 > M) {
        cur--;
        bit.range_add(in[A[cur].second], out[A[cur].second], -1);
    }
    vector<int> left, right;
    left.reserve(idxs.size());
    right.reserve(idxs.size());
    for (int qi : idxs) {
        auto& q = queries[qi];
        int u = q[0], v = q[1], l = q[2], id = q[3], k = q[4];
        int p = (l == 1 ? 1 : lc.kth(l, 1));
        int cnt = bit.sum(in[u]) + bit.sum(in[v]) - bit.sum(in[l]) - (l == 1 ? 0 : bit.sum(in[p]));
        if (cnt < k) right.push_back(qi);
        else left.push_back(qi);
    }
    solve(L, M, left, bit);
    solve(M + 1, R, right, bit);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    A.resize(n);
    for (int i = 0; i < n; i++) cin >> A[i].first, A[i].second = i + 1;
    adj.assign(n + 1, {});
    in.resize(n + 1);
    out.resize(n + 1);
    for (int i = 0, u, v; i < n - 1; i++) {
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    dfs(1, 0);
    lc.init(n);
    queries.resize(q);
    for (int i = 0; i < q; i++) {
        int u, v, k;
        cin >> u >> v >> k;
        int p = lc.lca(u, v);
        queries[i] = {u, v, p, i, k};
    }
    sort(A.begin(), A.end());
    BIT bit(n);
    ans.assign(q, 0);
    vector<int> all(q);
    iota(all.begin(), all.end(), 0);
    solve(0, n - 1, all, bit);
    for (int x : ans) cout << x << '\n';
    return 0;
}
