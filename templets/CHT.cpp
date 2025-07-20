#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define int long long

struct Line {
    mutable ll k, m, p;
    bool operator<(const Line &o) const { return k < o.k; }
    bool operator<(ll x) const { return p < x; }
};

struct HullDynamic : multiset<Line, less<> > {
    const ll inf = 2e18;
    ll type; // 1 = max, -1 = min

    HullDynamic(ll t = 1) : type(t) {}

    ll div(ll a, ll b) {
        // floored division
        return a / b - ((a ^ b) < 0 && a % b);
    }

    bool isect(iterator x, iterator y) {
        if (y == end()) {
            x->p = inf;
            return false;
        }
        if (x->k == y->k)
            x->p = x->m > y->m ? inf : -inf;
        else
            x->p = div(y->m - x->m, x->k - y->k);
        return x->p >= y->p;
    }

    void add(ll k, ll m) {
        k *= type;  m *= type;
        auto z = insert({k, m, 0}), y = z++, x = y;
        while (isect(y, z))            z = erase(z);
        if (x != begin() && isect(--x, y))
            isect(x, y = erase(y));
        while ((y = x) != begin() && (--x)->p >= y->p)
            isect(x, erase(y));
    }

    ll get(ll x) const {
        auto it = lower_bound(x);
        return type * (it->k * x + it->m);
    }
};

struct SegmentTree {
    int n;
    vector<HullDynamic> tree;

    SegmentTree(int _n)
            : n(_n), tree(4*_n, HullDynamic(-1)) {}

    ll query(int node, int l, int r, int ql, int qr, ll x) const {
        if (qr < l || ql > r)
            return LLONG_MAX;
        if (ql <= l && r <= qr) {
            if (tree[node].empty()) return LLONG_MAX;
            return tree[node].get(x);
        }
        int mid = (l + r) >> 1;
        return min(
                query(node<<1,   l,    mid, ql, qr, x),
                query(node<<1|1, mid+1, r,   ql, qr, x)
        );
    }

    ll query(int l, int r, ll x) const {
        return query(1, 1, n, l, r, x);
    }

    void update(int node, int l, int r, int idx, ll k, ll m) {
        tree[node].add(k, m);
        if (l == r) return;
        int mid = (l + r) >> 1;
        if (idx <= mid)
            update(node<<1,   l,    mid, idx, k, m);
        else
            update(node<<1|1, mid+1, r,   idx, k, m);
    }

    void update(int idx, ll k, ll m) {
        update(1, 1, n, idx, k, m);
    }
};

signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<ll> a(n+1), b(n+1);
    for (int i = 1; i <= n; i++) cin >> a[i];
    for (int i = 1; i <= n; i++) cin >> b[i];

    vector<vector<int>> adj(n+1);
    for (int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    vector<int> tin(n+1), tout(n+1);
    int timer = 0;
    function<void(int,int)> dfs = [&](int u, int p) {
        tin[u] = timer++;
        for (int v : adj[u]) if (v != p)
                dfs(v, u);
        tout[u] = timer - 1;
    };
    dfs(1, 0);

    vector<ll> ans(n+1);
    SegmentTree seg(n);

    function<void(int,int)> solve = [&](int u, int p) {
        bool isLeaf = true;
        for (int v : adj[u]) if (v != p) {
                solve(v, u);
                isLeaf = false;
            }
        if (isLeaf)
            ans[u] = 0;
        else
            ans[u] = seg.query(tin[u], tout[u], a[u]);
        seg.update(tin[u], b[u], ans[u]);
    };
    solve(1, 0);

    for (int i = 1; i <= n; i++)
        cout << ans[i] << (i<n ? ' ' : '\n');
    return 0;
}
