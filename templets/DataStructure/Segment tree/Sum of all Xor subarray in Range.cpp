#include <bits/stdc++.h>

// change elelemnt 
// compute the sum of Xor of all subarray in range [l, r]
using namespace std;
#define ll long long
const int M = 26, mod = 4001;

struct Node {
  ll ans = 0;
  int pre[2]{}, suf[2]{};
  int p = 0;

  Node(int x) {
    ans = x;
    pre[x] = 1;
    suf[x] = 1;
    p = x;
  }

  Node() {}
};

struct SegTree {
#define LF (x*2+1)
#define RT (x*2+2)
#define md ((lx+rx) >> 1)
  vector<Node> seg;
  int sz;

  Node mrg(const Node &lf, const Node &rt) {
    Node res{};
    res.p = lf.p ^ rt.p;
    res.ans = (lf.ans + rt.ans) % mod;
    (res.ans += lf.suf[0] * 1LL * rt.pre[1] % mod) %= mod;
    (res.ans += lf.suf[1] * 1LL * rt.pre[0] % mod) %= mod;
    (res.pre[0] = lf.pre[0] + rt.pre[lf.p]) %= mod;
    (res.pre[1] = lf.pre[1] + rt.pre[!lf.p]) %= mod;
    (res.suf[0] = rt.suf[0] + lf.suf[rt.p]) %= mod;
    (res.suf[1] = rt.suf[1] + lf.suf[!rt.p]) %= mod;
    return res;
  }

  void build(vector<int> &a, int x, int lx, int rx) {
    if (lx == rx) {
      seg[x] = Node(a[lx]);
      return;
    }
    build(a, LF, lx, md);
    build(a, RT, md + 1, rx);
    seg[x] = mrg(seg[LF], seg[RT]);
  }

  SegTree(int n) {
    sz = n;
    seg.assign(4 * n, {});
  }

  void update(int i, int v, int x = 0, int lx = 0, int rx = -1) {
    if (rx == -1)rx = sz - 1;
    if (lx == rx) {
      seg[x] = Node(v);
      return;
    }
    if (i <= md) {
      update(i, v, LF, lx, md);
    } else {
      update(i, v, RT, md + 1, rx);
    }
    seg[x] = mrg(seg[LF], seg[RT]);
  }

  Node query(int l, int r, int x = 0, int lx = 0, int rx = -1) {
    if (rx == -1)rx = sz - 1;
    if (l <= lx and rx <= r) return seg[x];
    if (r < lx or rx < l) return {};
    return mrg(
        query(l, r, LF, lx, md),
        query(l, r, RT, md + 1, rx)
    );
  }

#undef md
#undef LF
#undef RT
};

int32_t main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
#ifndef ONLINE_JUDGE
  freopen("out.txt", "w", stdout);
#endif
  int n, q;
  cin >> n >> q;
  vector<SegTree> seg(10, SegTree(n));
  for (int i = 0, x; i < n; ++i) {
    cin >> x;
    for (int j = 0; j < 10; ++j) {
      seg[j].update(i, x >> j & 1);
    }
  }
  int ty, l, r;
  while (q--) {
    cin >> ty;
    if (ty == 1) {
      int p, x;
      cin >> p >> x, --p;
      for (int i = 0; i < 10; ++i) {
        seg[i].update(p, x >> i & 1);
      }
    } else {
      cin >> l >> r, --l, --r;
      ll ans = 0;
      for (int i = 0; i < 10; ++i) {
        ll cur = seg[i].query(l, r).ans;
        ans += (cur * (1 << i)) % mod;
        ans %= mod;
      }
      cout << ans << '\n';
    }
  }
  return 0;
}
