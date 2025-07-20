#include "bits/stdc++.h"
using namespace std;

void fast() {
    ios::sync_with_stdio(0);
    cin.tie(NULL);
    cout.tie(0);
}

#define int long long
int mod = 1e9 + 7;

template<class T = int>
struct Matrix {
    vector<vector<T>> arr;
    int rows, cols;

    Matrix(int r, int c) {
        rows = r; cols = c;
        arr.resize(rows, vector<T>(cols, 4e18));
    }
    void makeIdentity(){
        assert(cols == cols);
        for(int i = 0; i < rows; i++) arr[i][i] = 1;
    }
    Matrix operator *(const Matrix& b) {
        Matrix product(rows, b.cols);
        assert(cols == b.rows);
        for (int i = 0; i < rows; i++) {
            for (int k = 0; k < cols; k++) {
                if (arr[i][k] == 4e18) continue;
                    for (int j = 0; j < b.cols; j++) {
                        if (b.arr[k][j] == 4e18) continue;
                        product.arr[i][j]  = min(product.arr[i][j], arr[i][k] + b.arr[k][j]);

                }
            }
        }
        return product;
    }
    vector<T>& operator [](int i){
        return arr[i];
    }
};

template<class T = int>
Matrix<T> power(Matrix<T> a, int b) {
    Matrix<T> res(a.rows, a.cols);
    for(int i = 0;  i < a.rows; i++) res[i][i] = 0;
    while (b > 0) {
        if (b & 1) res = res * a;
        a = a * a;
        b >>= 1;
    }
    return res;
}

signed main() {
    fast();
    int t = 1;
    // cin >> t;
    while (t--) {
        int n, m, k; cin >>  n >> m >> k;
        Matrix mat(n, n);
        for(int i = 0; i < m; i++){
            int u, v; cin >> u >> v;
            u--, v--;
            int w ; cin >> w;
            mat[u][v] = min(mat[u][v], w);
        }
        mat = power(mat, k);
        if (mat[0][n - 1] >= 4e18){
        cout << -1;
            }
        else cout << mat[0][n - 1];
    }
    return 0;
}
