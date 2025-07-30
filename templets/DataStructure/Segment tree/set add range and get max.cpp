// need to revison 
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define int long long
constexpr ll INF = (ll)1e18;
 
struct Node {
    ll mx;
    ll lazy;
    int op;
 
    Node() : mx(-1e18), lazy(INF), op(-1) {}
    Node(ll _mx, ll _lazy, int _op) : mx(_mx), lazy(_lazy), op(_op) {}
};
 
struct Seg {
    int sz;
    vector<Node> tree;
 
    void build(int n) {
        sz = 1;
        while (sz < n) sz <<= 1;
        tree.assign(sz << 1, Node());
    }
 
    Seg(int n = 0) { if (n > 0) build(n); }
    void init(int n) { build(n); }
 
    void prop(int k, int l, int r) {
        if (tree[k].lazy == INF) return;
        if (l != r) {
            int lc = k << 1, rc = k << 1 | 1;
            if (tree[k].op == 1) {
                tree[lc] = Node(0, tree[k].lazy, 1);
                tree[rc] = Node(0, tree[k].lazy, 1);
            } else {
                for (int c : {lc, rc}) {
                    if (tree[c].lazy == INF) {
                        tree[c].lazy = tree[k].lazy;
                        tree[c].op = 0;
                    } else {
                        if (tree[c].op == 1) {
                            tree[c].lazy += tree[k].lazy;
                        } else {
                            tree[c].lazy += tree[k].lazy;
                        }
                    }
                }
            }
        }
 
        if (tree[k].op == 1) {
            tree[k].mx = tree[k].lazy;
        } else {
            tree[k].mx += tree[k].lazy;
        }
 
        tree[k].lazy = INF;
        tree[k].op = -1;
    }
 
    void pointUpdate(int k, int lx, int rx, int idx, ll val) {
        prop(k, lx, rx);
        if (idx < lx || idx > rx) return;
        if (lx == rx) {
            ll nw = max(tree[k].mx, val);
            tree[k] = Node(nw, INF, -1);
            return;
        }
        int mid = (lx + rx) >> 1;
        pointUpdate(k<<1, lx, mid, idx, val);
        pointUpdate(k<<1|1, mid+1, rx, idx, val);
        prop(k<<1, lx, mid);
        prop(k<<1|1, mid+1, rx);
        tree[k].mx = max(tree[k<<1].mx, tree[k<<1|1].mx);
    }
 
    void update(int k, int lx, int rx, int l, int r, ll val, int op) {
        prop(k, lx, rx);
        if (lx > r || rx < l) return;
        if (lx >= l && rx <= r) {
            tree[k].lazy = val;
            tree[k].op = op;
            prop(k, lx, rx);
            return;
        }
        int mid = (lx + rx) >> 1;
        update(k<<1, lx, mid, l, r, val, op);
        update(k<<1|1, mid+1, rx, l, r, val, op);
        prop(k<<1, lx, mid);
        prop(k<<1|1, mid+1, rx);
        tree[k].mx = max(tree[k<<1].mx, tree[k<<1|1].mx);
    }
 
    ll query(int k, int lx, int rx, int l, int r) {
        prop(k, lx, rx);
        if (lx > r || rx < l) return -INF;
        if (lx >= l && rx <= r) return tree[k].mx;
        int mid = (lx + rx) >> 1;
        ll left_res = query(k<<1, lx, mid, l, r);
        ll right_res = query(k<<1|1, mid+1, rx, l, r);
        return max(left_res, right_res);
    }
 
    void pointUpdate(int idx, ll val) {
        pointUpdate(1, 0, sz-1, idx, val);
    }
 
    void updateRange(int l, int r, ll val, bool assign) {
        update(1, 0, sz-1, l, r, val, assign ? 1 : 0);
    }
 
    ll queryMax(int l, int r) {
        return query(1, 0, sz-1, l, r);
    }
};
