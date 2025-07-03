#include <bits/stdc++.h>
using namespace std;
#define  int long long
const int N = 2e5;
struct HLD{
    vector<int>big, head, sz, par, depth, in, pos, idchain, chainsz;
    int chains = 0;
    HLD(int n, int root, vector<int>adj[]) {
        big.resize(n + 1);
        head.resize(n + 1);
        sz.resize(n + 1);
        par.resize(n + 1);
        in.resize(n + 1);
        depth.resize(n + 1);
        pos.resize(n + 1);
        idchain.resize(n + 1);
        chainsz.resize(n + 1);

        depth[root] = 0;
        head[root] = root;
        idchain[root] = 0;
        pos[root] = 0;
        chainsz[0] = 1;
        dfs(root, 0, adj);
        int timer = 0;
        flat(root, adj, timer);
    }
    void dfs(int u, int p, vector<int>adj[]){
        sz[u] = 1;
        for(auto v : adj[u]){
            if (v == p) continue;
            depth[v] = depth[u] + 1;
            par[v] = u;
            dfs(v, u, adj);
            sz[u] += sz[v];
            if (!big[u] || sz[big[u]] < sz[v]) big[u] = v;
        }
    }
    void flat(int u, vector<int>adj[], int &timer){
        in[u] = timer++;
        if(big[u]){
            idchain[big[u]] = idchain[u];
            pos[big[u]] = pos[u] + 1;
            chainsz[idchain[u]]++;
            head[big[u]] = head[u], flat(big[u], adj, timer);
        }
        for(auto v : adj[u]){
            if (v == par[u] || big[u] == v) continue;
            head[v] = v;
            chains++;
            idchain[v] = chains;
            chainsz[chains] = 1;
            pos[v] = 0;
            flat(v, adj, timer);
        }
    }

//    int  get(int u , int v){
//        int res = 0;
//        while(true){
//            if (head[u] == head[v]){
//                if (depth[v] < depth[u]) swap(u,v);
//               res = max(res, )
//                return res;
//            }
//            if (depth[head[u]] > depth[head[v]]) swap(u, v);
//            res.push_back({in[head[v]], in[v]});
//            v = par[head[v]];
//        }
//    }
};
struct Seg {
    // 0 indexed
    int n;
    vector<int> seg;
    static constexpr int iden = 0;

    static int merge(const int &a, const int &b) {
        return max(a, b);
    }
    Seg() = default;
    Seg(int _n) : n(_n), seg(2 * n + 5, iden) {}

    void build(const vector<int> &a) {
        for (int i = 0; i < n; i++)
            seg[n + i] = a[i];
        for (int i = n - 1; i > 0; i--)
            seg[i] = merge(seg[i<<1], seg[i<<1|1]);
    }

    void update(int p, int v) {
        p += n;
        seg[p] = v;
        for (p >>= 1; p > 0;     p >>= 1)
            seg[p] = merge(seg[p<<1], seg[p<<1|1]);
    }

    int query(int l, int r) const {
        r++; // becase query(l, r) = [l, r[
        int resl = iden, resr = iden;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) resl = merge(resl, seg[l++]);
            if (r & 1) resr = merge(seg[--r], resr);
        }
        return merge(resl, resr);
    }
};
signed main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int t = 1;
//    cin >> t;
    while(t--){
        int n, q; cin >> n >> q;
        int val[n + 1]{};
        for(int i = 1; i <= n; i++) cin >> val[i];
        vector<int>adj[n  + 1];
        for(int i = 1; i < n; i++){
            int u, v; cin >> u >> v;
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
        HLD hd(n, 1, adj);
        vector<Seg>seg(hd.chains + 1);
        for(int i = 0; i < hd.chains + 1; i++){
            seg[i] = Seg(hd.chainsz[i]);
        }

        for(int i = 1; i <= n; i++){
             seg[hd.idchain[i]].update(hd.pos[i], val[i]);
        }
        while(q--){
            int ty; cin >> ty;
            if(ty == 1){
                int s, x; cin >> s >> x;
                seg[hd.idchain[s]].update(hd.pos[s], x);
            }
            else{
                int res = 0;
                int u, v; cin >> u >> v;
                while(true){
                    if (hd.head[u] == hd.head[v]){
                        if (hd.depth[u] > hd.depth[v]) swap(u, v);
                        res = max(res, seg[hd.idchain[u]].query(hd.pos[u], hd.pos[v]));
                        break;
                    }
                    if (hd.depth[hd.head[u]] > hd.depth[hd.head[v]]) swap(u, v);
                    res = max(res, seg[hd.idchain[v]].query(hd.pos[hd.head[v]], hd.pos[v]));
                    v = hd.par[hd.head[v]];
                }
                cout << res << " ";
            }
        }

    }

    return 0;









}
