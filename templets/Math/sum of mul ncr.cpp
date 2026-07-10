// we have a N elements that's divided to two sets we have to choose K elements form the N elements but we must at least choose X from the first set. 
// ans =  S C x * (n - S) C (k - x) + (SC(x + 1)) * (n - C) C (k - x - 1) and so on 

    vector<int> F(n + 1, 0);
 
    int X = k / 2; 
    int Y = 0; 
    for (int S = 1; S <= n; ++S) {
        long long gained = (1LL * ncr(S - 1, X - 1) * ncr(n - S, k - X)) % mod;
        long long lost = (1LL * ncr(S - 1, k - Y) * ncr(n - S, Y - 1)) % mod;
        long long term = (gained - lost) % mod;
        term = (term + mod) % mod;
        F[S] = (F[S - 1] + term) % mod;
    }

// we pre prossess for every S  and the answer if F[S] ,  the X that the minimum for set S and Y the minium for other set  and K is the number of elements we ant to choose on total 
