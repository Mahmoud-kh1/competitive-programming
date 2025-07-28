#include <bits/stdc++.h>
using namespace std;
#define int long long

void HOKSHA() {
    ios::sync_with_stdio(0);
    cin.tie(NULL);
    cout.tie(0);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
}

signed main() {
    int n, k; cin >> n >> k;
    vector<int> adj[n + 1];
    for (int i = 1; i < n; i++) {
        int u, v; cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    vector<vector<int>> dp(n + 1, vector<int>(n + 1, 0));
    vector<int> sz(n + 1, 0);

    function<void(int, int)> solve = [&](int u, int p) {
        sz[u] = 1;
        dp[u][1] = 1;

        for (int v : adj[u]) {
            if (v == p) continue;
            solve(v, u);

            vector<int> temp(sz[u] + sz[v] + 1, 0);
            for (int i = 1; i <= sz[u]; ++i) {
                for (int j = 1; j <= sz[v]; ++j) {
                    temp[i + j] = min(k, temp[i + j] + dp[u][i] * dp[v][j]);

                }
            }
            sz[u] += sz[v];
            for (int i = 1; i <= sz[u]; ++i) {
                dp[u][i] = min(k, dp[u][i] + temp[i]);
            }
        }
    };

    solve(1, 0);

    vector<int> count(n + 1, 0);
    for (int i = 1; i <= n; i++) {
        for (int s = 1; s <= n; s++) {
            count[s] += dp[i][s];
        }
    }

    int sum = 0;
    for (int s = 1; s <= n; s++) {
        sum += count[s];
        if (sum >= k) {
            cout << s << '\n';
            return 0;
        }
    }
    cout << -1;

    return 0;
}
