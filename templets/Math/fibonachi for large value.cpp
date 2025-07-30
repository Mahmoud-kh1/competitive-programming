#include <bits/stdc++.h>
using namespace std;
using int64 = long long;
static const int MOD = 1000000007;

// returns {F(n), F(n+1)} modulo MOD
pair<int64,int64> fib_pair(int64 n) {
    if (n == 0) 
        return {0, 1};
    auto [a, b] = fib_pair(n >> 1);
    // a = F(k), b = F(k+1)
    int64 c = (a * ((2*b % MOD - a + MOD) % MOD)) % MOD;  // F(2k)
    int64 d = ( (a*a) % MOD + (b*b) % MOD ) % MOD;        // F(2k+1)
    if (n & 1)
        // if n is odd, F(n) = d, F(n+1) = c + d
        return { d, (c + d) % MOD };
    else
        // if n is even, F(n) = c, F(n+1) = d
        return { c, d };
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int64 n;
    cin >> n;
    cout << fib_pair(n).first << "\n";
    return 0;
}
