#include <bits/stdc++.h>
using namespace std;
#define  int long long

const int N = 20;
int dp[(1 << N)], dp2[1 << N];
signed main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int t = 1;
//    cin >> t;
    while(t--){
        int n; cin >> n;
        int a[n];

        for(int i = 0; i < n; i++){
            cin >> a[i], dp[a[i]]++;
            dp2[a[i]]++;
        }
      // time bits of the largest value * largest value 
        for(int bit = 0; bit < N; bit++){
            for(int j = 0; j < (1 << N); j++){
                if (j >> bit & 1){
                    dp[j] += dp[j ^ (1 << bit)];
                }
                else {
                    dp2[j] += dp2[j ^ (1 << bit)];
                }
            }
        }
        // dp[j] how many submasks of j in the array it means j | a[i] = j 
        // dp2[j] how many supermasks to j in the array it means j & a[i] = j
        
        for(int i = 0; i < n; i++) cout << dp[a[i]] << " " << dp2[a[i]] << " " << n - dp[a[i] ^ ((1 << N) - 1)] << endl;



    }

    return 0;









}
