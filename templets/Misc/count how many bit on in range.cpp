//Count howmany ones from 1 to n in the bit
ll calc_ones(ll a, ll bit){
    ++bit;
    ll ones = a/(1ll << bit) * (1ll << (bit - 1));
    if(a % (1ll << bit) >= (1ll << (bit - 1))) ones += a % (1ll << bit) - (1ll << (bit - 1)) + 1;
    return ones;
} 
/ you can use in and on range or or or xor 
