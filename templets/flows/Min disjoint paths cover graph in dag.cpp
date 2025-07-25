the min disjonit paths that not share edges in dag that cover all vertices 
is N - maxMatching

  #include <bits/stdc++.h>
using namespace std;

// Hopcroft–Karp for maximum bipartite matching
const int INF = 1e9;
typedef vector<int> vi;

struct HopcroftKarp {
    int n, m;
    vector<vi> adj;
    vi dist, pairU, pairV;

    HopcroftKarp(int n, int m)
        : n(n), m(m), adj(n+1), dist(n+1), pairU(n+1), pairV(m+1) {}

    // add edge from left-side u (1..n) to right-side v (1..m)
    void addEdge(int u, int v) {
        adj[u].push_back(v);
    }

    bool bfs() {
        queue<int> q;
        for(int u = 1; u <= n; ++u) {
            if(pairU[u] == 0) {
                dist[u] = 0;
                q.push(u);
            } else dist[u] = INF;
        }
        bool reachableFree = false;
        while(!q.empty()) {
            int u = q.front(); q.pop();
            for(int v: adj[u]) {
                if(pairV[v] == 0) reachableFree = true;
                else if(dist[pairV[v]] == INF) {
                    dist[pairV[v]] = dist[u] + 1;
                    q.push(pairV[v]);
                }
            }
        }
        return reachableFree;
    }

    bool dfs(int u) {
        for(int v: adj[u]) {
            if(pairV[v] == 0 || (dist[pairV[v]] == dist[u] + 1 && dfs(pairV[v]))) {
                pairU[u] = v;
                pairV[v] = u;
                return true;
            }
        }
        dist[u] = INF;
        return false;
    }

    int maxMatching() {
        int result = 0;
        while(bfs()) {
            for(int u = 1; u <= n; ++u)
                if(pairU[u] == 0 && dfs(u))
                    ++result;
        }
        return result;
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;
    cin >> N >> M;
    vector<vector<int>> graph(N+1);
    for(int i = 0; i < M; ++i) {
        int u, v;
        cin >> u >> v;
        graph[u].push_back(v);  // assumes DAG input
    }

    // Build bipartite graph on [1..N] left and [1..N] right
    HopcroftKarp hk(N, N);
    for(int u = 1; u <= N; ++u)
        for(int v: graph[u])
            hk.addEdge(u, v);

    int matching = hk.maxMatching();
    int minPathCover = N - matching;
    cout << minPathCover << " paths in minimum path cover\n";

    // Build successor and predecessor from matching
    vector<int> succ(N+1, 0), pred(N+1, 0);
    for(int u = 1; u <= N; ++u){
        int v = hk.pairU[u];
        if(v != 0){ succ[u] = v; pred[v] = u; }
    }

    // Output the paths: start at vertices with no predecessor
    vector<bool> printed(N+1, false);
    for(int u = 1; u <= N; ++u) {
        if(pred[u] == 0) {
            int cur = u;
            vector<int> path;
            while(cur != 0) {
                path.push_back(cur);
                printed[cur] = true;
                cur = succ[cur];
            }
            // print
            for(int i = 0; i < (int)path.size(); ++i)
                cout << path[i] << (i+1 < (int)path.size() ? " -> " : "\n");
        }
    }

    return 0;
}
