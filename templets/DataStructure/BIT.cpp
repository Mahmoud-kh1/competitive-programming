struct BIT {
    int sz;
    std::vector<long long> b1, b2;

    BIT(int sz) : sz(sz), b1(sz + 2, 0), b2(sz + 2, 0) {}

    // Add range [l, r]
    void add_r(int l, int r, long long v) {
        for (int i = l + 1; i <= sz + 1; i += i & -i) b1[i] += v, b2[i] += v * l;
        for (int i = r + 2; i <= sz + 1; i += i & -i) b1[i] -= v, b2[i] -= v * (r + 1);
    }

    // Add point i
    void add_p(int i, long long v) { add_r(i, i, v); }

    // Prefix sum [0...i]
    long long get(int i) {
        if (i < 0) return 0;
        i = std::min(i, sz - 1);
        long long s1 = 0, s2 = 0;
        for (int j = i + 1; j > 0; j -= j & -j) s1 += b1[j], s2 += b2[j];
        return s1 * (i + 1) - s2;
    }

    // Get point i
    long long get_p(int i) { return get(i) - get(i - 1); }

    // Get range [l, r]
    long long get_r(int l, int r) { return get(r) - get(l - 1); }

    // =========================================================================
    // 1. POINT VALUE BOUNDS: O(log N)
    // REQUIRES: Monotonic / Sorted array (A[0] <= A[1] <= ... <= A[sz-1])
    // =========================================================================
    
    // First 0-based index where A[i] >= val
    int lower_bound_val(long long val) {
        long long sum = 0;
        int idx = 0;
        for (int i = 31 - __builtin_clz(sz); i >= 0; i--) {
            int nxt = idx + (1 << i);
            if (nxt <= sz && sum + b1[nxt] < val) {
                idx = nxt;
                sum += b1[idx];
            }
        }
        return idx; // Returns sz if all elements < val
    }

    // First 0-based index where A[i] > val
    int upper_bound_val(long long val) {
        long long sum = 0;
        int idx = 0;
        for (int i = 31 - __builtin_clz(sz); i >= 0; i--) {
            int nxt = idx + (1 << i);
            if (nxt <= sz && sum + b1[nxt] <= val) {
                idx = nxt;
                sum += b1[idx];
            }
        }
        return idx; // Returns sz if all elements <= val
    }

    // =========================================================================
    // 2. PREFIX SUM BOUNDS: O(log N)
    // REQUIRES: Non-negative elements (A[i] >= 0)
    // =========================================================================

    // First 0-based index where prefix sum [0...i] >= val
    int lower_bound_pref(long long val) {
        if (val <= 0) return 0;
        long long s1 = 0, s2 = 0;
        int idx = 0;
        for (int i = 31 - __builtin_clz(sz); i >= 0; i--) {
            int nxt = idx + (1 << i);
            if (nxt <= sz && (s1 + b1[nxt]) * nxt - (s2 + b2[nxt]) < val) {
                idx = nxt;
                s1 += b1[idx];
                s2 += b2[idx];
            }
        }
        return idx; // Returns sz if prefix sum < val
    }

    // First 0-based index where prefix sum [0...i] > val
    int upper_bound_pref(long long val) {
        if (val < 0) return 0;
        long long s1 = 0, s2 = 0;
        int idx = 0;
        for (int i = 31 - __builtin_clz(sz); i >= 0; i--) {
            int nxt = idx + (1 << i);
            if (nxt <= sz && (s1 + b1[nxt]) * nxt - (s2 + b2[nxt]) <= val) {
                idx = nxt;
                s1 += b1[idx];
                s2 += b2[idx];
            }
        }
        return idx; // Returns sz if prefix sum <= val
    }
};
