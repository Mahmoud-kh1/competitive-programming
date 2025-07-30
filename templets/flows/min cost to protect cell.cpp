#include <bits/stdc++.h>
using namespace std;
#define  int long long
static const int oo = 2e18;
n,m, (r,c) the cell 
3 4 2 2
9 1 1 9
1 0 9 1
9 1 1 9
the answer : 
6
.XX.
X..X
.XX.
struct Edge {

    int u, v, flow = 0, cap = 0; // keep the order

    Edge(int u, int v): u(u), v(v) {}
    Edge(int u, int v, int c): u(u), v(v), cap(c) {}

    int rem() { return cap - flow; }
};

struct Dinic {
    int n, s, t, id = 1;
    vector<Edge> edges;
    vector<vector<int>> adj;
    vector<int> lvl, ptr;

    Dinic(int n, int src, int sink): n(n), s(src), t(sink) {
        adj.assign(n + 1, {});
        ptr.assign(n + 1, {});
    }

    void addEdge(int u, int v, int w = oo, int undir = 0) {
        adj[u].push_back(edges.size());
        edges.push_back(Edge(u, v, w));
        adj[v].push_back(edges.size());
        edges.push_back(Edge(v, u, w * undir));
    }

    int flow() {
        int res = 0;
        while(bfs()) {
            ptr.assign(n + 1, {});
            while (int flow = dfs(s)) res += flow;
        }
        return res;
    }

    bool bfs() {
        lvl.assign(n + 1, -1);
        queue<int> q;
        q.push(s), lvl[s] = 0;

        while(!q.empty()) {
            auto u = q.front();
            q.pop();

            for(auto &i: adj[u]) {
                auto &[_, v, f, c] = edges[i];
                if(~lvl[v] || f == c) continue;
                lvl[v] = lvl[u] + 1;
                q.push(v);
            }
        }

        return lvl[t] != -1;
    }

    int dfs(int u, int flow = oo) {
        if(u == t) return flow;
        if(!flow) return 0;

        for(; ptr[u] < adj[u].size(); ++ptr[u]) {
            int i = adj[u][ptr[u]];
            auto [_, v, f, c] = edges[i];
            if(f == c || (lvl[v] != lvl[u] + 1)) continue;

            int bottleNeck = dfs(v, min(flow, c - f));
            if(!bottleNeck) continue;

            edges[i].flow += bottleNeck;
            edges[i ^ 1].flow -= bottleNeck;

            return bottleNeck;
        }

        return 0;
    }

    // get any set of edges to achieve the min cut (max flow)
    vector<array<int, 2>> getEdges() {
        vector<int> srcSide(n + 1);
        queue<int> q;
        q.push(s), srcSide[s] = 1;

        while(!q.empty()) {
            int u = q.front();
            q.pop();
            for(auto &i: adj[u]) {
                auto &[_, v, f, c] = edges[i];
                if(!srcSide[v] && f != c) q.push(v), srcSide[v] = 1;
            }
        }

        vector<array<int, 2>> res;
        for(int i = 0; i < edges.size(); i += 2) {
            auto &[u, v, f, c] = edges[i];
            if(srcSide[u] != srcSide[v] && c != oo) res.push_back({u, v});
        }

        return res;
    }
};
signed main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int t = 1;
//      cin >> t;
    while(t--){
        int n, m, x, y; cin >> n >> m >> x >> y;
        x--, y--;
        auto in  = [&](int i,int j){ return (i * m + j) + 1;   };
        auto out = [&](int i,int j){ return (i * m + j) + n * m + 1;};
        int total = 2 * n * m + 2;
        int src = total - 1;
        int sink = total;
        Dinic d(total, src, sink);
        int a[n][m];
        for(int i = 0;i <n; i++) for(int j = 0; j < m; j++) cin >> a[i][j];
        for(int i = 0; i < n; i++){
            for(int j = 0; j < m; j++){
                int cap = a[i][j];
                if (i == x && j == y){
                    cap = 1e9;
                }
                d.addEdge(in(i, j), out(i, j), cap);
            }
        }

        for(int i=0;i<n;i++)for(int j=0;j<m;j++){
                if(i==0||i==n-1||j==0||j==m-1){
                    d.addEdge(src, in(i,j), 1e9);
                }
            }

        long long INF = 1e15;
        int di[4]={1,-1,0,0}, dj[4]={0,0,1,-1};
        for(int i=0;i<n;i++)for(int j=0;j<m;j++){
                for(int k=0;k<4;k++){
                    int ni=i+di[k], nj=j+dj[k];
                    if(ni<0||ni>=n||nj<0||nj>=m) continue;
                    d.addEdge(out(i,j), in(ni,nj), INF);
                }
            }

        d.addEdge(out(x,y), sink, INF);
        int ans = d.flow();
        cout << ans << endl;
        auto edges = d.getEdges();
        char res[n][m];
        for(int i = 0; i < n; i++){
            for(int j = 0; j < m; j++) res[i][j] = '.';
        }
        for(auto [u, v] : edges){
            u--;
            res[u / m][u % m] = 'X';
        }
        for(int i = 0; i < n; i++){
            for(int j = 0; j < m; j++){
                cout << res[i][j];
            }
            cout << endl;
        }





    }

    return 0;









}
