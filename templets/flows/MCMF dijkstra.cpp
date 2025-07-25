#include <bits/stdc++.h>
using namespace std;
using ll = long long;


O(Flow * E log(V)) 
static const ll oo = 1e18;

struct Edge {
    int u, v;
    ll flow = 0, cap = 0, cost;
    Edge(int u, int v, ll cap, ll cost): u(u), v(v), cap(cap), cost(cost) {}
    ll rem() const { return cap - flow; }
};

struct MCMF {
    int n, s, t;
    ll flow = 0, cost = 0;
    vector<Edge> edges;
    vector<vector<int>> adj;
    vector<ll> pi; // potentials

    MCMF(int n, int s, int t): n(n), s(s), t(t) {
        adj.resize(n+1);
        pi.resize(n+1, 0);
    }

    void addEdge(int u, int v, ll cap, ll cost, bool undir = false) {
        adj[u].push_back(edges.size());
        edges.emplace_back(u, v, cap, cost);
        adj[v].push_back(edges.size());
        edges.emplace_back(v, u, undir ? cap : 0, -cost);
    }

    void initPotentials() {
        vector<ll> dist(n+1, oo);
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
                    if (!inq[e.v]) {
                        inq[e.v] = true;
                        dq.push_back(e.v);
                    }
                }
            }
        }
        for (int i = 1; i <= n; i++) 
            if (dist[i] < oo) 
                pi[i] = dist[i];
    }

    bool dijkstra() {
        vector<ll> dist(n+1, oo);
        vector<int> parent(n+1, -1), pedge(n+1, -1);
        dist[s] = 0;
        priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<>> pq;
        pq.push({0, s});

        while (!pq.empty()) {
            auto [d, u] = pq.top(); pq.pop();
            if (d != dist[u]) continue;
            for (int id : adj[u]) {
                auto &e = edges[id];
                if (e.rem() <= 0) continue;
                ll w = e.cost + pi[u] - pi[e.v]; // reduced cost
                if (dist[u] + w < dist[e.v]) {
                    dist[e.v] = dist[u] + w;
                    parent[e.v] = u;
                    pedge[e.v] = id;
                    pq.push({dist[e.v], e.v});
                }
            }
        }

        if (dist[t] == oo) return false;

        // Update potentials
        for (int i = 1; i <= n; i++) 
            if (dist[i] < oo) 
                pi[i] += dist[i];

        // Find min residual capacity
        ll addf = oo;
        for (int v = t; v != s; v = parent[v]) {
            int id = pedge[v];
            addf = min(addf, edges[id].rem());
        }

        // Update flow and cost
        for (int v = t; v != s; v = parent[v]) {
            int id = pedge[v];
            edges[id].flow += addf;
            edges[id^1].flow -= addf;
        }

        flow += addf;
        cost += addf * pi[t]; // Actual path cost = pi[t] (since pi[s]=0)
        return true;
    }

    void minCostMaxFlow() {
        initPotentials();
        while (dijkstra());
    }

    vector<tuple<int, int, ll, ll>> getFlowEdges() const {
        vector<tuple<int, int, ll, ll>> used;
        for (auto &e : edges) {
            if (e.flow > 0 && e.cap > 0) {
                used.emplace_back(e.u, e.v, e.flow, e.cost);
            }
        }
        return used;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N, M, s, t;
    cin >> N >> M >> s >> t;
    MCMF mcmf(N, s, t);
    for (int i = 0; i < M; i++) {
        int u, v;
        ll cap, cost;
        cin >> u >> v >> cap >> cost;
        mcmf.addEdge(u, v, cap, cost);
    }
    mcmf.minCostMaxFlow();
    cout << "Max flow = " << mcmf.flow << "\n";
    cout << "Min cost = " << mcmf.cost << "\n";
    auto used = mcmf.getFlowEdges();
    cout << "Edges in the flow (u->v flow cost):\n";
    for (auto &[u, v, f, c] : used) {
        cout << u << " -> " << v << "    flow=" << f << " cost=" << c << "\n";
    }
    return 0;
}
