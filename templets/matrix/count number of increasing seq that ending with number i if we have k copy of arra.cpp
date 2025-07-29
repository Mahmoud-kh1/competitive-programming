#include <bits/stdc++.h>
using namespace std;
#define int long long
static const int mod = 1e9 + 7;

template<class T = int>
struct Matrix {
    int rows, cols;
    vector<vector<T>> arr;
    Matrix(int r, int c) {
        rows = r; cols = c;
        arr.assign(rows, vector<T>(cols, 0));
    }
    vector<T>& operator[](int i) {
        return arr[i];
    }
    Matrix operator*(const Matrix& B) const {
        assert(cols == B.rows);
        Matrix prod(rows, B.cols);
        for (int i = 0; i < rows; i++) {
            for (int k = 0; k < cols; k++) {
                if (arr[i][k] == 0) continue;
                for (int j = 0; j < B.cols; j++) {
                    if (B.arr[k][j] == 0) continue;
                    // Fixed: mod the product before adding
                    prod.arr[i][j] = (prod.arr[i][j] + (__int128(arr[i][k]) * B.arr[k][j] % mod)) % mod;
                }
            }
        }
        return prod;
    }
};

template<class T = int>
Matrix<T> power(Matrix<T> base, int exp) {
    int n = base.rows;
    assert(n == base.cols);
    Matrix<T> result(n, n);
    for (int i = 0; i < n; i++) {
        result[i][i] = 1;
    }
    while (exp > 0) {
        if (exp & 1) {
            result = result * base;
        }
        base = base * base;
        exp >>= 1;
    }
    return result;
}

signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int tc;
    cin >> tc;
    while (tc--) {
        int n,k ; cin >> n >> k;
        int pos[n + 1]{};
        for(int i = 0; i < n; i++){
            int x; cin >> x;
            pos[x] = i;
        }
        int dp[n + 1]{};
        for(int i = 1; i <= n; i++){
            dp[i] = 1;
            for(int j = 1; j < i; j++) if (pos[i] > pos[j]) dp[i] = (dp[i] + dp[j]) % mod;
        }
        Matrix<int>trans(n + 2, n + 2);
        for(int i = 1; i <= n + 1; i++) trans[n + 1][i] = 1;
        for(int i = 1; i <= n; i++) trans[i][i] = 1;

        for(int i = 1; i <= n; i++){
            for(int j = 1; j < i; j++){
                if (pos[j] < pos[i]) {
                    for (int row = 1; row <= n + 1; row++) {
                        trans[row][i] += trans[row][j];
                        trans[row][i] %= mod;
                    }
                }
                else trans[j][i] = 1;

            }
        }

        Matrix<int>base(1, n + 2);
        for(int i = 1; i <= n; i++) base[0][i] = dp[i];
        base[0][n + 1] = 1;
        trans= power(trans, k - 1);
        trans = base * trans;
        for(int i = 1; i <= n; i++) cout << trans[0][i] << ' ';
        cout << endl;
    }

    return 0;
}
