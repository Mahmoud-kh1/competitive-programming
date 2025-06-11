#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

void PRE() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
#ifndef ONLINE_JUDGE
    freopen("in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);
    freopen("error.txt", "w", stderr);
#endif
}
Time is O(edges * sqrt(nodes))  O(E sqrt(V)) 
struct HopcroftKarp {
    int n;
    vector<vector<int>> adj;
    vector<int> mu, mv, dist;
    // mu[x] this mean x in left group and mu[x] is the node that x has matched from second to get max match 
    // mv[x] is the same but from the right group

    HopcroftKarp(int n , int m) : n(n), adj(n), mu(n, -1), mv(m, -1), dist(n) {}
     /// u is in the right group and v is in the left one 
    void add_edge(int u, int v) {
        adj[u].push_back(v);
    }

    bool bfs() {
        queue<int> q;
        for (int u = 0; u < n; ++u) {
            if (mu[u] == -1) {
                dist[u] = 0;
                q.push(u);
            } else {
                dist[u] = -1;
            }
        }
        bool found = false;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : adj[u]) {
                if (mv[v] == -1) {
                    found = true;
                }
                if (mv[v] != -1 && dist[mv[v]] == -1) {
                    dist[mv[v]] = dist[u] + 1;
                    q.push(mv[v]);
                }
            }
        }
        return found;
    }

    bool dfs(int u) {
        for (int v : adj[u]) {
            if (mv[v] == -1 || (dist[mv[v]] == dist[u] + 1 && dfs(mv[v]))) {
                mu[u] = v;
                mv[v] = u;
                return true;
            }
        }
        dist[u] = -1;
        return false;
    }

    int max_matching() {
        int res = 0;
        while (bfs()) {
            for (int u = 0; u < n; ++u) {
                if (mu[u] == -1 && dfs(u)) {
                    res++;
                }
            }
        }
        return res;
    }
};

int main() {
//    PRE();
    int t;cin>>t;
    while (t--) {
        int n; cin>>n;
        HopcroftKarp d(n + 1 , n + 1);
        for (int i = 1;i <= n;i++) {
            for (int j = 0;j < 20;j++) {
                int x = i ^ (1<<j);
                if (x >= 1 && x <= n)
                    d.add_edge(i , x);
            }
        }
        if (d.max_matching() != n) {
            cout<<"NO\n";
        }else {
            cout<<"YES\n";
            for (int i = 1;i <= n;i++) {
                cout<<d.mu[i]<<" ";
            }
            cout<<'\n';
        }
    }
}
