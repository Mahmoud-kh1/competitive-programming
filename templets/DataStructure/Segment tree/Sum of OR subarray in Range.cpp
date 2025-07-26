#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int MOD = 4001;

struct Node {
    int len;          // total length of this segment
    int pre0, suf0;   // longest prefix/suffix of zeros
    ll zeroSub;       // # of zero‐only subarrays in this segment

    Node(): len(0), pre0(0), suf0(0), zeroSub(0) {}
    // build a leaf: v==0 ? zero : one
    Node(int v) {
        len = 1;
        if (v == 0) {
            pre0 = suf0 = 1;
            zeroSub = 1;
        } else {
            pre0 = suf0 = 0;
            zeroSub = 0;
        }
    }
};

struct SegTree {
    int n;
    vector<Node> st;
    SegTree(int _n): n(_n), st(4*n) {}

    Node mrg(const Node &L, const Node &R) {
        if (L.len == 0) return R;
        if (R.len == 0) return L;
        Node res;
        res.len = L.len + R.len;
        // prefix zeros
        res.pre0 = (L.pre0 == L.len ? L.len + R.pre0 : L.pre0);
        // suffix zeros
        res.suf0 = (R.suf0 == R.len ? R.len + L.suf0 : R.suf0);
        // zero-only subarrays: left + right + those crossing boundary
        res.zeroSub = (L.zeroSub + R.zeroSub
                     + (ll)L.suf0 * R.pre0) % MOD;
        return res;
    }

    void update(int idx, int v, int p=1, int l=0, int r=-1) {
        if (r == -1) r = n-1;
        if (l == r) {
            st[p] = Node(v);
            return;
        }
        int m = (l + r) >> 1;
        if (idx <= m) update(idx, v, p<<1, l, m);
        else          update(idx, v, p<<1|1, m+1, r);
        st[p] = mrg(st[p<<1], st[p<<1|1]);
    }

    // returns Node for [i..j]
    Node query(int i, int j, int p=1, int l=0, int r=-1) {
        if (r == -1) r = n-1;
        if (j < l || r < i)   return Node();   // empty
        if (i <= l && r <= j) return st[p];
        int m = (l + r) >> 1;
        return mrg(
            query(i,j,p<<1,   l,  m),
            query(i,j,p<<1|1, m+1, r)
        );
    }
};

int32_t main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<SegTree> segs;
    for (int b = 0; b < 10; b++)
        segs.emplace_back(n);

    // read initial array and build per‐bit trees
    for (int i = 0, x; i < n; i++){
        cin >> x;
        for (int b = 0; b < 10; b++){
            segs[b].update(i, (x>>b)&1 ? 1 : 0);
        }
    }

    // precompute 2^b mod
    int pow2[10];
    pow2[0] = 1;
    for (int b = 1; b < 10; b++)
        pow2[b] = (pow2[b-1]*2) % MOD;

    while (q--) {
        int type;
        cin >> type;
        if (type == 1) {
            int p, x;
            cin >> p >> x;
            --p;
            for (int b = 0; b < 10; b++)
                segs[b].update(p, (x>>b)&1 ? 1 : 0);
        } else {
            int L, R;
            cin >> L >> R;
            --L; --R;
            ll answer = 0;
            for (int b = 0; b < 10; b++) {
                Node v = segs[b].query(L, R);
                // total subarrays in v.len
                ll total = (ll)v.len*(v.len+1)/2 % MOD;
                // those with at least one 1 in bit b:
                ll cntWith = (total - v.zeroSub + MOD) % MOD;
                answer = (answer + cntWith * pow2[b]) % MOD;
            }
            cout << answer << "\n";
        }
    }
    return 0;
}
