you are given n, k  and f[n, k] = sumf of i^k  where i from 0 to n 
you are asked to output sum of f(n,i) where i from 0 to n

#include <bits/stdc++.h>
using namespace std;

#define int long long
using bint = __int128;

bint read() {
    bint x = 0;
    char ch = getchar();
    while (ch >= '0' && ch <= '9') {
        x = x * 10 + ch - '0';
        ch = getchar();
    }
    return x;
}

void print(bint x) {
    if (x < 0) putchar('-'), x = -x;
    if (x > 9) print(x / 10);
    putchar(x % 10 + '0');
}
bint mod = 1;
template<class T = bint>
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
                    product.arr[i][j] += (__int128)(arr[i][k]) * b.arr[k][j] % mod;
                    if(product.arr[i][j] >= mod) product.arr[i][j] -=mod;
                }
            }
        }
        return product;
    }
    vector<T>& operator [](int i){
        return arr[i];
    }
};

template<class T = bint>
Matrix<T> power(Matrix<T> a, __int128 b) {
    Matrix<T> res(a.rows, a.cols);
    for(int i = 0;  i < a.rows; i++) res[i][i] = 1;
    while (b > 0) {
        if (b & 1) res = res * a;
        a = a * a;
        b >>= 1;
    }
    return res;
}

const int M = 104;
bint  c[M + 1][M + 1];

void pascal(){
    c[0][0] = 1;
    for(int n = 1;n < M; n++){
        c[n][0] = 1;
        c[n][n] = 1;
        for(int k = 1;k<n;k++){
            c[n][k] = c[n - 1][k - 1] + c[n - 1][k];
            c[n][k] %= mod;
        }
    }
}

signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t = 1;
    for(int i = 0; i < 64; i++){
        mod *= 2;
    }
    pascal();
    while(t--){
        __int128 n, k; n = read();
        k  = read();
        Matrix<bint>T(k + 2, k + 2);
        for(int i = 0; i <= k ;i++){
            for(int j = 0; j <= i; j++){
                T[j][i]  = c[i][j];
            }
        }
        for(int i = 0; i <= k + 1; i++) T[i][k + 1] = 1;

        Matrix<bint>base(1, k + 2);
        for(int i = 0; i <= k + 1; i++) base[0][i]=  1;
        T = power(T, n);
        T = base * T;
       print(T[0][k + 1]);


    }









    return 0;
}

