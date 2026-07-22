#include <bits/stdc++.h>
using namespace std;
const int mod = 998244353;
#define int long long
#define all(a) a.begin(), a.end()
const int MAXW = 2e5;
struct MobiusGcdSolver {

    int mu[MAXW];
    bool is_prime[MAXW];
    vector<int> primes;
    vector<int> divisors[MAXW];
    
    int cnt[MAXW];      // Frequency of each exact value in the multiset
    int mult_cnt[MAXW]; // Number of elements in the multiset that are multiples of d

    void sieve() {
        fill(is_prime, is_prime + MAXW, true);
        is_prime[0] = is_prime[1] = false;
        mu[1] = 1;
        for (int i = 2; i < MAXW; ++i) {
            if (is_prime[i]) {
                primes.push_back(i);
                mu[i] = -1;
            }
            for (int p : primes) {
                if (i * p >= MAXW) break;
                is_prime[i * p] = false;
                if (i % p == 0) {
                    mu[i * p] = 0;
                    break;
                } else {
                    mu[i * p] = -mu[i];
                }
            }
        }
    }

    void precompute_divisors() {
        for (int i = 1; i < MAXW; ++i) {
            for (int j = i; j < MAXW; j += i) {
                divisors[j].push_back(i);
            }
        }
    }

public:
    // Time Complexity: O(MAXW log MAXW)
    // - Linear sieve runs in O(MAXW) time.
    // - Divisor precomputation takes O(MAXW log MAXW) via harmonic series sum.
    MobiusGcdSolver() {
        sieve();
        precompute_divisors();
        fill(cnt, cnt + MAXW, 0);
        fill(mult_cnt, mult_cnt + MAXW, 0);
    }

    // Time Complexity: O(d(v)) where d(v) is the number of divisors of v.
    // - Loops through all precomputed divisors of v to increment mult_cnt.
    void add(int v) {
        cnt[v]++;
        for (int d : divisors[v]) {
            mult_cnt[d]++;
        }
    }

    // Time Complexity: O(d(v)) where d(v) is the number of divisors of v.
    // - Loops through all precomputed divisors of v to decrement mult_cnt.
    void remove(int v) {
        if (cnt[v] == 0) return;
        cnt[v]--;
        for (int d : divisors[v]) {
            mult_cnt[d]--;
        }
    }

    // Time Complexity: O(d(X)) where d(X) is the number of divisors of X.
    // - Iterates only over the precomputed divisors of X to apply Möbius inversion.
    long long count_coprime(int X) {
        long long ans = 0;
        for (int d : divisors[X]) {
            ans += 1LL * mu[d] * mult_cnt[d];
        }
        return ans;
    }

    // Time Complexity: O(d(n / x)) where d(n / x) is the number of divisors of K = n / x.
    // - Performs an O(1) divisibility check, then iterates over divisors of K.
    long long count_gcd(int n, int x) {
        if (n % x != 0) return 0;
        int K = n / x;
        long long ans = 0;
        for (int d : divisors[K]) {
            ans += 1LL * mu[d] * mult_cnt[d * x];
        }
        return ans;
    }
};
MobiusGcdSolver mob; 

signed main() {
  
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
   
    
    int T = 1;
    //    cin >> T;
    Z: while (T--) {
        int n; cin >> n;
        int val[n + 1];
        vector<vector<int>>node(MAXW + 1);
        for (int i = 1; i <= n; i++) {
            cin >> val[i];
            for (auto d : mob.divisors[val[i]]) {
                node[d].push_back(i);
            }
        }
        vector<vector<pair<int,int>>>edges(MAXW + 1);
        for (int i = 0; i < n - 1; i++) {
            int u, v; cin >> u >> v;
            int g = __gcd(val[u], val[v]);
            for (auto d : mob.divisors[g]) {
                edges[d].push_back({u, v});
            }
        }
        int ans = 0; 
        vector<int>sz,par;
       
            sz.resize(n + 1);
            par.resize(n + 1);
            for (int i = 1; i <= n; i++) {
                sz[i] = 1;
                par[i] = i;
            }
        
        function<int(int)>  find = [&] (int u)->int {
            if (u == par[u]) return u;
            return par[u] = find(par[u]);
        };
        auto  merge= [&] (int u, int v) {
            u = find(u);
            v = find(v);
            if (u == v) return; 
            if (sz[v] > sz[u]) swap(u, v);
            sz[u] += sz[v];
            par[v] = u;
        };
        
        for (int gc = 1; gc < MAXW; gc++) {
            if (mob.mu[gc] == 0) continue; 
            for (auto u : node[gc]) {
                sz[u] = 1;
                par[u] =u;
            }
            for (auto [u, v] : edges[gc]) {
                merge(u, v);
            }
            int all = 0;
            for (auto u : node[gc]) {
                if (u == find(u)) {
                    all+= sz[u] * sz[u];
                  
                }
            }
            ans += mob.mu[gc] * all;
        }
        cout << ans << endl;
        
        
        
        
        
        


    }
    return 0;

}
