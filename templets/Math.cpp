search for all version for solve_diophantine equation 
all templets that do magic things 

  fib rules  spically shift by pos, and negative 

gcd(fib[a] , fib[b]) = fib[gcd(a,b)] 


https://cp-algorithms.com/algebra/phi-function.html  important for (a^b) mod m 



long long safe_mul(long long a, long long b, long long mod) {
    a %= mod;
    b %= mod;
    long long result = 0;
    while (b > 0) {
        if (b & 1) {
            result = (result + a) % mod;
        }
        a = (a + a) % mod;
        b >>= 1;
    }
    return result;
} safe mul to compute (a * b) % mod without overflow 

rotation array 90 180 270
