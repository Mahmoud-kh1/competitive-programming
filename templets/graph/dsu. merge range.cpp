#include <bits/stdc++.h>
using namespace std;
#define int long long
struct  dsu{
    vector<int>leader, sz, nxt;
    void init(int n){
        leader.resize(n + 1);
        sz.resize(n + 1);
        nxt.resize(n + 1);
        for(int i = 1; i <= n; i++) nxt[i] = i + 1;
        for(int i = 1; i <= n; i++) leader[i] = i, sz[i] = 1;
    }

    int find_leader(int u){
        if(u == leader[u]) return u;
        return leader[u] = find_leader(leader[u]);
    }
    int mergeRange(int l, int r){
        if(nxt[l] > r){
            return nxt[l];
        }
        merge(l, nxt[l]);
       return nxt[l] =  mergeRange(nxt[l], r);
    }
    void merge(int u, int v){
        u = find_leader(u);
        v = find_leader(v);
        if (u == v) return;
        if (sz[u] < sz[v]){
            swap(u, v);
        }
        sz[u]+= sz[v];
        leader[v] = u;
    }
    bool same(int u, int v){
        return (find_leader(u) == find_leader(v));
    }
};
signed main() {
    int t = 1;
//     cin >> t;
    Z: while (t--) {
         int n, m; cin >> n >> m;
         dsu d; d.init(n);
         while(m--){
             int ty; cin >> ty;
             int x, y; cin >> x >> y;
             if (ty == 1){
                 d.merge(x, y);
             }
             else if (ty == 2){
                 d.mergeRange(x, y);
             }
             else {
                 cout << (d.same(x, y) ? "YES" : "NO") << endl;
             }
         }

   }

    return 0;
}
