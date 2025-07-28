#include <bits/stdc++.h>
using namespace std;
#define int long long
// change every value x in range l, r to value v , get sum of l,r get median or l, r
const int MAXV = 20;
struct Seg {
    int n, sz;
    vector<array<int,21>>lazy;
    vector<array<int,21>>tree;

    void build(int n) {
        n = n;
        sz = 1;
        while (sz < n) sz <<= 1;
        int total = 2 * sz;
        tree.resize(sz << 1);
        lazy.resize(sz << 1);
        for (int k = 1; k < total; ++k) {
            for (int v = 1; v <= MAXV; ++v) {
                tree[k][v] = 0;
                lazy[k][v] = v;
            }
        }
    }

    void propagate(int l, int r, int k) {
         int tmp[MAXV + 1]{};
        for (int v = 1; v <= MAXV; ++v) tmp[v] = 0;
        for (int v = 1; v <= MAXV; ++v) {
            int t = lazy[k][v];
            if (t < 1 || t > MAXV) t = v;
            tmp[t] += tree[k][v];
        }
        for (int v = 1; v <= MAXV; ++v) tree[k][v] = tmp[v];
        if (l != r) {
            int lc = k << 1, rc = k << 1 | 1;
            for (int v = 1; v <= MAXV; ++v) {
                lazy[lc][v] = lazy[k][lazy[lc][v]];
                lazy[rc][v] = lazy[k][lazy[rc][v]];
            }
        }

        for (int v = 1; v <= MAXV; ++v) lazy[k][v] = v;
    }

    void set(int pos, int x, int l, int r, int k) {
        if (l == r) {
            for (int v = 1; v <= MAXV; ++v) tree[k][v] = 0;
            tree[k][x] = 1;
            return;
        }
        propagate(l, r, k);
        int m = (l + r) >> 1;
        if (pos <= m) set(pos, x, l, m, k << 1);
        else set(pos, x, m + 1, r, k << 1 | 1);
        for (int v = 1; v <= MAXV; ++v)
            tree[k][v] = tree[k << 1][v] + tree[k << 1 | 1][v];
    }
    void set(int pos, int x) { set(pos, x, 1, sz, 1); }

    void update(int ql, int qr, int x, int y, int l, int r, int k) {
        propagate(l, r, k);
        if (r < ql || qr < l) return;
        if (ql <= l && r <= qr) {
            lazy[k][x] = y;
            propagate(l, r, k);
            return;
        }
        int m = (l + r) >> 1;
        update(ql, qr, x, y, l, m, k << 1);
        update(ql, qr, x, y, m + 1, r, k << 1 | 1);
        for (int v = 1; v <= MAXV; ++v)
            tree[k][v] = tree[k << 1][v] + tree[k << 1 | 1][v];
    }
    void update(int L, int R, int x, int y) {
        if (x != y) update(L, R, x, y, 1, sz, 1);
    }

    int query_sum(int ql, int qr, int l, int r, int k) {
        propagate(l, r, k);
        if (r < ql || qr < l) return 0;
        if (ql <= l && r <= qr) {
            int s = 0;
            for (int v = 1; v <= MAXV; ++v)
                s += v * tree[k][v];
            return s;
        }
        int m = (l + r) >> 1;
        return query_sum(ql, qr, l, m, k << 1)
               + query_sum(ql, qr, m + 1, r, k << 1 | 1);
    }
    int query1(int L, int R) { return query_sum(L, R, 1, sz, 1); }

    void getMed(int ql, int qr, int l, int r, int k, int freq[]) {
        propagate(l, r, k);
        if (r < ql || qr < l) return;
        if (ql <= l && r <= qr) {
            for (int v = 1; v <= MAXV; ++v)
                freq[v] += tree[k][v];
            return;
        }
        int m = (l + r) >> 1;
        getMed(ql, qr, l, m, k << 1, freq);
        getMed(ql, qr, m + 1, r, k << 1 | 1, freq);
    }
    int query2(int L, int R) {
        int freq[MAXV + 1] = {};
        getMed(L, R, 1, sz, 1, freq);
        int tot = 0;
        for (int v = 1; v <= MAXV; ++v) tot += freq[v];
        int need = (tot + 1) / 2, acc = 0;
        for (int v = 1; v <= MAXV; ++v) {
            acc += freq[v];
            if (acc >= need) return v;
        }
        return 1;
    }
};

int32_t main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    if (!(cin >> t)) return 0;
    while (t--) {
        int n;
        cin >> n;
        vector<int> a(n + 1);
        for (int i = 1; i <= n; ++i) cin >> a[i];

        Seg seg;
        seg.build(n);
        for (int i = 1; i <= n; ++i)
            seg.set(i, a[i]);

        int q;
        cin >> q;
        while (q--) {
            int ty;
            cin >> ty;
            if (ty == 1) {
                int l, r, x, y;
                cin >> l >> r >> x >> y;
                seg.update(l, r, x, y);
            } else if (ty == 2) {
                int l, r;
                cin >> l >> r;
                cout << seg.query1(l, r) << '\n';
            } else if (ty == 3) {
                int l, r;
                cin >> l >> r;
                cout << seg.query2(l, r) << '\n';
            }
        }
    }
    return 0;
}
