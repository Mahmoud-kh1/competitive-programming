#include <bits/stdc++.h>
using namespace std;
#define int long long
struct dsu{
    vector<int>leader, sz;
    int paths = 0;
    void init(int n){
        leader.resize(n + 4), sz.resize(n + 4, 0);
        for (int i = 1; i <= n; i++){
            leader[i] = i; sz[i] = 1;
        }
    }
    int  find_leader(int u){
        if (leader[u] == u)  return u;
        return leader[u] = find_leader(leader[u]);
    }
    void merge(int u, int v){
         u = find_leader(u);
         v = find_leader(v);
        if (v == u) {
            return;
        }
        if (sz[u] > sz[v]) swap(u ,v);
        paths += sz[u]  * sz[v];
        leader[u] = v;
        sz[v]+= sz[u];

    }
};

int32_t main() {
     int n, q; cin >> n >> q;
     vector<array<int,3>>edges;
     for(int i = 0; i < n  - 1; i++){
         int u,v, w; cin >> u >> v >> w;
         edges.push_back({w, u , v});
     }
     vector<array<int,2>>query;
     for(int i = 0; i < q; i++){
         int x; cin >> x;
         query.push_back({x, i});
     }
     sort(query.begin(), query.end());
     sort(edges.begin(), edges.end());
     int idx = 0;
     int ans[q];
     dsu d; d.init(n);
     for(auto [x, id] : query){
         while(idx < edges.size() && edges[idx][0] <= x){
             d.merge(edges[idx][1], edges[idx][2]);
             idx++;
         }
         ans[id] = d.paths;
     }
     for(int i = 0; i < q; i++) cout << ans[i] << ' ';


}
