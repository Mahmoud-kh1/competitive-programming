the minimum cost to cut source from sink 

#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int to, rev;
    long long cap;
};

struct Dinic {
    int N, s, t;
    vector<vector<Edge>> G;
    vector<int> level, ptr;

    Dinic(int n, int src, int sink): N(n), s(src), t(sink) {
        G.assign(N, {});
        level.assign(N, 0);
        ptr.assign(N, 0);
    }

    void addEdge(int u, int v, long long c) {
        G[u].push_back({v, (int)G[v].size(), c});
        G[v].push_back({u, (int)G[u].size()-1, 0});
    }

    bool bfs() {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        level[s] = 0;
        q.push(s);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto &e : G[u]) {
                if (e.cap > 0 && level[e.to] == -1) {
                    level[e.to] = level[u] + 1;
                    q.push(e.to);
                }
            }
        }
        return level[t] != -1;
    }

    long long dfs(int u, long long pushed) {
        if (u == t || pushed == 0) return pushed;
        for (int &cid = ptr[u]; cid < (int)G[u].size(); cid++) {
            auto &e = G[u][cid];
            if (level[e.to] != level[u] + 1 || e.cap == 0) continue;
            long long tr = dfs(e.to, min(pushed, e.cap));
            if (tr > 0) {
                e.cap -= tr;
                G[e.to][e.rev].cap += tr;
                return tr;
            }
        }
        return 0;
    }

    long long maxFlow() {
        long long flow = 0;
        while (bfs()) {
            fill(ptr.begin(), ptr.end(), 0);
            while (long long pushed = dfs(s, LLONG_MAX)) {
                flow += pushed;
            }
        }
        return flow;
    }

    vector<pair<int,int>> minCutEdges(const vector<tuple<int,int,long long>> &orig) {
        vector<bool> vis(N, false);
        queue<int> q;
        q.push(s);
        vis[s] = true;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto &e : G[u]) {
                if (e.cap > 0 && !vis[e.to]) {
                    vis[e.to] = true;
                    q.push(e.to);
                }
            }
        }
        vector<pair<int,int>> cut;
        for (auto &e : orig) {
            int u, v;
            long long c;
            tie(u,v,c) = e;
            if (vis[u] && !vis[v]) cut.emplace_back(u, v);
        }
        return cut;
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M, s, t;
    cin >> N >> M >> s >> t;
    // If input is 1-based, you may subtract 1 from s, t, u, v

    Dinic D(N, s, t);
    vector<tuple<int,int,long long>> orig;

    for (int i = 0; i < M; i++) {
        int u, v;
        long long c;
        cin >> u >> v >> c;
        D.addEdge(u, v, c);
        orig.emplace_back(u, v, c);
    }

    long long maxflow = D.maxFlow();
    cout << "Max flow = " << maxflow << '\n';

    auto cut = D.minCutEdges(orig);
    cout << "Min-cut edges (u -> v):\n";
    for (auto &e : cut) {
        cout << e.first << " -> " << e.second << '\n';
    }

    return 0;
}
