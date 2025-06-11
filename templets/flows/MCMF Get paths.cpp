const int INF = 1e9;

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
        Edge a = {v, capacity, cost, 0, (int)adj[v].size()};
        Edge b = {u, 0, -cost, 0, (int)adj[u].size()};
        adj[u].push_back(a);
        adj[v].push_back(b);
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
      Min cost to get flow with value maxflow 
    pair<int, int> solve(int source, int sink, int maxFlow) {
        int flow = 0, cost = 0;

        while (flow < maxFlow && spfa(source, sink)) {
            int pathFlow = INF;

            for (int u = sink; u != source; u = parent[u]) {
                Edge &e = adj[parent[u]][parentEdge[u]];
                pathFlow = min(pathFlow, e.capacity - e.flow);
            }

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
    paths that match from source to sink with flow k and min cost 
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
