#include <bits/stdc++.h>

using namespace std;

void fast() {
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
}

#define all(v)  v.begin(),v.end()
#define mem(arr, value) memset(arr, value, sizeof(arr))
#define mid(l, r)   (l+(r-l)/2)
#define endl "\n"
#define int long long

const int MOD = 1e9 + 7, N = 2e5 + 5;
const long long oo = LONG_LONG_MAX;
struct seg{
    vector<vector<int>>tree;
    int sz;

    void build(vector<int>&a){
        int old = a.size();
        if((old & (old - 1))) sz = (1 << (__lg(old) + 1) );
        else sz = old;
        tree.resize(sz << 1, {});
        for (int g = 0; g < old; g++){
            tree[g + sz] = {a[g]};
        }

        for (int g = sz - 1; g >= 1; g--) tree[g] = merge (tree[g << 1] , tree[g << 1 | 1]);
    }
    vector<int> merge(vector<int>&a, vector<int>&b){
        vector<int>res{};
        int L = 0, R = 0;
        while(L < a.size() && R < b.size()){
            if(a[L] < b[R])res.push_back(a[L++]);
            else res.push_back(b[R++]);
        }
        while(L < a.size()) res.push_back(a[L++]);
        while(R < b.size()) res.push_back(b[R++]);
        return res;
    }
    int query(int k, int lx, int rx, int l, int r,int val){
        if(lx > r || l > rx) return 0;
        if(lx >= l && rx <= r){
            return upper_bound(tree[k].begin(), tree[k].end(), val) - tree[k].begin();
        }
        int mid=  (lx + rx) / 2;
        return query(k << 1 , lx, mid, l, r, val) + query(k << 1 | 1 , mid + 1, rx, l, r, val);
    }
    int query(int l, int r, int val){
        return query(1, 0, sz - 1, l, r, val);
    }

};
void solve() {
   int n, q; cin >> n >> q;
   vector<int>a(n);
   for(int i = 0; i < n; i++) cin >> a[i];
   seg mer; mer.build(a);
   while(q--){
       int l,r, x, y;
       cin >> l >> r >> x >> y;
       int smL = mer.query(l - 1, r - 1, y);
       int smR = mer.query(l- 1, r - 1, x - 1);
       cout << smL - smR << endl;
   }
}

int32_t main() {
    fast();
    int tt = 1;
    //cin >> tt;
    for (int test = 1; test <= tt; test++) {
        solve();
    }
    return 0;
}
