#include <bits/stdc++.h>
using namespace std;

struct dsu {
    vector<int> parent, sz, checkPoints;
    vector<tuple<int, int, int>> updates; 
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
        if (u == parent[u]) return u;
        int root = find(parent[u]);
        updates.emplace_back(u, parent[u], -1); 
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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    dsu d;
    d.init(n);

    while (m--) {
        string s;
        cin >> s;
        if (s[0] == 'p') {
            d.snapShot();
        } else if (s[0] == 'u') {
            int u, v;
            cin >> u >> v;
            d.merge(u, v);
            cout << d.comp << endl;
        } else {
            d.rollBack();
            cout << d.comp << endl;
        }
    }
    return 0;
}
