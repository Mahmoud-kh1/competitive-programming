#include <bits/stdc++.h>
using namespace std;
vector<vector<int>> adj;
struct LCA {
    int LG, n;
    vector<int> lvl, pre;
    vector<vector<int>> anc;
    vector<int> euler, depth_euler, first, log_2;
    vector<vector<int>> st;

    void dfs_euler(int u, int p) {
        anc[0][u] = p;
        lvl[u] = (p ? lvl[p] + 1 : 0);
        first[u] = (int)euler.size();
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
        lvl.assign(n + 1, 0);
        pre.assign(n + 1, 0);
        anc.assign(LG, vector<int>(n + 1, 0));
        first.assign(n + 1, 0);
        euler.clear(); depth_euler.clear();
        dfs_euler(1, 0);
        for (int k = 1; k < LG; k++) {
            for (int u = 1; u <= n; u++) {
                anc[k][u] = anc[k-1][ anc[k-1][u] ];
            }
        }
        int m = euler.size();
        log_2.assign(m + 1, 0);
        for (int i = 2; i <= m; i++) log_2[i] = log_2[i/2] + 1;
        int K = log_2[m] + 1;
        st.assign(K, vector<int>(m));
        for (int i = 0; i < m; i++) st[0][i] = i;
        for (int k = 1; k < K; k++) {
            for (int i = 0; i + (1 << k) <= m; i++) {
                int x = st[k-1][i];
                int y = st[k-1][i + (1 << (k-1))];
                st[k][i] = (depth_euler[x] < depth_euler[y] ? x : y);
            }
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
};
inline int64_t hilbertOrder(int x, int y, int pow, int rotate) {
    if (pow == 0) {
        return 0;
    }
    int hpow = 1 << (pow - 1);
    int seg = (x < hpow) ? (
            (y < hpow) ? 0 : 3
    ) : (
                      (y < hpow) ? 1 : 2
              );
    seg = (seg + rotate) & 3;
    const int rotateDelta[4] = {3, 0, 0, 1};
    int nx = x & (x ^ hpow), ny = y & (y ^ hpow);
    int nrot = (rotate + rotateDelta[seg]) & 3;
    int64_t subSquareSize = int64_t(1) << (2 * pow - 2);
    int64_t ans = seg * subSquareSize;
    int64_t add = hilbertOrder(nx, ny, pow - 1, nrot);
    ans += (seg == 1 || seg == 2) ? add : (subSquareSize - add - 1);
    return ans;
}

struct Query {
    int l, r, iq, lca;
    int64_t ord = -1;
    Query() {}
    Query(int l, int r, int iq, int lca) : l(l), r(r), lca(lca), iq(iq) {
        calcOrder();
    }
    inline void calcOrder() {
        if (ord == -1)
            ord = hilbertOrder(l, r, 21, 0);
    }
    bool operator<(const Query &other) const {
        return ord < other.ord;
    }
};
const int N = 1e5 + 5, B = 317;
int in[N], out[N], tree[N << 1];
int t;
void dfs0(int u, int p = -1) {
    in[u] = t;
    tree[t++] = u;
    for (int v : adj[u]) {
        if (v != p) dfs0(v, u);
    }
    out[u] = t;
    tree[t++] = u;
}
LCA LC;
Query addQuery(int u, int v, int i) {
    if (in[u] > in[v]) swap(u, v);
    int lca = LC.lca(u, v);
    if (lca == u) {
        return Query(in[u], in[v], i, -1);
    } else {
        return Query(out[u], in[v], i, lca);
    }
}

signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q; cin >> n >> q;
    t = 0;
    int val[n + 1];
    vector<int> comp(n);
    for (int i = 1; i <= n; i++) {
        cin >> val[i];
        comp[i - 1] = val[i];
    }
    sort(comp.begin(), comp.end());
    comp.erase(unique(comp.begin(), comp.end()), comp.end());
    for (int i = 1; i <= n; i++) {
        val[i] = int(lower_bound(comp.begin(), comp.end(), val[i]) - comp.begin()) + 1;
    }
    adj.resize(n + 1);
    for(int i = 1; i < n; i++){
        int u,v; cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    LC.init(n);
    dfs0(1, 0);
    vector<Query> Queries(q);
    for(int i = 1; i <= q; i++){
        int u, v; cin >> u >> v;
        Queries[i - 1] = addQuery(u, v, i - 1);
        Queries[i - 1].calcOrder();
    }
    sort(Queries.begin(), Queries.end());
    int L = 0, R = -1;
    int vis[n + 1]{}, cnt[n + 1]{};
    int ans = 0;
    auto add = [&](int node){
        int w = val[node];
        if (!cnt[w]) ans++;
        cnt[w]++;
    };
    auto remove = [&](int node){
        int w = val[node];
        cnt[w]--;
        if (!cnt[w]) ans--;
    };
    auto update = [&](int idx){
        int node = tree[idx];
        if (vis[node]){
            remove(node);
        }
        else {
            add(node);
        }
        vis[node] ^= 1;
    };
    int final[q + 1];
    for (auto &qq : Queries) {
        while (R < qq.r) update(++R);
        while (L > qq.l) update(--L);
        while (R > qq.r) update(R--);
        while (L < qq.l) update(L++);
        if (qq.lca != -1) add(qq.lca);
        final[qq.iq] = ans;
        if (qq.lca != -1) remove(qq.lca);
    }
    for(int i = 0; i < q; i++){
        cout << final[i] << "\n";
    }
    return 0;
}
 
