// count divisor in n^(1 / 3)

N = input()
primes = array containing primes till 10^6
ans = 1
for all p in primes :
            if p*p*p > N:
                  break
            count = 1
            while N divisible by p:
                  N = N/p
                  count = count + 1
            ans = ans * count
if N is prime:
            ans = ans * 2
else if N is square of a prime:
            ans = ans * 3
else if N != 1:
            ans = ans * 4v

sum propeerts of GCD 
 gcd(a, lcm(b, c)) = lcm(gcd(a, b), gcd(a, c))

lcm(a, gcd(b, c)) = gcd(lcm(a, b), lcm(a, c)).

gcd(n^a-1,n^b-1)=n^gcd(a,b)-1 


// submask
for (int mask = 1;  mask < constrins; maks++){
for (int submask = mask; submask; submask = (submask - 1) & mask) {
        submasks.push_back(submask);
    }
}
time o(3 ^ n)
