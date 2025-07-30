// Author: _Sherbiny

#include "bits/stdc++.h"
using namespace std;

#ifdef DBG
#include "./debug.h"
#else
#define dbg(...)
#endif

typedef long long ll;
#define endl '\n'
//====================//

void magic() {
    int n; cin >> n;
    map<int, int> mp;
    vector<array<int, 2>> v(n);

    for(auto &[x, y]: v) {
        cin >> x >> y;
        mp[x], mp[y];
    }

    int c = 0;
    for(auto &[a, b]: mp) b = c++;
    c *= 2;

    vector<vector<array<int, 2>>> adj(c);
    vector<int> deg(c), odd, ans, vis(n);

    int st = 0;
    for(int i = 0; i < n; ++i) {
        auto [x, y] = v[i];
        x = mp[x] * 2, y = mp[y] * 2 + 1;
        adj[x].push_back({y, i});
        adj[y].push_back({x, i});
        st = x;
        ++deg[x], ++deg[y];
    }

    for(int i = 0; i < c; ++i)
        if(deg[i] & 1) odd.push_back(i), st = i;

    if(!odd.empty() && odd.size() != 2)
        return void(cout << "NO\n");

    auto go = [&](auto &&go, int u) -> void {
        while (!adj[u].empty()) {
            auto [v, i] = adj[u].back();
            adj[u].pop_back();

            if(vis[i]) continue;
            vis[i] = 1, go(go, v);
            ans.push_back(++i);
        }
    };

    go(go, st);

    if(ans.size() != n)
        return void(cout << "NO\n");

    cout << "YES" << endl;
    for(int &i: ans) cout << i << ' ';
    cout << endl;
}

signed main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0), cout.tie(0);

    int t = 1;
    cin >> t;
    while (t--) magic();
}
