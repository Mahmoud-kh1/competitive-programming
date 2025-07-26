#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// Segment Tree Beats supporting:
// 1) range chmin: ai = min(ai, x) for all i in [L,R]
// 2) range chmax: ai = max(ai, x) for all i in [L,R]
// 3) range sum query on [L,R]

struct SegBeats {
    struct Node {
        ll sum;
        ll max1, max2; int cntMax;
        ll min1, min2; int cntMin;
        Node(): sum(0), max1(LLONG_MIN), max2(LLONG_MIN), cntMax(0), min1(LLONG_MAX), min2(LLONG_MAX), cntMin(0) {}
    };

    int n;
    vector<Node> st;
    vector<ll> lazyMin, lazyMax;

    SegBeats(int _n): n(_n) {
        st.resize(4*n);
        lazyMin.assign(4*n, LLONG_MAX);
        lazyMax.assign(4*n, LLONG_MIN);
    }

    Node merge(const Node &a, const Node &b) {
        Node res;
        res.sum = a.sum + b.sum;
        // max
        if (a.max1 > b.max1) {
            res.max1 = a.max1;
            res.cntMax = a.cntMax;
            res.max2 = max(a.max2, b.max1);
        } else if (b.max1 > a.max1) {
            res.max1 = b.max1;
            res.cntMax = b.cntMax;
            res.max2 = max(b.max2, a.max1);
        } else {
            res.max1 = a.max1;
            res.cntMax = a.cntMax + b.cntMax;
            res.max2 = max(a.max2, b.max2);
        }
        // min
        if (a.min1 < b.min1) {
            res.min1 = a.min1;
            res.cntMin = a.cntMin;
            res.min2 = min(a.min2, b.min1);
        } else if (b.min1 < a.min1) {
            res.min1 = b.min1;
            res.cntMin = b.cntMin;
            res.min2 = min(b.min2, a.min1);
        } else {
            res.min1 = a.min1;
            res.cntMin = a.cntMin + b.cntMin;
            res.min2 = min(a.min2, b.min2);
        }
        return res;
    }

    void build(int k, int l, int r, const vector<ll> &a) {
        if (l == r) {
            ll v = a[l];
            st[k].sum = v;
            st[k].max1 = st[k].min1 = v;
            st[k].max2 = LLONG_MIN;
            st[k].min2 = LLONG_MAX;
            st[k].cntMax = st[k].cntMin = 1;
        } else {
            int m = (l+r)>>1;
            build(k<<1, l, m, a);
            build(k<<1|1, m+1, r, a);
            st[k] = merge(st[k<<1], st[k<<1|1]);
        }
    }

    void applyChmin(int k, ll x) {
        if (st[k].max1 <= x) return;
        st[k].sum -= (st[k].max1 - x) * st[k].cntMax;
        st[k].max1 = x;
        if (st[k].min1 > x) st[k].min1 = x;
        lazyMin[k] = min(lazyMin[k], x);
    }

    void applyChmax(int k, ll x) {
        if (st[k].min1 >= x) return;
        st[k].sum += (x - st[k].min1) * st[k].cntMin;
        st[k].min1 = x;
        if (st[k].max1 < x) st[k].max1 = x;
        lazyMax[k] = max(lazyMax[k], x);
    }

    void push(int k) {
        if (lazyMin[k] != LLONG_MAX) {
            applyChmin(k<<1,   lazyMin[k]);
            applyChmin(k<<1|1, lazyMin[k]);
            lazyMin[k] = LLONG_MAX;
        }
        if (lazyMax[k] != LLONG_MIN) {
            applyChmax(k<<1,   lazyMax[k]);
            applyChmax(k<<1|1, lazyMax[k]);
            lazyMax[k] = LLONG_MIN;
        }
    }

    // range chmin
    void updateChmin(int k, int l, int r, int L, int R, ll x) {
        if (r < L || l > R || st[k].max1 <= x) return;
        if (L <= l && r <= R && st[k].max2 < x) {
            applyChmin(k, x);
            return;
        }
        push(k);
        int m = (l+r)>>1;
        updateChmin(k<<1,   l,   m, L, R, x);
        updateChmin(k<<1|1, m+1, r, L, R, x);
        st[k] = merge(st[k<<1], st[k<<1|1]);
    }

    // range chmax
    void updateChmax(int k, int l, int r, int L, int R, ll x) {
        if (r < L || l > R || st[k].min1 >= x) return;
        if (L <= l && r <= R && st[k].min2 > x) {
            applyChmax(k, x);
            return;
        }
        push(k);
        int m = (l+r)>>1;
        updateChmax(k<<1,   l,   m, L, R, x);
        updateChmax(k<<1|1, m+1, r, L, R, x);
        st[k] = merge(st[k<<1], st[k<<1|1]);
    }

    ll querySum(int k, int l, int r, int L, int R) {
        if (r < L || l > R) return 0;
        if (L <= l && r <= R) return st[k].sum;
        push(k);
        int m = (l+r)>>1;
        return querySum(k<<1,   l,   m, L, R)
             + querySum(k<<1|1, m+1, r, L, R);
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<ll> a(n);
    for(int i = 0; i < n; i++) cin >> a[i];

    SegBeats seg(n);
    seg.build(1, 0, n-1, a);

    while(q--) {
        int type;
        cin >> type;
        if (type == 1) {
            int l, r; ll x;
            cin >> l >> r >> x;
            --l; --r;
            seg.updateChmin(1, 0, n-1, l, r, x);
        } else if (type == 2) {
            int l, r; ll x;
            cin >> l >> r >> x;
            --l; --r;
            seg.updateChmax(1, 0, n-1, l, r, x);
        } else {
            int l, r;
            cin >> l >> r;
            --l; --r;
            cout << seg.querySum(1, 0, n-1, l, r) << "\n";
        }
    }

    return 0;
}
