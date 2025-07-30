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

struct dsu {
    vector<int> parent, sz, checkPoints;
    vector<tuple<int, int, int>> updates;
    int comp;

    void init(int n) {
        parent.resize(n + 1);
        sz.resize(n + 1, 1);
        for (int i = 0; i < n; i++) parent[i] = i, sz[i] = 1;
        checkPoints.clear();
        updates.clear();
        comp = n;
    }

    int find(int u) {
        if (u == parent[u]) return u;
        int root = find(parent[u]);
        parent[u] = root;
        return root;
    }

    void merge(int u, int v) {
        u = find(u), v = find(v);
        if (u == v) return;
        if (sz[u] < sz[v]) swap(u, v);
        updates.emplace_back(v, parent[v], u);
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
            auto [node, oldP, ne] = updates.back();
            updates.pop_back();
            if (ne != -1) sz[ne] -= sz[node], comp++;
            parent[node] = oldP;

        }
        checkPoints.pop_back();
    }
};
const int N = 3e5 + 7;



signed main() {
//    fast();
    int t = 1;
    cin >> t;
    while (t--) {
        int n; cin >> n;
        vector<array<int,2>>edges;
        for(int i = 0; i <= n - 1; i++){
            for(int j = 0; j < 19; j++){
                if (i >> j & 1){
                    int x = i;
                    x ^= (1 << j);
                    edges.push_back({x, i});
                }
            }
        }
        sort(edges.begin(), edges.end(), greater<>());
        int ans = 0;
        dsu d;
        d.init(n);
        for(auto [u, v] : edges){
            if (!d.same(u , v)){
                ans += u;
                d.merge(u, v);
            }
        }
        cout << ans << endl;
    }

    return 0;
}
