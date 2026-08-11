#include <iostream>
#include <vector>

using namespace std;

struct OnlineRangeParallelDSU {
    int n, K;
    vector<vector<int>> parent;
    vector<vector<int>> sz;
    vector<long long> comp_sum;
    long long current_X;
    long long MOD = 998244353;

    void init(int n) {
        K = 32 - __builtin_clz(n <= 0 ? 1 : n);
        parent.assign(K, vector<int>(n));
        sz.assign(K, vector<int>(n, 1)); // Union by size ensures strict O(N log N) bounds
        current_X = 0;
        
        for (int j = 0; j < K; j++) {
            for (int i = 0; i < n; i++) {
                parent[j][i] = i;
            }
        }
    }
    OnlineRangeParallelDSU(int n) : n(n + 1) {
       init(n + 1);
    }

    int find(int j, int i) {
        if (parent[j][i] == i) return i;
        return parent[j][i] = find(j, parent[j][i]);
    }

    void unite(int j, int u, int v) {
        int root_u = find(j, u);
        int root_v = find(j, v);
        if (root_u == root_v) return;

        if (sz[j][root_u] > sz[j][root_v]) {
            swap(root_u, root_v);
        }
        parent[j][root_u] = root_v;
        sz[j][root_v] += sz[j][root_u];
            
        if (j) {
            unite(j - 1, u, v);
            unite(j - 1, u + (1 << (j - 1)), v + (1 << (j - 1)));
        }
        else {
            // any thing you want to do with the comp any thing you want to store any op like the usual dsu (track sum) or sumthing
        }
        
    }

   
    // Merge range [l1, r1] with [l2, r2] online
    void unite_range(int l1, int r1, int l2, int r2) {
        int len = r1 - l1 + 1;
        if (len <= 0) return;
        int j = 31 - __builtin_clz(len); 
        unite(j, l1, l2);
        unite(j, r1 - (1 << j) + 1, r2 - (1 << j) + 1);
    }
};

int main() {
    int n,m ; cin >> n >> m;
    OnlineRangeParallelDSU ds(n);
    for (int i = 0; i < m; i++) {
        int l1, r1, l2, r2; 
        cin >> l1 >> r1 >> l2 >> r2;
        ds.unite_range(l1, r1, l2, r2);
    }
    int ans=0;
    for (int i = 1; i <= n; i++) {
        ans += ds.find(0, i) == i;
    }
    cout << ans << endl;

    return 0;
}
