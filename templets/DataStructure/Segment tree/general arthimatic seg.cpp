#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Seg {
    int n;
    vector<ll> tree, lazyC, lazyD;
    Seg(int _n): n(_n) {
        tree.assign(4*n, 0);
        lazyC.assign(4*n, 0);
        lazyD.assign(4*n, 0);
    }

    // build from initial array a[0..n-1]
    void build(int k, int l, int r, const vector<ll>& a) {
        if (l == r) {
            tree[k] = a[l];
        } else {
            int m = (l+r)>>1;
            build(k<<1,  l,   m, a);
            build(k<<1|1,m+1, r, a);
            tree[k] = tree[k<<1] + tree[k<<1|1];
        }
    }

    // push down lazyC[k], lazyD[k] into children
    void push(int k, int l, int r) {
        ll c = lazyC[k];
        ll d = lazyD[k];
        if (c==0 && d==0) return;
        // apply to this node:
        int len = r-l+1;
        // sum_{i=l..r} i = (l+r)*len/2
        ll sumI = ( (ll)l + r ) * len / 2;
        tree[k] += c * len + d * sumI;

        if (l < r) {
            // propagate to children
            lazyC[k<<1]   += c;
            lazyD[k<<1]   += d;
            lazyC[k<<1|1] += c;
            lazyD[k<<1|1] += d;
        }
        lazyC[k] = lazyD[k] = 0;
    }

    // add AP on [L..R]: + (a + d*(i-L)) to x_i
    void updateAP(int k, int l, int r, int L, int R, ll a, ll d) {
        push(k,l,r);
        if (r < L || l > R) return;
        if (L <= l && r <= R) {
            // decompose a + d*(i-L) = (a - d*L) + d*i
            ll c0 = a - d * L;
            lazyC[k] += c0;
            lazyD[k] += d;
            push(k,l,r);
            return;
        }
        int m = (l+r)>>1;
        updateAP(k<<1,   l,   m, L,R,a,d);
        updateAP(k<<1|1, m+1, r, L,R,a,d);
        tree[k] = tree[k<<1] + tree[k<<1|1];
    }

    // query sum on [L..R]
    ll querySum(int k, int l, int r, int L, int R) {
        push(k,l,r);
        if (r < L || l > R) return 0;
        if (L <= l && r <= R) return tree[k];
        int m = (l+r)>>1;
        return querySum(k<<1,   l,   m, L,R)
             + querySum(k<<1|1, m+1, r, L,R);
    }
};


int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<ll> a(n);
    for(int i = 0; i < n; i++) cin >> a[i];

    Seg seg(n);
    seg.build(1, 0, n-1, a);

    while(m--){
        int t;
        cin >> t;
        if (t == 1) {
            int l, r;
            ll A, D;
            cin >> l >> r >> A >> D;
            // convert to 0‑based
            --l; --r;
            seg.updateAP(1, 0, n-1, l, r, A, D);
        } else {
            int l, r;
            cin >> l >> r;
            --l; --r;
            cout << seg.querySum(1, 0, n-1, l, r) << "\n";
        }
    }
    return 0;
}
