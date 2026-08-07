// Step 1: Standard Sieve to find all primes up to sqrt(R)
vector<long long> simpleSieve(long long limit) {
    vector<bool> mark(limit + 1, true);
    vector<long long> primes;
    
    for (long long p = 2; p * p <= limit; p++) {
        if (mark[p]) {
            for (long long i = p * p; i <= limit; i += p) {
                mark[i] = false;
            }
        }
    }
    
    for (long long p = 2; p <= limit; p++) {
        if (mark[p]) {
            primes.push_back(p);
        }
    }
    return primes;
}

// Step 2: Segmented Sieve to find primes in the range [L, R]
void segmentedSieve(long long L, long long R) {
    // We only need primes up to sqrt(R) to sieve the range
    long long limit = sqrt(R);
    vector<long long> primes = simpleSieve(limit);

    // Create a boolean array for the range [L, R].
    // isPrime[i] will represent whether (i + L) is prime.
    vector<bool> isPrime(R - L + 1, true);

    for (long long p : primes) {
        // Find the smallest multiple of p that is >= L
        // We use max(p * p, ...) because any multiple smaller than p * p 
        // will have already been marked by a smaller prime factor.
        long long start = max(p * p, (L + p - 1) / p * p);

        // Mark all multiples of p in the range [L, R] as non-prime
        for (long long j = start; j <= R; j += p) {
            isPrime[j - L] = false;
        }
    }

    // Edge cases: 0 and 1 are not prime numbers
    if (L == 0) {
        if (R >= 0) isPrime[0 - L] = false;
        if (R >= 1) isPrime[1 - L] = false;
    } else if (L == 1) {
        isPrime[0] = false;
    }

    // Print all prime numbers in the range
    cout << "Primes in range [" << L << ", " << R << "]:\n";
    for (long long i = 0; i <= R - L; i++) {
        if (isPrime[i]) {
            cout << (i + L) << " ";
        }
    }
    cout << endl;
}
