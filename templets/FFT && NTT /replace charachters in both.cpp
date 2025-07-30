
// Suppose you have two strings s and t, and their length is equal. You may perform the following operation any number of times: choose two different characters c1 and c2, and replace every occurence of c1 in both strings with c2. Let's denote the distance between strings s and t as the minimum number of operations required to make these strings equal. For example, if s is abcd and t is ddcb, the distance between them is 2 — we may replace every occurence of a with b, so s becomes bbcd, and then we may replace every occurence of b with d, so both strings become ddcd.

// You are given two strings S and T. For every substring of S consisting of |T| characters you have to determine the distance between this substring and T.

#define _USE_MATH_DEFINES
#include <bits/stdc++.h>

using namespace std;
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

void fileIO(void) {
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
#ifdef ONLINE_JUDGE
    //    freopen("walls.in", "r", stdin);
#endif
}

void fastIO(void) {
    ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0);
}

#define endl '\n'
typedef unsigned long long ll;
#define int ll
typedef long double ld;

#define all(a)   a.begin(),a.end()
#define all_r(a)   a.rbegin(),a.rend()
#define flsh cout.flush()

#define N (ll)(2e5 + 5)
#define MOD (ll)(1e9 + 7)
const long long oo = 1e18;
const ld EPS = 1e-8;

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
        result[i] = round(fa[i].real());
    return result;
}

struct DSU {
    int n;
    int numberOfComponents;
    vector<int> parent, size;

    DSU(int sz) {
        n = sz;
        numberOfComponents = 0;
        parent.resize(n + 1);
        size.resize(n + 1);
        for(int u = 1; u <= n; ++u) {
            if (!size[u]) numberOfComponents++;
            parent[u] = u;
            size[u] = 1;
        }
    }


    int find(int u) {
        if(u == parent[u])
            return u;
        return parent[u] = find(parent[u]);
    }

    void merge(int u, int v) {
        int uP = find(u);
        int vP = find(v);

        if(vP == uP) return;

        if(size[vP] > size[uP]) swap(uP, vP);
        size[uP] += size[vP];
        parent[vP] = uP;
        numberOfComponents--;
    }
};

void solve(){
    string s,t;cin>>s>>t;
    int n=(int) s.length();
    int m=(int) t.length();
    vector<DSU>d(n-m+1, DSU(6));

    for(char c1='a';c1<='f';c1++){
        for(char c2='a';c2<='f';c2++){
            if(c1==c2)continue;
            vector<int>poly1(n+1),poly2(m+1);
            int sh=m;
            for(int i=0;i<n;i++){
                if(s[i]==c1)
                    poly1[i]=1;
            }
            for(int i=0;i<m;i++){
                if(t[i]==c2)
                    poly2[sh-i]=1;
            }
            vector<int>ans= multiply(poly1,poly2);
            for(int i=0;i<n-m+1;i++){
                if(ans[i+sh]){
                    d[i].merge(c1-'a',c2-'a');
                }
            }
        }
    }
    for(int i=0;i<n-m+1;i++){
        cout << 6 - d[i].numberOfComponents << " ";
    }
}


signed main() {
    fastIO(); fileIO();
    cout << fixed << setprecision(9);
    int test = 1, t = 1; //cin>>test;
    while (test--) {
//        cout << "Case #" << t << ": ";
        solve();
        t++;
    }
}
