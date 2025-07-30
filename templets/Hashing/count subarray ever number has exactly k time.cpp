#include <bits/stdc++.h>

#define ll long long
const int M = 60;
#define item array<char,M>
using namespace std;

random_device rd;
mt19937 mt(rd());

int rnd(int l, int r) {
  return uniform_int_distribution<int>(l, r)(mt);
}


item operator^(const item &a, const item &b) {
  item res{};
  for (int i = 0; i < M; ++i) {
    res[i] = (a[i] + b[i]) % 3;
  }
  return res;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int n;
  cin >> n;
  vector<int> where(n + 1), vals(n + 1);
  vector<item > arr(n + 1);
  for (int i = 1, x; i <= n; ++i) {
    cin >> x;
    vals[i] = x;
    if (where[x]) {
      arr[i] = arr[where[x]];
    } else {
      where[x] = i;
      for (int j = 0; j < M; ++j)
        arr[i][j] = rnd(0, 2);
    }
  }
  map<item, int> mp{{{}, 1}};
  int l = 0;
  long long res{};
  vector<int> freq(n + 1);
  for (int r = 1; r <= n; ++r) {

    arr[r] = arr[r] ^ arr[r - 1];

    freq[vals[r]]++;
    while (freq[vals[r]] == 4) {
      mp[arr[l]]--;
      ++l;
      freq[vals[l]]--;
    }
    res += mp[arr[r]]++;
  }
  cout << res;
  return 0;
}
