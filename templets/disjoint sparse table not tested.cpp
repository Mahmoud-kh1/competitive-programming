#include <bits/stdc++.h>
using namespace std;

template<typename T, typename F>
struct DisjointSparseTable {
    int n, LOG;
    vector<vector<T>> st;
    F func;

    // v: input array, f: associative function (must be commutative only at query time, no idempotent required)
    DisjointSparseTable(const vector<T>& v, F f): func(f) {
        n = v.size();
        LOG = 32 - __builtin_clz(n);
        st.assign(LOG, vector<T>(n));
        // level 0 is just the original array
        st[0] = v;
        // build for levels 1..LOG-1
        for(int k = 1; k < LOG; k++) {
            int len = 1 << k;
            // process blocks of size len*2, centered at mid
            for(int start = 0; start < n; start += len << 1) {
                int mid = min(start + len, n);
                int end = min(start + (len << 1), n);
                // build left part: [start..mid)
                st[k][mid - 1] = v[mid - 1];
                for(int i = mid - 2; i >= start; --i) {
                    st[k][i] = func(v[i], st[k][i + 1]);
                }
                // build right part: [mid..end)
                if (mid < n) {
                    st[k][mid] = v[mid];
                    for(int i = mid + 1; i < end; ++i) {
                        st[k][i] = func(st[k][i - 1], v[i]);
                    }
                }
            }
        }
    }

    // query on [l..r], inclusive
    T query(int l, int r) const {
        if (l == r) return st[0][l];
        int k = 31 - __builtin_clz(l ^ r);
        return func(st[k + 1][l], st[k + 1][r]);
    }
};

// Example usage:
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<long long> a(n);
    for(int i = 0; i < n; i++) cin >> a[i];

    // Build a DST for range minimum
    DisjointSparseTable<long long, function<long long(long long,long long)>> dst(
        a,
        [](long long x, long long y){ return min(x, y); }
    );

    while(q--) {
        int l, r;
        cin >> l >> r;
        --l; --r;
        cout << dst.query(l, r) << "\n";
    }
    return 0;
}
