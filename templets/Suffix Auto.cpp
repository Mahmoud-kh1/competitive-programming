struct state {
    map<int,int>nxt, primes;
    int len, link;
    int id = 0;
};
struct SAM {
    vector<state>st;
    vector<int>ans;
    vector<vector<int>>adj;
    vector<int>prefixNode;
    vector<int>nodePrefix;
    vector<vector<int>>up;
    int sz = 0, last = 0, lgn; 
    void init(int n) {
        st.resize(2 * n  + 5);
        ans.resize(2 * n + 5);
        nodePrefix.resize(2 * n + 5);
        lgn = __lg(2*n);
        prefixNode.resize(2 * n + 5);
        up.resize(2 * n + 1, vector<int>(__lg(2 * n + 1) + 5, -1));
        sz++;
        st[0].len = 0;
        st[0].link = -1;
        last = 0;
    } 
    void extend(int c, int idx) {
        int cur = sz++;
        st[cur].len = st[last].len + 1;
        int p = last;
        prefixNode[idx] = cur;
        nodePrefix[cur] = (idx + 1);
        while (p != -1 && st[p].nxt.count(c) == 0) {
            st[p].nxt[c] = cur;
            p = st[p].link;
        }
        if (p == -1) {
            st[cur].link = 0;
        }
        else {
            int q = st[p].nxt[c];
            if (st[p].len + 1 == st[q].len) {
                st[cur].link = q;
            }
            else {
                int clone = sz++;
                st[clone].len = st[p].len + 1;
                st[clone].nxt = st[q].nxt;
                st[clone].link = st[q].link;
                st[clone].id = st[q].id;
                while (p != -1 && st[p].nxt.find(c) != st[p].nxt.end()&&st[p].nxt[c] == q) {
                    st[p].nxt[c] = clone;
                    p = st[p].link;
                }
                st[q].link = st[cur].link = clone;
            }
        }
        last = cur;
    }
    void dfs(int u , int parent) {
        up[u][0] = parent;
        for (int lg = 1; lg <= lgn; lg++) {
            if (up[u][lg - 1] != -1) up[u][lg] = up[up[u][lg - 1]][lg - 1];
            else up[u][lg] = -1;
        }
       
        for (auto v : adj[u]) { 
            dfs(v, u);
        }
    }
    
    void perp() {
        adj.resize(sz);
        for (int i = 1; i < sz; i++) adj[st[i].link].push_back(i);
        dfs(0, -1);
    }
    
    int getIdSubstring(int l, int r) {
        int node = prefixNode[r];
        int wantedLen = r - l + 1;
        for (int lg = lgn; lg >= 0; lg--) {
            if (up[node][lg] != -1 && st[up[node][lg]].len >= wantedLen) {
                node = up[node][lg];
            }
        }
        return node;
    }
    
};
