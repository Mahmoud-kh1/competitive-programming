#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// Segment tree for range-add updates and point queries
struct SegmentTree {
    int n;
    vector<ll> tree, lazy;

    SegmentTree(int _n): n(_n) {
        tree.assign(4*n, 0);
        lazy.assign(4*n, 0);
    }

    void apply(int node, int l, int r, ll val) {
        tree[node] += (r - l + 1) * val;
        lazy[node] += val;
    }

    void push(int node, int l, int r) {
        if (lazy[node] == 0) return;
        int m = (l + r) >> 1;
        apply(node*2, l, m, lazy[node]);
        apply(node*2+1, m+1, r, lazy[node]);
        lazy[node] = 0;
    }

    void update(int node, int l, int r, int ql, int qr, ll val) {
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) {
            apply(node, l, r, val);
            return;
        }
        push(node, l, r);
        int m = (l + r) >> 1;
        update(node*2, l, m, ql, qr, val);
        update(node*2+1, m+1, r, ql, qr, val);
        tree[node] = tree[node*2] + tree[node*2+1];
    }

    // Add 'val' to all elements in [l..r]
    void range_add(int l, int r, ll val) {
        update(1, 1, n, l, r, val);
    }

    // Query single element at position pos
    ll point_query(int node, int l, int r, int pos) {
        if (l == r) return tree[node];
        push(node, l, r);
        int m = (l + r) >> 1;
        if (pos <= m)
            return point_query(node*2, l, m, pos);
        else
            return point_query(node*2+1, m+1, r, pos);
    }

    ll get(int pos) {
        return point_query(1, 1, n, pos);
    }
};

// Segment tree on differences for GCD queries
struct SegmentTreeGCD {
    int n;
    vector<ll> tree;

    SegmentTreeGCD(int _n): n(_n) {
        tree.assign(4*n, 0);
    }

    ll gcd(ll a, ll b) { return b == 0 ? llabs(a) : gcd(b, a % b); }

    void point_update(int node, int l, int r, int idx, ll val) {
        if (l == r) {
            tree[node] = val;
            return;
        }
        int m = (l + r) >> 1;
        if (idx <= m) point_update(node*2, l, m, idx, val);
        else          point_update(node*2+1, m+1, r, idx, val);
        tree[node] = gcd(tree[node*2], tree[node*2+1]);
    }

    // Set the difference at idx
    void set_diff(int idx, ll val) {
        if (idx < 1 || idx >= n) return;
        point_update(1, 1, n, idx, val);
    }

    ll range_gcd(int node, int l, int r, int ql, int qr) {
        if (qr < l || r < ql)     return 0;
}

/*
Sample Test:
Input:
6 5
3 6 9 12 15 18
2 2 5
1 3 6 -3
2 1 6
1 1 2 4
2 1 3

Output:
3
3
1
*/
        if (ql <= l && r <= qr) return tree[node];
        int m = (l + r) >> 1;
        ll left_g = range_gcd(node*2, l, m, ql, qr);
        ll right_g = range_gcd(node*2+1, m+1, r, ql, qr);
        return gcd(left_g, right_g);
    }

    // Get gcd of all differences in (l..r]
    ll get_gcd(int l, int r) {
        if (l + 1 > r) return 0;
        return range_gcd(1, 1, n, l+1, r);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<ll> A(n+1);
    for (int i = 1; i <= n; i++) cin >> A[i];

    SegmentTree seg(n);
    SegmentTreeGCD seg_gcd(n);

    // Build initial array in seg and diff in seg_gcd
    for (int i = 1; i <= n; i++) {
        seg.range_add(i, i, A[i]);
    }
    for (int i = 1; i < n; i++) {
        ll diff = A[i+1] - A[i];
        seg_gcd.set_diff(i, diff);
    }

    while (q--) {
        int type;
        cin >> type;
        if (type == 1) {
            int l, r;
            ll v;
            cin >> l >> r >> v;
            seg.range_add(l, r, v);
            if (l > 1) {
                ll new_diff = seg.get(l) - seg.get(l-1);
                seg_gcd.set_diff(l-1, new_diff);
            }
            if (r < n) {
                ll new_diff = seg.get(r+1) - seg.get(r);
                seg_gcd.set_diff(r, new_diff);
            }
        } else if (type == 2) {
            int l, r;
            cin >> l >> r;
            ll head = seg.get(l);
            ll g = seg_gcd.get_gcd(l, r);
            ll ans = seg_gcd.gcd(head, g);
            cout << ans << "\n";
        }
    }

    return 0;
}
