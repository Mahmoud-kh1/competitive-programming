add edge from u, v;
add edge from u to range [l,r]
add range from [l,r] to u 


#include "bits/stdc++.h"
using namespace std;
int n;
#define int long long
const int N = 4 * 1e5;
vector<pair<int,int>>adj[N];
int seg[N][2];
int id;
void build(int lx, int rx, int k){
    if(lx == rx){
        seg[k][0] = lx;
        seg[k][1] = lx;
        return ;
    }
    seg[k][0] = id++;
    seg[k][1] = id++;
    int md = (lx + rx) / 2;
    build(lx, md, k << 1);
    build(md + 1, rx , k << 1 | 1);
    adj[seg[k][0]].push_back({seg[k << 1][0], 0});
    adj[seg[k][0]].push_back({seg[k << 1 | 1][0], 0});
    adj[seg[k << 1][1]].push_back({seg[k][1], 0});
    adj[seg[k << 1 | 1][1]].push_back({seg[k][1], 0});
}
void conectT1(int l, int r, int lx, int rx, int k, int w, int u){
    if(l > rx || r < lx) return;

    if(l <= lx && r >= rx){
        adj[u].push_back({seg[k][0], w});
        return;
    }
    int mid = (lx + rx) / 2;
    conectT1(l, r, lx, mid , k << 1 , w , u);
    conectT1(l, r, mid + 1, rx , k << 1 | 1 , w , u);
}
void conectT2(int l, int r, int lx, int rx, int k, int w, int u){
    if(l > rx || r < lx) return;

    if(l <= lx && r >= rx){
        adj[seg[k][1]].push_back({u, w});
        return;
    }
    int mid = (lx + rx) / 2;
    conectT2(l, r, lx, mid , k << 1 , w , u);
    conectT2(l, r, mid + 1, rx , k << 1 | 1 , w , u);
}


signed main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
     int q,s; cin >> n >>  q >> s;
     id = n;
     s--;
     build(0, n - 1, 1);
     while(q--){
         int ty;  cin >> ty;
         if (ty == 1){
             int u, v, w;
             cin >> u >> v >> w;
             u--, v--;
             adj[u].push_back({v, w});
         }
         else if (ty == 2){
             int u, l, r, w; cin >> u >> l >> r >> w;
             u--, l--, r--;
             conectT1(l, r, 0, n - 1, 1, w, u);
         }
         else {
             int u, l, r, w; cin >> u >> l >> r >> w;
             u--, l--, r--;
             conectT2(l, r, 0, n - 1, 1, w, u);
         }

     }
     priority_queue<pair<int,int>>pq;
     int dist[N]; memset(dist, -1, sizeof dist);
     dist[s] = 0;
     pq.push({0, s});
     while(!pq.empty()){
         int node = pq.top().second;
         int d = -pq.top().first;
         pq.pop();
         if (d > dist[node]) continue;
         for(auto [nx, we] : adj[node]){
             if(dist[nx] == -1 || dist[nx] > d + we){
                 dist[nx] = d + we;
                 pq.push({-dist[nx], nx});
             }
         }
     }
     for(int i = 0; i < n; i++) cout << dist[i] << ' ';


    return 0;
}




 
