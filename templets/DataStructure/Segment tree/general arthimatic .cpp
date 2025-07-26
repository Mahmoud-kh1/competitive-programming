#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Fenwick {
    int n;
    vector<ll> f;
    Fenwick(int _n): n(_n), f(n+1,0) {}

    // add v at index i
    void add(int i, ll v) {
        for(; i <= n; i += i & -i)
            f[i] += v;
    }
    // sum over [1..i]
    ll sum(int i) const {
        ll s = 0;
        for(; i > 0; i -= i & -i)
            s += f[i];
        return s;
    }
    // range add v over [l..r]
    void range_add(int l, int r, ll v) {
        add(l, v);
        if (r+1 <= n) add(r+1, -v);
    }
    // point query at i
    ll point(int i) const {
        return sum(i);
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    Fenwick bitC(n), bitD(n);

    while(m--){
        int t;
        cin >> t;
        if(t == 1){
            int l, r;
            ll a, d;
            cin >> l >> r >> a >> d;
            // decompose a + d*(i-l) = (a - d*l) + d*i
            ll c = a - d * l;
            bitC.range_add(l, r, c);
            bitD.range_add(l, r, d);
        } else {
            int i;
            cin >> i;
            // x_i = C + D * i
            ll C = bitC.point(i);
            ll D = bitD.point(i);
            cout << C + D * i << "\n";
        }
    }
    return 0;
}
