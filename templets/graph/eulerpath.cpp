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



from chat, 
#include <bits/stdc++.h>
using namespace std;

// Undirected Eulerian Path/Circuit
//   If exactly 0 odd‐degree vertices → returns circuit
//   If exactly 2 odd‐degree vertices → returns path from odd1→odd2
//   Otherwise returns empty vector (no Eulerian trail)
// Graph is 0…N-1

vector<int> euler_path_undirected(int N, vector<vector<pair<int,int>>>& adj) {
    vector<int> deg(N);
    for(int u=0;u<N;u++)
        for(auto [v, eid]: adj[u])
            deg[u]++;

    int start = -1, odd = 0;
    for(int i=0;i<N;i++){
        if(deg[i]%2){
            odd++;
            if(start<0) start = i;
        }
    }
    if(odd!=0 && odd!=2) return {};    
    if(start<0) start = 0;  // all even: can start anywhere

    vector<bool> used_edge(adj.size(), false);
    vector<int> stk, path;
    stk.push_back(start);

    // iterative Hierholzer
    while(!stk.empty()){
        int u = stk.back();
        while(!adj[u].empty() && used_edge[adj[u].back().second]) {
            adj[u].pop_back();
        }
        if(adj[u].empty()){
            path.push_back(u);
            stk.pop_back();
        } else {
            auto [v,eid] = adj[u].back();
            used_edge[eid] = true;
            stk.push_back(v);
        }
    }
    // path.size()==#edges+1?
    return path;  
}

// Directed Eulerian Path/Circuit
//   If in==out for all→circuit
//   If one vertex has out=in+1 (start) and one has in=out+1 (end), all others in==out→path
//   Otherwise empty
vector<int> euler_path_directed(int N, vector<vector<pair<int,int>>>& adj) {
    vector<int> indeg(N), outdeg(N);
    for(int u=0;u<N;u++){
        for(auto [v,eid]: adj[u]){
            outdeg[u]++;
            indeg[v]++;
        }
    }
    int start = -1, end = -1;
    for(int i=0;i<N;i++){
        if(outdeg[i] - indeg[i] == 1){
            if(start<0) start = i;
            else return {};
        } else if(indeg[i] - outdeg[i] == 1){
            if(end<0) end = i;
            else return {};
        } else if(indeg[i]!=outdeg[i]){
            return {};
        }
    }
    if(start<0) {
        // circuit case: pick any vertex with edges
        for(int i=0;i<N;i++) if(outdeg[i]>0){ start = i; break; }
        if(start<0) return {};  // no edges at all
    }

    vector<bool> used_edge(adj.size(), false);
    vector<int> stk, path;
    stk.push_back(start);

    while(!stk.empty()){
        int u = stk.back();
        while(!adj[u].empty() && used_edge[adj[u].back().second]) {
            adj[u].pop_back();
        }
        if(adj[u].empty()){
            path.push_back(u);
            stk.pop_back();
        } else {
            auto [v,eid] = adj[u].back();
            used_edge[eid] = true;
            stk.push_back(v);
        }
    }
    return path;
}


int main(){
    int N, M;
    cin >> N >> M;
    // build undirected:
    vector<vector<pair<int,int>>> adj(N);
    for(int i=0;i<M;i++){
        int u,v;
        cin >> u >> v;
        // zero‐based
        adj[u].emplace_back(v,i);
        adj[v].emplace_back(u,i);
    }
    auto path = euler_path_undirected(N, adj);
    if(path.empty()){
        cout << "NO EULERIAN TRAIL\n";
    } else {
        // path is reversed order end→start
        reverse(path.begin(), path.end());
        for(int x: path) cout<<x<<" ";
        cout<<"\n";
    }
    return 0;
}


