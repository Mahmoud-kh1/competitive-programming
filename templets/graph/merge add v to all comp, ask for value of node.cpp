#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace std;
using namespace __gnu_pbds;

// Multiset with less_equal
typedef tree<int, null_type, less_equal<int>, rb_tree_tag, tree_order_statistics_node_update> pbds;

void M() {
    ios::sync_with_stdio(0);
    cin.tie(NULL);
    cout.tie(0);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
}
#define all(a) a.begin(), a.end()

struct dsu{
    vector<int>leader;
    vector<int>sums, sz, delta;
    void init(int n){
        leader.resize(n + 1);
        sums.resize(n + 3);
        sz.resize(n + 3);
        delta.resize(n + 2);
        for (int i = 1; i <= n; i++){
            leader[i] = i; sz[i] = 1;
        }
    }

    int  find_leader(int u){
        if (leader[u] == u)  return u;
        int p = find_leader(leader[u]);
        if(leader[u] != p)
        sums[u]+= sums[leader[u]];
        return  leader[u] = p;
    }

    void merge(int u, int v) {
        u = find_leader(u);
         v = find_leader(v);
          if(u == v) return;
            if (sz[u] > sz[v]) {
                swap(u, v);
            }
            sz[v] += sz[u];
            sums[u]-= sums[v];
            leader[u] = v;

    }

    void update(int x, short v){
        sums[find_leader(x)] += v;
    }
    int find(int x){
        if (find_leader(x) == x) return  sums[x];
        return sums[x] + sums[find_leader(x)];
    }

};
signed main() {
    int t = 1;
//     cin >> t;
    Z: while (t--) {
      int  n, m; cin >> n >> m;
      dsu d; d.init(n);
      int x, y; int v; string s;
      while(m--){

           cin >> s;
          if (s == "add"){
               cin >> x >> v;
              d.update(x, v);
          }
          else if (s == "join"){
               cin >> x >> y;
              d.merge(x, y);
          }
          else {
               cin >> x;
//               cout << sums[1] << ' ' << sums[3] << ' ' << ans[1] << endl;
              cout << d.find(x) << endl;
          }
      }


}

    return 0;
}
