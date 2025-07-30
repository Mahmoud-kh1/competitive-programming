#include <bits/stdc++.h>
using namespace std;
#define  int long long

const int N = 23;
int dp[(1 << N)], dp2[1 << N];
signed main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int t = 1;
//    cin >> t;
    while(t--){
        int n; cin >> n;
        int a[n];
        memset(dp, -1, sizeof dp);
        for(int i = 0; i < n; i++){
            cin >> a[i], dp[a[i]] = a[i];
        }
        for(int bit = N - 1; bit >= 0; bit--){
            for(int j = (1 << N) - 1; j >= 0; j--){
                if (j >> bit & 1){
                    dp[j] = max(dp[j], dp[j ^ (1 << bit)]);
                }
            }
        }

        int full = (1 << N) - 1;
        for(int i = 0; i < n; i++){
            cout << dp[a[i] ^ full] << ' ';
        }
        cout << endl;




    }

    return 0;









}
