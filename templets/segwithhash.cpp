const int M = 1, N = 1e6 + 7;
// be carful with date types
int bases[M], mods[M], powers[M][N], pre[M][N];
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
        pre[i][0] = 1;
        for (int j = 1; j < N; ++j) {
            powers[i][j] = 1LL * powers[i][j - 1] * bases[i] % mods[i];
            pre[i][j] = (pre[i][j - 1] + powers[i][j]) % mods[i];
        }

    }

}

struct Node{
    array<int,M>pre, rev;
    int sz;
};

struct segTree{
    vector<Node>tree;
    vector<int>lazy;
    int segSz;
    Node merge(Node leftNode, Node rightNode){
        array<int,M>hashLeftPre = leftNode.pre;
        array<int,M>hashRightPre = rightNode.pre;
        array<int,M>hashLeftRev = leftNode.rev;
        array<int,M>hashRightRev = rightNode.rev;
        Node res{};
        for(int j = 0; j < M; j++){
            res.pre[j] = (1LL * hashLeftPre[j] * powers[j][rightNode.sz] + hashRightPre[j]) % mods[j];
            res.rev[j] = (1LL * hashRightRev[j] * powers[j][leftNode.sz] + hashLeftRev[j] ) % mods[j];
        }
        res.sz = leftNode.sz + rightNode.sz;
        return res;
    }
    void build(string &s){
        int old = s.size();
        segSz = old;
        if (old & (old - 1)) segSz = (1 << (__lg(old) + 1));
        tree.resize(segSz << 1,{{}, {}});
        lazy.resize(segSz << 1 , -1);
        for(int i = 0; i < old; i++){
            for(int j = 0; j < M; j++){
                tree[i + segSz].pre[j] = s[i];
                tree[i + segSz].rev[j] = s[i];
            }
            tree[i + segSz].sz = 1;
        }
        for(int i = segSz - 1; i >= 1; i--){
            tree[i] = merge(tree[i << 1], tree[i << 1 | 1]);
        }
    }
    void propogate(int l, int r, int k){
        int val = lazy[k];
        if (lazy[k] == -1) return;
        if (l != r){
            lazy[k << 1] = val;
            lazy[k << 1 | 1] = val;
        }
        lazy[k] = -1;
        for(int i = 0; i < M; i++){
            tree[k].pre[i] = (val * pre[i][tree[k].sz - 1]) % mods[i];
            tree[k].rev[i] = (val * pre[i][tree[k].sz - 1]) % mods[i];
        }
    }
    void update (int ql, int qr, int L, int R, int k, int val){
        propogate(L, R, k);
        if (R < ql || L > qr) return;
        if (L >= ql && R <= qr){
            lazy[k] = val;
            propogate(L, R, k);
            return;
        }
        int mid = (L + R) / 2;
        update(ql, qr, L, mid, k << 1 , val);
        update(ql, qr, mid + 1, R, k << 1 | 1, val);
        tree[k] =  merge(tree[k << 1], tree[k << 1| 1]);
    }
    Node query (int ql, int qr, int L, int R, int k){
        propogate(L, R, k);
        if (R < ql || L > qr) {
            Node res;
            res.sz = 0;
            for(int j = 0; j < M; j++) res.pre[j] = res.rev[j] = 0;
            return res;
        }
        if (L >= ql && R <= qr){
            return tree[k];
        }
        int mid = (L + R) / 2;
        return merge(query(ql, qr, L, mid, k << 1),
                     query(ql, qr, mid + 1 , R, k << 1 | 1)
        );
    }
    Node query(int l, int r){
        return query(l, r , 0, segSz - 1, 1);
    }
};
