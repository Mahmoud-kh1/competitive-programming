#include <bits/stdc++.h>
using namespace std;

#define int long long

signed main() {
    int t  = 1; 
    // cin >> t;
    while (t--) {
        // dp ranges we want to answer dp[l][r] , and we want to find the best split point k 
        // so this can be done in N^3 , but let's say k is the optimal for l, r
        // this condition should be hold opt[l][r - 1] <= opt[l][r] <= opt[l][r +1]
        int n; cin >> n;
        int a[n + 1];
        for (int i = 1; i <= n; i++) cin >> a[i];
        int pre[n + 1]; pre[0] = 0;
        for (int i = 1; i <= n; i++) pre[i] = pre[i - 1] + a[i];
        vector<vector<int>> dp(n + 1, vector<int>(n + 1, 1e18));
        vector<vector<int>> opt(n + 1, vector<int>(n + 1, 1e18));
        
        for (int i = 1; i <= n; i++) {
            dp[i][i] = 0;
            opt[i][i] = i; 
        }
          
        for (int len = 2; len <= n; len++) {
            for (int i = 1; i <= n - len + 1; i++) {
                int j = i + len - 1;
                dp[i][j] = 1e18;
        
                // Knuth Optimization: restrict the search space for k
                int left_bound = opt[i][j - 1];
                int right_bound = opt[i + 1][j];
        
                for (int k = left_bound; k <= right_bound; k++) {
                    // Note: boundary conditions for k depend on the specific problem 
                    // (e.g., whether k can equal j or not).
                    if (k >= j) continue; 
            
                    long long current_cost = dp[i][k] + dp[k + 1][j] + pre[j] - pre[i - 1];
                    if (current_cost < dp[i][j]) {
                        dp[i][j] = current_cost;
                        opt[i][j] = k;
                    }
                }
            }
        }
        cout << dp[1][n] << endl;
    }
}
