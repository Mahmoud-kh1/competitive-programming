#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
#define rep(i , st , ed) for(int i = st; i < ed; i++)
#define f first
#define s second
#define all(v) v.begin() , v.end()
#ifndef ONLINE_JUDGE 
#define debug(x) cerr << #x << ": " << x << '\n';
#else
#define debug(x)
#endif
template<class T = int>
struct Sparetable{
    vector<vector<T>> mx;
    int n , LOG;
    void init(vector<T> &a){
        this-> n = (int) a.size();
        this->LOG = 0;
        int size = 1;
        while(size <= n) size *= 2 , LOG++;
        mx.assign(n , vector<T>(LOG));
        for (int i = 0; i < n; i++)mx[i][0] = a[i];
        for (int j = 1; (1 << j) <= n; j++){
            for (int i = 0; (i + (1 << j) - 1) < n; i++){
                mx[i][j] = merge(mx[i][j - 1] , mx[i + (1 << (j - 1))][j - 1]);
            }
        }
    }
    T merge(T a , T b){
        return a | b; // change the operation
    }
    T qry(int l, int r){
        int len = r - l + 1;
        int j = 31 - __builtin_clz(len);
        T res = merge(mx[l][j] , mx[r - (1 << j) + 1][j]);
        return res; // determine what you want to return 
    }
};
template<class T = int>
struct BIT{
    vector<T> tree;
    int n;
    void init(int n){
        this->n = n;
        tree.assign(n , 0);
    }
    void add(int pos , T val){
        for(pos++; pos <= n ; pos += (pos & (-pos)))
            tree[pos - 1] += val;
    }
    T sum(int pos){
        T ret = 0;
        for(pos++; pos ; pos -= (pos & (-pos)))
            ret += tree[pos - 1];
        return ret;
    }
    T qry(int l , int r){ return sum(r) - sum(l - 1); }
    T qry(int i){ return sum(i , i); }
};
int main(){
    ios::sync_with_stdio(0); cin.tie(NULL); cout.tie(0);
    #ifndef ONLINE_JUDGE
    freopen("in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);
    freopen("error.txt", "w", stderr);
    #endif
    int n , q; cin >> n >> q;
    vector<ll> a(n);
    for(int i = 0; i < n; ++i) cin >> a[i];
    Sparetable<ll> spt; spt.init(a);
    vector<pair<int,int>> query[n];
    int ans[q];
    for(int i = 0; i < q; ++i){
        int l , r; cin >> l >> r;
        --l; --r;
        query[r].emplace_back(l , i);
    }
    BIT<int> B; B.init(n);
    map<ll,int> lt;
    for(int j = 0; j < n; ++j){
        int i = 0;
        while(i <= j){
            int tar = spt.qry(i , j);
            int lo = i , hi = j , mid;
            while(lo < hi){
                mid = lo + (hi - lo + 1) / 2;
                if(spt.qry(mid , j) == tar) lo = mid;
                else hi = mid - 1;
            }
            if( !lt.count(tar) || lt[tar] < lo ){
                if(lt.count(tar)) B.add(lt[tar] , -1);
                lt[tar] = lo;
                B.add(lt[tar] , 1);
            }
            i = lo + 1;
        }
 
        for(auto &[i , idx] : query[j]){
            ans[idx] = B.qry(i , j);
        }
    }
    for(int i = 0; i < q; ++i){
        cout << ans[i] << '\n';
    }
}
