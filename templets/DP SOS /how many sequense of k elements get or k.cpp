#include <bits/stdc++.h>
using namespace std;
#define  int long long

const int N = 22;
int dp[(1 << N)],power[(1<< N)]{};

const int mod = 1e9 + 7;
const int M = 2e6;
int fact[(1 << N)], inv_fact[(1 << N)];
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
int mul(int a, int b){return (a * b) % mod;}
void pre() {
    fact[0] = inv_fact[0] = 1;
    for (int i = 1; i < M; i++) {
        fact[i] = mul(fact[i - 1], i);
    }
    inv_fact[M - 1] = fast(fact[M - 1], mod - 2);
    for (int i = M - 2; i >= 0; i--) {
        inv_fact[i] = mul(inv_fact[i + 1], (i + 1));
    }
}

int ncr(int n,int r) {
    if (r > n) return 0;
    if (r == n or r == 0) return 1;
    return (mul(fact[n], mul(inv_fact[r], inv_fact[n - r]))) % mod;
}

signed main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int t = 1;
    pre();
//    cin >> t;
    while(t--){
        int n , k;
        cin >> n >> k;
        for(int i = 0; i < n; i++){
            int x; cin >> x;
            dp[x]++;
        }
        power[0] = 1;
        for(int i = 1; i < (1 << N); i++){
            power[i] = (power[i - 1] * 2) % mod;
        }
        for(int bit = N - 1; bit >=0 ;bit--){
            for(int mask = (1 << N) - 1; mask >=0; mask--){
                if((mask >> bit & 1)){
                    dp[mask] += dp[mask ^ (1 << bit)];
                }
            }
        }

        for(int i = 0; i < (1 << N); i++){
            dp[i] = ncr(dp[i], k);
        }

        for(int bit = N  - 1; bit >= 0; bit--){
            for(int mask = (1 << N) - 1; mask >= 0; mask--){
                if((mask >> bit & 1)){
                    dp[mask] -= dp[mask ^ (1 << bit)];
                    if(dp[mask] < 0) dp[mask]+=mod;
                }
            }
        }

        int q; cin >> q;
        while(q--){
            int x; cin >> x;
            cout << dp[x] << endl;
        }



    }

    return 0;









}
