int xorUpTo(int n) {
    if (n % 4 == 0) return n;
    if (n % 4 == 1) return 1;
    if (n % 4 == 2) return n + 1;
    return 0;
}
it will return xor in range 0 to n 
if you want l, r handle it as prefix 
