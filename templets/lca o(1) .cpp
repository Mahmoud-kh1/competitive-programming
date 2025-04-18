struct LCA {
    int LG, n;
    vector<int> lvl, pre;
    vector<vector<int>> anc;
    // for O(1) LCA
    vector<int> euler, depth_euler, first, log_2;
    vector<vector<int>> st;
 
    void dfs_euler(int u, int p) {
        anc[0][u] = p;
        lvl[u] = (p ? lvl[p] + 1 : 0);
        first[u] = (int)euler.size();
        euler.push_back(u);
        depth_euler.push_back(lvl[u]);
        for (int v : adj[u]) {
            if (v == p) continue;
            dfs_euler(v, u);
            euler.push_back(u);
            depth_euler.push_back(lvl[u]);
        }
    }
 
    void init(int nn) {
        n = nn;
        LG = __lg(n) + 1;
        lvl.assign(n + 1, 0);
        pre.assign(n + 1, 0);
        anc.assign(LG, vector<int>(n + 1, 0));
        first.assign(n + 1, 0);
        euler.clear(); depth_euler.clear();
 
        // build Euler tour + binary-lifting parents
        dfs_euler(1, 0);
        // build binary lifting table for kth
        for (int k = 1; k < LG; k++) {
            for (int u = 1; u <= n; u++) {
                anc[k][u] = anc[k-1][ anc[k-1][u] ];
            }
        }
        // build RMQ sparse table on depth_euler
        int m = euler.size();
        log_2.assign(m + 1, 0);
        for (int i = 2; i <= m; i++) log_2[i] = log_2[i/2] + 1;
        int K = log_2[m] + 1;
        st.assign(K, vector<int>(m));
        for (int i = 0; i < m; i++) st[0][i] = i;
        for (int k = 1; k < K; k++) {
            for (int i = 0; i + (1 << k) <= m; i++) {
                int x = st[k-1][i];
                int y = st[k-1][i + (1 << (k-1))];
                st[k][i] = (depth_euler[x] < depth_euler[y] ? x : y);
            }
        }
    }
 
    int lca(int u, int v) {
        int L = first[u], R = first[v];
        if (L > R) swap(L, R);
        int len = R - L + 1;
        int k = log_2[len];
        int x = st[k][L];
        int y = st[k][R - (1 << k) + 1];
        return depth_euler[x] < depth_euler[y] ? euler[x] : euler[y];
    }
 
    int kth(int u, int k) {
        for (int i = 0; k; i++, k >>= 1) {
            if (k & 1) u = anc[i][u];
        }
        return u ? u : -1;
    }
 
    int dist(int u, int v) {
        int w = lca(u, v);
        return pre[u] + pre[v] - 2 * pre[w];
    }
 
    int dist2(int u, int v, int lc) {
        return lvl[u] + lvl[v] - 2*lvl[lc] + 1;
    }
 
    int get(int u, int v, int k) {
        if (k == 1) return u;
        int lc = lca(u, v);
        int ulc = lvl[u] - lvl[lc] + 1;
        if (k <= ulc) return kth(u, k - 1);
        else return kth(v, dist2(u, v, lc) - k);
    }
};
 
