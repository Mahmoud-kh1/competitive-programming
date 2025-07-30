#include <bits/stdc++.h>
using namespace std;
#define  int long long
static const int oo = 2e18;
Ivan clearly remembers that there were n elements in the array, and each element was not less than 1 and not greater than n. Also he remembers q facts about the array. There are two types of facts that Ivan remembers:

1 li ri vi — for each x such that li ≤ x ≤ ri ax ≥ vi;
2 li ri vi — for each x such that li ≤ x ≤ ri ax ≤ vi.

Also Ivan thinks that this array was a permutation, but he is not so sure about it. He wants to restore some array that corresponds to the q facts that he remembers and is very similar to permutation. Formally, Ivan has denoted the cost of array as follows:

sum of (cnt(i))^2, where cnt(i) is the number of occurences of i in the array.

struct Edge {
    int u, v, flow = 0, cap = 0, cost; // keep the order
    Edge(int u, int v, int c, int cost): u(u), v(v), cap(c), cost(cost) {}
    int rem() { return cap - flow; }
};


struct MCMF {
    int n, s, t, cost = 0, flow = 0;
    vector<Edge> edges;
    vector<vector<int>> adj;
    vector<int> from;

    MCMF(int n, int s, int t): n(n), s(s), t(t) {
        adj.assign(n + 1, {});
    }

    void addEdge(int u, int v, int w = oo, int cost = 0, int undir = 0) {
        adj[u].push_back(edges.size());
        edges.push_back(Edge(u, v, w, cost));
        adj[v].push_back(edges.size());
        edges.push_back(Edge(v, u, w * undir, -cost));
    }

    void move() {
        while (bfs()) {
            int u = t, addflow = oo;
            while (u != s) {
                Edge& e = edges[from[u]];
                addflow = min(addflow, e.rem());
                u = e.u;
            }

            u = t;
            while (u != s) {
                int i = from[u];
                edges[i].flow += addflow;
                edges[i ^ 1].flow -= addflow;
                cost += edges[i].cost * addflow;
                u = edges[i].u;
            }

            flow += addflow;
        }
    }

    bool bfs() {
        from.assign(n + 1, -1);
        vector<int> d(n + 1, oo), state(n + 1, 2);
        deque<int> q;

        state[s] = 1, d[s] = 0;
        q.clear();
        q.push_back(s);

        while (!q.empty()) {
            int u = q.front();
            q.pop_front();
            state[u] = 0;
            for (auto& i : adj[u]) {
                auto& [_, v, f, c, cost] = edges[i];

                if (f >= c || d[v] <= d[u] + cost) continue;

                d[v] = d[u] + cost;

                from[v] = i;
                if (state[v] == 1) continue;
                if (!state[v] || (!q.empty() && d[q.front()] > d[v]))
                    q.push_front(v);
                else q.push_back(v);
                state[v] = 1;
            }
        }

        return ~from[t];
    }
};
signed main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int t = 1;
//      cin >> t;
    while(t--){
        int n, q; cin >> n >> q;
        int adj[n + 1][n + 1];
        memset(adj, 0, sizeof adj);
        for(int i = 1; i <= n; i++){
            for(int j = 1; j <= n; j++){
                adj[i][j] = 1;
            }
        }
        while(q--){
            int ty, l, r, v; cin >> ty >> l >> r >> v;
            if (ty == 1){
                for(int i = l ; i <= r; i++){
                    for(int j = v - 1; j >= 1; j--){
                        adj[i][j] = 0;
                    }
                }
            }
            else{
                for(int i = l ; i <= r; i++){
                    for(int j = v + 1; j <= n ; j++){
                        adj[i][j] = 0;
                    }
                }
            }
        }
        int src = 2 * n + 1, sink = 2 * n + 2;
        MCMF d(2*n + 2, 2*n + 1, 2*n + 2);
        for(int i = 1; i <= n; i++){
            d.addEdge(src, i, 1, 0);
        }
        for(int i = 1; i <= n; i++){
            bool ok = false;
            for(int j = 1; j <= n; j++){
                if (adj[i][j]) {
                    ok = true;
                    d.addEdge(i, j + n, 1, 0);
                }
            }
            if(!ok){
                cout << -1; return 0;
            }
        }
        for(int j = n  + 1 ; j <= 2 * n; j++){
            int cost = 1;
            for(int k = 0; k < n; k++){
                d.addEdge(j , sink, 1, cost);
                cost+=2;
            }
        }
        d.move();
        cout << d.cost << endl;





    }

    return 0;









}
