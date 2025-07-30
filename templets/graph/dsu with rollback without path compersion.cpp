#include <bits/stdc++.h>
using namespace std;

struct dsu{
    vector<int>parent, sz, checkPoints;
    vector<pair<int,int>>updates;
    int comp;
    void init(int n){
        parent.resize(n + 1);
        sz.resize(n + 1, 1);
        for(int i = 1; i <= n; i++) parent[i] = i, sz[i]  = 1;
        checkPoints.clear();
        updates.clear();
        comp = n;
    }
    int find(int u){
        while(u ^ parent[u]) u = parent[u];
        return u;
    }
    void merge(int u, int v){
        u = find(u), v = find(v);
        if (u == v) return;
        if (sz[u] < sz[v]) swap(u, v);
        updates.push_back({u , v});
        sz[u]+= sz[v];
        comp--;
        parent[v] = u;
    }
    bool same(int u, int v){
        return find(u) == find(v);
    }
    void snapShot(){
        checkPoints.push_back(updates.size());
    }
    void rollBack(){
        if (checkPoints.empty()) return;
        while(checkPoints.back() != updates.size()){
            auto [u, v] = updates.back();
            sz[u] -= sz[v];
            parent[v] = v;
            comp++;
            updates.pop_back();
        }
        checkPoints.pop_back();
    }
};

int main(){
    int n, m; cin >> n >> m;
    dsu d;
    d.init(n);
    while(m--){
        string s; cin >> s;
        if (s[0]== 'p'){
            d.snapShot();
        }
        else if (s[0] == 'u'){
            int u, v; cin>> u >> v;
            d.merge(u , v);
            cout << d.comp << endl;
        }
        else {
            d.rollBack();
            cout << d.comp << endl;
        }
    }
    return 0;
}
