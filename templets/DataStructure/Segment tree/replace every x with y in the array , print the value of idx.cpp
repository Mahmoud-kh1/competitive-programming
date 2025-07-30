#include <bits/stdc++.h>

using namespace std;
#define ll long long
#define vll vector<ll>
#define pll pair<ll,ll>
#define vpll vector<pll>
#define Yarab_Accept ios_base::sync_with_stdio(false), cin.tie(NULL), cout.tie(NULL);
#define yes cout << "YES\n";
#define no cout << "NO\n";
#define all(x) (x).begin(),(x).end()
#define rall(x) (x).rbegin(),(x).rend()
#define pb emplace_back
#define test(n) while((n)--)
#define input(a) for(auto&i:a)cin>>i
#define print(a) for(auto&i:a) cout << i << " "; cout << '\n'
#define ld long double
#define inf LLONG_MAX
ll dx[] = {1,-1,0,0};
ll dy[] = {0,0,1,-1};
const ll N = 1e5+5;
vll v;
struct node{
    ll size;
    ll val;
};
int cmp(const node& u, const node& v){
    return (u.size > v.size) - (u.size < v.size);
}
void merge(node& v, node& u){
    v.size += u.size;
}
class DSU{
    private:
        vector<node> info;
        vll parent;
        ll N;
    public:
        void init(ll n){
            N = n+10;
            parent.resize(N);
            info.resize(N);
            for(ll i = 0; i < N; ++i){
                parent[i] = i;
                info[i] = {1,v[i]};
            }
        }
        ll find(ll u){
            if(u == parent[u]) return u;
            return parent[u] = find(parent[u]);
        }
        bool union_set(ll u, ll v){
            ll a = find(u);
            ll b = find(v);
            if(a != b){
                if(cmp(info[a], info[b]) == -1) swap(a,b);
                parent[b] = a;
                merge(info[a],info[b]);
                return 1;
            }
            return 0;
        }
        void update(ll u, ll val){
            u = find(u);
            info[u].val = val;
        }
        const ll& get_info(ll u){
            return info[find(u)].val;
        }
        bool connected(ll u, ll v){
            return find(u) == find(v);
        }
};

void solve(ll t)
{
    ll n, q; cin >> n >> q;
    v.resize(n);
    unordered_map<ll,ll> mp;
    for(ll i = 0; i < n; ++i){
        cin >> v[i];
    }
    DSU d; d.init(n);
    for(ll i = 0; i < n; ++i){
        if(mp.find(v[i]) == mp.end()){
            mp[v[i]] = i;
            continue;
        }
        d.union_set(i,mp[v[i]]);
    }
    ll op, x, y, idx;
    cout << "Case " << t << ":\n";
    while(q--){
        cin >> op >> x;
        if(op == 1){
            cin >> y;
            if(mp.find(x) == mp.end()) continue;
            idx = mp[x];
            d.update(idx,y);
            if(mp.find(y) != mp.end()) d.union_set(mp[y],idx);
            mp.erase(x);
            mp[y] = idx;
        } else{
            cout << d.get_info(x-1) << '\n';
        }
    }
}
int main()
{
    Yarab_Accept
    int t = 1;
    cin >> t;
    ll cnt = 1;
    test(t) solve(cnt++);
    return 0;
}
