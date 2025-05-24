#include <bits/stdc++.h>
using namespace std;


#define int long long
#define all(a) a.begin(), a.end()
#define sz(u) (out[u] - in[u] + 1)
signed main() {
    int n; cin >> n;
    int val[n + 1];
    vector<pair<int,int>>comp;
    for(int i = 1; i <= n; i++){
        cin >> val[i];
        comp.push_back({val[i], i});
    }
    sort(comp.begin(), comp.end());
    val[comp[0].second] = 0;
    for(int i = 1; i < n; i++){
        val[comp[i].second] = val[comp[i - 1].second];
        if (comp[i].first  != comp[i - 1].first) val[comp[i].second]++;
    }
    vector<int>adj[n + 1];
    for(int i = 1; i < n; i++){
        int u, v; cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    int in[n + 1], out[n + 1], timer = 0, tree[n + 1];
    function<void(int,int)> dfs = [&](int u,int p){
        tree[timer] = u;
        in[u] = timer++;
        for(auto v : adj[u]){
            if (v == p) continue;
            dfs(v, u);
        }
        out[u] = timer - 1;
    };
    dfs(1, 0);
    int freq[n + 1]{}, ans = 0, res[n + 1]{};
    auto add =[&](int u){
        freq[val[u]]++;
        if (freq[val[u]] == 1) ans++;
    };
    auto erase =[&](int u){
        freq[val[u]]--;
        if (freq[val[u]] == 0) ans--;
    };
    function<void(int,int, int)> solve = [&](int u,int p, int keep){
        int mx = -1, bg = -1;
        for(auto v : adj[u]){
            if (v == p) continue;
            if (bg == -1 || sz(v) > mx){
                bg = v;
                mx = sz(v);
            }
        }
        for(auto v : adj[u]){
            if (v == p || v == bg)   continue;
            solve(v, u , false);
        }
        if (~bg) solve(bg, u, true);
        add(u);
        for(auto v : adj[u]){
            if (v == p || v == bg) continue;
            for(int i = in[v]; i <= out[v]; i++){
                add(tree[i]);
            }
        }
        res[u] = ans;
        if (!keep){
            for(int i = in[u]; i <= out[u]; i++){
                erase(tree[i]);
            }
        }
    };
    solve(1, 0, 1);
    for(int i = 1; i<=n; i++) cout << res[i] << ' ';

    return 0;
}
