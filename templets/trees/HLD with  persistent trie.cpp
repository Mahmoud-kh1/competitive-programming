#include <bits/stdc++.h>
using namespace std;

struct PersistentTrie {
    struct Node {
        int child[2];   // indexes of children (0 = null)
        int count;      // number of values through this node
        Node() : child{0, 0}, count(0) {}
    };

    vector<Node> pool;
    vector<int> versions;
    static constexpr int MAXBIT = 30;  // enough for values <= 10^9

    PersistentTrie(int n) {
        pool.reserve(n * (MAXBIT + 1) + 1);
        versions.reserve(n + 1);
        pool.emplace_back();        // null root at index 0
        versions.push_back(0);     // initial empty version
    }

    // Create a new node
    int newNode() {
        pool.emplace_back();
        return int(pool.size()) - 1;
    }

    // Clone an existing node; return its new index
    int cloneNode(int idx) {
        pool.push_back(pool[idx]);
        return int(pool.size()) - 1;
    }

    // Insert value `x` into version `ver`, returns new version index
    int insert(int ver, int x) {
        int oldRoot = versions[ver];
        int newRoot = cloneNode(oldRoot);
        versions.push_back(newRoot);

        int curOld = oldRoot;
        int curNew = newRoot;
        pool[curNew].count++;

        for (int b = MAXBIT - 1; b >= 0; --b) {
            int bit = (x >> b) & 1;
            int other = bit ^ 1;
            pool[curNew].child[other] = pool[curOld].child[other];

            int nextOld = pool[curOld].child[bit];
            int nextNew = (nextOld == 0 ? newNode() : cloneNode(nextOld));
            pool[curNew].child[bit] = nextNew;

            curOld = (nextOld == 0 ? 0 : nextOld);
            curNew = nextNew;
            pool[curNew].count++;
        }
        return int(versions.size()) - 1;
    }

    // Query max XOR with `x` among elements in versions (lVer..rVer]
    int queryMax(int lVer, int rVer, int x) const {
        int curL = versions[lVer];
        int curR = versions[rVer];
        int res = 0;
        for (int b = MAXBIT - 1; b >= 0; --b) {
            int bit = (x >> b) & 1;
            int want = bit ^ 1;
            int cntR = pool[ pool[curR].child[want] ].count;
            int cntL = pool[ pool[curL].child[want] ].count;
            if (cntR - cntL > 0) {
                res |= (1 << b);
                curR = pool[curR].child[want];
                curL = pool[curL].child[want];
            } else {
                int same = want ^ 1;
                curR = pool[curR].child[same];
                curL = pool[curL].child[same];
            }
        }
        return res;
    }

    // Query min XOR with `x` among elements in versions (lVer..rVer]
    int queryMin(int lVer, int rVer, int x) const {
        int curL = versions[lVer];
        int curR = versions[rVer];
        int res = 0;
        for (int b = MAXBIT - 1; b >= 0; --b) {
            int bit = (x >> b) & 1;
            // prefer same bit to minimize
            int want = bit;
            int cntR = pool[ pool[curR].child[want] ].count;
            int cntL = pool[ pool[curL].child[want] ].count;
            if (cntR - cntL > 0) {
                // go same
                curR = pool[curR].child[want];
                curL = pool[curL].child[want];
            } else {
                // must flip
                res |= (1 << b);
                int other = want ^ 1;
                curR = pool[curR].child[other];
                curL = pool[curL].child[other];
            }
        }
        return res;
    }

    // Count number of values v in (lVer..rVer] such that (v ^ x) <= K
    int countXorLE(int lVer, int rVer, int x, int K) const {
        return countXorCompare(lVer, rVer, x, K, true);
    }

    // Count number of values v in (lVer..rVer] such that (v ^ x) >= K
    int countXorGE(int lVer, int rVer, int x, int K) const {
        return countXorCompare(lVer, rVer, x, K, false);
    }

private:
    // Helper to count <=K or >=K
    int countXorCompare(int lVer, int rVer, int x, int K, bool le) const {
        // We traverse trie bits and K bits together
        int curL = versions[lVer];
        int curR = versions[rVer];
        int cnt = 0;
        for (int b = MAXBIT - 1; b >= 0; --b) {
            if (!curR && !curL) break;
            int xb = (x >> b) & 1;
            int kb = (K >> b) & 1;
            if (kb == 1) {
                // if K bit is 1:
                // for <=K: we can take branch where v^x has 0 here => same bit
                // for >=K: we can take branch where v^x has 1 here => flip bit
                int same = xb;
                int other = xb ^ 1;
                // count those matching branch
                int cR = pool[ pool[curR].child[same] ].count;
                int cL = pool[ pool[curL].child[same] ].count;
                cnt += (cR - cL);
                // then we must go down the opposite branch for next bits
                if (le) {
                    curR = pool[curR].child[1^xb];
                    curL = pool[curL].child[1^xb];
                } else {
                    curR = pool[curR].child[xb];
                    curL = pool[curL].child[xb];
                }
            } else {
                // K bit is 0: to stay <=K, or >=K, we must go same side
                int want = le ? xb : (xb ^ 1);
                curR = pool[curR].child[want];
                curL = pool[curL].child[want];
            }
        }
        // if we finish path and want >=K and K==0, then all remaining count
        if (!le && K == 0) {
            cnt += pool[curR].count - pool[curL].count;
        }
        return cnt;
    }
};


// ----------------------------------------
// Heavy-Light Decomposition (HLD) on tree
// ----------------------------------------
struct HLD {
    int n;
    vector<vector<pair<int,int>>> adj;
    vector<int> parent, depth, heavy, sz, head, pos, xorToRoot;
    int curPos;

    HLD(int n_) : n(n_), adj(n_+1), parent(n_+1), depth(n_+1),
                  heavy(n_+1,-1), sz(n_+1), head(n_+1), pos(n_+1), xorToRoot(n_+1),
                  curPos(0) {}

    void addEdge(int u, int v, int w) {
        adj[u].emplace_back(v,w);
        adj[v].emplace_back(u,w);
    }

    void build(int root = 1) {
        dfs(root, 0);
        decompose(root, root);
    }

    // applies process(l,r) over each segment of path u→v
    template<typename F>
    int queryPath(int u, int v, F process) const {
        int ans = 0;
        while (head[u] != head[v]) {
            if (depth[head[u]] > depth[head[v]]) {
                ans = max(ans, process(pos[head[u]], pos[u]));
                u = parent[head[u]];
            } else {
                ans = max(ans, process(pos[head[v]], pos[v]));
                v = parent[head[v]];
            }
        }
        int l = min(pos[u], pos[v]);
        int r = max(pos[u], pos[v]);
        ans = max(ans, process(l, r));
        return ans;
    }

private:
    int dfs(int u, int p) {
        parent[u] = p;
        depth[u] = (p == 0 ? 0 : depth[p] + 1);
        xorToRoot[u] = (p == 0 ? 0 : xorToRoot[p] ^
                                     // find weight of (p,u)
                                     [&]{ for(auto &e:adj[u]) if(e.first==p) return e.second; return 0; }());
        sz[u] = 1;
        int maxSub = 0;
        for (auto &e : adj[u]) {
            int v = e.first, w = e.second;
            if (v == p) continue;
            xorToRoot[v] = xorToRoot[u] ^ w;
            int sub = dfs(v, u);
            if (sub > maxSub) maxSub = sub, heavy[u] = v;
            sz[u] += sub;
        }
        return sz[u];
    }

    void decompose(int u, int h) {
        head[u] = h;
        pos[u] = ++curPos;
        if (heavy[u] != -1) decompose(heavy[u], h);
        for (auto &e : adj[u]) {
            int v = e.first;
            if (v == parent[u] || v == heavy[u]) continue;
            decompose(v, v);
        }
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T; cin >> T;
    while(T--) {
        int N, Q;
        cin >> N >> Q;
        HLD hld(N);
        for(int i=0;i<N-1;i++){
            int u,v,w; cin>>u>>v>>w;
            hld.addEdge(u,v,w);
        }
        hld.build(1);

        // build base array: xorToRoot in pos-order
        vector<int> base(N+1);
        for(int u=1;u<=N;u++)
            base[ hld.pos[u] ] = hld.xorToRoot[u];

        // persistent trie
        PersistentTrie trie(N);
        for(int i=1;i<=N;i++) trie.insert(i-1, base[i]);

        while(Q--) {
            int U,V; cin>>U>>V;
            int C = hld.xorToRoot[U];
            auto proc = [&](int l, int r){
                return trie.queryMax(l-1,r,C);
            };
            int ans = hld.queryPath(U,V,proc);
            cout<<ans<<"\n";
        }
    }
    return 0;
}
