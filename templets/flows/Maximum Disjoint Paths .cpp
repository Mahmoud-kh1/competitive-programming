{
maximum disjoint paths : 
from s to t , find max number of disjoint path that not have the same edge  
-SOLUTION : 
  we just put for each edge flow with capcity 1 and run Max Flow the value is the number of paths  
}
#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;

struct Edge {
    int u, v, flow = 0, cap = 0;
    Edge(int u, int v): u(u), v(v) {}
    Edge(int u, int v, int c): u(u), v(v), cap(c) {}
    int rem() const { return cap - flow; }
};

struct Dinic {
    int n, s, t;
    vector<Edge> edges;
    vector<vector<int>> adj;
    vector<int> lvl, ptr;

    Dinic(int n, int src, int sink): n(n), s(src), t(sink) {
        adj.assign(n + 1, {});
        lvl.assign(n + 1, 0);
        ptr.assign(n + 1, 0);
    }

    void addEdge(int u, int v, int w = INF, bool undir = false) {
        adj[u].push_back(edges.size());
        edges.emplace_back(u, v, w);
        adj[v].push_back(edges.size());
        // reverse edge: for directed undir=false, cap=0; for undirected, cap=w
        edges.emplace_back(v, u, undir ? w : 0);
    }

    bool bfs() {
        fill(lvl.begin(), lvl.end(), -1);
        queue<int> q;
        lvl[s] = 0;
        q.push(s);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int idx : adj[u]) {
                auto &e = edges[idx];
                if (lvl[e.v] < 0 && e.rem() > 0) {
                    lvl[e.v] = lvl[u] + 1;
                    q.push(e.v);
                }
            }
        }
        return lvl[t] >= 0;
    }

    int dfs(int u, int pushed) {
        if (u == t || pushed == 0) return pushed;
        for (int &cid = ptr[u]; cid < (int)adj[u].size(); cid++) {
            int idx = adj[u][cid];
            auto &e = edges[idx];
            if (lvl[e.v] != lvl[u] + 1 || e.rem() <= 0) 
                continue;
            int tr = dfs(e.v, min(pushed, e.rem()));
            if (tr > 0) {
                e.flow += tr;
                edges[idx ^ 1].flow -= tr;
                return tr;
            }
        }
        return 0;
    }

    int maxFlow() {
        int flow = 0;
        while (bfs()) {
            fill(ptr.begin(), ptr.end(), 0);
            while (int pushed = dfs(s, INF)) {
                flow += pushed;
            }
        }
        return flow;
    }

    // extract a single path following edges with flow>0
    bool extractPath(int u, vector<int> &path) {
        if (u == t) return true;
        for (int idx : adj[u]) {
            auto &e = edges[idx];
            // edge used if flow > 0
            if (e.flow > 0) {
                e.flow -= 1;                     // consume flow
                edges[idx ^ 1].flow += 1;       // update reverse
                path.push_back(e.v);
                if (extractPath(e.v, path))
                    return true;
                // backtrack
                path.pop_back();
                // restore if dead-end
                e.flow += 1;
                edges[idx ^ 1].flow -= 1;
            }
        }
        return false;
    }

    vector<vector<int>> getEdgeDisjointPaths() {
        vector<vector<int>> paths;
        while (true) {
            vector<int> path;
            path.push_back(s);
            if (!extractPath(s, path)) break;
            paths.push_back(path);
        }
        return paths;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    int s, t;
    cin >> s >> t;
    // assume 1-based input
    Dinic dinic(n, s, t);
    
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        // for undirected, set undir=true
        dinic.addEdge(u, v, 1, true);
    }

    int k = dinic.maxFlow();
    cout << k << " edge-disjoint paths found:\n";
    auto paths = dinic.getEdgeDisjointPaths();

    for (int i = 0; i < (int)paths.size(); i++) {
        for (int j = 0; j < (int)paths[i].size(); j++) {
            cout << paths[i][j] << (j + 1 < (int)paths[i].size() ? " -> " : "\n");
        }
    }

    return 0;
}
