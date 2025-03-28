#include "bits/stdc++.h"
using namespace std;

void fast() {
    ios::sync_with_stdio(0);
    cin.tie(NULL);
    cout.tie(0);
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif
}
#define int long long
struct Seg{
    vector<int>tree;
    vector<int>lazy;
    int sz = 0, buffer = 0;

    void build(int n){
        int old = n;
        if(old & (old - 1)) sz = (1 << (__lg(old) + 1));
        else sz = old;
        tree.resize(sz << 1, 0); lazy.resize(sz << 1, -1);
    }

    void propagate(int l , int r , int k){
        if (lazy[k] == -1) return;
        if (l != r){
            lazy[k << 1] = lazy[k];
            lazy[k << 1| 1] = lazy[k];
        }
        tree[k] = (lazy[k]  * (r - l + 1));
        lazy[k] = -1;
    }
    void update(int lx, int rx, int l , int r , int k, int value){
        propagate(l, r , k);
        if (l > rx || r < lx) return;
        if  (lx <= l && rx >= r){
            lazy[k] = value;
            propagate(l, r , k);
            return;
        }
        int mid = (l + r) / 2;
        update(lx, rx , l , mid, k << 1 , value);
        update(lx, rx , mid + 1, r, k << 1 | 1, value);
        tree[k] = tree[k << 1] + tree[k << 1 | 1];
    }
    int query(int lx ,int rx , int l, int r ,int k){
        propagate(l, r , k);
        if (l > rx || r < lx) return 0;
        if (lx <= l && rx >= r){
            return tree[k];
        }
        int mid = (l + r) / 2;
        return query(lx, rx , l , mid, k << 1 ) +
               query(lx, rx , mid + 1, r, k << 1 | 1);
    }
    int query(int l, int r){
        return query(l, r ,0 , sz - 1, 1);
    }
    void update(int l, int r, int val){
        update(l, r , 0, sz - 1, 1, val);
    }
};
const int N = 2e5;
vector<int>adj[N];
int timer, in[N], out[N];
void dfs(int node, int par){
    in[node] = timer++;
    for(auto i : adj[node]){
        if (i == par) continue;
        dfs(i, node);
    }
    out[node] = timer;
}
#define ll long long
using ul = uint64_t;
using db = long double;
class Pollard {
public:
    map<ul, int> cnt_primes;
    vector<ul> primes, divisors;
    ul modMul(ul a, ul b, const ul mod) {
        ll ret = a * b - mod * (ul)((db)a * b / mod);
        return ret + ((ret < 0) - (ret >= (ll)mod)) * mod;
    }
    ul modPow(ul a, ul b, const ul mod) {
        if (b == 0) return 1;
        ul res = modPow(a, b / 2, mod);
        res = modMul(res, res, mod);
        return b & 1 ? modMul(res, a, mod) : res;
    }

    bool rabin_miller(ul n) {  // not ll!
        if (n < 2 || n % 6 % 4 != 1) return n - 2 < 2;
        ul A[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022},
                s = __builtin_ctzll(n - 1), d = n >> s;
        for (auto a : A) {  // ^ count trailing zeroes
            ul p = modPow(a, d, n), i = s;
            while (p != 1 && p != n - 1 && a % n && i--) p = modMul(p, p, n);
            if (p != n - 1 && i != s) return 0;
        }
        return 1;
    }
    ul pollard(ul n) {  // return some nontrivial factor of n
        auto f = [n, this](ul x) { return modMul(x, x, n) + 1; };
        ul x = 0, y = 0, t = 30, prd = 2, i = 1, q;
        while (t++ % 40 ||
               __gcd(prd, n) == 1) {  /// speedup: don't take gcd every it
            if (x == y) x = ++i, y = f(x);
            if ((q = modMul(prd, max(x, y) - min(x, y), n))) prd = q;
            x = f(x), y = f(f(y));
        }
        return __gcd(prd, n);
    }
    void factor_rec(ul n, map<ul, int> &cnt) {
        if (n == 1) return;
        if (rabin_miller(n)) {
            ++cnt[n];
            return;
        }
        ul u = pollard(n);
        factor_rec(u, cnt), factor_rec(n / u, cnt);
    }
    void calcDivisorsRec(ul cur, int i) {
        if (i >= primes.size()) {
            divisors.push_back(cur);
            return;
        }
        int r = cnt_primes[primes[i]];
        for (int j = 0; j <= r; j++) {
            calcDivisorsRec(cur, i + 1);
            cur = cur * primes[i];
        }
    }
    void calcDivisors(ul x) {
        cnt_primes.clear();
        primes.clear();
        divisors.clear();
        factor_rec(x, cnt_primes);
        for (auto &u : cnt_primes) {
            primes.push_back(u.first);
        }
        calcDivisorsRec(1, 0);
    }
} pollard;
signed main() {
//    fast();
    int t = 1;
//    cin >> t;
    while (t--) {
           int n; cin >> n;
           int val[ n + 1];
           for(int i = 1; i <= n; i++) cin >> val[i];
            for(int i = 0; i < n-  1; i++){
                int u,v; cin >> u >> v;
                adj[u].push_back(v);
                adj[v].push_back(u);
            }
            dfs(1, -1);
            Seg seg; seg.build(timer);
            for(int i =1; i <= n; i++){
                seg.update(in[i], in[i], val[i]);
            }
            int q; cin >> q;
            while(q--){
                int ty; cin >> ty;
                if (ty == 2){
                    int u; cin >> u;
                    int sum = seg.query(in[u], out[u] - 1);
                    if (sum % 2 == 0){
                        if (sum == 2){
                            cout << "NO";
                        }
                        else cout << "YES";
                    }
                    else if (pollard.rabin_miller(sum - 2)){
                      cout << "YES";
                    }
                    else cout << "NO";

                    cout << endl;
                }
                else {
                    int u, vals; cin >> u >> vals;
                    seg.update(in[u], out[u ] - 1, vals);
                }
            }



    }

    return 0;
}
