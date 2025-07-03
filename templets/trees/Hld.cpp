#include <bits/stdc++.h>
using namespace std;
#define  int long long
const int N = 2e5;

#define ll long long
struct Node {
    ll sum;
    ll maxPref;
    ll maxSuf;
    ll maxSeg;
    int len;
    int mnSeg, mnPref, mnSuf;
    Node(): sum(0), maxPref(0), maxSuf(0), maxSeg(0), len(0), mnSeg(1e11), mnPref(1e11), mnSuf(1e11){}
    Node(ll v) : sum(v), maxPref(max(v,0LL)), maxSuf(max(v,0LL)), maxSeg(max(v,0LL)), len(1) , mnSeg(min(v, 0ll)), mnPref(min(v, 0ll)), mnSuf(min(v, 0ll)) {}

};

struct SegTree {
    int n;
    vector<Node> seg;
    vector<bool> lazyFlag;
    vector<ll> lazyVal;
    SegTree() = default;
    SegTree(int _n) : n(_n) {
        seg.assign(4*n, Node());
        lazyFlag.assign(4*n, false);
        lazyVal.assign(4*n, 0);
        initLen(1, 0, n - 1);
    }
    void initLen(int idx, int l, int r) {
        seg[idx].len = r - l + 1;
        if (l == r) return;
        int mid = (l + r) >> 1;
        initLen(idx * 2, l, mid);
        initLen(idx * 2 + 1, mid + 1, r);
    }
    static Node merge(const Node &a, const Node &b) {
        Node res;
        res.len = a.len + b.len;
        res.sum = a.sum + b.sum;
        res.maxPref = max(a.maxPref, a.sum + b.maxPref);
        res.maxSuf = max(b.maxSuf, b.sum + a.maxSuf);
        res.maxSeg = max({a.maxSeg, b.maxSeg, a.maxSuf + b.maxPref});

        res.mnPref = min(a.mnPref, a.sum + b.mnPref);
        res.mnSuf = min(b.mnSuf, b.sum + a.mnSuf);
        res.mnSeg = min({a.mnSeg, b.mnSeg, a.mnSuf + b.mnPref});

        return res;
    }

    void applyAssign(int idx, ll v) {
        // assign all in this node to v
        seg[idx].sum = v * seg[idx].len;
        seg[idx].maxPref = seg[idx].maxSuf = seg[idx].maxSeg = max(v, 0LL) * seg[idx].len;
        lazyFlag[idx] = true;
        lazyVal[idx] = v;
    }

    void push(int idx) {
        if (!lazyFlag[idx]) return;
        applyAssign(idx*2, lazyVal[idx]);
        applyAssign(idx*2+1, lazyVal[idx]);
        lazyFlag[idx] = false;
    }

    void build(int idx, int l, int r, const vector<ll> &a) {
        if (l == r) {
            seg[idx] = Node(a[l]);
            return;
        }
        int mid = (l + r) >> 1;
        build(idx*2, l, mid, a);
        build(idx*2+1, mid+1, r, a);
        seg[idx] = merge(seg[idx*2], seg[idx*2+1]);
    }

    void updateAssign(int idx, int l, int r, int ql, int qr, ll v) {
        if (ql > r || qr < l) return;
        if (ql <= l && r <= qr) {
            seg[idx] = Node(v);
            return;
        }
        int mid = (l + r) >> 1;
        updateAssign(idx*2, l, mid, ql, qr, v);
        updateAssign(idx*2+1, mid+1, r, ql, qr, v);
        seg[idx] = merge(seg[idx*2], seg[idx*2+1]);
    }

    Node query(int idx, int l, int r, int ql, int qr) {
        if (ql > r || qr < l) return Node();
        if (ql <= l && r <= qr) return seg[idx];
        int mid = (l + r) >> 1;
        if (qr <= mid) return query(idx*2, l, mid, ql, qr);
        if (ql > mid) return query(idx*2+1, mid+1, r, ql, qr);
        Node left = query(idx*2, l, mid, ql, qr);
        Node right = query(idx*2+1, mid+1, r, ql, qr);
        return merge(left, right);
    }

    // Public wrappers:
    void build(const vector<ll> &a) {
        build(1, 0, n-1, a);
    }
    void update(int l, int r, ll v) {
        updateAssign(1, 0, n-1, l, r, v);
    }
    Node query(int l, int r) {
        Node ans = query(1, 0, n-1, l, r);
        return ans;
    }
};

struct HLD{
    vector<int>big, head, sz, par, depth, in;
    HLD(int n, int root, vector<int>adj[]) {
        big.resize(n + 1);
        head.resize(n + 1);
        sz.resize(n + 1);
        par.resize(n + 1);
        in.resize(n + 1);
        depth.resize(n + 1);


        depth[root] = 0;
        head[root] = root;

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
            head[big[u]] = head[u], flat(big[u], adj, timer);
        }
        for(auto v : adj[u]){
            if (v == par[u] || big[u] == v) continue;
            head[v] = v;
            flat(v, adj, timer);
        }
    }

    Node  get(int u , int v, SegTree &seg){
        Node valu, valv;
        while(head[u] != head[v]){
            if (depth[head[v]] < depth[head[u]]){
                swap(u, v);
                swap(valu, valv);
            }
            valv = SegTree::merge(seg.query(in[head[v]],in[v]), valv);
            v = par[head[v]];
        }
        if (depth[u] > depth[v]){
            swap(u, v);
            swap(valv, valu);
        }
        valv = SegTree::merge(seg.query(in[u],in[v]), valv);
        swap(valv.maxPref, valv.maxSuf);
        swap(valv.mnPref, valv.mnSuf);
        Node res = SegTree:: merge(valv, valu);

        return res;
    }
};
signed main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int t = 1;
    cin >> t;
    while(t--){
        int n; cin >> n;
        vector<array<int,3>>ask;
        vector<int>adj[n + 5];
        int val[n + 5]{};
        val[1] = 1;
        int cnt = 2;
        for(int i = 0; i < n; i++){
            char x; cin >> x;
            if (x == '+'){
                int u, y; cin >> u >> y;
                val[cnt] = y;
                adj[cnt].push_back(u);
                adj[u].push_back(cnt++);
            }
            else{
                int u, v, k; cin >> u >> v >> k;
                ask.push_back({u, v, k});
            }
        }
        HLD hd(cnt - 1, 1, adj);
        SegTree seg(cnt + 5);

        for(int i = 1; i <= cnt - 1; i++){
            seg.update(hd.in[i], hd.in[i], val[i]);
        }
        for(auto [u, v, k] : ask){
            Node x = hd.get(u, v, seg);
            if (x.mnSeg <= k && x.maxSeg >= k){
                cout << "YES" << endl;
            }
            else cout<< "NO" << endl;
        }



    }

    return 0;












}
