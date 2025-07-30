#include <bits/stdc++.h>
using namespace std;
void HOKSHA() {
    ios::sync_with_stdio(0);
    cin.tie(NULL);
    cout.tie(0);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
}
#define int long long
struct SCCResult {
    int comp_cnt;
    vector<int> comp_id;               // 1..n -> 0..comp_cnt-1
    vector<int> leader;                // 0..comp_cnt-1 -> representative original index
    vector<vector<int>> comp_nodes;    // nodes in each component (original 1..n)
    vector<vector<int>> condensed;     // DAG on components not leaders
};

// Compute SCC and build condensed DAG for 1-indexed graph
SCCResult condense(const vector<vector<int>>& g, int n) {
    vector<vector<int>> gr(n+1);
    for (int u = 1; u <= n; ++u)
        for (int v : g[u])
            gr[v].push_back(u);

    vector<bool> used(n+1, false);
    vector<int> order;
    order.reserve(n);
    function<void(int)> dfs1 = [&](int u) {
        used[u] = true;
        for (int v : g[u]) if (!used[v]) dfs1(v);
        order.push_back(u);
    };
    for (int i = 1; i <= n; ++i)
        if (!used[i]) dfs1(i);

    vector<int> comp_id(n+1, -1);
    int comp_cnt = 0;
    function<void(int)> dfs2 = [&](int u) {
        comp_id[u] = comp_cnt;
        for (int v : gr[u]) if (comp_id[v] == -1) dfs2(v);
    };
    for (int i = n - 1; i >= 0; --i) {
        int u = order[i];
        if (comp_id[u] == -1) {
            dfs2(u);
            ++comp_cnt;
        }
    }

    vector<vector<int>> comp_nodes(comp_cnt);
    for (int u = 1; u <= n; ++u)
        comp_nodes[comp_id[u]].push_back(u);
    vector<int> leader(comp_cnt);
    for (int c = 0; c < comp_cnt; ++c)
        leader[c] = comp_nodes[c][0];

    vector<vector<int>> condensed(comp_cnt);
    for (int u = 1; u <= n; ++u)
        for (int v : g[u]) {
            int cu = comp_id[u], cv = comp_id[v];
            if (cu != cv)
                condensed[cu].push_back(cv);
        }
    for (auto& adj : condensed) {
        sort(adj.begin(), adj.end());
        adj.erase(unique(adj.begin(), adj.end()), adj.end());
    }

    return {comp_cnt, comp_id, leader, comp_nodes, condensed};
}

#define all(a) a.begin(), a.end()
signed main() {
    int n,m; cin >> n >> m;
    vector<vector<int>>adj(n + 1);
    int val[n + 1];
    for(int i = 1; i <=n ; i++) cin >> val[i];
    for(int j = 0; j < m; j++){
        int u, v; cin >> u >> v;
        adj[u].push_back(v);
    }
    SCCResult graph = condense(adj, n);
    int sum[n + 1]{};
    for(int i = 0; i < graph.comp_cnt; i++){
        for(auto j : graph.comp_nodes[i]){
            sum[i] += val[j];
        }
    }
    vector<int>dp(graph.comp_cnt, -1e6);
    function<int(int)> solve = [&](int  comp)-> int{
        int &ret = dp[comp];
        if (ret != -1e6) return ret;
        ret = sum[comp];
        for(auto j : graph.condensed[comp]){
            ret = max(ret, solve(j) + sum[comp]);
        }
        return ret;
    };

    int ans = 0;
    for(int i = 0; i < graph.comp_cnt; i++){
        ans = max(ans, solve(i));
    }
    cout << ans;









    return 0;
}
