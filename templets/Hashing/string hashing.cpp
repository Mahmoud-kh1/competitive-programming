const int M = 1, N = 1e6 + 7;
// be carful with date types
int bases[M], mods[M], powers[M][N];
struct Hash {
    // be carful with data type
    vector<array<int,M>> pre, suf;
    void build(int maxValue){
        if (bases[0])return;
        mt19937 rng(chrono::system_clock::now().time_since_epoch().count());
        auto rnd = [&](int a, int b) {
            return a + rng() % (b - a + 1);
        };
        auto checkPrime = [&](int x) {
            for (long long i = 2; i * i <= x; ++i)if (x % i == 0) return false;
            return true;
        };
        for (int i = 0; i < M; ++i) {
            bases[i] = rnd(maxValue, 10LL * maxValue);
            mods[i] = rnd(max(bases[i] + 5, (int)1e9 + 20 ), 2e9 );
            while (!checkPrime(mods[i]))mods[i]--;
            powers[i][0] = 1;
            for (int j = 1; j < N; ++j)
                powers[i][j] = 1LL * powers[i][j - 1] * bases[i] % mods[i];
        }
    }
    void init(string &s, int sf, int maxValue) {
        build(maxValue);
        int n = s.size();
        pre.assign(n, {});
        array<int, M> preHash{};
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < M; ++j) {
                preHash[j] = (1LL * preHash[j] * bases[j]) % mods[j];
                preHash[j] = (preHash[j] + (s[i]));
                if (preHash[j] >= mods[j]) preHash[j]-= mods[j];
            }
            pre[i] = preHash;
        }
        if (sf) {
            suf.assign(n, {});
            array<int, M> sufHash{};
            for (int i = n - 1; i >= 0; --i) {
                for (int j = 0; j < M; ++j) {
                    sufHash[j] = (1LL * sufHash[j] * bases[j]) % mods[j];
                    sufHash[j] = (sufHash[j] + (s[i]));
                    if (sufHash[j] >= mods[j]) sufHash[j] -= mods[j];
                }
                suf[i] = sufHash;
            }
        }
    }


    array<int, M> get_substr_Hash(int l, int r) {
        if (l == 0)return pre[r];
        array<int, M> res = pre[r];
        for (int i = 0; i < M; ++i) {
            res[i] -= 1LL * pre[l - 1][i] * powers[i][r - l + 1] % mods[i];
            if (res[i] < 0) res[i] += mods[i];
        }
        return res;
    }
    array<int, M> get_reverse_Hash(int l, int r) {
        if (r + 1 == suf.size())return suf[l];

        array<int, M> res = suf[l];
        for (int i = 0; i < M; ++i) {
            res[i] -= 1LL * suf[r + 1][i] * powers[i][r - l + 1] % mods[i];
            if (res[i] < 0) res[i] += mods[i];
        }
        return res;
    }
    bool isPalindrome(int l, int r) {
        return get_substr_Hash(l, r) == get_reverse_Hash(l, r);
    }
};
