#include <bits/stdc++.h>
using namespace std;

static const int oo = 2e9;

struct Edge {
    int u, v, flow = 0, cap = 0, cost;
    Edge(int u, int v, int c, int cost): u(u), v(v), cap(c), cost(cost) {}
    int rem() const { return cap - flow; }
};

struct MCMF {
    int n, s, t;
    int flow = 0, cost = 0;
    vector<Edge> edges;
    vector<vector<int>> adj;
    vector<int> from, pvEdge;

    MCMF(int n, int s, int t): n(n), s(s), t(t) {
        adj.assign(n+1, {});
    }

    void addEdge(int u, int v, int w = oo, int c = 0, bool undir = false) {
        adj[u].push_back(edges.size());
        edges.emplace_back(u, v, w, c);
        adj[v].push_back(edges.size());
        edges.emplace_back(v, u, undir ? w : 0, -c);
    }

    bool spfa() {
        from.assign(n+1, -1);
        pvEdge.assign(n+1, -1);
        vector<int> dist(n+1, oo);
        vector<bool> inq(n+1, false);
        deque<int> dq;
        dist[s] = 0;
        dq.push_back(s);
        inq[s] = true;

        while (!dq.empty()) {
            int u = dq.front(); dq.pop_front();
            inq[u] = false;
            for (int id : adj[u]) {
                auto &e = edges[id];
                if (e.rem() > 0 && dist[e.v] > dist[u] + e.cost) {
                    dist[e.v] = dist[u] + e.cost;
                    from[e.v] = u;
                    pvEdge[e.v] = id;
                    if (!inq[e.v]) {
                        inq[e.v] = true;
                        if (!dq.empty() && dist[e.v] < dist[dq.front()])
                            dq.push_front(e.v);
                        else dq.push_back(e.v);
                    }
                }
            }
        }
        return from[t] != -1;
    }

    void minCostMaxFlow() {
        while (spfa()) {
            int addf = oo;
            for (int v = t; v != s; v = from[v]) {
                auto &e = edges[pvEdge[v]];
                addf = min(addf, e.rem());
            }
            for (int v = t; v != s; v = from[v]) {
                int id = pvEdge[v];
                edges[id].flow += addf;
                edges[id^1].flow -= addf;
                cost += edges[id].cost * addf;
            }
            flow += addf;
        }
    }

    vector<tuple<int,int,int,int>> getFlowEdges() const {
        vector<tuple<int,int,int,int>> used;
        for (auto &e : edges) {
            if (e.flow > 0 && e.cap > 0) {
                used.emplace_back(e.u, e.v, e.flow, e.cost);
            }
        }
        return used;
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;
    int s, t;
    cin >> s >> t;

    MCMF mcmf(N, s, t);
    for (int i = 0; i < M; ++i) {
        int u, v, cap, cost;
        cin >> u >> v >> cap >> cost;
        mcmf.addEdge(u, v, cap, cost);
    }

    mcmf.minCostMaxFlow();
    cout << "Max flow = " << mcmf.flow << "\n";
    cout << "Min cost = " << mcmf.cost << "\n";

    auto used = mcmf.getFlowEdges();
    cout << "Edges in the flow (u->v flow cost):\n";
    for (auto &tup : used) {
        int u,v,f,c;
        tie(u,v,f,c) = tup;
        cout << u << " -> " << v 
             << "    flow=" << f << " cost=" << c << "\n";
    }
    return 0;
}
