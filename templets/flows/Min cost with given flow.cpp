#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
#define int long long
const int INF = 2e18;

struct Edge {
    int to, capacity, cost, flow, rev;
};

class MinCostMaxFlow {
private:
    int n;
    vector<vector<Edge>> adj;
    vector<int> dist, parent, parentEdge;
    vector<bool> inQueue;

public:
    MinCostMaxFlow(int n) : n(n),
                            adj(n + 1), dist(n + 1), parent(n + 1),
                            parentEdge(n + 1), inQueue(n + 1) {}

    void addEdge(int u, int v, int capacity, int cost) {
        adj[u].push_back({v, capacity, cost, 0, (int)adj[v].size()});
        adj[v].push_back({u, 0, -cost, 0, (int)adj[u].size() - 1});
    }

    bool spfa(int source, int sink) {
        fill(dist.begin(), dist.end(), INF);
        fill(inQueue.begin(), inQueue.end(), false);
        queue<int> q;

        dist[source] = 0;
        q.push(source);
        inQueue[source] = true;

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            inQueue[u] = false;

            for (int i = 0; i < (int)adj[u].size(); ++i) {
                Edge &e = adj[u][i];
                if (e.flow < e.capacity && dist[e.to] > dist[u] + e.cost) {
                    dist[e.to] = dist[u] + e.cost;
                    parent[e.to] = u;
                    parentEdge[e.to] = i;
                    if (!inQueue[e.to]) {
                        q.push(e.to);
                        inQueue[e.to] = true;
                    }
                }
            }
        }
        return dist[sink] < INF;
    }
        // get min cost with given flow 
    pair<int, int> solve(int source, int sink, int maxFlow) {
        int flow = 0, cost = 0;

        while (flow < maxFlow && spfa(source, sink)) {
            int pathFlow = INF;

            // find bottleneck capacity on path
            for (int u = sink; u != source; u = parent[u]) {
                Edge &e = adj[parent[u]][parentEdge[u]];
                pathFlow = min(pathFlow, e.capacity - e.flow);
            }
            // clamp to remaining needed flow
            pathFlow = min(pathFlow, maxFlow - flow);

            // augment flow & accumulate cost
            for (int u = sink; u != source; u = parent[u]) {
                Edge &e = adj[parent[u]][parentEdge[u]];
                e.flow += pathFlow;
                adj[u][e.rev].flow -= pathFlow;
                cost += pathFlow * e.cost;
            }

            flow += pathFlow;
        }

        if (flow < maxFlow) return {-1, -1};
        return {flow, cost};
    }

    vector<vector<int>> getRoutes(int source, int sink, int k) {
        vector<vector<int>> routes;
        for (int i = 0; i < k; ++i) {
            vector<int> path;
            int u = source;

            while (u != sink) {
                path.push_back(u);
                for (auto &e : adj[u]) {
                    if (e.flow > 0) {
                        e.flow--;
                        u = e.to;
                        break;
                    }
                }
            }
            path.push_back(sink);
            routes.push_back(path);
        }

        return routes;
    }
};

signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t = 1;
    // cin >> t;
    while (t--) {
        int n, m, k;
        cin >> n >> m >> k;
        MinCostMaxFlow M(n);
        for (int i = 0; i < m; i++) {
            int u, v, cap, cost;
            cin >> u >> v >> cap >> cost;
            M.addEdge(u, v, cap, cost);
        }
        auto res = M.solve(1, n, k);
        cout << res.second << "\n";
    }
    return 0;
}
