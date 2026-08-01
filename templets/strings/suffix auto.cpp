#include <bits/stdc++.h>
using namespace std;
#define int long long

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
    vector<int>in, out, val;
    int sz = 0, last = 0, lgn, timer = 0; 

    void init(int n) {
        st.resize(2 * n  + 5);
        ans.resize(2 * n + 5);
        nodePrefix.resize(2 * n + 5);
        lgn = __lg(2*n);
        prefixNode.resize(2 * n + 5);
        up.resize(2 * n + 1, vector<int>(__lg(2 * n + 1) + 5, -1));
        in.resize(2 * n + 5);
        out.resize(2 * n + 5);
        val.resize(2 * n + 5);
        sz++;
        st[0].len = 0;
        st[0].link = -1;
        last = 0;
        timer = 0;
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
                while (p != -1 && st[p].nxt.find(c) != st[p].nxt.end() && st[p].nxt[c] == q) {
                    st[p].nxt[c] = clone;
                    p = st[p].link;
                }
                st[q].link = st[cur].link = clone;
            }
        }
        last = cur;
    }

    void dfs(int u , int parent) {
        in[u] = timer++;
        val[in[u]] = nodePrefix[u];
        up[u][0] = parent;
        for (int lg = 1; lg <= lgn; lg++) {
            if (up[u][lg - 1] != -1) up[u][lg] = up[up[u][lg - 1]][lg - 1];
            else up[u][lg] = -1;
        }
       
        for (auto v : adj[u]) { 
            dfs(v, u);
        }
        out[u] = timer - 1;
    }
    
    void perp() {
        adj.resize(sz);
        for (int i = 1; i < sz; i++) adj[st[i].link].push_back(i);
        timer = 0;
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

const int MAXN = 2e5  + 5 ;
 
int mu[MAXN + 1];
int lp[MAXN + 1];
vector<int> primes;
vector<vector<int>> sive(2e5 + 1);

void pre () {
    for (int i = 1 ;i <= 2e5 ; i++ ) {
        for (int j = i ;j  <= 2e5 ; j+= i ) {
            sive[j].push_back(i);
        }
    }
}

void mobius_sieve() {
    mu[1] = 1;
    for (int i = 2; i <= MAXN; i++) {
        if (lp[i] == 0) {
            lp[i] = i;
            primes.push_back(i);
            mu[i] = -1;            // prime
        } else {
            if (lp[i / lp[i]] == lp[i])
                mu[i] = 0;         // has squared prime factor
            else
                mu[i] = -mu[i / lp[i]];
        }
        for (int p : primes) {
            if (p > lp[i] || 1LL * p * i > MAXN) break;
            lp[p * i] = p;
        }
    }
}

signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    pre();
    mobius_sieve();
    
    SAM st; st.init(2e5 + 6);
    
    string s; cin >> s;
    int n = s.size();
    for (int i = 0; i < n; i++) {
        st.extend(s[i],  i); 
    } 
    st.perp();
    
    int m = st.sz + 1;
    int pre[m + 1]; memset(pre, 0, sizeof pre);
    for (int i = 0; i < m; i++) {
        pre[i] = (st.val[i] > 0);
        if (i) pre[i] += pre[i - 1];
    }
    
    int q; cin >> q;
    vector<array<int,3>>query;
    for (int i = 0; i < q; i++) {
        int l,r; cin >> l >> r;
        l--, r--;
        int node = st.getIdSubstring(l, r);
        query.push_back({st.in[node], st.out[node], i}); 
    }
   
  
}
