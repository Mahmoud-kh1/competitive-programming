{
maximum disjoint paths : 
from s to t , find max number of disjoint path that not have the same edge  
-SOLUTION : 
  we just put for each edge flow with capcity 1 and run Max Flow the value is the number of paths  
}
#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int to, rev, cap;
};

struct Dinic {
    int N;
    vector<vector<Edge>> G;
    vector<int> level, ptr;
    queue<int> q;

    Dinic(int n) : N(n), G(n), level(n), ptr(n) {}

    void addEdge(int u, int v, int c) {
        G[u].push_back({v, (int)G[v].size(), c});
        G[v].push_back({u, (int)G[u].size()-1, 0});
    }

    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        level[s] = 0; q = queue<int>(); q.push(s);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto &e : G[u]) {
                if (e.cap > 0 && level[e.to] < 0) {
                    level[e.to] = level[u] + 1;
                    q.push(e.to);
                }
            }
        }
        return level[t] >= 0;
    }

    int dfs(int u, int t, int pushed) {
        if (u == t || pushed == 0) return pushed;
        for (int &cid = ptr[u]; cid < (int)G[u].size(); cid++) {
            auto &e = G[u][cid];
            if (e.cap > 0 && level[e.to] == level[u] + 1) {
                int tr = dfs(e.to, t, min(pushed, e.cap));
                if (tr > 0) {
                    e.cap -= tr;
                    G[e.to][e.rev].cap += tr;
                    return tr;
                }
            }
        }
        return 0;
    }

    int maxFlow(int s, int t) {
        int flow = 0;
        while (bfs(s,t)) {
            fill(ptr.begin(), ptr.end(), 0);
            while (int pushed = dfs(s, t, INT_MAX))
                flow += pushed;
        }
        return flow;
    }

    // --- new: extract one path via used edges ---
    bool extractPath(int u, int t, vector<int> &path) {
        if (u == t) return true;
        for (auto &e : G[u]) {
            // an edge is “used” if its reverse edge has cap > 0
            auto &rev = G[e.to][e.rev];
            if (rev.cap > 0) {
                // consume this unit of flow so we don't reuse it
                rev.cap--;
                path.push_back(e.to);
                if (extractPath(e.to, t, path))
                    return true;
                // backtrack if dead end
                path.pop_back();
                rev.cap++;
            }
        }
        return false;
    }

    vector<vector<int>> getEdgeDisjointPaths(int s, int t) {
        vector<vector<int>> allPaths;
        // We know `maxFlow(s,t)` was already called.
        while (true) {
            vector<int> path = {s};
            if (!extractPath(s, t, path))
                break;
            allPaths.push_back(path);
        }
        return allPaths;
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    int s, t;
    cin >> s >> t;
    s--; t--;

    Dinic dinic(n);
    for(int i=0;i<m;i++){
        int u,v;
        cin>>u>>v;
        u--; v--;
        dinic.addEdge(u, v, 1);
    }

    int k = dinic.maxFlow(s, t);
    cout << k << " edge‑disjoint paths found:\n";
   // we print them here like 
  // 1 --> 2 --5 -->6
  // 3 --> 7 --- >
    auto paths = dinic.getEdgeDisjointPaths(s, t);
    for(int i = 0; i < (int)paths.size(); i++) {
        for(int j = 0; j < (int)paths[i].size(); j++){
            cout << (paths[i][j] + 1)
                 << (j+1 < (int)paths[i].size() ? " → " : "\n");
        }
    }
    return 0;
}


