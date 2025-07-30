// sum of all pairs ((a[i] * a[j]) % mod)  the sumiiton is not under mod

#include <bits/stdc++.h>
using namespace std;
#define int long long
#define ll long long
using cd = complex<double>;
const double PI = acos(-1);
// this you should run first gernertor of ntt to get it with the mod
int g = 2;
void fft(vector<cd> & a, bool invert) {
    int n = a.size();

    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1)
            j ^= bit;
        j ^= bit;

        if (i < j)
            swap(a[i], a[j]);
    }

    for (int len = 2; len <= n; len <<= 1) {
        double ang = 2 * PI / len * (invert ? -1 : 1);
        cd wlen(cos(ang), sin(ang));
        for (int i = 0; i < n; i += len) {
            cd w(1);
            for (int j = 0; j < len / 2; j++) {
                cd u = a[i+j], v = a[i+j+len/2] * w;
                a[i+j] = u + v;
                a[i+j+len/2] = u - v;
                w *= wlen;
            }
        }
    }

    if (invert) {
        for (cd & x : a)
            x /= n;
    }
}

vector<int> multiply(vector<int> const& a, vector<int> const& b) {
    vector<cd> fa(a.begin(), a.end()), fb(b.begin(), b.end());
    int n = 1;
    while (n < (int)a.size() + (int)b.size())
        n <<= 1;
    fa.resize(n);
    fb.resize(n);

    fft(fa, false);
    fft(fb, false);
    for (int i = 0; i < n; i++)
        fa[i] *= fb[i];
    fft(fa, true);

    vector<int> result(n);
    for (int i = 0; i < n; i++)
        result[i] = round(fa[i].real());
    return result;
}
int mod = 200003;
int ge[200003];
int exp(int a, int b){
    int res = 1;
    while(b){
        if(b & 1){
            res = (res * a) % mod;
        }
        b/=2;
        a = (a * a) % mod;
    }
    return res;
}
signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
int t = 1;
//cin >> t;
    while(t--) {
         for(int i = 1; i < mod; i++){
             ge[exp(g, i)] = i;
         }
         int n; cin >> n;
         int a[n];
         vector<int>p(mod + 1);
         for(int i = 0; i < n;i++){
             cin >> a[i];
             if(!a[i]) continue;
             p[ge[a[i]]]++;
         }
         p = multiply(p, p);
         for(int i = 0; i < n; i++){
             if(a[i])
             p[2 * ge[a[i]]]--;
         }
         int res = 0;
         for(int i = 0; i < 2 * mod; i++){
             res += p[i] / 2 * exp(g, i);
         }
         cout << res;



    }

}
