#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int INF = 1e9+5;
// change a[i], 
// check if l,r contain no dublicates
struct SegTree {
    int n;
    vector<int> st;
    SegTree(int _n): n(_n), st(4*n+4, INF) {}

    void update(int p, int val, int node=1, int l=1, int r=-1) {
        if (r==-1) r = n;
        if (l == r) {
            st[node] = val;
            return;
        }
        int m = (l + r) >> 1;
        if (p <= m) update(p, val, node<<1, l, m);
        else        update(p, val, node<<1|1, m+1, r);
        st[node] = min(st[node<<1], st[node<<1|1]);
    }

    int range_min(int L, int R, int node=1, int l=1, int r=-1) {
        if (r==-1) r = n;
        if (R < l || r < L) return INF;
        if (L <= l && r <= R) return st[node];
        int m = (l + r) >> 1;
        return min(
            range_min(L,R,node<<1,   l, m),
            range_min(L,R,node<<1|1,m+1,r)
        );
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<int> A(n+1);
    for (int i = 1; i <= n; i++) {
        cin >> A[i];
    }

    // pos[v] = set of positions where A[pos] == v, with sentinels 0,n+1
    unordered_map<int, set<int>> pos;
    pos.reserve(n*2);

    for (int i = 1; i <= n; i++) {
        pos[A[i]];  // ensure key exists
    }
    for (auto &kv : pos) {
        kv.second.insert(0);
        kv.second.insert(n+1);
    }
    for (int i = 1; i <= n; i++) {
        pos[A[i]].insert(i);
    }

    // compute next[i] = next occurrence of A[i], or n+1
    vector<int> nxt(n+1, n+1);
    for (int i = 1; i <= n; i++) {
        auto &s = pos[A[i]];
        auto it = s.upper_bound(i);
        nxt[i] = *it;  // either >i or n+1
    }

    // build segment-tree over nxt[1..n]
    SegTree seg(n);
    for (int i = 1; i <= n; i++) {
        seg.update(i, nxt[i]);
    }

    while (q--) {
        int type;
        cin >> type;
        if (type == 1) {
            int k, u;
            cin >> k >> u;
            int old = A[k];
            if (old == u) {
                // no change
                A[k] = u;
                continue;
            }
            // 1) remove k from old
            {
                auto &s = pos[old];
                auto it = s.find(k);
                int pr = *prev(it);
                int nx = *next(it);
                // erase k
                s.erase(it);
                // fix next[pr] = nx
                nxt[pr] = nx;
                seg.update(pr, nx);
                // mark k dead
                nxt[k] = n+1;
                seg.update(k, n+1);
            }
            // 2) insert k into new value u
            {
                auto &s = pos[u];
                if (s.empty()) {
                    s.insert(0);
                    s.insert(n+1);
                }
                auto it = s.insert(k).first;
                int pr = *prev(it);
                int nx = *next(it);
                // fix next[pr] = k
                nxt[pr] = k;
                seg.update(pr, k);
                // set next[k] = nx
                nxt[k] = nx;
                seg.update(k, nx);
            }
            A[k] = u;
        }
        else {
            int a, b;
            cin >> a >> b;
            int m = seg.range_min(a, b);
            // if no next inside [a,b], then distinct
            cout << (m > b ? "YES\n" : "NO\n");
        }
    }

    return 0;
}
