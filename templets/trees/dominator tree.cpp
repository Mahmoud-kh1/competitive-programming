#include <bits/stdc++.h>
using namespace std;

struct Dominator_Tree {
    vector<vector<int>>& adj;
    vector<vector<int>> tree, radj, event;
    vector<int> reff, tin, parent, low, dsu, sdom, idom, dom;
    int timer = 0;

    Dominator_Tree(int n, int root, vector<vector<int>>& al)
        : adj(al), tree(n + 1), radj(n + 1), event(n + 1),
          reff(n + 1), tin(n + 1), parent(n + 1), low(n + 1),
          dsu(n + 1), sdom(n + 1), idom(n + 1), dom(n + 1) {

        build(root);

        for (int i = 1; i <= n; ++i) {
            if (!dom[i]) continue;
            tree[dom[i]].push_back(i);
        }
    }

    int find(int u, int x = 0) {
        if (u == dsu[u]) return x ? -1 : u;
        int v = find(dsu[u], x + 1);
        if (v < 0) return u;
        if (sdom[low[dsu[u]]] < sdom[low[u]]) low[u] = low[dsu[u]];
        dsu[u] = v;
        return x ? v : low[u];
    }

    void dfs(int u) {
        tin[u] = ++timer;
        reff[timer] = u;
        low[timer] = sdom[timer] = dsu[timer] = timer;
        for (int v : adj[u]) {
            if (!tin[v]) {
                dfs(v);
                parent[tin[v]] = tin[u];
            }
            radj[tin[v]].push_back(tin[u]);
        }
    }

    void build(int root) {
        dfs(root);
        for (int i = timer; i >= 2; i--) {
            for (int u : radj[i]) {
                sdom[i] = min(sdom[i], sdom[find(u)]);
            }
            event[sdom[i]].push_back(i);
            int p = parent[i];
            dsu[i] = p;
            for (int v : event[p]) {
                int best = find(v);
                idom[v] = (sdom[best] < sdom[v]) ? best : sdom[v];
            }
            event[p].clear();
        }
        for (int i = 2; i <= timer; ++i) {
            if (idom[i] != sdom[i]) {
                idom[i] = idom[idom[i]];
            }
            dom[reff[i]] = reff[idom[i]];
        }
    }
};

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
     while (cin >> n >> m) {
         vector<vector<int>> adj(n + 1);
         vector<pair<int, int>> edges(m + 1);

         for (int i = 1; i <= m; i++) {
             int u, v;
             cin >> u >> v;
             adj[u].push_back(v);
             edges[i] = {u, v};
         }

         Dominator_Tree dt(n, 1, adj);
         vector<int> in(n + 1, 0), out(n + 1, 0);
         int timer = 0;
         function<void(int)>  dfs = [&](int u) {
             in[u] = ++timer;
             for (int v : dt.tree[u]) {
                 dfs(v);
             }
             out[u] = ++timer;
         }; 
         dfs(1);
     
         auto is_anc = [&](int anc, int node) -> bool {
             if (in[anc] == 0 || in[node] == 0) return false;
             return in[anc] <= in[node] && out[node] <= out[anc];
         }; 

         vector<int> road;
         for (int i = 1; i <= m; i++) {
             auto [u, v] = edges[i];
             if (dt.tin[u] > 0 && !is_anc(v, u)) {
                 road.push_back(i);
             }
         }

         cout << road.size() << endl;
         for (int i = 0; i < (int)road.size(); i++) {
             cout << road[i] << (i + 1 == (int)road.size() ? "" : " ");
         }
         cout << endl;
     }
    return 0;
}
