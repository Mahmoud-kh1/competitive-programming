#include <bits/stdc++.h>
using namespace std;
#define int long long

const int mod = 1e9 + 7;

int power(int a, int b) {
    int res = 1;
    while (b) {
        if (b & 1) res = res * a % mod;
        a = a * a % mod;
        b >>= 1;
    }
    return res;
}

int inverse(int a) { return power(a, mod - 2); }

signed main() {
    ios::sync_with_stdio(0);
    cin.tie(NULL);
    cout.tie(0);

    int n;
    cin >> n;

    vector<int> adj[n];
    for (int i = 1; i < n; i++) {
        int x;
        cin >> x;
        adj[x].push_back(i);
        adj[i].push_back(x);
    }

    vector<int> black(n);
    for (int i = 0; i < n; i++) cin >> black[i];

    vector<vector<int>> dp(n, vector<int>(2, 0));
    function<void(int, int)> solve = [&](int u, int p) {
        vector<int> children;
        for (int v : adj[u]) {
            if (v != p) {
                children.push_back(v);
                solve(v, u);

            }
        }

        int N = children.size();
        vector<int> pre(N + 1, 1), suf(N + 2, 1);

        for (int j = 0; j < N; j++) {
            int v = children[j];
            pre[j + 1] = (pre[j] * (dp[v][0] + dp[v][1])) % mod;
        }

        for (int j = N - 1; j >= 0; j--) {
            int v = children[j];
            suf[j + 1] = (suf[j + 2] * (dp[v][0] + dp[v][1])) % mod;
        }

        if (black[u]) {
            dp[u][1] = pre[N];
            dp[u][0] = 0;
        } else {
            dp[u][0] = pre[N];
            dp[u][1] = 0;
            for (int j = 0; j < N; j++) {
                int v = children[j];
                int without = (pre[j] * suf[j + 2]) % mod;
                dp[u][1] = (dp[u][1] + dp[v][1] * without) % mod;
            }
        }
    };

    solve(0, -1);
    cout << dp[0][1] << "\n";

    return 0;
}
