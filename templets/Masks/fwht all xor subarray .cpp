#include <bits/stdc++.h>
using namespace std;
static const int MOD = 1000000007;
using ll = long long;

// modular add/sub
inline int add(int a, int b){ a += b; if(a >= MOD) a -= MOD; return a; }
inline int sub(int a, int b){ a -= b; if(a < 0) a += MOD; return a; }

// fast exponentiation
ll modexp(ll a, ll e=MOD-2){
    ll r = 1;
    while(e){
        if(e & 1) r = r * a % MOD;
        a = a * a % MOD;
        e >>= 1;
    }
    return r;
}

// in‐place FWHT on a (length must be a power of two)
// f = 0: AND, 1: OR, 2: XOR
// inv = false => forward, true => inverse
void fwht(vector<int> &a, bool inv, int f){
    int n = a.size();
    for(int len = 1; len < n; len <<= 1){
        for(int i = 0; i < n; i += len<<1){
            for(int j = 0; j < len; ++j){
                int u = a[i + j];
                int v = a[i + j + len];
                if(f == 0){
                    // AND
                    if(!inv){
                        a[i + j]     = v;
                        a[i + j + len] = add(u, v);
                    } else {
                        // inverse AND
                        a[i + j]     = sub(v, u);
                        a[i + j + len] = u;
                    }
                }
                else if(f == 1){
                    // OR
                    if(!inv){
                        a[i + j]     = add(u, v);
                        a[i + j + len] = u;
                    } else {
                        // inverse OR
                        a[i + j]     = v;
                        a[i + j + len] = sub(v, u);
                    }
                }
                else {
                    // XOR (same for forward & inverse)
                    a[i + j]        = add(u, v);
                    a[i + j + len]  = sub(u, v);
                }
            }
        }
    }
}

// convolution under AND/OR/XOR
vector<int> convolution(vector<int> A, vector<int> B, int f){
    int n = A.size();
    fwht(A, false, f);
    fwht(B, false, f);
    vector<int> C(n);
    for(int i = 0; i < n; i++){
        C[i] = (ll)A[i] * B[i] % MOD;
    }
    fwht(C, true, f);
    if(f == 2){
        // only XOR needs the 1/n factor
        ll inv_n = modexp(n);
        for(int &x : C) x = x * inv_n % MOD;
    }
    return C;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> a(n);
    for(int i = 0; i < n; i++){
        cin >> a[i];
        if(i) a[i] ^= a[i-1];        // prefix‐xor
    }

    int mx = *max_element(a.begin(), a.end());
    int freq[mx + 1]{};
    int N = 1;
    while(N <= mx) N <<= 1;

    // build indicator polynomial of size N
    vector<int> poly(N, 0);
    vector<int>ans;
    poly[0] = 1;                    // include empty prefix
    for(int x : a) {
        poly[x]= 1;
        freq[x]++;
        // just for zero case 
        if(freq[x] == 2 || x == 0){
            ans.push_back(0);
        }
    }

    // compute XOR‐convolution
    auto res = convolution(poly, poly, 2);
    // print all i with res[i] > 0
    bool first = true;
    for(int i = 1; i < N; i++){
        if(res[i]){
           ans.push_back(i);
        }
    }
    sort(ans.begin(), ans.end());
    ans.erase(unique(ans.begin(), ans.end()), ans.end());
    cout << ans.size() << endl;
    for(auto j : ans) cout << j << ' ';

    cout << "\n";
    return 0;
}
