#include <bits/stdc++.h>
using namespace std;
#define int long long

const uint64_t HashMod = (1ULL << 61) - 1;
const uint64_t seed = 1337; // Fixed seed for reproducible sample testing

// Three distinct bases for 3D hashing (X, Y, and Z axes)
const uint64_t base1 = mt19937_64(seed)() % (HashMod / 3) + (HashMod / 3);
const uint64_t base2 = mt19937_64(seed + 1)() % (HashMod / 3) + (HashMod / 3);
const uint64_t base3 = mt19937_64(seed + 2)() % (HashMod / 3) + (HashMod / 3);

const int N = 205; 
uint64_t base_pow1[N], base_pow2[N], base_pow3[N];

int64_t MUL(uint64_t a, uint64_t b) {
    uint64_t l1 = (uint32_t) a, h1 = a >> 32, l2 = (uint32_t) b, h2 = b >> 32;
    uint64_t l = l1 * l2, m = l1 * h2 + l2 * h1, h = h1 * h2;
    uint64_t ret = (l & HashMod) + (l >> 61) + (h << 3) + (m >> 29) + (m << 35 >> 3) + 1;
    ret = (ret & HashMod) + (ret >> 61);
    ret = (ret & HashMod) + (ret >> 61);
    return (int64_t) ret - 1;
}

void init3D() {
    base_pow1[0] = 1;
    base_pow2[0] = 1;
    base_pow3[0] = 1;
    for (int i = 1; i < N; i++) {
        base_pow1[i] = MUL(base_pow1[i - 1], base1);
        base_pow2[i] = MUL(base_pow2[i - 1], base2);
        base_pow3[i] = MUL(base_pow3[i - 1], base3);
    }
}

struct PolyHash3D {
    vector<vector<vector<int64_t>>> pref;
    
    template<typename T>
    PolyHash3D(const vector<vector<T>> &grid) {
        if (!base_pow1[0]) init3D();

        int D = grid.size();       
        int R = grid[0].size();    
        int C = grid[0][0].size(); 
        assert(D < N && R < N && C < N);
        
        pref.assign(D + 3, vector<vector<int64_t>>(R + 3, vector<int64_t>(C + 3, 0)));
        vector<vector<vector<int64_t>>> H1(D + 3, vector<vector<int64_t>>(R + 3, vector<int64_t>(C + 3, 0)));
        vector<vector<vector<int64_t>>> H2(D + 3, vector<vector<int64_t>>(R + 3, vector<int64_t>(C + 3, 0)));

        // 1. Hash horizontally along X (columns)
        for (int z = 1; z <= D; z++) {
            for (int y = 1; y <= R; y++) {
                for (int x = 1; x <= C; x++) {
                    H1[z][y][x] = MUL(H1[z][y][x - 1], base1) + grid[z - 1][y - 1][x - 1] + 997;
                    if (H1[z][y][x] >= HashMod) H1[z][y][x] -= HashMod;
                }
            }
        }

        // 2. Hash along Y (rows)
        for (int z = 1; z <= D; z++) {
            for (int y = 1; y <= R; y++) {
                for (int x = 1; x <= C; x++) {
                    H2[z][y][x] = MUL(H2[z][y - 1][x], base2) + H1[z][y][x];
                    if (H2[z][y][x] >= HashMod) H2[z][y][x] -= HashMod;
                }
            }
        }

        // 3. Hash along Z (layers)
        for (int z = 1; z <= D; z++) {
            for (int y = 1; y <= R; y++) {
                for (int x = 1; x <= C; x++) {
                    pref[z][y][x] = MUL(pref[z - 1][y][x], base3) + H2[z][y][x];
                    if (pref[z][y][x] >= HashMod) pref[z][y][x] -= HashMod;
                }
            }
        }
    }

    uint64_t get_hash(int z1, int y1, int x1, int z2, int y2, int x2) {
        int len_z = z2 - z1 + 1;
        int len_y = y2 - y1 + 1;
        int len_x = x2 - x1 + 1;
        
        z1++; y1++; x1++; z2++; y2++; x2++;

        int64_t ans = pref[z2][y2][x2];

        int64_t sub_z = MUL(pref[z1 - 1][y2][x2], base_pow3[len_z]);
        ans -= sub_z; if (ans < 0) ans += HashMod;

        int64_t sub_y = MUL(pref[z2][y1 - 1][x2], base_pow2[len_y]);
        ans -= sub_y; if (ans < 0) ans += HashMod;

        int64_t sub_x = MUL(pref[z2][y2][x1 - 1], base_pow1[len_x]);
        ans -= sub_x; if (ans < 0) ans += HashMod;

        int64_t add_zy = MUL(pref[z1 - 1][y1 - 1][x2], MUL(base_pow3[len_z], base_pow2[len_y]));
        ans += add_zy; if (ans >= HashMod) ans -= HashMod;

        int64_t add_zx = MUL(pref[z1 - 1][y2][x1 - 1], MUL(base_pow3[len_z], base_pow1[len_x]));
        ans += add_zx; if (ans >= HashMod) ans -= HashMod;

        int64_t add_yx = MUL(pref[z2][y1 - 1][x1 - 1], MUL(base_pow2[len_y], base_pow1[len_x]));
        ans += add_yx; if (ans >= HashMod) ans -= HashMod;

        int64_t sub_zyx = MUL(pref[z1 - 1][y1 - 1][x1 - 1], MUL(MUL(base_pow3[len_z], base_pow2[len_y]), base_pow1[len_x]));
        ans -= sub_zyx; if (ans < 0) ans += HashMod;

        return ans;
    }
};

signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    // --- SAMPLE TEST CASE ---
    
    // 1. Define a 3D Haystack (2 layers, 3 rows, 3 columns)
    vector<vector<string>> haystack = {
        {
            "abc",
            "def",
            "ghi"
        },
        {
            "jkl",
            "mno",
            "pqr"
        }
    };

    // 2. Define a 3D Needle/Subcube to search for (2 layers, 2 rows, 2 columns)
    // We will carve out a piece from the haystack starting at z=0, y=1, x=1:
    // Layer 0 (rows 1-2, cols 1-2): "ef", "hi"
    // Layer 1 (rows 1-2, cols 1-2): "no", "qr"
    vector<vector<string>> needle = {
        {
            "ef",
            "hi"
        },
        {
            "no",
            "qr"
        }
    };

    // Build hashes
    PolyHash3D hashHaystack(haystack);
    PolyHash3D hashNeedle(needle);

    uint64_t targetHash = hashNeedle.get_hash(0, 0, 0, 1, 1, 1);

    int D_h = haystack.size(), R_h = haystack[0].size(), C_h = haystack[0][0].size();
    int D_k = needle.size(), R_k = needle[0].size(), C_k = needle[0][0].size();

    cout << "Searching for subcube in 3D Haystack...\n";
    int matches = 0;

    // Scan the 3D space
    for (int z = 0; z <= D_h - D_k; z++) {
        for (int y = 0; y <= R_h - R_k; y++) {
            for (int x = 0; x <= C_h - C_k; x++) {
                if (hashHaystack.get_hash(z, y, x, z + D_k - 1, y + R_k - 1, x + C_k - 1) == targetHash) {
                    cout << "Match found at Top-Left-Front coordinate: (Layer z=" << z << ", Row y=" << y << ", Col x=" << x << ")\n";
                    matches++;
                }
            }
        }
    }

    cout << "Total matches found: " << matches << "\n";

    return 0;
}
