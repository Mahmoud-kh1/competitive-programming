#include <bits/stdc++.h>
using namespace std;

using u64 = unsigned long long;
using u128 = __uint128_t;

static const int MOD1 = 998244353;
static const int MOD2 = 1004535809;
static const int MOD3 = 469762049;
static const int G = 3;

long long mod_pow(long long a, long long e, long long mod) {
    long long r = 1 % mod;
    while (e > 0) {
        if (e & 1) r = (__int128)r * a % mod;
        a = (__int128)a * a % mod;
        e >>= 1;
    }
    return r;
}

template<int MOD, int PRIMITIVE_ROOT>
void ntt(vector<int>& a, bool invert) {
    int n = (int)a.size();
    vector<int> rev(n);
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        rev[i] = j;
    }
    for (int i = 0; i < n; i++) {
        if (i < rev[i]) swap(a[i], a[rev[i]]);
    }

    for (int len = 2; len <= n; len <<= 1) {
        long long wlen = mod_pow(PRIMITIVE_ROOT, (MOD - 1) / len, MOD);
        if (invert) wlen = mod_pow(wlen, MOD - 2, MOD);

        for (int i = 0; i < n; i += len) {
            long long w = 1;
            int half = len >> 1;
            for (int j = 0; j < half; j++) {
                int u = a[i + j];
                int v = (long long)a[i + j + half] * w % MOD;
                int x = u + v;
                if (x >= MOD) x -= MOD;
                int y = u - v;
                if (y < 0) y += MOD;
                a[i + j] = x;
                a[i + j + half] = y;
                w = (__int128)w * wlen % MOD;
            }
        }
    }

    if (invert) {
        long long n_inv = mod_pow(n, MOD - 2, MOD);
        for (int &x : a) x = (__int128)x * n_inv % MOD;
    }
}

template<int MOD, int PRIMITIVE_ROOT>
vector<int> convolution_mod(const vector<u64>& A, const vector<u64>& B) {
    int n = (int)A.size(), m = (int)B.size();
    if (!n || !m) return {};
    int sz = 1;
    while (sz < n + m - 1) sz <<= 1;

    vector<int> fa(sz), fb(sz);
    for (int i = 0; i < n; i++) fa[i] = A[i] % MOD;
    for (int i = 0; i < m; i++) fb[i] = B[i] % MOD;

    ntt<MOD, PRIMITIVE_ROOT>(fa, false);
    ntt<MOD, PRIMITIVE_ROOT>(fb, false);
    for (int i = 0; i < sz; i++) fa[i] = (long long)fa[i] * fb[i] % MOD;
    ntt<MOD, PRIMITIVE_ROOT>(fa, true);

    fa.resize(n + m - 1);
    return fa;
}

static const u64 P1 = MOD1, P2 = MOD2, P3 = MOD3;
static const u64 INV_P1_MOD_P2 = mod_pow(P1, P2 - 2, P2);
static const u64 P12_MOD_P3 = (u128)P1 * P2 % P3;
static const u64 INV_P12_MOD_P3 = mod_pow(P12_MOD_P3, P3 - 2, P3);

u128 crt3(int r1, int r2, int r3) {
    long long t1 = (r2 - (long long)r1) % (long long)P2;
    if (t1 < 0) t1 += P2;
    t1 = (u128)t1 * INV_P1_MOD_P2 % P2;

    u128 x12 = (u128)r1 + (u128)P1 * (u64)t1;
    long long x12_mod_p3 = (long long)(x12 % P3);

    long long t2 = (r3 - x12_mod_p3) % (long long)P3;
    if (t2 < 0) t2 += P3;
    t2 = (u128)t2 * INV_P12_MOD_P3 % P3;

    return x12 + (u128)P1 * P2 * (u64)t2;
}

vector<u128> convolution_exact(const vector<u64>& A, const vector<u64>& B) {
    auto c1 = convolution_mod<MOD1, G>(A, B);
    auto c2 = convolution_mod<MOD2, G>(A, B);
    auto c3 = convolution_mod<MOD3, G>(A, B);

    int n = (int)c1.size();
    vector<u128> res(n);
    for (int i = 0; i < n; i++) {
        res[i] = crt3(c1[i], c2[i], c3[i]);
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    vector<u64> a(N), b(M);
    for (int i = 0; i < N; i++) cin >> a[i];
    for (int i = 0; i < M; i++) cin >> b[i];

    const u64 MASK32 = 0xffffffffULL;

    vector<u64> a0(N), a1(N), b0(M), b1(M), as(N), bs(M);
    for (int i = 0; i < N; i++) {
    
        a0[i] = a[i] & MASK32;
        a1[i] = a[i] >> 32; // change 32 to half of the power 
        as[i] = a0[i] + a1[i];
    }
    for (int i = 0; i < M; i++) {
        b0[i] = b[i] & MASK32;
        b1[i] = b[i] >> 32; // here too
        bs[i] = b0[i] + b1[i];
    }

    auto P = convolution_exact(a0, b0);
    auto Q = convolution_exact(a1, b1);
    auto R = convolution_exact(as, bs);

    int L = N + M - 1;
    for (int i = 0; i < L; i++) {
        u128 cross = R[i] - P[i] - Q[i];
        u128 ans = P[i] + (cross << 32);
        unsigned long long out = (unsigned long long)ans; // low 64 bits
        if (i) cout << ' ';
        cout << out;
    }
    cout << '\n';
    return 0;
}
