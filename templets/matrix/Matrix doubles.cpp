#include <bits/stdc++.h>
using namespace std;

const int mod = 1e9 + 7;
#define int long long
template<class T = double>
struct Matrix {
    vector<vector<T>> arr;
    int rows, cols;

    Matrix(int r, int c) {
        rows = r; cols = c;
        arr.resize(rows, vector<T>(cols, 0));
    }
    void makeIdentity(){
        assert(cols == rows);
        for(int i = 0; i < rows; i++) arr[i][i] = 1;
    }
    Matrix operator *(const Matrix& b) {
        Matrix product(rows, b.cols);
        assert(cols == b.rows);
        for (int i = 0; i < rows; i++) {
            for (int k = 0; k < cols; k++) {
                if (arr[i][k] == 0) continue;
                for (int j = 0; j < b.cols; j++) {
                    if (b.arr[k][j] == 0) continue;
                    product.arr[i][j] = (product.arr[i][j] +  (arr[i][k]) * b.arr[k][j]) ;
                }
            }
        }
        return product;
    }
    vector<T>& operator [](int i){
        return arr[i];
    }
};

template<class T = double>
Matrix<T> power(Matrix<T> a, int b) {
    Matrix<T> res(a.rows, a.cols);
    for(int i = 0;  i < a.rows; i++) res[i][i] = 1;
    while (b > 0) {
        if (b & 1) res = res * a;
        a = a * a;
        b >>= 1;
    }
    return res;
}
signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n; cin >> n;
    double p; cin >> p;

    Matrix<double>T(2, 2), base(1, 2);
    base[0][0] = base[0][1] = 1;
    T[0][0] = 1 - 2*p;
    T[1][0] = p;
    T[1][1] = 1;
    Matrix<double>res = power(T, n);
    res = base * res;
    cout << fixed << setprecision(16);
    cout << res[0][0];

    return 0;
}
