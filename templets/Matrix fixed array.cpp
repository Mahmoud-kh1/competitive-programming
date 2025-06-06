#include <bits/stdc++.h>
using namespace std;

#define int long long
static const int MOD = 1000000007;
static const int MAXN = 201;

template<class T = int>
struct Matrix {
    int rows, cols;
    T arr[MAXN][MAXN];

    Matrix(int r, int c) : rows(r), cols(c) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                arr[i][j] = 0;
            }
        }
    }

    T* operator[](int i) {
        return arr[i];
    }
    const T* operator[](int i) const {
        return arr[i];
    }

    Matrix operator*(const Matrix& B) const {
        assert(cols == B.rows);
        Matrix prod(rows, B.cols);

        for (int i = 0; i < rows; i++) {
            for (int k = 0; k < cols; k++) {
                if (arr[i][k] == 0) continue;
                T aik = arr[i][k];
                for (int j = 0; j < B.cols; j++) {
                    if (B.arr[k][j] == 0) continue;
                    prod.arr[i][j] +=( 1LL * aik * B.arr[k][j] ) % MOD;
                    if (prod.arr[i][j] >= MOD) prod.arr[i][j] -= MOD;
                }
            }
        }

        return prod;
    }

    static Matrix identity(int n) {
        Matrix I(n, n);
        for (int i = 0; i < n; i++) {
            I.arr[i][i] = 1;
        }
        return I;
    }
};

// Fast exponentiation of a square matrix: base^exp
template<class T = int>
Matrix<T> mat_pow(Matrix<T> base, long long exp) {
    int n = base.rows;
    assert(n == base.cols);

    Matrix<T> result = Matrix<T>::identity(n);
    while (exp > 0) {
        if (exp & 1LL) {
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

    int tc = 1;
    while (tc--) {
        int n, m, q;
        cin >> n >> m >> q;

        Matrix<int> trans(n, n);
        for (int i = 0; i < m; i++) {
            int u, v;
            cin >> u >> v;
            u--; v--;
            trans[u][v] = (trans[u][v] + 1) % MOD;
        }

        vector<Matrix<int>> up(32, Matrix<int>(n, n));
        up[0] = trans;
        for (int i = 1; i < 32; i++) {
            up[i] = up[i - 1] * up[i - 1];
        }

        while (q--) {
            int s, t, k;
            cin >> s >> t >> k;
            s--; t--;

            Matrix<int> base(1, n);
            base[0][s] = 1;

            for (int bit = 0; bit < __lg(k) + 1; bit++) {
                if ((k >> bit) & 1LL) {
                    base = base * up[bit];
                }
            }

            cout << base[0][t] << "\n";
        }
    }

    return 0;
}
