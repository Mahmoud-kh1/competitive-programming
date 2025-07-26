#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Node {
    int l, r;
    int cnt;
    ll  sum;
    Node(): l(0), r(0), cnt(0), sum(0) {}
};

static const int MAXN = 200000;
static const int MAXNODE = MAXN * 20;
// ~ (n+q)*log2(n) ≈ 2e5*18 = 3.6e6, we allocate 4e6 to be safe

Node seg[MAXNODE];
int nodes = 1;  // seg[0] is the empty node

int new_node(int from=0){
    int id = nodes++;
    seg[id] = seg[from];
    return id;
}

// point‐update: in version `prev`, add +c to cnt and +v to sum at position pos.
// returns new root index.
int update(int prev, int l, int r, int pos, int c, ll v) {
    int cur = new_node(prev);
    if (l == r) {
        seg[cur].cnt += c;
        seg[cur].sum += v;
        return cur;
    }
    int m = (l + r) >> 1;
    if (pos <= m)
        seg[cur].l = update(seg[prev].l, l, m, pos, c, v);
    else
        seg[cur].r = update(seg[prev].r, m+1, r, pos, c, v);
    seg[cur].cnt = seg[ seg[cur].l ].cnt + seg[ seg[cur].r ].cnt;
    seg[cur].sum = seg[ seg[cur].l ].sum + seg[ seg[cur].r ].sum;
    return cur;
}

// query [ql..qr] on version `root`, returns pair(cnt,sum)
pair<int,ll> query(int root, int l, int r, int ql, int qr) {
    if (!root || qr < l || r < ql) return {0,0LL};
    if (ql <= l && r <= qr) {
        return { seg[root].cnt, seg[root].sum };
    }
    int m = (l + r) >> 1;
    auto L = query(seg[root].l, l, m, ql, qr);
    auto R = query(seg[root].r, m+1, r, ql, qr);
    return { L.first + R.first, L.second + R.second };
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<ll> A(n);
    for(int i = 0; i < n; i++)
        cin >> A[i];

    // 1) Coordinate‐compress
    vector<ll> V = A;
    sort(V.begin(), V.end());
    V.erase(unique(V.begin(), V.end()), V.end());
    int m = V.size();

    // 2) Build persistent roots
    vector<int> root(n+1);
    root[0] = 0;  // empty tree
    for(int i = 1; i <= n; i++){
        int c = int(lower_bound(V.begin(), V.end(), A[i-1]) - V.begin()) + 1;
        // copy previous version, add one count and A[i] to sum at position c
        root[i] = update(root[i-1], 1, m, c, 1, A[i-1]);
    }

    // 3) Answer queries
    while(q--){
        int L, R;
        cin >> L >> R;
        ll cur = 0;
        while (true) {
            // find largest idx p with V[p] <= cur+1
            int p = int(upper_bound(V.begin(), V.end(), cur+1) - V.begin());
            if (p == 0) break;   // no coin <= cur+1
            auto qR = query(root[R],   1, m, 1, p);
            auto qL = query(root[L-1], 1, m, 1, p);
            ll sum = qR.second - qL.second;
            if (sum == cur) break;
            cur = sum;
        }
        cout << (cur + 1) << "\n";
    }
    return 0;
}
 
