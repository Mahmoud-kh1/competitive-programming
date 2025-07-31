#include <bits/stdc++.h>

using namespace std;
#define int long long

template<typename T, class CMP = function<T(const T &, const T &)>>
class SparseTable {
public:
  int n;
  vector<vector<T>> sp;
  CMP func;

  SparseTable(const vector<T> &a, const CMP &f) : func(f) {
    n = a.size();
    int max_log = 32 - __builtin_clz(n);
    sp.resize(max_log);
    sp[0] = a;
    for (int j = 1; j < max_log; ++j) {
      sp[j].resize(n - (1 << j) + 1);
      for (int i = 0; i + (1 << j) <= n; ++i) {
        sp[j][i] = func(
            sp[j - 1][i],
            sp[j - 1][i + (1 << (j - 1))]
        );
      }
    }
  }

  T query(int l, int r) const {
    int lg = __lg(r - l + 1);
    return func(
        sp[lg][l],
        sp[lg][r - (1 << lg) + 1]
    );
  }
};

int32_t main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
#ifndef ONLINE_JUDGE
  freopen("out.txt", "w", stdout);
#endif
  int n;
  cin >> n;
  vector<int> arr(n);
  for (int &x: arr) cin >> x;
  SparseTable sp(arr, [](int a, int b) { return gcd(a, b); });
  map<int, int> mp;
  for (int l = 0; l < n; ++l) {
    int g = 0, last_index = l;
    do {
      g = gcd(arr[last_index], g);
      int s = last_index, e = n - 1, md, ans;
      while (s <= e) {
        md = (e + s) >> 1;
        int cur_gcd = sp.query(l, md);
        if (cur_gcd == g) {
          s = md + 1, ans = md;
        } else {
          e = md - 1;
        }
      }
      /// l --> [ last_index, ans ] = g
      mp[g] += ans - last_index + 1;
      last_index = ans + 1;
    } while (last_index < n);
  }
  int q;cin >> q;
  while (q--){
    int x;  cin >> x;
    cout << mp[x] << '\n';
  }
  return 0;
}
