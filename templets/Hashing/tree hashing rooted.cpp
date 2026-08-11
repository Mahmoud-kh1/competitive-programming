#include <bits/stdc++.h>
using namespace std;

#include <bits/stdc++.h>
using namespace std;

// Rooted Subtree Hasher (AHU Canonical Encoding)
// Compatible with C++11 and above
// Time Complexity: O(N log N)
// Space Complexity: O(N)
struct RootedTreeHasher {
    map<vector<int>, int> id_map;

    void clear() {
        id_map.clear();
    }

private:
    void dfs(int u, int p, const vector<vector<int>>& adj, vector<int>& hash_id) {
        vector<int> children;
        for (int v : adj[u]) {
            if (v != p) {
                dfs(v, u, adj, hash_id);
                children.push_back(hash_id[v]);
            }
        }

        // Sort child IDs so branch order doesn't matter
        sort(children.begin(), children.end());

        // Assign a unique ID to this child arrangement
        if (!id_map.count(children)) {
            id_map[children] = (int)id_map.size() + 1;
        }
        hash_id[u] = id_map[children];
    }

public:
    // Computes canonical subtree IDs for a tree rooted at 'root'
    vector<int> get_subtree_hashes(int root, const vector<vector<int>>& adj) {
        int n = (int)adj.size();
        vector<int> hash_id(n, 0);
        dfs(root, -1, adj, hash_id);
        return hash_id;
    }

    // Returns the canonical ID for the full tree rooted at 'root'
    int get_root_hash(int root, const vector<vector<int>>& adj) {
        return get_subtree_hashes(root, adj)[root];
    }
};
signed main() {
    int t ; 
    cin >> t;
    while (t--) {
        int n;
        cin >> n;

        vector<vector<int>> adj1(n + 1), adj2(n + 1);

        for (int i = 1; i < n; i++) {
            int u, v; cin >> u >> v;
            adj1[u].push_back(v); adj1[v].push_back(u);
        }
        for (int i = 1; i < n; i++) {
            int u, v; cin >> u >> v;
            adj2[u].push_back(v); adj2[v].push_back(u);
        }
        RootedTreeHasher hasher;

        // Both trees are rooted at node 1
        int id1 = hasher.get_root_hash(1, adj1);
        int id2 = hasher.get_root_hash(1, adj2);

        cout << (id1 == id2 ? "YES\n" : "NO\n");
    }
}
