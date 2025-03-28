#include "bits/stdc++.h"
using namespace std;
const int N = 2e5 + 10;
vector<int>adj[N];
struct LCA{
    int LG, n;
    vector<int>lvl, pre;
    vector<vector<int>>anc;

    void dfs(int node, int par){
        for(auto v : adj[node]){
            if (v == par) continue;
            anc[0][v] = node;
            lvl[v] = lvl[node]  + 1;
            dfs(v, node);
        }
    }
    int dist(int u,int v){
        return pre[u] + pre[v] - 2* pre[lca(u, v)];
    }

    void init(int nn){
        n = nn;
        LG = __lg(n) + 4;
        lvl.resize(n + 1);
        pre.resize(n + 1);
        anc.resize(LG, vector<int>(n + 1));
        dfs(1, 0);
        for(int mask = 1; mask < LG; mask ++){
            for(int u = 1; u <= n; u++){
                anc[mask][u] = anc[mask - 1][anc[mask - 1][u]];
            }
        }
    }

    int kth(int u, int k){
        for(int  i = 0; k; i++, k >>= 1){
            if (k & 1) u = anc[i][u];
        }
        return u ? u : -1;
    }
    int lca(int u, int v){
        if (lvl[u] > lvl[v]) swap(u, v);
        int dif = lvl[v] - lvl[u];
        v = kth(v, dif);
        if (u == v) return u;
        for(int i = LG - 1; i >= 0; i--){
            int au = anc[i][u], av = anc[i][v];
            if (au != av) {
                u = au;
                v = av;
            }
        }
        return anc[0][u];
    }
    int dist2(int u, int v, int lc){
        return lvl[u] + lvl[v] - 2*lvl[lc];
    }
    int get(int u, int v, int k){
        if(k == 1) return u;
        int lc = lca(u, v);
        int ulc = lvl[u] - lvl[lc] + 1;
        if (k <= ulc) {
            return kth(u, k - 1);
        }
        else return kth(v, dist2(u, v, lc) - k);
    }

    bool inpath(int a, int b, int c) {
        return dist2(a, b, lca(a,b)) == dist2(a, c,lca(a, c)) + dist2(c, b,lca(c, b));
    }

    int commonNodes(int u, int v, int x, int y) {
        vector<int>cand;
        int a[] = {u, v, x, y};
        for(int i = 0; i < 4; i++){
            for(int j = i; j < 4; j++){
                int lc = lca(a[i], a[j]);
                if (inpath(x, y, lc) && inpath(u, v, lc)) {
                    cand.push_back(lc);
                }
            }
        }
        int ans = 0;
        for (int node : cand) {
            for(int k : cand){
                ans = max(dist2(node, k, lca(k, node)) + 1, ans);
            }
        }
        return min(ans, min(dist2(u, v, lca(u, v)) + 1, dist2(x, y , lca(x, y)) + 1));
    }


};
void fast() {
    ios::sync_with_stdio(0);
    cin.tie(NULL);
    cout.tie(0);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
}
int main() {
ios::sync_with_stdio(0);
    cin.tie(NULL);
    cout.tie(0);


    int t= 1;
//    cin >> t;
    while(t--) {
        int n, m; cin >> n >> m;
        for(int i = 2; i <= n; i++){
            int x; cin >> x;
            adj[x].push_back(i);
            adj[i].push_back(x);
        }
        LCA lca; lca.init(n);
        while(m--){
            int a, b ,c;
            cin >> a >> b >> c;
            cout << max({lca.commonNodes(a, b, a, c), lca.commonNodes(b, a, b, c), lca.commonNodes(c, a, c ,b)}) << endl;
        }

    }




    return 0;
}
