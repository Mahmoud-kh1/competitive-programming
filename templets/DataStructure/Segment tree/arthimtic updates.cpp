#include <bits/stdc++.h>
using namespace std;
#define int long long
const int mod = 998244353;
int mul(int a, int b){
    return ((a % mod) * (b % mod)) % mod;
}
int add (int a, int b){
    return (a + b)  % mod;
}
int n;
struct Seg{
    vector<long long>tree;
    vector<pair<int,int>>lazy;
    int sz = 0, buffer = 0;
    long long merge(long long a , long long b){
       return a + b;
    }
    void build(vector<int>&a){
        int old = a.size();
        if(old & (old - 1)) sz = (1 << (__lg(old) + 1));
        else sz = old;
        tree.resize(sz << 1, {}); lazy.resize(sz << 1, {0, 0});
        for (int i = 0; i < old; i++){
            tree[i + sz] = a[i];
        }
        for (int node = sz - 1; node >=1 ; node -- ) tree[node] = merge(tree[node << 1] , tree[node << 1 | 1]);
    }
    void propagate(int l , int r , int k){
         if (lazy[k].first == 0) return;
         if (l != r){
             lazy[k << 1].first += lazy[k].first;
             lazy[k << 1].second+= lazy[k].second;
             lazy[k << 1 | 1].first += lazy[k].first;
             lazy[k << 1 | 1].second+= lazy[k].second;
         }
         l++, r++;
         int indexSum = (r - l + 1) * (r + l) / 2;
         int cntStart = lazy[k].second;
         int sumStart = lazy[k].first;
         tree[k]+= (indexSum * cntStart - sumStart * (r - l + 1) + (r - l + 1) * cntStart);
         lazy[k] = {0, 0};
    }
    void update(int lx, int rx, int l , int r , int k){
        propagate(l, r , k);
        if (l > rx || r < lx) return;
        if  (lx <= l && rx >= r){
             lazy[k]= {lx + 1 , 1};
            propagate(l, r , k);
            return;
        }
        int mid = (l + r) / 2;
        update(lx, rx , l , mid, k << 1);
        update(lx, rx , mid + 1, r, k << 1 | 1);
        tree[k] = merge(tree[k << 1], tree[k << 1 | 1]);
    }
    long long query(int lx ,int rx , int l, int r ,int k){
        propagate(l, r , k);
        if (l > rx || r < lx){
            return 0;
        }
        if (lx <= l && rx >= r){
            return tree[k];
        }
        int mid = (l + r) / 2;
        return merge(query(lx, rx , l , mid, k << 1 ),
               query(lx, rx , mid + 1, r, k << 1 | 1));
 
    }
    long long query(int l, int r){
        return query(l, r ,0 , sz - 1, 1);
    }
    void update(int l, int r){
        update(l, r , 0, sz - 1, 1);
    }
};
signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t = 1;
//    cin >> t;
    while (t--) {
        int  q; cin >> n >> q;
        vector<int>a(n);
        for(int i = 0; i < n; i++) cin >> a[i];
        Seg seg; seg.build(a);
        while(q--){
            int ty, l, r; cin >> ty >> l >> r;
            l--,r--;
            if (ty == 1){
                seg.update(l, r);
            }
            else cout << seg.query(l,r) << endl;
        }
    }
    return 0;
}
