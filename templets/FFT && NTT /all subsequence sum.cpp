
#include <bits/stdc++.h>
using namespace std;
#define int long long
using cd = complex<double>;
const double PI = acos(-1);

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
        result[i] = (int)round(fa[i].real());
    return result;
}
string mul_two_big_int(const string &s1, const string &s2) {
    int n = s1.size(), m = s2.size();

    vector<int> poly1(n), poly2(m);
    for (int i = 0; i < n; ++i) {
        poly1[n-i-1] = s1[i] - '0';
    }

    for (int i = 0; i < m; ++i) {
        poly2[m-i-1] = s2[i] - '0';
    }

    vector<int> ans = multiply(poly1, poly2);
    int k = ans.size();

    for (int i = 0; i < k - 1; ++i) {
        ans[i + 1] += ans[i] / 10;
        ans[i] = ans[i] % 10;
    }

    string final = to_string(ans[k - 1]);
    for (int i = k - 2; i >= 0; --i) {
        final += (char)(ans[i] + '0');
    }

    for (int i = 0; i < k; ++i) {
        if(final[i] != '0') return final.substr(i);
    }
    return "0";
}

string power_of_big_int(string s, int p) {
    string ans = "1";
    while (p) {
        if(p&1) ans = mul_two_big_int(ans, s);
        s = mul_two_big_int(s, s);
        p >>= 1;
    }
    return ans;
}
vector<int> power(vector<int>&a, int b){
    vector<int>res ={1};
    while(b){
        if(b & 1){
            res = multiply(res, a);
        }
        b/=2;
        a = multiply(a, a);
    }
    return res;
}

pair<int,vector<int>> matching (string &s, string &pat){
    int n = s.size();
    int m = pat.size();

    vector<int>mat(n);
    for(auto ch : {'a', 'b', 'c'}){
        vector<int>pa(n);
        vector<int>pb(m);
        for(int i = 0;i < n; i++){
            pa[i]= (s[i] == ch);
        }
        for(int j = 0; j < m; j++){
            pb[m - j - 1] =(pat[j] == ch);
        }
        vector<int>res = multiply(pa, pb);
        for (int i = 0; i < n; i++) {
            mat[i] += res[i + m - 1];
        }
    }
    int mx=-1, ind = -1;
    for(int i = 1; i < mat.size(); i++){
        if(mat[i] > mx){
            mx = mat[i];
        }
    }
    vector<int>ans;
    for(int i = 1; i < mat.size(); i++){
        if(mat[i] == mx){
            ans.push_back(i);
        }
    }
    return {mx, ans};

}
signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t = 1;
cin >> t;
    while(t--) {
        int n; cin >> n;
        vector<vector<int>>polys;
        polys.reserve(n);
        for(int i =0 ; i < n; i++){
            int v; cin >>v;
            vector<int>cur(v + 1);
            
            // we represent each number as poly  (1 + x^v) and multiply 
            // suppose we only from each number we can take one only 
            // so we make the poly like (1 + freq[v] * x^v) 
            
            // suppose there is gain for taking element and one for untaking it 
            // (g1 + g2 * x^v)
            cur[v]   = 1; cur[0] = 1;
            polys.push_back(cur);
        }

        int m = polys.size();
        for(int len = 1; len < m; len <<= 1){
            for(int i = 0; i  + len < m; i+=2*len){
                polys[i] = multiply(polys[i], polys[i + len]);
            }
        }
        auto res = polys[0];
        for(int i = 0; i < res.size(); i++){
            cout << "sum :" << i << " ways:" << res[i] << endl;
        }
    }

}

