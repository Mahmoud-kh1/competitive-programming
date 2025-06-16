struct Seg {
    // 0 indexed
    int n;
    vector<int> seg;
    static constexpr int iden = 0;

    static int merge(const int &a, const int &b) {
        return max(a, b);
    }

    Seg(int _n) : n(_n), seg(2 * n + 5, iden) {}

    void build(const vector<int> &a) {
        for (int i = 0; i < n; i++)
            seg[n + i] = a[i];
        for (int i = n - 1; i > 0; i--)
            seg[i] = merge(seg[i<<1], seg[i<<1|1]);
    }

    void update(int p, int v) {
        p += n;
        seg[p] = v;
        for (p >>= 1; p > 0;     p >>= 1)
            seg[p] = merge(seg[p<<1], seg[p<<1|1]);
    }

    int query(int l, int r) const {
        r++; // becase query(l, r) = [l, r[
        int resl = iden, resr = iden;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) resl = merge(resl, seg[l++]);
            if (r & 1) resr = merge(seg[--r], resr);
        }
        return merge(resl, resr);
    }
};
