#include <bits/stdc++.h>
using namespace  std;
using cd = complex<double>;
const double PI = acos(-1);
int limit = 2e5;
const int mod  = 1009;
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
 
    vector<int> result(min(n, limit));
    for (int i = 0; i < min(n, limit); i++)
        result[i] = (int)round(fa[i].real());
    return result;
}
int matching(string &s, string &p) {
    int n = s.size();
    int m = p.size();
    vector<int>mat(n); 
    int sh = m - 1;
    int N = 1;
    while (N < n + m) N <<= 1;
    vector<vector<cd>>pa(26, vector<cd>(N));
    vector<vector<cd>>pb(26, vector<cd>(N));
    for (int i = 0; i < m; i++) pb[p[i] - 'a'][sh - i] = 1;
    for (int i = 0; i < n; i++) pa[s[i] - 'a'][i] = 1;
    for (int i = 0; i < 26; i++) {
        fft(pa[i], false);
        fft(pb[i], false);
    }
    int ans = 1e9;
    
    for (int k = 0; k <= 25; k++) {
        vector<cd>res(N, 0);
        for (int ch = 0; ch <= 25; ch++) {
            int tr = (ch + k)  % 26;
            for (int i = 0; i < N; i++) {
                res[i] += pa[tr][i] * pb[ch][i];
            }
        }
        fft(res, true);
        for (int i = 0; i < m; i++) {
            if (i + sh < N) {
                int matches = (int)round(res[sh + i].real());
                ans = min(ans, m - matches);
            }
        }
    }
  
   return ans;

}
signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t = 1; 
    // cin >> t;
    while (t--) {
         int n, m, k; cin >> n;
         string s, p; cin >> p >> s; 
         s += s;
       
         cout << matching(s, p);
        
        
        
    }
  
}
