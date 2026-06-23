#include <bits/stdc++.h>
using namespace std;

int z = 1e7 ;
vector<int> primes, lp(z + 1);
signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int cur = 1;
    for (int i = 2; i <= z; i++) {
        if (lp[i] == 0) {
            lp[i] = i;
            primes.push_back(i);
        }

        for (int p : primes) {
            if (p > lp[i] || 1LL * p * i > z) break;
            lp[p * i] = p;
        }
    }
    
    
}  
