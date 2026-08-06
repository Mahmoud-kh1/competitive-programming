#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

// =============================================================================
// FULL FWHT COMPETITIVE PROGRAMMING TEMPLATE
// =============================================================================

enum class ConvType { XOR, AND, OR };

// Helper function to normalize mod values
inline long long norm(long long x, long long mod) {
    if (mod == 0) return x;
    x %= mod;
    if (x < 0) x += mod;
    return x;
}

// In-place Fast Walsh-Hadamard Transform
// Supports optional prime modulo arithmetic (pass mod = 0 for pure integers)
void fwht(vector<long long>& a, bool invert, ConvType type, long long mod = 0) {
    int n = a.size();
    long long inv2 = (mod > 0) ? (mod + 1) / 2 : 0; // Modular inverse of 2 for odd primes

    for (int len = 1; 2 * len <= n; len <<= 1) {
        for (int i = 0; i < n; i += 2 * len) {
            for (int j = 0; j < len; j++) {
                long long u = a[i + j];
                long long v = a[i + len + j];

                if (type == ConvType::OR) {
                    if (!invert) {
                        a[i + len + j] = norm(u + v, mod);
                    } else {
                        a[i + len + j] = norm(v - u, mod);
                    }
                } else if (type == ConvType::AND) {
                    if (!invert) {
                        a[i + j] = norm(u + v, mod);
                    } else {
                        a[i + j] = norm(u - v, mod);
                    }
                } else if (type == ConvType::XOR) {
                    if (!invert) {
                        a[i + j] = norm(u + v, mod);
                        a[i + len + j] = norm(u - v, mod);
                    } else {
                        if (mod > 0) {
                            a[i + j] = norm(norm(u + v, mod) * inv2, mod);
                            a[i + len + j] = norm(norm(u - v, mod) * inv2, mod);
                        } else {
                            a[i + j] = (u + v) / 2;
                            a[i + len + j] = (u - v) / 2;
                        }
                    }
                }
            }
        }
    }
}

// -----------------------------------------------------------------------------
// 1. General K-way Convolution (A1 * A2 * ... * Ak)
// -----------------------------------------------------------------------------
vector<long long> convolve(vector<vector<long long>> vec, ConvType type, long long mod = 0) {
    if (vec.empty()) return {};
    int max_sz = 0;
    for (const auto& v : vec) max_sz = max(max_sz, (int)v.size());
    int n = 1;
    while (n < max_sz) n <<= 1;

    for (auto& v : vec) {
        v.resize(n, 0);
        fwht(v, false, type, mod);
    }

    vector<long long> C(n, 1);
    for (int i = 0; i < n; i++) {
        for (size_t k = 0; k < vec.size(); k++) {
            if (mod > 0) {
                C[i] = (C[i] * vec[k][i]) % mod;
            } else {
                C[i] *= vec[k][i];
            }
        }
    }

    fwht(C, true, type, mod);
    return C;
}

// -----------------------------------------------------------------------------
// 2. Convolution of 2 arrays with distinct indices (i != j)
// -----------------------------------------------------------------------------
vector<long long> convolve_distinct_2(vector<long long> A, vector<long long> B, ConvType type, long long mod = 0) {
    int max_sz = max(A.size(), B.size());
    int n = 1;
    while (n < max_sz) n <<= 1;
    A.resize(n, 0); B.resize(n, 0);

    // Unrestricted convolution
    vector<long long> C = convolve({A, B}, type, mod);

    // Subtract i == j contributions
    if (type == ConvType::OR || type == ConvType::AND) {
        for (int i = 0; i < n; i++) {
            long long self_term = (mod > 0) ? (A[i] * B[i]) % mod : A[i] * B[i];
            C[i] = norm(C[i] - self_term, mod);
        }
    } else if (type == ConvType::XOR) {
        long long sum_self = 0;
        for (int i = 0; i < n; i++) {
            long long self_term = (mod > 0) ? (A[i] * B[i]) % mod : A[i] * B[i];
            sum_self = norm(sum_self + self_term, mod);
        }
        C[0] = norm(C[0] - sum_self, mod);
    }
    return C;
}

// -----------------------------------------------------------------------------
// 3. Convolution of 3 arrays with distinct indices (i != j != l)
// Uses Principle of Inclusion-Exclusion (PIE):
// Distinct(A, B, C) = (A * B * C) - (T_ij + T_il + T_jl) + 2 * T_all
// -----------------------------------------------------------------------------
vector<long long> convolve_distinct_3(vector<long long> A, vector<long long> B, vector<long long> C_arr, ConvType type, long long mod = 0) {
    int max_sz = max({A.size(), B.size(), C_arr.size()});
    int n = 1; 
    while (n < max_sz) n <<= 1;
    A.resize(n, 0); B.resize(n, 0); C_arr.resize(n, 0);

    // 1. Total unrestricted sum U
    vector<long long> U = convolve({A, B, C_arr}, type, mod);

    // 2. Compute T_ij (i=j), T_il (i=l), T_jl (j=l)
    vector<long long> T_ij(n, 0), T_il(n, 0), T_jl(n, 0);

    if (type == ConvType::OR || type == ConvType::AND) {
        vector<long long> E_AB(n), E_AC(n), E_BC(n);
        for (int i = 0; i < n; i++) {
            E_AB[i] = (mod > 0) ? (A[i] * B[i]) % mod : A[i] * B[i];
            E_AC[i] = (mod > 0) ? (A[i] * C_arr[i]) % mod : A[i] * C_arr[i];
            E_BC[i] = (mod > 0) ? (B[i] * C_arr[i]) % mod : B[i] * C_arr[i];
        }
        T_ij = convolve({E_AB, C_arr}, type, mod);
        T_il = convolve({E_AC, B}, type, mod);
        T_jl = convolve({E_BC, A}, type, mod);
    } else if (type == ConvType::XOR) {
        long long S_AB = 0, S_AC = 0, S_BC = 0;
        for (int i = 0; i < n; i++) {
            long long p_ab = (mod > 0) ? (A[i] * B[i]) % mod : A[i] * B[i];
            long long p_ac = (mod > 0) ? (A[i] * C_arr[i]) % mod : A[i] * C_arr[i];
            long long p_bc = (mod > 0) ? (B[i] * C_arr[i]) % mod : B[i] * C_arr[i];

            S_AB = norm(S_AB + p_ab, mod);
            S_AC = norm(S_AC + p_ac, mod);
            S_BC = norm(S_BC + p_bc, mod);
        }
        for (int k = 0; k < n; k++) {
            T_ij[k] = (mod > 0) ? (S_AB * C_arr[k]) % mod : S_AB * C_arr[k];
            T_il[k] = (mod > 0) ? (S_AC * B[k]) % mod : S_AC * B[k];
            T_jl[k] = (mod > 0) ? (S_BC * A[k]) % mod : S_BC * A[k];
        }
    }

    // 3. Compute T_all where i = j = l
    vector<long long> T_all(n, 0);
    for (int i = 0; i < n; i++) {
        long long prod = (mod > 0) ? (A[i] * B[i]) % mod : A[i] * B[i];
        if (mod > 0) prod = (prod * C_arr[i]) % mod;
        else prod *= C_arr[i];
        T_all[i] = prod;
    }

    // 4. Combine via PIE: U - T_ij - T_il - T_jl + 2 * T_all
    vector<long long> Res(n);
    for (int k = 0; k < n; k++) {
        long long val = U[k];
        val = norm(val - T_ij[k], mod);
        val = norm(val - T_il[k], mod);
        val = norm(val - T_jl[k], mod);
        val = norm(val + 2 * T_all[k], mod);
        Res[k] = val;
    }

    return Res;
}

// =============================================================================
// PROBLEM SOLUTION: Bubble Cup 12 - Finals: D. Xor Spanning Tree
// =============================================================================

const int MOD1 = 1000000007;
const int MOD2 = 998244353;
const int MAX_WEIGHT = 131072; // 2^17, covers all XOR sums up to 100,000

struct Edge {
    int to, weight;
};

int N, M;
vector<vector<Edge>> adj;
vector<int> vis, depth, parent_node, parent_weight;
vector<vector<int>> cycles;
int T_all = 0;

void dfs(int u, int p) {
    vis[u] = 1;
    for (const auto& edge : adj[u]) {
        int v = edge.to;
        int w = edge.weight;
        if (v == p) continue;

        if (!vis[v]) {
            parent_node[v] = u;
            parent_weight[v] = w;
            depth[v] = depth[u] + 1;
            dfs(v, u);
        } else if (depth[v] < depth[u]) {
            // Found a cycle in the cactus graph
            vector<int> cycle = {w};
            int curr = u;
            while (curr != v) {
                cycle.push_back(parent_weight[curr]);
                curr = parent_node[curr];
            }
            cycles.push_back(cycle);
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> N >> M;

    adj.resize(N + 1);
    vis.assign(N + 1, 0);
    depth.assign(N + 1, 0);
    parent_node.assign(N + 1, 0);
    parent_weight.assign(N + 1, 0);

    for (int i = 0; i < M; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
        T_all ^= w;
    }

    dfs(1, 0);

    if (cycles.empty()) {
        cout << T_all << " 1\n";
        return 0;
    }

    vector<vector<long long>> poly_list;
    for (const auto& cycle : cycles) {
        vector<long long> freq(MAX_WEIGHT, 0);
        for (int w : cycle) {
            freq[w]++;
        }
        poly_list.push_back(freq);
    }

    vector<long long> ways_mod1 = convolve(poly_list, ConvType::XOR, MOD1);
    vector<long long> ways_mod2 = convolve(poly_list, ConvType::XOR, MOD2);

    int min_cost = 1e9;
    long long answer_ways = 0;

    for (int x = 0; x < MAX_WEIGHT; x++) {
        if (ways_mod1[x] > 0 || ways_mod2[x] > 0) {
            int cost = T_all ^ x;
            if (cost < min_cost) {
                min_cost = cost;
                answer_ways = ways_mod1[x] % MOD1;
            }
        }
    }

    cout << min_cost << " " << answer_ways << "\n";
    return 0;
}
