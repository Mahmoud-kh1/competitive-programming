#include <bits/stdc++.h>
using namespace std;
#define int long long
const int N = 1e6 + 7;
const int mod = 1e9 + 7;

int fib[N]{};

signed main() {
    freopen("fib.in", "r", stdin);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    fib[0] = 0;
    fib[1] = 1;
    for (int i = 2; i < N; i++) {
        fib[i] = (fib[i - 1] + fib[i - 2]) % mod;
    }

    while (t--) {
        int m;
        cin >> m;

        int sum = 0, sum_1 = 0, sum_2 = 0;

        while (m--) {
            int ty;
            cin >> ty;
            if (ty == 1) {
                int n;
                cin >> n;
                sum = (sum + fib[n]) % mod;
                sum_1 = (sum_1 + fib[n - 1]) % mod;
            } else {
                int k;
                cin >> k;
                int fk   = fib[k];
                int fkp1 = fib[k + 1];
                int fkm1 = (k >= 1 ? fib[k - 1] : 0);
                int new_sum   = (sum   * fkp1 % mod + sum_1 * fk  % mod) % mod;
                int new_sum_1 = (sum   * fk   % mod + sum_1 * fkm1% mod) % mod;
                sum   = new_sum;
                sum_1 = new_sum_1;
            }
            cout << sum << endl;
        }
    }

    return 0;
}
