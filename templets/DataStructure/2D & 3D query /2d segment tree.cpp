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

// query time is log(n) * log(m) , build is o(4nm) 
struct Node{
    int v;
    Node(){ v = 0;} // Deafult constructor Node
    Node(int x){ this->v = x; } 
    Node operator +(const Node &other) const{
        Node res;
        res.v = __gcd(v , other.v);
        return res;
    }
};
struct segTree_2d{
    vector<vector<Node>> t;
    int n , m;
    void init(int n, int m , vector<vector<int>> &g){
        /*
            Initial the grid[n][m]
        */
        this->n = n;
        this->m = m;
        int r = 1 , c = 1;
        while(r < n) r *= 2;
        while(c < m) c *= 2;
        t = vector<vector<Node>>(2 * r , vector<Node>(2 * c));    
        build(g);
    }
    void build_y(int vx, int lx, vector<vector<int>> &g ,int rx, int vy, int ly, int ry) {
        if (ly == ry) {
            if (lx == rx){
                t[vx][vy] = Node(g[lx][ly]);
            }
            else{
                t[vx][vy] = t[vx*2][vy] + t[vx*2+1][vy];
            }
        } else {
            int my = (ly + ry) / 2;
            build_y(vx, lx, g, rx, vy*2, ly, my);
            build_y(vx, lx, g, rx, vy*2+1, my+1, ry);
            t[vx][vy] = t[vx][vy*2] + t[vx][vy*2+1];
        }
    }
    void build_x(vector<vector<int>> &g , int vx, int lx, int rx) {
        if (lx != rx) {
            int mx = (lx + rx) / 2;
            build_x(g , vx*2, lx, mx);
            build_x(g , vx*2+1, mx+1, rx);
        }
        build_y(vx, lx, g, rx, 1, 0, m-1);
    }
    Node qry_y(int vx, int vy, int tly, int try_, int ly, int ry) {
        if (ly > ry) 
            return Node();
        if (ly == tly && try_ == ry)
            return Node(t[vx][vy]);
        int tmy = (tly + try_) / 2;
        return qry_y(vx, vy*2, tly, tmy, ly, min(ry, tmy)) + 
               qry_y(vx, vy*2+1, tmy+1, try_, max(ly, tmy+1), ry);
    }
 
    Node qry_x(int vx, int tlx, int trx, int lx, int rx, int ly, int ry) {
        if (lx > rx)
            return Node();
        if (lx == tlx && trx == rx)
            return qry_y(vx, 1, 0, m-1, ly, ry);
        int tmx = (tlx + trx) / 2;
        return qry_x(vx*2, tlx, tmx, lx, min(rx, tmx), ly, ry) + 
               qry_x(vx*2+1, tmx+1, trx, max(lx, tmx+1), rx, ly, ry);
    }
    void build(vector<vector<int>> &g){ build_x(g , 1 , 0 , n - 1); }
    int qry(int x , int y , int xx , int yy){ return qry_x(1 , 0 , n - 1 , x , xx , y , yy).v; }
};


const int mod = 1e9 + 7;
int main(){
    ios::sync_with_stdio(0); cin.tie(NULL); cout.tie(0);
    #ifndef ONLINE_JUDGE
    freopen("in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);
    freopen("error.txt", "w", stderr);
    #endif
    int tc; cin >> tc;
    while(tc--){
        int n , m; cin >> n >> m;
        vector<vector<int>> g(n , vector<int> (m));
        rep(i , 0 , n) rep(j ,0  , m) cin >> g[i][j];
        segTree_2d seg;
        seg.init(n , m , g);
        
        int q;cin>>q;
        ll res = 1;
        while(q--) {
            int l , r , k;cin>>l>>r>>k;
            l-- , r--;   
             // query from rows [l, l+ k  -1] and column from [r, r + k -1] 
            int g = seg.qry(l , r , l + k - 1 , r + k - 1);
            res = res * g % mod;
        }
        cout << res  << '\n';
    }
}
