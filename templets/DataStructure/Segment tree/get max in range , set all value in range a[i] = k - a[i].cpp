#include <bits/stdc++.h>
using namespace std;
#define int long long

const int mod = 1e9 + 7;

int power(int a, int b) {
    int res = 1;
    while (b) {
        if (b & 1) res = res * a % mod;
        a = a * a % mod;
        b >>= 1;
    }
    return res;
}

int inverse(int a) { return power(a, mod - 2); }
struct Node{
    int mn = 1e18, mx = -1e18;
    Node operator+ (const Node &other){
        Node res;
        res.mn =min(mn, other.mn);
        res.mx =max(mx, other.mx);
        return res;
    }
};
struct lzy{
    int sum = 0, op = 0;
};
struct Seg{
    vector<Node>tree;
    vector<lzy>lazy;
    int sz = 0, buffer = 0;
    void build(vector<int>&a){
        int old = a.size();
        if(old & (old - 1)) sz = (1 << (__lg(old) + 1));
        else sz = old;
        tree.resize(sz << 1, {}); lazy.resize(sz << 1, {});
        for (int i = 0; i < old; i++){
            tree[i + sz] = {a[i], a[i]};
        }
        for (int node = sz - 1; node >=1 ; node -- ) tree[node] = tree[node << 1]+tree[node << 1 | 1];
    }

    void propagate(int l , int r , int k){
        if (lazy[k].op == 0) return;
        bool odd = false;
        if (lazy[k].op % 2 == 1){
            odd = true;
            tree[k].mn*= -1;
            tree[k].mx*= -1;
            swap(tree[k].mn, tree[k].mx);
        }
        tree[k].mn+= lazy[k].sum;
        tree[k].mx+= lazy[k].sum;
        if (l != r){
            if (odd){
                lazy[k << 1].sum = lazy[k].sum - lazy[k << 1].sum;
                lazy[k << 1 | 1].sum = lazy[k].sum - lazy[k << 1 | 1].sum;
            }
            else {
                lazy[k << 1].sum = lazy[k].sum + lazy[k << 1].sum;
                lazy[k << 1 | 1].sum = lazy[k].sum + lazy[k << 1 | 1].sum;
            }
            lazy[k << 1| 1].op+= lazy[k].op;
            lazy[k << 1].op += lazy[k].op;
        }
        lazy[k] = {0, 0};
    }
    void update(int lx, int rx, int l , int r , int k, int val){
        propagate(l , r, k);
        if (l > rx || r < lx) return;
        if (l >= lx  && r <= rx){
            lazy[k] = {val, 1};
            propagate(l, r, k);
            return;
        }
        int mid = (l + r) / 2;
        update(lx, rx , l , mid, k << 1, val);
        update(lx, rx , mid + 1, r, k << 1 | 1, val);
        tree[k] = tree[k << 1] + tree[k << 1 | 1];
    }
    Node query(int lx ,int rx , int l, int r ,int k){
        propagate(l, r , k);
        if (l > rx || r < lx){
            Node dum{};
            return dum;
        }
        if (lx <= l && rx >= r){
            return tree[k];
        }
        int mid = (l + r) / 2;
        return query(lx, rx , l , mid, k << 1 )+
               query(lx, rx , mid + 1, r, k << 1 | 1);

    }
    Node query(int l, int r){
        return query(l, r ,0 , sz - 1, 1);
    }
    void update(int l, int r,int val){
        update(l, r , 0, sz - 1, 1, val);
    }
};
signed main() {
    freopen("rmq.in" , "r" , stdin);
    ios::sync_with_stdio(0);
    cin.tie(NULL);
    cout.tie(0);
    int t = 1; cin >> t;
    while(t--) {
        int n, q; cin >> n >> q;
        vector<int>a(n);
        for(auto &I : a) cin >> I;
        Seg seg;
        seg.build(a);
        while(q--){
            int ty; cin >> ty;
            int l,r; cin >> l >> r;
            l--,r--;
            if (ty == 1){
                int k = seg.query(l, r).mx;
                seg.update(l, r, k);
            }
            else {
                cout << seg.query(l,r).mx << endl;;
            }
        }


    }

    return 0;
}
