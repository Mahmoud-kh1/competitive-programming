#include <iostream>
#include <bits/stdc++.h>
using namespace std;
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct WaveletTree {
    int lo, hi;
    WaveletTree *lC, *rC;
    vector<int> b;      // b[i] = # of elements among first i that go left
    vector<ll> csum;    // csum[i] = sum of first i elements

    // build on [from,to) with values in [x..y]
    WaveletTree(int *from, int *to, int x, int y)
            : lo(x), hi(y), lC(nullptr), rC(nullptr),
              b(1,0), csum(1,0)
    {
        if (from >= to || lo == hi) return;
        int mid = (lo + hi) >> 1;
        for (auto it = from; it != to; ++it) {
            b.push_back(b.back() + ((*it) <= mid));
            csum.push_back(csum.back() + *it);
        }
        auto pivot = stable_partition(from, to,
                                      [&](int v){ return v <= mid; });
        lC = new WaveletTree(from, pivot, lo, mid);
        rC = new WaveletTree(pivot, to, mid+1, hi);
    }

    // internally convert 0‑based [l..r] to 1‑based [l1..r1]
    inline pair<int,int> lr1(int l, int r) {
        return {l+1, r+1};
    }

    // kth smallest in A[l..r] (0‑based)
    int kthSmall(int l, int r, int k) {
        auto [l1, r1] = lr1(l, r);
        if (l1 > r1) return 0;
        if (lo == hi) return lo;
        int inLeftL = b[l1-1], inLeftR = b[r1];
        int cntLeft = inLeftR - inLeftL;
        if (k <= cntLeft)
            return lC->kthSmall(inLeftL, inLeftR-1, k);
        else {
            int skipL = (l1-1) - inLeftL;
            int skipR =  r1    - inLeftR - 1;
            return rC->kthSmall(skipL, skipR, k - cntLeft);
        }
    }

    // kth largest = (len-k+1)th smallest
    int kthLarge(int l, int r, int k) {
        int len = r - l + 1;
        return kthSmall(l, r, len - k + 1);
    }

    // count ≤ x in A[l..r]
    int LTE(int l, int r, int x) {
        auto [l1,r1] = lr1(l, r);
        if (l1 > r1 || x < lo) return 0;
        if (hi <= x) return r1 - l1 + 1;
        int inLeftL = b[l1-1], inLeftR = b[r1];
        return lC->LTE(inLeftL, inLeftR-1, x)
               + rC->LTE(l1-1-inLeftL, r1-inLeftR-1, x);
    }

    // count ≥ x
    int GTE(int l, int r, int x) {
        return (r-l+1) - LTE(l, r, x-1);
    }

    // count == x
    int EQ(int l, int r, int x) {
        return LTE(l, r, x) - LTE(l, r, x-1);
    }

    // sum of ≤ x in A[l..r]
    ll sumLTE(int l, int r, int x) {
        auto [l1,r1] = lr1(l, r);
        if (l1 > r1 || x < lo) return 0;
        if (hi <= x)
            return csum[r1] - csum[l1-1];
        int inLeftL = b[l1-1], inLeftR = b[r1];
        ll leftSum  = lC->sumLTE(inLeftL, inLeftR-1, x);
        ll rightSum = rC->sumLTE(l1-1-inLeftL, r1-inLeftR-1, x);
        return leftSum + rightSum;
    }

    // sum of ≥ x
    ll sumGTE(int l, int r, int x) {
        auto [l1,r1] = lr1(l, r);
        ll total = csum[r1] - csum[l1-1];
        return total - sumLTE(l, r, x-1);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q; cin >> n >> q;
    vector<int>a(n);
    for(int i = 0; i < n; i ++) cin >> a[i];
    int mn = *min_element(a.begin(), a.end());
    int mx = *max_element(a.begin(), a.end());
    WaveletTree wt(&a[0], &a[0]+n, mn, mx);
    while(q--){
        int l,r; cin >> l >> r;
        int k; cin >> k;
        l--,r--;
        cout << wt.kthSmall(l, r, k) << endl;
    }

    return 0;
}
