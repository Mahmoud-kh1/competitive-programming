#include <bits/stdc++.h>
using namespace std;

vector<int> mods = {2000000011, 2000000019, 2000000000};
vector<int> bases = {231, 531, 644};

const int N = 1e6 + 5;
long long powers[2][N];

void prePowers() {
    for (int i = 0; i < 2; i++) {
        powers[i][0] = 1;
        for (int j = 1; j < N; j++) {
            powers[i][j] = (powers[i][j - 1] * bases[i]) % mods[i];
        }
    }
}

struct Hash {
    vector<array<long long, 2>> pre, suf;
    int n;

    void init(string s) {
        n = s.size();
        pre.resize(n);
//        suf.resize(n);

        for (auto &i : s) i -= 'a', i++;

        pre[0] = {s[0], s[0]};
//        suf[n - 1] = {s[n - 1], s[n - 1], s[n - 1]};

        for (int i = 0; i < 2; i++) {
            for (int j = 1; j < n; j++) {
                pre[j][i] = (pre[j - 1][i] * bases[i] + s[j]) % mods[i];
            }
//            for (int j = n - 2; j >= 0; j--) {
//                suf[j][i] = (suf[j + 1][i] * bases[i] + s[j]) % mods[i];
//            }
        }
    }

    array<long long, 2> queryPre(int l, int r) {
        if (l == 0) return pre[r];
        array<long long, 2> res;
        for (int i = 0; i < 2; i++) {
            res[i] = (pre[r][i] - pre[l - 1][i] * powers[i][r - l + 1] % mods[i] + mods[i]);
            if (res[i] >= mods[i]) res[i] -= mods[i];
        }
        return res;
    }

//    array<long long, 3> querySuf(int l, int r) {
//        if (r == n - 1) return suf[l];
//        array<long long, 3> res;
//        for (int i = 0; i < 3; i++) {
//            res[i] = (suf[l][i] - suf[r + 1][i] * powers[i][r - l + 1] % mods[i] + mods[i]);
//            if (res[i] >= mods[i]) res[i] -= mods[i];
//        }
//        return res;
//    }
};
typedef unsigned long long ull;
int gen_base(int before, int after) {
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed ^ ull(new ull));
    std::uniform_int_distribution<int> dist(before+2, after-1);
    int base = dist(gen);
    return base % 2 == 0 ? base - 1 : base;
}

template <int M>
struct HashTableForArray {
    // Use one of these mod values: 2000177, 3000077, 4000277, etc.
    static const int mod = 2000177;
    // step is chosen randomly from [mod/10, mod)
    const int step;

    // Each entry stores whether it is used and the key, which is an array of M long long.
    struct Entry {
        bool used;
        array<long long, M> key;
        Entry() : used(false) {}
    };

    // Our table will be stored in a fixed-size array.
    array<Entry, mod> data;

    // Custom hash function for an array<long long, M>
    inline int get_hash(const array<long long, M>& key) const {
        ull h = 0;
        // Combine elements using a prime multiplier (131 here)
        for (int i = 0; i < M; i++) {
            // Casting to ull ensures we work with unsigned arithmetic.
            h = h * 131ULL + (ull) key[i];
        }
        return int((h + step) % mod);
    }

    // Constructor: choose a random step and initialize entries.
    HashTableForArray() : step(gen_base(mod / 10, mod)) {
        // Entries are default-constructed (used is false).
    }

    // Insert the key if not already present.
    void insert(const array<long long, M>& key) {
        int hash = get_hash(key);
        while (true) {
            if (!data[hash].used) {
                data[hash].used = true;
                data[hash].key = key;
                return;
            }
            if (data[hash].key == key) {
                // Key already exists.
                return;
            }
            hash += step;
            if (hash >= mod) hash -= mod;
        }
    }

    // Search for a key. Returns true if found.
    bool search(const array<long long, M>& key) const {
        int hash = get_hash(key);
        while (true) {
            if (!data[hash].used) return false;
            if (data[hash].key == key) return true;
            hash += step;
            if (hash >= mod) hash -= mod;
        }
        return false;
    }
};
random_device rd;
mt19937 mt(rd());

int rnd(int l, int r) { return uniform_int_distribution<int>(l, r)(mt); }

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    prePowers();
    string a, b; cin >> a >> b;
    Hash hasha; hasha.init(a);
    Hash hashb; hashb.init(b);
    int L = 1, R = min(a.size(),  b.size()), ans = 0, beg = 0;
    while(L <= R){
        int mid = (L + R) / 2;
        HashTableForArray<2> ht;
        for(int i = 0; i + mid - 1 < a.size(); i++){
            ht.insert(hasha.queryPre(i, i + mid - 1));
        }
        int ok = -1;
        for(int i = 0; i + mid - 1 < b.size(); i++){
            if (ht.search(hashb.queryPre(i, i + mid - 1))){
                ok = i;
            }
        }
        if (ok != -1){
            ans = mid;
            beg = ok;
            L = mid + 1;
        }
        else R = mid - 1;
    }
    if (!ans)cout << "0 0 0 0";
    else {
       array<long long,2> subB = hashb.queryPre(beg, beg + ans - 1);
       for(int i = 0; i < a.size(); i++){
           if (hasha.queryPre(i, i + ans - 1) == subB){
               cout << i << ' ' << i + ans << ' ' << beg << ' ' << beg + ans << endl;
               return 0;
           }
       }
    }









    return 0;
}
