#include <bits/stdc++.h>
using namespace std;
#define  int long long
static const int oo = 2e18;

Time is o(V^2 * E)  in max flow 
Time is (E sqrt(V)) in matching 


struct Edge {
    int u, v, flow = 0, cap = 0; // keep the order

    Edge(int u, int v): u(u), v(v) {}
    Edge(int u, int v, int c): u(u), v(v), cap(c) {}

    int rem() { return cap - flow; }
};

struct Dinic {
    int n, s, t, id = 1;
    vector<Edge> edges;
    vector<vector<int>> adj;
    vector<int> lvl, ptr;

    Dinic(int n, int src, int sink): n(n), s(src), t(sink) {
        adj.assign(n + 1, {});
        ptr.assign(n + 1, {});
    }

    void addEdge(int u, int v, int w = oo, int undir = 0) {
        adj[u].push_back(edges.size());
        edges.push_back(Edge(u, v, w));
        adj[v].push_back(edges.size());
        edges.push_back(Edge(v, u, w * undir));
    }

    int flow() {
        int res = 0;
        while(bfs()) {
            ptr.assign(n + 1, {});
            while (int flow = dfs(s)) res += flow;
        }
        return res;
    }

    bool bfs() {
        lvl.assign(n + 1, -1);
        queue<int> q;
        q.push(s), lvl[s] = 0;

        while(!q.empty()) {
            auto u = q.front();
            q.pop();

            for(auto &i: adj[u]) {
                auto &[_, v, f, c] = edges[i];
                if(~lvl[v] || f == c) continue;
                lvl[v] = lvl[u] + 1;
                q.push(v);
            }
        }

        return lvl[t] != -1;
    }

    int dfs(int u, int flow = oo) {
        if(u == t) return flow;
        if(!flow) return 0;

        for(; ptr[u] < adj[u].size(); ++ptr[u]) {
            int i = adj[u][ptr[u]];
            auto [_, v, f, c] = edges[i];
            if(f == c || (lvl[v] != lvl[u] + 1)) continue;

            int bottleNeck = dfs(v, min(flow, c - f));
            if(!bottleNeck) continue;

            edges[i].flow += bottleNeck;
            edges[i ^ 1].flow -= bottleNeck;

            return bottleNeck;
        }

        return 0;
    }

    // get any set of edges to achieve the min cut (max flow)
    vector<array<int, 2>> getEdges() {
        vector<int> srcSide(n + 1);
        queue<int> q;
        q.push(s), srcSide[s] = 1;

        while(!q.empty()) {
            int u = q.front();
            q.pop();
            for(auto &i: adj[u]) {
                auto &[_, v, f, c] = edges[i];
                if(!srcSide[v] && f != c) q.push(v), srcSide[v] = 1;
            }
        }

        vector<array<int, 2>> res;
        for(int i = 0; i < edges.size(); i += 2) {
            auto &[u, v, f, c] = edges[i];
            if(srcSide[u] != srcSide[v] && c != oo) res.push_back({u, v});
        }

        return res;
    }
};
signed main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
      int t = 1;
//      cin >> t;
      while(t--){
          int n,m ; cin >> n >> m;
          Dinic dc(n, 1, n);
          for(int i = 0; i < m; i++){
              int u,v,w; cin  >> u >> v >> w;
              dc.addEdge(u, v,w, 0);
          }
          cout << dc.flow()<< endl;
      }

    return 0;









}
