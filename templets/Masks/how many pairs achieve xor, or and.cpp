#include <bits/stdc++.h>
using namespace std;
static const int MOD = 1000000007;
using ll = long long;

//--- modular arithmetic ---
inline int add(int a, int b) { a += b; if (a >= MOD) a -= MOD; return a; }
inline int sub(int a, int b) { a -= b; if (a <  0) a += MOD; return a; }
ll modexp(ll a, ll e = MOD-2) {
    ll r = 1;
    while (e) {
        if (e & 1) r = r * a % MOD;
        a = a * a % MOD;
        e >>= 1;
    }
    return r;
}

//--- in‐place FWHT over size‐n (power of two) ---
// f = 0: AND, 1: OR, 2: XOR
// inv = false: forward, true: inverse
void fwht(vector<int> &A, bool inv, int f) {
    int n = A.size();
    for (int len = 1; len < n; len <<= 1) {
        for (int i = 0; i < n; i += len << 1) {
            for (int j = 0; j < len; ++j) {
                int u = A[i + j], v = A[i + j + len];
                if (f == 0) {
                    // AND
                    if (!inv) {
                        A[i + j]       = v;
                        A[i + j + len] = add(u, v);
                    } else {
                        A[i + j]       = sub(v, u);
                        A[i + j + len] = u;
                    }
                }
                else if (f == 1) {
                    // OR
                    if (!inv) {
                        A[i + j]       = add(u, v);
                        A[i + j + len] = u;
                    } else {
                        A[i + j]       = v;
                        A[i + j + len] = sub(v, u);
                    }
                }
                else {
                    // XOR (same forward & inverse, normalization later)
                    A[i + j]       = add(u, v);
                    A[i + j + len] = sub(u, v);
                }
            }
        }
    }
}

// Returns the convolution A⊕B under op f (0=AND,1=OR,2=XOR)
vector<int> convolution(vector<int> A, vector<int> B, int f) {
    int n = A.size();
    fwht(A, false, f);
    fwht(B, false, f);
    vector<int> C(n);
    for (int i = 0; i < n; i++)
        C[i] = (ll)A[i] * B[i] % MOD;
    fwht(C, true, f);
    if (f == 2) {
        // XOR needs division by n
        ll inv_n = modexp(n);
        for (int &x : C) x = x * inv_n % MOD;
    }
    return C;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;
    vector<int> a(n);
    int mx = 0;
    for (int i = 0; i < n; i++) {
        cin >> a[i];
        mx = max(mx, a[i]);
    }

    // next power-of-two ≥ mx
    int N = 1;
    while (N <= mx) N <<= 1;

    // build frequency array
    vector<int> freq(N, 0);
    for (int x : a) freq[x]++;

    // do three convolutions
    auto C_xor = convolution(freq, freq, 2);
    auto C_and = convolution(freq, freq, 0);
    auto C_or  = convolution(freq, freq, 1);

    // extract unordered‐pair counts for target k
    // ordered pairs include (i,i), and count each (i,j) twice
    ll cnt_xor = C_xor[k];
    if (k == 0) {
        // self‐pairs (i,i) all satisfy a[i]^a[i]=0
        // there are n of those in C_xor[0]
        cnt_xor = (cnt_xor - n + MOD) % MOD;
    }
    cnt_xor /= 2;

    ll cnt_and = C_and[k];
    // self‐pairs satisfying a[i]&a[i]=a[i]==k are exactly freq[k]
    cnt_and = (cnt_and - freq[k] + MOD) % MOD;
    cnt_and /= 2;

    ll cnt_or = C_or[k];
    // self‐pairs satisfying a[i]|a[i]=a[i]==k are exactly freq[k]
    cnt_or = (cnt_or - freq[k] + MOD) % MOD;
    cnt_or /= 2;

    cout << cnt_xor << " " << cnt_and << " " << cnt_or << "\n";
    return 0;
}
