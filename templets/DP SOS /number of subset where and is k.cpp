 #include <bits/stdc++.h>
using namespace std;
#define  int long long

const int N = 20;
int dp[(1 << N)],power[(1<< N)]{};
const int mod = 1e9 + 7;
int fast(int a, int b){
    int res = 1;
    while(b){
        if(b & 1){
            res = (res * a) % mod;
        }
        b/=2;
        a = (a * a) % mod;
    }
    return res;
}
signed main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int t = 1;
//    cin >> t;
    while(t--){
       int n;
       cin >> n;
       for(int i = 0; i < n; i++){
           int x; cin >> x;
           dp[x]++;
       }
       power[0] = 1;
       for(int i = 1; i < (1 << N); i++){
           power[i] = (power[i - 1] * 2) % mod;
       }
       for(int bit = 0; bit < N; bit++){
           for(int mask = 0; mask < (1 << N); mask++){
               if(!(mask >> bit & 1)){
                   dp[mask] += dp[mask ^ (1 << bit)];
               }
           }
       }

       for(int i = 0; i < (1 << N); i++){
           dp[i] = fast(2, dp[i]) - 1;
           if(dp[i] < 0) dp[i] +=mod;
       }
       for(int bit = 0; bit < N; bit++){
           for(int mask = 0; mask < (1 << N); mask++){
               if(!(mask >> bit & 1)){
                   dp[mask] -= dp[mask ^ (1 << bit)];
                   if(dp[mask] < 0) dp[mask]+=mod;
               }
           }
       }
       for(int i = 0; i <= n; i++) cout << dp[i] << ' ';



    }

    return 0;









}
