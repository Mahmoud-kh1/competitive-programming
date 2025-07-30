#include <bits/stdc++.h>
using namespace std;


random_device rd;
mt19937 mt(rd());

int rnd(int l, int r) { return uniform_int_distribution<int>(l, r)(mt); }

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
     int t; cin >> t;
     while(t--) {
         int n, q;
         cin >> n >> q;
         map<int, long long> val1, val2;
         long long a[n], b[n], pre1a[n + 1]{}, pre2a[n + 1]{};
         for (int i = 0; i < n; i++) {
             cin >> a[i];
             if (val1.find(a[i]) == val1.end()) val1[a[i]] = rnd(5, 2e11);
             if (val2.find(a[i]) == val2.end()) val2[a[i]] = rnd(5, 2e11);
         }


         for (int i = 1; i <= n; i++) {
             if (val1[a[i]] == val2[a[i]]) val1[a[i]]++;
             pre1a[i] = pre1a[i - 1] ^ val1[a[i - 1]];
             pre2a[i] = pre2a[i - 1] ^ val2[a[i - 1]];
         }
         while (q--) {
             int l, r;
             cin >> l >> r;
             if (!(pre1a[r] ^ pre1a[l - 1]) && !(pre2a[r] ^ pre2a[l - 1])) {
                 cout << "Yes" << endl;
             } else cout << "No" << endl;
         }

     }


    return 0;
}
