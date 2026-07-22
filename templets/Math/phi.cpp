struct TotientSieve {
    int MAX_N;
    long long MOD;
    vector<int> phi;
    vector<long long> phi_fact;

    // Constructor precomputes everything in O(N log log N)
    TotientSieve(int max_n, long long mod = 1e9 + 7) : MAX_N(max_n), MOD(mod) {
        phi.assign(MAX_N + 1, 0);
        phi_fact.assign(MAX_N + 1, 0);
        
        // 1. Initialize phi array
        for (int i = 0; i <= MAX_N; i++) {
            phi[i] = i;
        }

        // 2. Sieve for standard phi(N)
        for (int i = 2; i <= MAX_N; i++) {
            // If phi[i] hasn't changed, i is prime
            if (phi[i] == i) { 
                for (int j = i; j <= MAX_N; j += i) {
                    phi[j] -= phi[j] / i;
                }
            }
        }

        // 3. Compute phi(N!) modulo MOD
        phi_fact[0] = 1;
        if (MAX_N >= 1) phi_fact[1] = 1;
        
        for (int i = 2; i <= MAX_N; i++) {
            // Shortcut: i is prime if and only if phi[i] == i - 1
            if (phi[i] == i - 1) { 
                phi_fact[i] = (phi_fact[i - 1] * (i - 1)) % MOD;
            } else {
                phi_fact[i] = (phi_fact[i - 1] * i) % MOD;
            }
        }
    }
    
    // O(1) query for standard Totient
    int get_phi(int n) {
        return phi[n];
    }

    // O(1) query for Factorial Totient (Modulo)
    long long get_phi_fact(int n) {
        return phi_fact[n];
    }
};
