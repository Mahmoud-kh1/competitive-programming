#include <bits/stdc++.h>
using namespace std;
#define int long long
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
        bases[i] = rnd(maxValue, 100LL * maxValue);
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
    void update (int ql, int qr, int L, int R, int k, int val){
        if (R < ql || L > qr) return;
        if (L >= ql && R <= qr){
            for(int j = 0; j < M; j++){
                tree[k].pre[j] = val;
                tree[k].rev[j] = val;
            }
            return;
        }
        int mid = (L + R) / 2;
        update(ql, qr, L, mid, k << 1 , val);
        update(ql, qr, mid + 1, R, k << 1 | 1, val);
        tree[k] =  merge(tree[k << 1], tree[k << 1| 1]);
    }
    Node query (int ql, int qr, int L, int R, int k){
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
signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    build(200);

    int t; cin >> t;
    while(t--){
        int n; cin >> n;
        string s; cin >> s;
        segTree seg;
        seg.build(s);
        int q; cin >> q;
        while(q--){
            int ty; cin >> ty;
            if (ty == 1){
                int in; cin >> in;
                in--;
                char x; cin >> x;
                seg.update(in, in , 0, seg.segSz -1, 1, x);
            }
            else {
                auto getMaxLen = [&](int l, int r ){
                    int L = 1, R = (r - l + 1) / 2, ans = 0;
                    while(L <= R){
                        int mid = (L + R) / 2;
                        if (seg.query(l, l + mid - 1).pre == seg.query(r - mid + 1, r).rev){
                            ans = mid;
                            L = mid + 1;
                        }
                        else R  = mid - 1;
                    }
                    return ans;
                };

                int l, r; cin >> l >> r;
                l--, r--;
                int len = getMaxLen(l, r);
                if (len == (r - l + 1) / 2){
                    cout << "YES" << endl;
                    continue;
                }
                int newL = l + len + 1, newR = r - len - 1;
                if (newL > newR){
                    cout << "YES" << endl;
                    continue;
                }
                if (getMaxLen(newL, newR) == (newR - newL + 1) / 2){
                    cout << "YES" << endl;
                }
                else cout << "NO" << endl;
            }
        }

    }




    return 0;
}
