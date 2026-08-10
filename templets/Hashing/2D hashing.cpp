#include <bits/stdc++.h>
using namespace std;
#define int long long

const uint64_t HashMod = (1ULL << 61) - 1;
const uint64_t seed = chrono::system_clock::now().time_since_epoch().count();

// We need two distinct bases for 2D hashing (one for rows, one for columns)
const uint64_t base1 = mt19937_64(seed)() % (HashMod / 3) + (HashMod / 3);
const uint64_t base2 = mt19937_64(seed + 1)() % (HashMod / 3) + (HashMod / 3);

const int N = 2005; // 2000x2000 grid maximum
uint64_t base_pow1[N], base_pow2[N];

int64_t MUL(uint64_t a, uint64_t b) {
    uint64_t l1 = (uint32_t) a, h1 = a >> 32, l2 = (uint32_t) b, h2 = b >> 32;
    uint64_t l = l1 * l2, m = l1 * h2 + l2 * h1, h = h1 * h2;
    uint64_t ret = (l & HashMod) + (l >> 61) + (h << 3) + (m >> 29) + (m << 35 >> 3) + 1;
    ret = (ret & HashMod) + (ret >> 61);
    ret = (ret & HashMod) + (ret >> 61);
    return (int64_t) ret - 1;
}

void init2D() {
    base_pow1[0] = 1;
    base_pow2[0] = 1;
    for (int i = 1; i < N; i++) {
        base_pow1[i] = MUL(base_pow1[i - 1], base1);
        base_pow2[i] = MUL(base_pow2[i - 1], base2);
    }
}

struct PolyHash2D {
    vector<vector<int64_t>> pref;
    
    template<typename T>
    PolyHash2D(const vector<T> &ar) {
        if (!base_pow1[0]) init2D();

        int n = ar.size();
        int m = ar[0].size();
        assert(n < N && m < N);
        
        pref.assign(n + 3, vector<int64_t>(m + 3, 0));
        vector<vector<int64_t>> R(n + 3, vector<int64_t>(m + 3, 0));

        // 1. Hash horizontally
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                R[i][j] = MUL(R[i][j - 1], base1) + ar[i - 1][j - 1] + 997;
                if (R[i][j] >= HashMod) R[i][j] -= HashMod;
            }
        }

        // 2. Hash vertically
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                pref[i][j] = MUL(pref[i - 1][j], base2) + R[i][j];
                if (pref[i][j] >= HashMod) pref[i][j] -= HashMod;
            }
        }
    }

    uint64_t get_hash(int r1, int c1, int r2, int c2) {
        int len_r = r2 - r1 + 1;
        int len_c = c2 - c1 + 1;
        
        r1++; c1++; r2++; c2++;

        int64_t ans = pref[r2][c2];

        int64_t top = MUL(pref[r1 - 1][c2], base_pow2[len_r]);
        ans -= top;
        if (ans < 0) ans += HashMod;

        int64_t left = MUL(pref[r2][c1 - 1], base_pow1[len_c]);
        ans -= left;
        if (ans < 0) ans += HashMod;

        int64_t top_left = MUL(pref[r1 - 1][c1 - 1], MUL(base_pow1[len_c], base_pow2[len_r]));
        ans += top_left;
        if (ans >= HashMod) ans -= HashMod;

        return ans;
    }
};

signed main() {
    // Fast I/O
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    // 1. Read Search Key
    int rk, ck;  
    if (!(cin >> rk >> ck)) return 0;
    vector<string> key(rk);
    for (int i = 0; i < rk; i++) cin >> key[i];
    
    // 2. Read Haystack
    int rh, ch; 
    cin >> rh >> ch;
    vector<string> haystack(rh);
    for (int i = 0; i < rh; i++) cin >> haystack[i];
    
    // 3. Precompute Hashes
    PolyHash2D hashKey(key);
    PolyHash2D hashHay(haystack); 
    
    // The target hash of the entire search key
    uint64_t target = hashKey.get_hash(0, 0, rk - 1, ck - 1);
    
    // 2D Difference Array to mark overlaps in O(1) time
    vector<vector<int>> diff(rh + 2, vector<int>(ch + 2, 0));
    
    // 4. Slide the window over the haystack
    for (int i = 0; i <= rh - rk; i++) {
        for (int j = 0; j <= ch - ck; j++) {
            
            // If the subgrid matches the key
            if (hashHay.get_hash(i, j, i + rk - 1, j + ck - 1) == target) {
                // Add +1 to the rectangle covering the match
                diff[i][j]++;
                diff[i][j + ck]--;
                diff[i + rk][j]--;
                diff[i + rk][j + ck]++;
            }
        }
    }
    
    // 5. Evaluate the prefix sums and print the grid
    for (int i = 0; i < rh; i++) {
        for (int j = 0; j < ch; j++) {
            // Standard 2D Prefix Sum construction in-place
            if (i > 0) diff[i][j] += diff[i - 1][j];
            if (j > 0) diff[i][j] += diff[i][j - 1];
            if (i > 0 && j > 0) diff[i][j] -= diff[i - 1][j - 1];
            
            // If the cell is covered by at least 1 match, print the char. Else print '.'
            if (diff[i][j] > 0) {
                cout << haystack[i][j];
            } else {
                cout << '.';
            }
        }
        cout << "\n";
    }
    
    return 0;
}
