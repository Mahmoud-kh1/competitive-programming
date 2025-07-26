#include <bits/stdc++.h>

using namespace std;
#define int long long

struct SegTree {
#define LF (x*2+1)
#define RT (x*2+2)
#define md ((lx+rx) >> 1)
  vector<int> seg;
  int sz;

  SegTree(int n) {
    sz = n;
    seg.assign(4 * n, {});
  }

  int merge(const int &lf, const int &rt) {
    return max(lf, rt);
  }

  void update(int i, int v, int x = 0, int lx = 0, int rx = -1) {
    if (rx == -1)rx = sz - 1;
    if (lx == rx) {
      seg[x] = v;
      return;
    }
    if (i <= md) {
      update(i, v, LF, lx, md);
    } else {
      update(i, v, RT, md + 1, rx);
    }
    seg[x] = merge(seg[LF], seg[RT]);
  }

  int query(int r, int x = 0, int lx = 0, int rx = -1) {
    if (rx == -1)rx = sz - 1;
    if (lx == rx) {
      return lx;
    }
    if (seg[LF] > r) {
      return query(r, LF, lx, md);
    }
    return query(r, RT, md + 1, rx);
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
  cin >> n;
  vector<int> arr(n);
  for (int &x: arr) {
    cin >> x;
    x = min(x, n + 1);
  }
  cin >> q;
  vector<pair<int, int>> queries[n];
  for (int i = 0, l, r; i < q; ++i) {
    cin >> l >> r;
    --l, --r;
    queries[l].emplace_back(r, i);
  }
  vector<int> where(n + 2, n);
  vector<int> nxt(n, n);
  for (int i = n - 1; i >= 0; --i) {
    nxt[i] = where[arr[i]];
    where[arr[i]] = i;
  }
  SegTree seg(n + 2);
  for (int i = 0; i <= n; ++i) {
    seg.update(i, where[i]);
  }
  vector<int> res(q);
  for (int l = 0; l < n; ++l) {
    for (const auto &[r, iq]: queries[l]) {
      res[iq] = seg.query(r);
    }
    seg.update(arr[l], nxt[l]);
  }
  for (int &x: res) {
    cout << x << '\n';
  }
  return 0;
}
