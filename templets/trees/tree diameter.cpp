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
 
#define all(a) a.begin(), a.end()
signed main() {
    int n,m; cin >> n;
    vector<int>adj[n + 1];
    for(int i = 1; i < n ; i++){
        int u, v; cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    vector<int>dp(n + 1);
    int ans = 0;
    function<void(int, int)> solve = [&](int u, int p) {
        int sum0 = 0;
        dp[u] = 1;
        for (int v : adj[u]) {
            if (v == p) continue;
            solve(v, u);
            dp[u] = max(dp[u], dp[v] + 1);
        }
        vector<int>cur;
        for (int v : adj[u]) {
            if (v == p) continue;
            if (cur.size() < 2){
                cur.push_back(dp[v]);
            }
            else if (dp[v] > cur[0]) cur[0] = dp[v];
 
            sort(cur.begin(), cur.end());
        }
        if (!cur.empty()){
            int now = 0;
            while(!cur.empty()) now+= cur.back(), cur.pop_back();
            ans = max(ans, now);
        }
    };
    solve(1, 0);
    cout << ans << endl;
 
 
 
 
    return 0;
}
