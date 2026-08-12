// ---------------------------------------------------------
// SEGMENT TREE FOR OUTER MAX (Initializes to NINF)
// ---------------------------------------------------------
struct seg_max {
    vector<long long> tree, lazy;
    int sz;
    
    void init(int n) {
        if (n & (n - 1)) sz = (1 << (__lg(n) + 1));
        else sz = n;
        tree.assign(sz << 1, NINF);
        lazy.assign(sz << 1, 0);
    }
    
    void propagate(int l, int r, int k) {
        if (lazy[k] == 0) return;
        if (l != r) {
            lazy[k << 1] += lazy[k];
            lazy[k << 1 | 1] += lazy[k];
        }
        // For Min/Max, adding to a range increases the max by exactly lazy[k]
        tree[k] += lazy[k]; 
        lazy[k] = 0;
    }
    
    void point_set(int lnode, int rnode, int idx, long long val, int k) {
        propagate(lnode, rnode, k);
        if (lnode > idx || rnode < idx) return;
        if (lnode == rnode) {
            tree[k] = val;
            lazy[k] = 0;
            return;
        }
        int mid = (lnode + rnode) / 2;
        point_set(lnode, mid, idx, val, k << 1);
        point_set(mid + 1, rnode, idx, val, k << 1 | 1);
        tree[k] = max(tree[k << 1], tree[k << 1 | 1]);
    }
    
    void update(int lnode, int rnode, int lq, int rq, long long val, int k) {
        propagate(lnode, rnode, k);
        if (lnode > rq || rnode < lq) return;
        if (lnode >= lq && rnode <= rq) {
            lazy[k] += val;
            propagate(lnode, rnode, k);
            return;
        }
        int mid = (lnode + rnode) / 2;
        update(lnode, mid, lq, rq, val, k << 1);
        update(mid + 1, rnode, lq, rq, val, k << 1 | 1);
        tree[k] = max(tree[k << 1], tree[k << 1 | 1]);
    }
    
    long long query(int lnode, int rnode, int lq, int rq, int k) {
        propagate(lnode, rnode, k);
        if (lnode > rq || rnode < lq) return NINF;
        if (lnode >= lq && rnode <= rq) return tree[k];
        int mid = (lnode + rnode) / 2;
        return max(query(lnode, mid, lq, rq, k << 1), 
                   query(mid + 1, rnode, lq, rq, k << 1 | 1));
    }
    
    void point_set(int idx, long long v) { point_set(0, sz - 1, idx, v, 1); }
    void update(int l, int r, long long v) { update(0, sz - 1, l, r, v, 1); }
    long long query(int l, int r) { return query(0, sz - 1, l, r, 1); }
};

// ---------------------------------------------------------
// SEGMENT TREE FOR OUTER MIN (Initializes to INF)
// ---------------------------------------------------------
struct seg_min {
    vector<long long> tree, lazy;
    int sz;
    
    void init(int n) {
        if (n & (n - 1)) sz = (1 << (__lg(n) + 1));
        else sz = n;
        tree.assign(sz << 1, INF);
        lazy.assign(sz << 1, 0);
    }
    
    void propagate(int l, int r, int k) {
        if (lazy[k] == 0) return;
        if (l != r) {
            lazy[k << 1] += lazy[k];
            lazy[k << 1 | 1] += lazy[k];
        }
        tree[k] += lazy[k]; 
        lazy[k] = 0;
    }
    
    void point_set(int lnode, int rnode, int idx, long long val, int k) {
        propagate(lnode, rnode, k);
        if (lnode > idx || rnode < idx) return;
        if (lnode == rnode) {
            tree[k] = val;
            lazy[k] = 0;
            return;
        }
        int mid = (lnode + rnode) / 2;
        point_set(lnode, mid, idx, val, k << 1);
        point_set(mid + 1, rnode, idx, val, k << 1 | 1);
        tree[k] = min(tree[k << 1], tree[k << 1 | 1]);
    }
    
    void update(int lnode, int rnode, int lq, int rq, long long val, int k) {
        propagate(lnode, rnode, k);
        if (lnode > rq || rnode < lq) return;
        if (lnode >= lq && rnode <= rq) {
            lazy[k] += val;
            propagate(lnode, rnode, k);
            return;
        }
        int mid = (lnode + rnode) / 2;
        update(lnode, mid, lq, rq, val, k << 1);
        update(mid + 1, rnode, lq, rq, val, k << 1 | 1);
        tree[k] = min(tree[k << 1], tree[k << 1 | 1]);
    }
    
    long long query(int lnode, int rnode, int lq, int rq, int k) {
        propagate(lnode, rnode, k);
        if (lnode > rq || rnode < lq) return INF;
        if (lnode >= lq && rnode <= rq) return tree[k];
        int mid = (lnode + rnode) / 2;
        return min(query(lnode, mid, lq, rq, k << 1), 
                   query(mid + 1, rnode, lq, rq, k << 1 | 1));
    }
    
    void point_set(int idx, long long v) { point_set(0, sz - 1, idx, v, 1); }
    void update(int l, int r, long long v) { update(0, sz - 1, l, r, v, 1); }
    long long query(int l, int r) { return query(0, sz - 1, l, r, 1); }
}; 

// dp[i] = max(d[j - 1] + max(j , i))
long long solve_max_max(int n, vector<long long>& a) {
    vector<long long> dp(n + 1, NINF);
    dp[0] = 0;
    
    seg_max st;
    st.init(n + 2); 
    
    vector<int> st_idx;
    a[0] = 1e18; // Sentinel
    st_idx.push_back(0);

    for (int i = 1; i <= n; i++) {
        st.point_set(i, dp[i - 1]);
        
        while (st_idx.size() > 1 && a[st_idx.back()] <= a[i]) {
            int p = st_idx.back();
            st_idx.pop_back();
            st.update(st_idx.back() + 1, p, a[i] - a[p]);
        }
        
        st.update(i, i, a[i]);
        st_idx.push_back(i);
        
        dp[i] = st.query(1, i);
    }
    return dp[n];
}


long long solve_max_min(int n, vector<long long>& a) {
    vector<long long> dp(n + 1, NINF);
    dp[0] = 0;
    
    seg_max st;
    st.init(n + 2); 
    
    vector<int> st_idx;
    a[0] = -1e18; // Sentinel
    st_idx.push_back(0);

    for (int i = 1; i <= n; i++) {
        st.point_set(i, dp[i - 1]);
        
        while (st_idx.size() > 1 && a[st_idx.back()] >= a[i]) {
            int p = st_idx.back();
            st_idx.pop_back();
            st.update(st_idx.back() + 1, p, a[i] - a[p]);
        }
        
        st.update(i, i, a[i]);
        st_idx.push_back(i);
        
        dp[i] = st.query(1, i);
    }
    return dp[n];
} 

// the same but min (dp[j - 1] + max(i , j))
long long solve_min_max(int n, vector<long long>& a) {
    vector<long long> dp(n + 1, INF);
    dp[0] = 0;
    
    seg_min st;
    st.init(n + 2); 
    
    vector<int> st_idx;
    a[0] = 1e18; // Sentinel
    st_idx.push_back(0);

    for (int i = 1; i <= n; i++) {
        st.point_set(i, dp[i - 1]);
        
        while (st_idx.size() > 1 && a[st_idx.back()] <= a[i]) {
            int p = st_idx.back();
            st_idx.pop_back();
            st.update(st_idx.back() + 1, p, a[i] - a[p]);
        }
        
        st.update(i, i, a[i]);
        st_idx.push_back(i);
        
        dp[i] = st.query(1, i);
    }
    return dp[n];
}

long long solve_min_min(int n, vector<long long>& a) {
    vector<long long> dp(n + 1, INF);
    dp[0] = 0;
    
    seg_min st;
    st.init(n + 2); 
    
    vector<int> st_idx;
    a[0] = -1e18; // Sentinel
    st_idx.push_back(0);

    for (int i = 1; i <= n; i++) {
        st.point_set(i, dp[i - 1]);
        
        while (st_idx.size() > 1 && a[st_idx.back()] >= a[i]) {
            int p = st_idx.back();
            st_idx.pop_back();
            st.update(st_idx.back() + 1, p, a[i] - a[p]);
        }
        
        st.update(i, i, a[i]);
        st_idx.push_back(i);
        
        dp[i] = st.query(1, i);
    }
    return dp[n];
}
