#include <bits/stdc++.h>
using namespace std;
const int mod = 998244353;
#define int long long
#define all(a) a.begin(), a.end()
signed main() {
    int T = 1;
//    cin >> T;
    Z: while (T--) {
    int n , q; cin >> n >> q;
    int a[n];
    int sq = sqrt(n) + 1;
    int blk[sq]{};
    for(auto &i : blk) i = 1e18;
    for(int i = 0; i < n; i++){
        cin >> a[i];
        blk[i / sq]  = min(blk[i / sq] , a[i]);
    }



    auto update = [&](int idx, int v){
        a[idx] = v;
        blk[idx / sq] = 1e18;
        for(int j = (idx / sq) * sq; j < min(n, (idx / sq + 1) * sq); j++) blk[idx / sq] = min(blk[idx / sq] , a[j]);
    };
    auto query = [&](int l ,int r){
        int ans = 1e18;
        if (l / sq == r / sq){
            for(int j = l ; j <= r; j++) ans =min(a[j], ans);
            return ans;
        }
        else {
            int Lq = l / sq + 1;
            Lq *= sq;
            for(int j = l; j < Lq; j++)  ans =min(a[j], ans);
            int rq = r / sq;
            rq *= sq;
            for(int j = rq; j <= r; j++) ans =min(a[j], ans);

            for(int j = l / sq + 1; j < r / sq; j++) ans =min(blk[j], ans);
            return ans ;
        }
    };
    while(q--){
        int ty; cin >> ty;
        if (ty == 1){
            int k , u; cin >> k >> u;
            k--;
            update(k,u);
        }
        else {
           int l ,r; cin >> l >> r;
           l--, r--;
           cout << query(l ,r)  << endl;
        }
    }


}
    return 0;

}
