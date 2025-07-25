max paths that not have the same vertix 

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

    Dinic(int n, int src, int sink)
        : n(n), s(src), t(sink), adj(n+1), lvl(n+1), ptr(n+1) {}

    void addEdge(int u, int v, int w = INF, bool undir = false) {
        adj[u].push_back(edges.size());
        edges.emplace_back(u, v, w);
        adj[v].push_back(edges.size());
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
                const auto &e = edges[idx];
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
            if (lvl[e.v] != lvl[u] + 1 || e.rem() <= 0) continue;
            int tr = dfs(e.v, min(pushed, e.rem()));
            if (tr > 0) {
                e.flow += tr;
                edges[idx^1].flow -= tr;
                return tr;
            }
        }
        return 0;
    }

    int maxFlow() {
        int flow = 0;
        while (bfs()) {
            fill(ptr.begin(), ptr.end(), 0);
            while (int pushed = dfs(s, INF)) flow += pushed;
        }
        return flow;
    }

    // extract one path of used flow (flow > 0)
    bool extractPath(int u, vector<int> &path) {
        if (u == t) return true;
        for (int idx : adj[u]) {
            auto &e = edges[idx];
            if (e.flow > 0) {
                // consume
                e.flow -= 1;
                edges[idx^1].flow += 1;
                path.push_back(e.v);
                if (extractPath(e.v, path)) return true;
                // backtrack
                path.pop_back();
                e.flow += 1;
                edges[idx^1].flow -= 1;
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
    // build on 2*n nodes: 1..n are "in", n+1..2n are "out"
    auto in  = [&](int v){ return v;         };
    auto out = [&](int v){ return v + n;     };

    // source = out(s), sink = in(t)
    Dinic dinic(2*n, out(s), in(t));

    // 1) vertex-splitting edges
    for (int v = 1; v <= n; v++) {
        int cap = (v == s || v == t) ? INF : 1;
        dinic.addEdge(in(v), out(v), cap, false);
    }
    // 2) original edges
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        // directed: u_out -> v_in
        dinic.addEdge(out(u), in(v), INF, false);
        // if undirected, also:
        // dinic.addEdge(out(v), in(u), INF, false);
    }

    int k = dinic.maxFlow();
    cout << k << " vertex-disjoint paths found:\n";

    auto rawPaths = dinic.getEdgeDisjointPaths();
    // convert transformed paths back to original
    for (auto &p : rawPaths) {
        vector<int> verts;
        // first element p[0] == out(s); print s first
        verts.push_back(s);
        for (int i = 1; i < (int)p.size(); i++) {
            int id = p[i];
            if (1 <= id && id <= n) {
                verts.push_back(id);
            }
        }
        // print
        for (int i = 0; i < (int)verts.size(); i++) {
            cout << verts[i] << (i+1 < (int)verts.size() ? " -> " : "\n");
        }
    }

    return 0;
}
