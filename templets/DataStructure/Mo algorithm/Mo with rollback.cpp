#include <bits/stdc++.h>
using namespace std;

struct dsu {
    vector<int> parent, sz, checkPoints;
    vector<pair<int,int>> updates;
    int comp;

    void init(int n) {
        parent.resize(n + 1);
        sz.resize(n + 1, 1);
        for (int i = 1; i <= n; i++) parent[i] = i, sz[i] = 1;
        checkPoints.clear();
        updates.clear();
        comp = n;
    }

    int find(int u) {
        while(u ^ parent[u]) u = parent[u];
        return u;
    }

    void merge(pair<int,int>edge) {
        auto[u, v] = edge;
        u = find(u), v = find(v);
        if (u == v) return;
        if (sz[u] < sz[v]) swap(u, v);
        updates.emplace_back(u, v);
        parent[v] = u;
        sz[u] += sz[v];
        comp--;
    }

    bool same(int u, int v) {
        return find(u) == find(v);
    }

    void snapShot() {
        checkPoints.push_back(updates.size());
    }

    void rollBack() {
        if (checkPoints.empty()) return;

        while (updates.size() != checkPoints.back()) {
            auto [u, v] = updates.back();
            updates.pop_back();
            sz[u] -= sz[v];
            parent[v] = v;
            comp++;

        }
        checkPoints.pop_back();
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n,m; cin >> n >> m;
    vector<pair<int,int>>edges;
    for(int i = 0; i < m; i++){
        int u, v;  cin >> u >> v;
        edges.push_back({u, v});
    }
    int SQ = sqrt(m) + 3;
    vector<array<int,3>>queries[SQ];
    dsu d;
    d.init(n);
    int q;  cin >> q;
    vector<int>ans(q);
    for(int i = 0; i < q; i++){
        int l,r; cin >> l >> r;
        l--, r--;
        if (r - l + 1 > SQ){
            queries[l/SQ].push_back({r, l, i});
            continue;
        }
        d.snapShot();
        while(l <= r){
            d.merge(edges[l++]);
        }
        ans[i] = d.comp;
        d.rollBack();
    }

    for(int  b = 0; b < SQ ; b++){
        if (queries[b].empty()) continue;
        sort(queries[b].begin(), queries[b].end());
        int R = (b + 1) * SQ - 2;
        d.snapShot();
        for(const auto [rq, lq, id] : queries[b]){
            while(rq > R){
                d.merge(edges[++R]);
            }
            d.snapShot();
            int l = (b + 1) * SQ - 1;
            while(l > lq){
                d.merge(edges[--l]);
            }
            ans[id] = d.comp;
            d.rollBack();
        }
        d.rollBack();
    }
    for(auto i  : ans) cout << i << endl;




    return 0;
}
