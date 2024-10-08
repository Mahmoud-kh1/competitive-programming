#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace std;
using namespace __gnu_pbds;

// Multiset with less_equal
typedef tree<int, null_type, less_equal<int>, rb_tree_tag, tree_order_statistics_node_update> pbds;



// MST from undirectd graph is a tree with all nodes in graph and the sum of wights is minimum 
// the algo used here is kruksal ... greedy algo 
// the tree has no cycle then we must from every cycle remove one edge has the greater wight how ... we sort edges by wight and if cur (u, v) has not been connect before (means u is part of componot and v part of another)
// just take the wight and merge the two com .. and if they in same do nothing .... how can this remove the greates wight in every cycle .... in every cycle the wight is sorted this means the last edges of this cycle come last
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
    vector<int>leader, siz;
    void init(int n){
        leader.resize(n + 4), siz.resize(n + 4, 0);
        for (int i = 1; i <= n; i++){
            leader[i] = i; siz[i] = 1;
        }
    }
    // find leader
    
    // complexity o(alpha(n)) alpha(1e9) almost = 4
    int  find_leader(int u){
        if (leader[u] == u)  return u;
        // path compression 
        // if we didn't make leader[u] = find_leader(....); the time will be log
        // because if I come from another leader the componont will be >= same my size 
        // and when I search again I want >= double my original size because the the last one >= 2* my size 
        return leader[u] = find_leader(leader[u]);
    }
    // merge
    void merge(int u, int v){
        int l1 = find_leader(u);
        int l2 = find_leader(v);
        if (l2 == l1) {
            return;
        }
        // union by size()  
        // you can union by anything the time will depend how you merge the time for find root is constant
        if (siz[l1] > siz[l2]) swap(l1 ,l2);
        leader[l1] = l2;
        siz[l2]+= siz[l1];

    }

};
signed main() {
    M();
    int t = 1;
//     cin >> t;
    Z: while (t--) {
        int n,m; cin >> n >> m;
        vector<array<int,3>>edges;
        for (int i = 0;i < m; i++){
            int u,v, w; cin >> u >> v >> w;
            edges.push_back({w, u, v});
        }
        sort(all(edges));
        long long mn = 0;
        dsu d;
        d.init(n);
        for (auto [w, u , v] : edges){
            if (d.find_leader(u) == d.find_leader(v)) continue;
            mn+=w;
            d.merge(u, v);
        }
        cout << mn;




}

    return 0;
}
