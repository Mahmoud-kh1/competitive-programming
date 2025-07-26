#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int MOD = 4001;

struct Node {
    int len;        // segment length
    int pre1, suf1; // longest prefix/suffix of 1’s
    ll oneSub;      // # of all‑1 subarrays in this segment

    Node(): len(0), pre1(0), suf1(0), oneSub(0) {}

    // Leaf constructor from bit v (0 or 1)
    Node(int v) {
        len    = 1;
        pre1   = suf1   = v;
        oneSub = v;     // if v==1 there is exactly one valid subarray [i..i]
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
        res.len  = L.len + R.len;
        // prefix of 1s
        res.pre1 = (L.pre1 == L.len ? L.len + R.pre1 : L.pre1);
        // suffix of 1s
        res.suf1 = (R.suf1 == R.len ? R.len + L.suf1 : R.suf1);
        // one‐only subarrays: left + right + cross‐boundary
        res.oneSub = (L.oneSub
                    + R.oneSub
                    + (ll)L.suf1 * R.pre1) % MOD;
        return res;
    }

    void update(int idx, int v, int p=1, int l=0, int r=-1) {
        if (r == -1) r = n-1;
        if (l == r) {
            st[p] = Node(v);
            return;
        }
        int m = (l + r) >> 1;
        if (idx <= m) update(idx, v, p<<1,   l, m);
        else          update(idx, v, p<<1|1, m+1, r);
        st[p] = mrg(st[p<<1], st[p<<1|1]);
    }

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

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<SegTree> segs;
    segs.reserve(10);
    for (int b = 0; b < 10; b++)
        segs.emplace_back(n);

    vector<int> A(n);
    for (int i = 0, x; i < n; i++){
        cin >> x;
        A[i] = x;
        for (int b = 0; b < 10; b++){
            int bit = (x >> b) & 1;
            segs[b].update(i, bit);
        }
    }

    // precompute 2^b mod
    int pow2[10];
    pow2[0] = 1;
    for (int b = 1; b < 10; b++)
        pow2[b] = (pow2[b-1] * 2) % MOD;

    while (q--) {
        int type;
        cin >> type;
        if (type == 1) {
            int p, x;
            cin >> p >> x;
            --p;
            A[p] = x;
            for (int b = 0; b < 10; b++){
                int bit = (x >> b) & 1;
                segs[b].update(p, bit);
            }
        } else {
            int L, R;
            cin >> L >> R;
            --L; --R;
            ll answer = 0;
            for (int b = 0; b < 10; b++) {
                Node v = segs[b].query(L, R);
                // v.oneSub = #subarrays whose AND at bit b is 1
                answer = (answer + v.oneSub * pow2[b]) % MOD;
            }
            cout << answer << "\n";
        }
    }
    return 0;
}
